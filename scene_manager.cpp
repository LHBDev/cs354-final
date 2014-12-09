#include "scene_manager.h"
#include <algorithm>
#include <fstream>
#include <iostream>

static void display_element (Element *e) { e->draw(); }

/* Main display function */
void Scene::display () {
    std::for_each (elements.begin(), elements.end(), display_element);
}

/**********************************************************
 * Vertex Functions                                       *
 **********************************************************/

/* Adds a new vertex to the scene, returns a pointer to that vertex */
Vertex *Scene::addVertex3f (GLfloat x, GLfloat y, GLfloat z) {
    Vertex *vertex = new Vertex(x,y,z,0.f);
    
    if (vertex) {
      elements.push_back (vertex);
      verts.push_back (vertex);
      vertexes[vertex->id] = vertex;
    }

    return vertex;
}

Vertex *Scene::addVertex (glm::vec4 vector) {
    return addVertex3f (vector.x, vector.y, vector.z);
}

/* Gets the Vertex object associated with vert v and returns a pointer to it,
   returns NULL if the vert is invalid */
list<Vertex*> &Scene::getVertexList () {
    return verts;
}

/* Guesses at the best selection vertex based on the coordinates */
Vertex *Scene::selectVertex (int x, int y) {
  return verts.front();
}

/* Removes the vertex at the index specified, returning true if successful.
   Also destroys any dependent elements
   Returns false when there is no entry at this location */
void Scene::removeVertex (Vertex *vertex) {
    if (vertex) {
        elements.remove (vertex);
        verts.remove (vertex);
        vertexes[vertex->id] = NULL;
        delete vertex;
    }
}

/**********************************************************
 * Element Functions                                      *
 **********************************************************/

/* Create functions return NULL if the verts are not found, or memory allocation fails 
   otherwise a pointer to the element created is returned.  These elements are self-
   drawing once created. */

/* a simple line between two verts */
Line *Scene::createLine (Vertex *a, Vertex *b) {
    Line *line = NULL;
    if (a && b) {
        line = new Line(a, b);
        elements.push_back (line);
    }
    return line;
}

/* static helper for angle checks (must share one and only one vertex) */
static enum LM validAngle (Line &a, Line &b) {
  enum LM elm = LM_NONE;
  if (&a && &b) {
    if (a.a == b.a)
      elm = LM_AA;
      
    if (a.b == b.b) {
      if (elm)
        return LM_NONE;
      else
        elm = LM_BB;
    }        
    
    if (a.a == b.b) {
      if (elm)
        return LM_NONE;
      else
        elm = LM_AB;
    }
        
    if (a.b == b.a) {
      if (elm)
        return LM_NONE;
      else
        elm = LM_BA;
    }
  }
  return elm;
}

/* an angle (number) is displayed for the lines defined.  The lines must share a vertex, or the
   angle will return NULL */
Angle *Scene::createAngle (Line *a, Line *b) {
    Angle *angle = NULL;
    enum LM elm = validAngle (*a, *b);
    if (elm > LM_NONE) {
        angle = new Angle (a, b, elm);
        elements.push_back (angle);
    }
    return angle;
}

/* an ellipse is drawn at the foci calculated from the midpoints of the lines provided. */
Fano *Scene::createFano (Line *a, Line *b, Line *c) {
    Fano *fano = NULL;
    if (validAngle (*a,*b) && validAngle (*b,*c) && validAngle (*c,*a)) {
        fano = new Fano (a, b, c);
        elements.push_back (fano);
    }
    return fano;
}

/* Displays a specific element (used by the display function on each element) */
void Scene::displayElement (Element *e) {
    display_element (e);
}

/* Removes lines, angles, circles, etcetera from the scene */
void Scene::removeElement (Element *e) {
    elements.remove (e);
}

/**********************************************************************
 * FILE PARSING                                                       *
 **********************************************************************/
 
 /* load the file, parse the text, add the elements */
bool Scene::loadFile (std::string filename) {
    std::fstream fs;
    fs.open (filename);
    if (!fs.is_open ()) {
        fs.close ();
        return false;
    }
    
    //...
    return true;
}
 
