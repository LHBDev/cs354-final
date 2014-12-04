#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H
#include <list>
#include "GL.h"
#include "Element.h"
#include "Vertex.h"
#include "Line.h"
//#include "Angle.h"
//#include "Elipse.h"
//#include "Fano.h"

#define MAX_VERTS 255

using std::list;
class Scene;

/* vert enumerations are just unsigned ints, like memory addresses */
typedef unsigned int vert;

class Scene {
private:
    /* Private variables, including an array of BSS data for the verts */
    vertex_entry   verts[MAX_VERTS];
    unsigned int   verts_size;
    list<Element*> *elements;

public:
    /* ctor/dtor */
    Scene  () : verts_size(0), elements(new list<Element*>) {}
    ~Scene () { delete elements; }

    /* Displays all objects in the scene at the current matrix */
    void display (void);

    /* Vertexes are added one at a time.  addVertex returns the ID of the vertex
       created, which is sequential (0, 1, 2, 3, etc).  Vertex IDs may be used to
       create lines, circles, elipses and fano planes */
    vert   addVertex    (GLfloat x, GLfloat y, GLfloat z);
    bool   removeVertex (vert v);
    Vertex *getVertex   (vert v);
    bool   moveVertex   (vert v, GLfloat x, GLfloat y, GLfloat z);

    /* Higher-level functions for adding specific element types by vertex IDs
       Returns a pointer to the object type created */
    Line   *createLine  (vert a, vert b);
    //Angle  *createAngle  (vert source, vert a, vert b);
    //Elipse *createElipse (vert a, vert b, vert c);
    //Fano   *createFano   (vert a, vert b, vert c);

    /* Displays a specific element (used by the display function on each element) */
    void displayElement (Element *e);

    /* Removes lines, angles, circles, etcetera from the scene */
    void removeElement  (Element *e);
};

#endif
