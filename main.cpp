#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <list>
#include "scene_manager.h"
#include "GL.h"
#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

#pragma warning( disable: 4305 )
#pragma warning( disable: 4244 )

#define FRAME_RATE 45 //target frame rate

Scene *scene;

/* Default Viewport **************************************************************/
static GLfloat vfov = 90.f;
static GLfloat vzoom = 1.f;
static GLfloat veyex = 5.f;
static GLfloat veyey = 5.f;
static GLfloat veyez = -3.f;
static GLfloat vupx = 0.f;
static GLfloat vupy = 1.f;
static GLfloat vupz = 0.f;

static GLfloat vx = 0.f;                //vector of viewport direction
static GLfloat vy = 0.f;                //calculated at init()
static GLfloat vz = -1.f;

static GLfloat vmovespeed = 0.05f;      //movement
static GLfloat vmovexdelta = 0.f;
static GLfloat vmoveydelta = 0.f;

static GLfloat vanglex = 0.f;
static GLfloat vanglexdelta = 0.f;
static GLfloat vangley = 1.f;         //default angle to spin the view by
static GLfloat vangleydelta = 0.f;
static GLfloat vangledampener = 300.f;  //slowness of the turn, higher = slower

static int window_x = 800;
static int window_y = 600;
/* Mouse Vars ********************************************************************/
static bool  bMouseDown = false;
static float xsource = 0.f; 
static float ysource = 0.f;
/* Timing Info *******************************************************************/
static double time = 0.0;
static bool   bInProgress = true;
#ifdef _WIN32
static double qmult = 1.0;
static DWORD next_redraw;
static DWORD redraw_interval;
static DWORD last_idle_time;
#else
static double qmult = 1.5;
static struct timeval next_redraw;
static struct timeval redraw_interval;
static struct timeval last_idle_time;
#endif
/*********************************************************************************/
static vert active_vertex = 0;
/*********************************************************************************/

