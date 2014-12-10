#include "Angle.h"
#include <cstdio>
#include <iostream>
#include "glm/gtx/vector_angle.hpp"
#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

/* draws a line between two vertexes */
void Angle::doDraw (void) {
    glm::vec4 src, v1, v2;
    
    switch (elm) {
      case LM_AA:
        src = a.a.vector;
        v1 = a.b.vector;
        v2 = b.b.vector;
        break;
      case LM_AB:
        src = a.a.vector;
        v1 = a.b.vector;
        v2 = b.a.vector;
        break;
      case LM_BA:
        src = a.b.vector;
        v1 = a.a.vector;
        v2 = b.b.vector;
        break;
      case LM_BB:
        src = a.b.vector;
        v1 = a.a.vector;
        v2 = b.a.vector;
        break;
      default:
        return;
    };
    
    GLfloat magnitude;
    glm::vec3 l1 = (glm::vec3)(v1 - src);
    glm::vec3 l2 = (glm::vec3)(v2 - src);
    GLfloat mag1 = glm::length (l1);
    GLfloat mag2 = glm::length (l2);
    //GLfloat angle = glm::orientedAngle<GLfloat, glm::highp> (l1, l2, (glm::vec3)(src));
    GLfloat angle = glm::angle<GLfloat, glm::highp> (l1, l2);
    
    /* orthonormal */
    glm::vec3 rot_axis = glm::cross (l1, l2);
        
    GLfloat mag = std::min (mag1, mag2) / 8;
    
    /* move to the draw spot */
    glTranslatef (src.x, src.y, src.z);   
    
    l1 *= (mag/mag1);
    l2 *= (mag/mag2);
    
    /* set up our stepping direction */
    GLfloat step = M_PI/50;
    if (angle < 0)
      step = -step;
    
    /* draw */
    GLfloat i;
    glColor3f (0.f, 0.7f, 0.7f);
    glBegin (GL_LINES);
    for (i = 0; i + std::abs(step) < std::abs(angle); i += std::abs(step)) {
        l1 = glm::rotate (l1, step, rot_axis);
        glVertex3f (l1.x, l1.y, l1.z);
    }
    glEnd ();
    
}
