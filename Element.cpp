#include "Element.h"

void Element::draw (void) {
    if (bDraw) {
        glColor3f (red, green, blue);
        glPushMatrix ();
        doDraw ();
        glPopMatrix ();
    }
}

void Element::init () {
}

void Element::setColor4f (GLfloat r, GLfloat g, GLfloat b, GLfloat a) {
    setColor3f (r, g, b);
    alpha = a;
}

void Element::setColor3f (GLfloat r, GLfloat g, GLfloat b) {
    red = r;
    green = g;
    blue = b;
}

void Element::setColor (glm::vec4 color) {
    setColor4f (color.x, color.y, color.z, color.w);
}

void Element::getColor4f (GLfloat *r, GLfloat *g, GLfloat *b, GLfloat *a) {
    getColor3f(r, g, b);
    *a = alpha;
}

void Element::getColor3f (GLfloat *r, GLfloat *g, GLfloat *b) {
    *r = red;
    *g = green;
    *b = blue;
}

glm::vec4 Element::getColor () {
  return glm::vec4(red, green, blue, alpha);
}

void Element::show () {
    bDraw = true;
}

void Element::hide () {
    bDraw = false;
}