/* sets camera to its correct position */
static void updateView(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(vzoom * vfov, 1.f, 0.25, 230.0);
    gluLookAt(veyex, veyey, veyez, veyex + vx, veyey + vy, veyez + vz, vupx, vupy, vy);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/* updates the viewport and the view (camera) */
static void reshape (int width, int height) {
    window_x = width;
    window_y = height;
    glViewport(0, 0, (GLsizei)width, (GLsizei)height);
    updateView();
}


/* initialize the scene */
static void init() {
    scene = new Scene ();

    /* For now, these vertexes are just added manually, but we
       should parse them in from a file with a format that makes
       this process easier.  Then we can make scenes for each
       loaded file, and switch between them on the fly. */
    vert a = scene->addVertex (4.5, 5.5, -5.0);
    vert b = scene->addVertex (5.5, 5.5, -5.0);
    vert c = scene->addVertex (5.5, 4.5, -5.0);
    vert d = scene->addVertex (4.5, 4.5, -5.0);
    vert e = scene->addVertex (5.0, 5.0, -4.5);
    vert f = scene->addVertex (5.0, 5.0, -5.5);
    scene->createLine (a, b);
    scene->createLine (b, c);
    scene->createLine (c, d);
    scene->createLine (d, a);
    scene->createLine (d, e);
    scene->createLine (a, e);
    scene->createLine (b, e);
    scene->createLine (c, e);
    scene->createLine (d, e);
    scene->createLine (a, f);
    scene->createLine (b, f);
    scene->createLine (c, f);
    scene->createLine (d, f);
    
    /* Multi-Platform Timings *******************************/

#ifdef _WIN32
    next_redraw = 0;
    redraw_interval = 1 / FRAME_RATE;
#else
    next_redraw.tv_sec = 0;
    redraw_interval.tv_sec = 1 / FRAME_RATE;
#endif

    //vx = sin(vanglex);
    //vz = -cos(vangley);
    //vy = sin(vangley + vangleydelta) + cos(vangley + vangleydelta);

    glClearColor(0.3, 0.3, 0.3, 0.0);
    glEnable(GL_DEPTH_TEST);

    vmoveydelta = 0.0;
    vmovexdelta = 0.0;
}

/* The idle function just advances time and updates the scene, forcing redraws on the
   frame rate */
static void idle (void) {
    float dt;
#ifdef _WIN32
    DWORD time_now;
    time_now = GetTickCount();
    dt = (float) (time_now - last_idle_time) / 1000.0;
#else
    struct timeval time_now;
    gettimeofday(&time_now, NULL);
    dt = (float)(time_now.tv_sec  - last_idle_time.tv_sec) +
    1.0e-6*(time_now.tv_usec - last_idle_time.tv_usec);
#endif
    time += 1 * qmult * bInProgress;
    

    /* Update veye position */
    if(vmoveydelta || vmovexdelta){
        veyex += vmoveydelta * vx * 0.075f;
        veyez += vmoveydelta * vz * 0.075f;
        veyey += vmoveydelta * vy * 0.075f;
        veyex += vmovexdelta * vz * 0.05f;
        veyez += vmovexdelta * -vx * 0.05;
        updateView();
    }

    // Force redraw
#ifdef _WIN32
    last_idle_time = time_now;
    
    if (time_now >= next_redraw) {
        next_redraw = time_now + redraw_interval;
#else
    last_idle_time.tv_sec = time_now.tv_sec;
    last_idle_time.tv_usec = time_now.tv_usec;
    if (time_now.tv_sec >= next_redraw.tv_sec) {
        next_redraw.tv_sec = time_now.tv_sec + redraw_interval.tv_sec;
#endif
        glutPostRedisplay();
    }
}

static void mouse (int button, int state, int x, int y) {
    switch(button){
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN){
                xsource = x;
                ysource = y;
                bMouseDown = true;
            } else {
                vanglex += vanglexdelta;
                vanglexdelta = 0;
                vangley += vangleydelta;
                vangleydelta = 0;
                bMouseDown = false;
            }
            break;
        case 3: //wheel up
            if(state == GLUT_DOWN && vzoom > 0.34){
                vzoom -= 0.08;
                vangledampener += 150;
            }
            updateView();
            break;
        case 4: //wheel down
            if(state == GLUT_DOWN && vzoom < 1.0){
                vzoom += 0.08;
                vangledampener -= 150;
            }
            updateView();
            break;
    }
}

static void motion (int x, int y) {
    if(bMouseDown){
        vanglexdelta = x-xsource;
        vanglexdelta /= vangledampener;
        vx = sin(vanglex + vanglexdelta);
        vz = -cos(vanglex + vanglexdelta);

        vangleydelta = y-ysource;
        vangleydelta /= vangledampener;
        vy = -sin(vangley + vangleydelta) + cos(vangley + vangleydelta);

        updateView();
    }
}

static void keyboard (unsigned char key, int x, int y) {
    Vertex *vertex = NULL;
    if (scene)
        vertex = scene->getVertex (active_vertex);

    switch(key){
        case 'w':
        case 'W':
            vmoveydelta = vmovespeed;
            break;
        case 's':
        case 'S':
            vmoveydelta = -vmovespeed;
            break;
        case 'a':
        case 'A':
            vmovexdelta = vmovespeed;
            break;
        case 'd':
        case 'D':
            vmovexdelta = -vmovespeed;
            break;
        if (vertex) {
            case 'j':
                vertex->x -= 0.05;
                break;
            case 'l':
                vertex->x += 0.05;
                break;
            case 'i':
                vertex->z -= 0.05;
                break;
            case 'k':
                vertex->z += 0.05;
                break;
        }
        default:
            /* if it's a number key, set the active vertex to
               that number */
            if (key >= '0' && key <= '9')
                active_vertex = key - '0';
            break;
    };
}

static void keyboardup (unsigned char key, int x, int y) {
    vmoveydelta = 0.f;
    vmovexdelta = 0.f;
}

static void special (int key, int x, int y) {
        switch(key){
        case GLUT_KEY_UP:
            vmoveydelta = vmovespeed;
            break;
        case GLUT_KEY_DOWN:
            vmoveydelta = -vmovespeed;
            break;
        case GLUT_KEY_RIGHT:
            vmovexdelta = vmovespeed;
            break;
        case GLUT_KEY_LEFT:
            vmovexdelta = -vmovespeed;
            break;
        case GLUT_KEY_F1:
        case GLUT_KEY_F2:
        case GLUT_KEY_F3:
        case GLUT_KEY_F4:
        case GLUT_KEY_F5:
        case GLUT_KEY_F6:
        case GLUT_KEY_F7:
        case GLUT_KEY_F8:
        case GLUT_KEY_F9:
        case GLUT_KEY_F10:
            break;
    };
}

static void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -4.0f);
    glColor3f(0.0f, 0.0f, 1.0f);

    scene->display();

    glutSwapBuffers();
}

int main(int argc, char **argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH ); // Initialize modes
    glutInitWindowSize(window_x, window_y);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Final Project");
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);
    glutSpecialFunc(special);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardup);

    glutMainLoop();
    return 0;
}
