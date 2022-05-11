#include <cmath>
#include <cstdio>
#include <cstdint>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "utils/stb_image.h"

#include "GameEngine.h"


#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

std::vector<GLuint> textures;

// load a texture from file using the stb_image library
uint32_t loadTexture(const char* filename) {
  int width, height, components;
  unsigned char* data = stbi_load(filename, &width, &height, &components, STBI_rgb);

  glPushAttrib(GL_TEXTURE_BIT);

  unsigned int id;
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);
  //glGenerateMipmap(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

  glPopAttrib();


  return id;
}

GameEngine* game = new GameEngine();

void idle() {

  game->idle();
  glutPostRedisplay();
}

// Display callback
void display()
{
  game->display(textures);
  glutSwapBuffers();
}

void reshape (int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30.0, (GLfloat) w/(GLfloat) h, 1.0, 60.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0, -1, -6);
}

void keyboardCB(unsigned char key, int x, int y)
{
  game->keyboardCB(key, x, y);

  glutPostRedisplay();
}

void keyboardUpCB(unsigned char key, int x, int y)
{
  game->keyboardUpCB(key, x, y);

  glutPostRedisplay();
}

void specialKCB(int key, int x, int y)
{
  game->specialKCB(key, x, y);
  glutPostRedisplay();
}

void specialKUpCB(int key, int x, int y)
{
  game->specialKUpCB(key, x, y);
  glutPostRedisplay();
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Assignment 2");
  glutKeyboardFunc(keyboardCB);
  glutKeyboardUpFunc(keyboardUpCB);
  glutSpecialFunc(specialKCB);
  glutSpecialUpFunc(specialKUpCB);
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  textures.push_back(loadTexture("images/zpos.png"));
  textures.push_back(loadTexture("images/xpos.png"));
  textures.push_back(loadTexture("images/zneg.png"));
  textures.push_back(loadTexture("images/xneg.png"));
  textures.push_back(loadTexture("images/sand.jpg"));
  textures.push_back(loadTexture("images/ypos.png"));
  glutMainLoop();
}
