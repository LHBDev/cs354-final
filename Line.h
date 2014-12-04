#ifndef LINE_H
#define LINE_H
#include "Element.h"
#include "Vertex.h"

class Line;
class Line : public Element {
private :
    Vertex &a, &b;
public :
    Line (Vertex *a, Vertex *b) : Element (), a(*a), b(*b) { }
    ~Line () {}

    virtual void doDraw (void);
};


#endif
