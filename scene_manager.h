#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#include <list>
#include <string>
#include "GL.h"
#include "Element.h"
#include "Vertex.h"
#include "Line.h"
#include "Angle.h"
#include "Fano.h"

#define MAX_VERTS 1024

using std::list;
class Scene;

/* vert enumerations are just unsigned ints, like memory addresses */
typedef Vertex * vert;

class Scene {
private:
    /* Private variables, including an array of data for the verts */
    list<Vertex*> verts;
    list<Element*> elements;
    
public:
    Vertex *vertexes[MAX_VERTS]; /* lookup verts by their ID (order they were added) */
    int count = 0;
    /* ctor/dtor */
    Scene  () {}
    Scene  (std::string filename) { loadFile(filename); }
    ~Scene () {}

    /* Displays all objects in the scene at the current matrix */
    void display  (void);
    
    /* Automatically loads and parses vertexes from a file */
    bool loadFile (std::string fs);

    /* Vertexes are added one at a time.  addVertex returns the ID of the vertex
       created, which is sequential (0, 1, 2, 3, etc).  Vertex IDs may be used to
       create lines, circles, elipses and fano planes */
    Vertex        *addVertex3f   (GLfloat x, GLfloat y, GLfloat z, int r=0);
    Vertex        *addVertex     (glm::vec4 vector);
    Vertex        *selectVertex  (int x, int y);
    list<Vertex*> &getVertexList (void);
    void          removeVertex   (Vertex *vertex);

    /* Higher-level functions for adding specific element types by vertex IDs
       Returns a pointer to the object type created */
    Line   *createLine  (Vertex *a, Vertex *b);
    Angle  *createAngle (Line *a, Line *b);
    Fano   *createFano  (Line *a, Line *b, Line *c);

    /* Displays a specific element (used by the display function on each element) */
    void displayElement (Element *e);

    /* Removes lines, angles, circles, etcetera from the scene */
    void removeElement  (Element *e);

    /*Clears the scene */
    void clearScene ();
};

#endif
