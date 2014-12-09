#ifndef ANGLE_H
#define ANGLE_H
#include "Element.h"
#include "Line.h"

/* LINEMATCH ENUM - specifies which verts match */
enum LM { LM_NONE, LM_AA, LM_BB, LM_AB, LM_BA };

class Angle;
class Angle : public Element {
private :
    Line &a, &b;
    enum LM elm;
public :
    Angle (Line *a, Line *b, enum LM elm) : Element (), a(*a), b(*b), elm(elm) { }
    ~Angle () {}

    virtual void doDraw (void);
};


#endif
