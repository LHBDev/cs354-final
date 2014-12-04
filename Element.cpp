#include "Element.h"

void Element::draw (void) {
    if (bDraw) {
        glColor3f (red, green, blue);
        glPushMatrix ();
        doDraw ();
        glPopMatrix ();
    }
}

void Element::setColor (GLfloat r, GLfloat g, GLfloat b) {
    red = r;
    green = g;
    blue = b;
}

void Element::show () {
    bDraw = true;
}

void Element::hide () {
    bDraw = false;
}