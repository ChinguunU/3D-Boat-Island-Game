#include "drawHealthBar.h"

void drawHealthBar(int health, int maxHealth, float r, float g, float b)
{
  glBegin(GL_QUADS);
    glColor3f(r, g, b);
    glVertex2f(0, 0);
    glVertex2f(0, 0.05);
    glVertex2f((float)health/(float)maxHealth * 0.55, 0.05);
    glVertex2f((float)health/(float)maxHealth * 0.55, 0);
  glEnd();
}
