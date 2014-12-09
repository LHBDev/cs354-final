#ifndef LINE_H
#define LINE_H
#include "Element.h"
#include "Vertex.h"

class Line;
class Line : public Element {
public:
    Vertex &a, &b;
    Line (Vertex *a, Vertex *b) : Element (), a(*a), b(*b) { }
    ~Line () {}

    virtual void doDraw (void);
    
};


#endif
