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
    GLfloat alpha;
    bool bDraw;
public:
    /* ctor/dtor */
    Element () : dependents(new list<Element*>), red(0.f), green(0.f), blue(1.f), alpha (1.f), bDraw(true) { init (); }
    virtual ~Element () { delete dependents; }

    /* enable/disable drawing of object */
    void show (void);
    void hide (void);

    /* change color of the object */
    void setColor4f (GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void setColor3f (GLfloat red, GLfloat green, GLfloat blue);
    void setColor (glm::vec4 color);
    
    /* retrieve color */
    void      getColor4f (GLfloat *red, GLfloat *green, GLfloat *blue, GLfloat *alpha);
    void      getColor3f (GLfloat *red, GLfloat *green, GLfloat *blue);
    glm::vec4 getColor (void);

    /* main draw function - not subclassed */
    void draw (void);

    /* purely virtual draw function for subclasses */
    virtual void doDraw (void) = 0;
    
    /* init */
    virtual void init (void);
};

#endif
