/* Abstract class for elements */
#ifndef ELEMENT_H
#define ELEMENT_H
#include <list>
#include "GL.h"

using std::list;

class Element;
class Element {
protected:
    /* Components are elements that this element depends on (such as
       a list of 2 lines for an angle to be drawn properly)
       verts are the vertexes this element needs to draw properly */
    list<Element*> *dependents;
    GLfloat red;
    GLfloat green;
    GLfloat blue;
    bool bDraw;
public:
    /* ctor/dtor */
    Element () : dependents(new list<Element*>), bDraw(true), red(0.f), green(0.f), blue(1.f) {}
    virtual ~Element () { delete dependents; }

    /* enable/disable drawing of object */
    void show (void);
    void hide (void);

    /* change color of the object */
    void setColor (GLfloat red, GLfloat green, GLfloat blue);

    /* main draw function - not subclassed */
    void draw (void);

    /* purely virtual draw function for subclasses */
    virtual void doDraw (void) = 0;
};

#endif
