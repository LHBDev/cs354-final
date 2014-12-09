#ifndef FANO_H
#define FANO_H
#include "Element.h"
#include "Line.h"

class Fano;
class Fano : public Element {
private :
    Line &a, &b, &c;
public :
    Fano (Line *a, Line *b, Line *c) : Element (), a(*a), b(*b), c(*c) { }
    ~Fano () {}

    virtual void doDraw (void);
};


#endif
