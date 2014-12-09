#include "Line.h"

/* draws a line between two vertexes */
void Line::doDraw (void) {
    glBegin (GL_LINES);
    glVertex3f (a.x, a.y, a.z);
    glVertex3f (b.x, b.y, b.z);
    glEnd ();
}
