#include "Vertex.h"

static const glm::vec4 color(0.8f, 0.05f, 0.05f, 1.f);
static volatile unsigned int vert_idx = 0;

Vertex::Vertex (glm::vec4 inVector) 
  : Element(), vector(inVector), x(vector.x), y(vector.y), z(vector.z), w(vector.w), id(vert_idx++) { init(); }
Vertex::Vertex (GLfloat x, GLfloat y, GLfloat z, GLfloat w) 
  : Element(), vector(glm::vec4(x,y,z,w)), x(vector.x), y(vector.y), z(vector.z), w(vector.w), id(vert_idx++) { init(); }

bool Vertex::operator== (const Vertex &b) const {
  return (x == b.x && y == b.y && z == b.z && w == b.w);
}
  
void Vertex::doDraw (void) {
    glTranslatef (x, y, z);
    glutSolidSphere(VERTEX_RADIUS,20,20); 
}

void Vertex::init () {
  Element::setColor (color);
}
