/* As the most elemental element, Vertexes are quite simple */
#ifndef VERTEX_H
#define VERTEX_H
#include "Element.h"

#define VERTEX_RADIUS 0.015

class Vertex;

/* A vertex entry holds the globally accessed memory addresses for each
   vertex loaded into the scene with addVertex */
typedef struct vertex_entry_t {
    GLfloat x, y, z;
    Vertex  *vertex; 
} vertex_entry;


class Vertex : public Element {
private :
    /* each vertex object is linked to a vertex entry in memory */
    vertex_entry *ve;

public :
    /* references to the X, Y and Z floats of the vertex entry */
    GLfloat &x, &y, &z;

    /* ctor/dtor */
    Vertex (vertex_entry *ve) : Element(), ve(ve), x(ve->x), y(ve->y), z(ve->z) 
                              { Element::setColor (0.8f, 0.05f, 0.05f); }
    ~Vertex ()                { ve->vertex = NULL; }

    /* virtualized draw function */
    virtual void doDraw (void);
};

#endif
