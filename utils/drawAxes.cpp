#include "drawAxes.h"

void drawAxes(float len) {
#if 0
  glPushAttrib(GL_CURRENT_BIT);
#endif
  glBegin(GL_LINES);
  glColor3f(1.0, 0.0, 0.0);
  glVertex3f(-len, 0.0, 0.0);
  glVertex3f(len, 0.0, 0.0);
  glColor3f(0.0, 1.0, 0.0);
  glVertex3f(0.0, -len, 0.0);
  glVertex3f(0.0, len, 0.0);
  glColor3f(0.0, 0.0, 1.0);
  glVertex3f(0.0, 0.0, -len);
  glVertex3f(0.0, 0.0, len);
  glEnd();
#if 0
  glPopAttrib();
#endif
}
