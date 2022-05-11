#ifndef DRAW_VECTOR
#define DRAW_VECTOR

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>

void drawVector(float x, float y, float z, 
                float u, float v, float k,
                float s, bool normalize,
                float r, float g, float b);


#endif //DRAW_VECTOR
