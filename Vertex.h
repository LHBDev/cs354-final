/* As the most elemental element, Vertexes are quite complex */
#ifndef VERTEX_H
#define VERTEX_H
#include "Element.h"
#include "GL.h"

#define VERTEX_RADIUS 0.015

class Vertex;

class Vertex : public Element {
public :
    /* references to the X, Y and Z floats of the vertex entry */
    glm::vec4 vector;
    GLfloat &x, &y, &z, &w;
    unsigned int id; /* the order this vert was added into the system in 
                        0, 1, 2, 3 ... */

    /* ctor/dtor */
    Vertex (glm::vec4 inVector);
    Vertex (GLfloat x, GLfloat y, GLfloat z, GLfloat w);
    Vertex (GLfloat x, GLfloat y, GLfloat z, GLfloat w, int r);
    virtual ~Vertex () { };
    
    bool operator== (const Vertex &b) const;

    /* virtualized draw function */
    virtual void doDraw (void);
    
    /* init */
    virtual void init (void);

};

#endif