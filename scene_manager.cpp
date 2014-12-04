#include "scene_manager.h"
#include <algorithm>

static void display_element (Element *e) { e->draw(); }

/* Main display function */
void Scene::display () {
    std::for_each (elements->begin(), elements->end(), display_element);
}

/**********************************************************
 * Vertex Functions                                       *
 **********************************************************/

static bool vertexIsValid (Scene *s, vert v) {
    return (v < MAX_VERTS && s->getVertex (v) != NULL);
}

/* Makes a new vertex entry for the specified coordinates,
   returns the vertex's index in the vertex array (which is
   the "Vertex ID" and can be treated like a pointer when
   using the scene manager's *Vertex functions) 
   returns -1 when the array is full */
vert Scene::addVertex (GLfloat x, GLfloat y, GLfloat z) {
    if (verts_size == MAX_VERTS)
        return -1;

    verts[verts_size].x = x;
    verts[verts_size].y = y;
    verts[verts_size].z = z;
    verts[verts_size].vertex = new Vertex (&verts[verts_size]);
    elements->push_back (verts[verts_size].vertex);

    ++verts_size;

    return verts_size - 1;
}

/* Removes the vertex at the index specified, returning true if successful.
   Also destroys any dependent elements
   Returns false when there is no entry at this location */
bool Scene::removeVertex (vert v) {
    bool success = (getVertex (v) != NULL);
    if (success) 
        delete verts[v].vertex;
    return success;
}

/* Gets the Vertex object associated with vert v and returns a pointer to it,
   returns NULL if the vert is invalid */
Vertex *Scene::getVertex (vert v) {
    Vertex *vertex = NULL;
    if (v < MAX_VERTS)
        vertex = verts[v].vertex;
    return vertex;
}

/* Moves the specified vertex to the specified coordinates */
bool Scene::moveVertex (vert v, GLfloat x, GLfloat y, GLfloat z) {
   bool success = (getVertex (v) != NULL);
    if (success) {
        verts[v].x = x;
        verts[v].y = y;
        verts[v].z = z;
    }
    return success;
}

/**********************************************************
 * Element Functions                                      *
 **********************************************************/

/* Create functions return NULL if the verts are not found, or memory allocation fails 
   otherwise a pointer to the element created is returned.  These elements are self-
   drawing once created. */

/* a simple line between two verts */
Line *Scene::createLine (vert a, vert b) {
    Line *line = NULL;
    Vertex *vertex_a = getVertex (a);
    Vertex *vertex_b = getVertex (b);
    if (vertex_a && vertex_b) {
        line = new Line(vertex_a, vertex_b);
        elements->push_back (line);
    }
    return line;
}

/* an angle (number) is displayed for the lines defined by [source, a] and [source, b] *
Angle *Scene::createAngle (vert source, vert a, vert b) {
    Angle *angle = NULL;
    if (vertexIsValid (source) && vertexIsValid (a) && vertexIsValid (b)) {
        angle = new Angle (source, a, b);
        elements.push_back (angle);
    }
    return angle;
}

/* a circle is drawn around the specified vertex with a uniform radius *
Circle *Scene::createCircle (vert v, GLfloat radius) {
    Circle *circle = NULL;
    if (vertexIsValid (v)) {
        circle = new Circle (v, radius);
        elements.push_back (circle);
    }
    return circle;
}

/* an elipse is fitted around the three verts specified *
Elipse *Scene::createElipse (vert a, vert b, vert c) {
    Elipse *elipse = NULL;
    if (vertexIsValid (a) && vertexIsValid (b) && vertexIsValid (c)) {
        elipse = new Elipse (a, b, c);
        elements.push_back (elipse);
    }
    return elipse;
}

/* an elipse is drawn at the midpoints of each line defined by the verts 
   (like createElipse, but the circle is entirely contained by the triangle
   specified by the verts, whereas with an elipse the triangle is entirely
   contained within the circle) *
Fano *Scene::createFano (vert a, vert b, vert c) {
    Fano *fano = NULL;
    if (vertexIsValid (a) && vertexIsValid (b) && vertexIsValid (c)) {
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
    elements->remove (e);
}
