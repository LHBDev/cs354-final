#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <list>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include "scene_manager.h"
#include <iostream>
#include "GL.h"
#ifdef _WIN32
#	include <windows.h>
#else
#	include <sys/time.h>
#endif

#define FRAME_RATE 45 //target frame rate

static std::vector<Scene *>  scenes;
static std::vector<char *> names;
Scene *scene;

/* Default Viewport **************************************************************/
static GLfloat vfov = 55.f;
static GLfloat vzoom = 1.f;
static GLfloat veyex = 5.f;
static GLfloat veyey = 5.f;
static GLfloat veyez = -5.5f;
static GLfloat vupx = 0.f;
static GLfloat vupy = 1.f;
//static GLfloat vupz = 0.f;

#ifdef _WIN32
static GLfloat vmovespeed = 0.05f;      //movement
#else
static GLfloat vmovespeed = 0.25f;      //movement
#endif
static GLfloat vmovexdelta = 0.f;
static GLfloat vmoveydelta = 0.f;

static GLfloat vanglex = 0.f;
static GLfloat vanglexdelta = 0.f;
static GLfloat vangley = 1.f;         //default angle to spin the view by
static GLfloat vangleydelta = 0.f;
static GLfloat vangledampener = 400.f;  //slowness of the turn, higher = slower

static GLfloat vx = sin(vanglex);
static GLfloat vy = -sin(vangley) + cos(vangley);
static GLfloat vz = -cos(vanglex);

