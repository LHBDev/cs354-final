#include "Vertex.h"

void Vertex::doDraw (void) {
    glTranslatef (x, y, z);
    glutSolidSphere(VERTEX_RADIUS,20,20); 
}