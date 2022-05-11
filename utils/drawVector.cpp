#include "drawVector.h"

void drawVector(float x, float y, float z, 
                float u, float v, float k,
                float s, bool normalize,
                float r, float g, float b) {
  float ePointX, ePointY, ePointZ;
  if(!normalize) {
    ePointX = x + u * s;
    ePointY = y + v * s;
    ePointZ = z + k * s;
  }
  else {
    float magnitude = sqrtf(powf(u, 2) + powf(v, 2) + powf(k, 2));
    ePointX = x + u / (10.0 * magnitude);
    ePointY = y + v / (10.0 * magnitude);
    ePointZ = z + k / (10.0 * magnitude);
  }

  glBegin(GL_LINES);
  glColor3f(r, g, b);
  glVertex3f(x, y, z);
  glVertex3f(ePointX, ePointY, ePointZ);
  glEnd();
}