static int window_x = 800;
static int window_y = 600;
/* Mouse Vars ********************************************************************/
static bool  bLMouseDown = false;
static bool  bRMouseDown = false;
static float xsource = 0.f; 
static float ysource = 0.f;
static GLfloat select_thresh = 0.00001f;
/* Timing Info *******************************************************************/
static double qtime = 0.0;
static bool   bInProgress = true;
#ifdef _WIN32
static double qmult = 100.d;
static DWORD next_redraw;
static DWORD redraw_interval;
static DWORD last_idle_time;
#else
static double qmult = 250.d;
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
    glutPostRedisplay();
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
    scenes.push_back(scene);

    /* For now, these vertexes are just added manually, but we
       should parse them in from a file with a format that makes
       this process easier.  Then we can make scenes for each
       loaded file, and switch between them on the fly. */
    vert a = scene->addVertex3f (4.5, 5.5, -5.0);
    vert b = scene->addVertex3f (5.5, 5.5, -5.0);
    vert c = scene->addVertex3f (5.5, 4.5, -5.0);
    vert d = scene->addVertex3f (4.5, 4.5, -5.0);
    vert e = scene->addVertex3f (5.0, 5.0, -4.5);
    vert f = scene->addVertex3f (5.0, 5.0, -5.5);
    scene->createLine (a, b)->setColor3f (0.1f, 0.8f, 0.1f);
    scene->createLine (b, c)->setColor3f (0.1f, 0.8f, 0.1f);
    scene->createLine (c, d)->setColor3f (0.1f, 0.8f, 0.1f);
    scene->createLine (d, a)->setColor3f (0.1f, 0.8f, 0.1f);
    scene->createAngle (
      scene->createLine (d, e),
      scene->createLine (a, e)
    );
    scene->createAngle (
      scene->createLine (b, e),
      scene->createLine (c, e)
    );
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

    glClearColor(0.1, 0.1, 0.1, 0.0);
    glEnable(GL_DEPTH_TEST);
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
    qtime += 1 * bInProgress;
    dt *= qmult;
    

    /* Update veye position */
    if(vmoveydelta || vmovexdelta){
        veyex += vmoveydelta * vx * dt;
        veyez += vmoveydelta * vz * dt;
        veyey += vmoveydelta * vy * dt;

        veyex += vmovexdelta * vz * 0.5f * dt;
        veyez += vmovexdelta * -vx * 0.5 * dt;
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

//Move this up
GLuint selectBuf[512];

static void startPicking(int x, int y){
    GLint viewport[4];
    glSelectBuffer(512, selectBuf);
    glRenderMode(GL_SELECT);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glGetIntegerv(GL_VIEWPORT, viewport);
    gluPickMatrix(x, viewport[3] - y, 20, 20, viewport);
    gluPerspective(vzoom * vfov, 0.5f, 0.25, 230.0);
    glMatrixMode(GL_MODELVIEW);
    glInitNames();
    glutPostRedisplay();
}

static void processHits(GLint hits, GLuint buffer[]){
    unsigned int i, j;
    GLuint names, *ptr, minZ, *ptrNames, numberOfNames;

    printf("hits = %d\n", hits);
}

static void stopPicking(){
    int hits;

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glFlush();

    hits = glRenderMode(GL_RENDER);
    printf("%d\n", hits);

    if(hits != 0)
        processHits(hits, selectBuf);
}

static double select_expire = 0;
static void mouse (int button, int state, int x, int y) {
    switch(button){
        // case GLUT_RIGHT_BUTTON:
        //     if(state == GLUT_DOWN)
        //         scene->loadFile("test.txt");
        //         glutPostRedisplay();


        //     break;
        case GLUT_LEFT_BUTTON:
            if(state == GLUT_DOWN){
                xsource = x;
                ysource = y;
                bRMouseDown = true;
                select_expire = qtime + 500;
            } else {
                if (scene && select_expire >= qtime && vanglexdelta < select_thresh && vangleydelta < select_thresh)
                  active_vertex = scene->selectVertex (x, y);
                vanglex += vanglexdelta;
                vanglexdelta = 0;
                vangley += vangleydelta;
                vangleydelta = 0;
                bRMouseDown = false;
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
    if(bRMouseDown){
        vanglexdelta = x-xsource;
        vanglexdelta /= vangledampener;
        vx = sin(vanglex + vanglexdelta);
        vz = -cos(vanglex + vanglexdelta);

        vangleydelta = y-ysource;
        vangleydelta /= vangledampener;
        vy = -sin(vangley + vangleydelta) + cos(vangley + vangleydelta);

        updateView();
    }
    else if(bLMouseDown){

    }
}

static void keyboard (unsigned char key, int x, int y) {

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
        if (active_vertex) {
            case 'j':
                active_vertex->x -= 0.05;
                break;
            case 'l':
                active_vertex->x += 0.05;
                break;
            case 'i':
                active_vertex->y += 0.05;
                break;
            case 'k':
                active_vertex->y -= 0.05;
                break;
        }
        default:
            /* if it's a number key, set the active vertex to
               that number */
            if (scene && key >= '0' && key <= '9' && (scene->count > ((int)(key - '0'))))
                active_vertex = scene->vertexes[key - '0'];
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

static void specialUp(int key, int x, int y){
    vmovexdelta = 0.0f;
    vmoveydelta = 0.0f;
}

static void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -4.0f);
    glColor3f(0.0f, 0.0f, 1.0f);

    scene->display();

    glutSwapBuffers();
}

static void menu(int op){
    switch(op){
        case -1:
            exit(0);
            break;
        default:
            scene = scenes[op];
    }
    glutPostRedisplay();
}

void loadFiles(int argc, char **argv){
    if(argc <= 1)
        return;
    for(int i = 1; i < argc; ++i){
        Scene *a = new Scene();
        scenes.push_back(a);
        names.push_back(argv[i]);
        a->loadFile(argv[i]);
    }
}

int main(int argc, char **argv) {
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH ); // Initialize modes
    glutInitWindowSize(window_x, window_y);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("Final Project");
    init();
    char *s = new char[8];
    strcpy (s, "Default");
    names.push_back(s);

    loadFiles(argc, argv);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardup);

    glutCreateMenu(menu);
    int length = names.size();
    for(int i = 0; i < length; ++i){
        glutAddMenuEntry(names[i], i);
    }
    glutAddMenuEntry("Exit", -1);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();
    return 0;
}
