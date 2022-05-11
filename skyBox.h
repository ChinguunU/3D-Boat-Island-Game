#ifndef SKY_BOX
#define SKY_BOX

#include <vector>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "gameMeasure.h"


typedef struct {float u, v;} TexCoord;
typedef struct {float x, y, z;} Vertex;
static float dim = GAME_DIM;
static float a = dim / 2;
static std::vector<Vertex> skyBox = {{a, -a, a}, {a, a, a}, {-a, a, a}, {-a, -a, a},
                            {a, -a, -a}, {a, a, -a}, {a, a, a}, {a, -a, a},
                            {-a, -a, -a}, {-a, a, -a}, {a, a, -a}, {a, -a, -a},
                            {-a, -a, a}, {-a , a, a}, {-a, a, -a}, {-a, -a, -a},
                            {-a, -a, a}, {-a, -a, -a}, {a, -a, -a}, {a, -a, a},
                            {a, a, a}, {a, a, -a}, {-a, a, -a}, {-a, a, a}};

void drawBox(bool text, std::vector<GLuint> textures);
#endif // SKY_BOX