#ifndef GAME_ENGINE
#define GAME_ENGINE

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cstdio>
#include <iostream>

#include "boat.h"
#include "boatAi.h"
#include "island.h"
#include "wave.h"
#include "skyBox.h"
#include "islandMeasures.h"
#include "gameMeasure.h"
#include "utils/drawAxes.h"
#include "utils/osd.h"
#include "utils/gameStats.h"
#include "utils/resume.h"

typedef struct {bool polygonMode, light, normTan, text, gameOver;
                float t, startTime, pauseTime, delayTime;
                int tess, score;
                int frames;
                float frameRate;
                float frameRateInterval;
                float lastFrameRateT; } Global;

typedef struct {bool up, down;} VR;
typedef struct {bool left, right;} HR;
typedef struct {bool increase, decrease;} Power;

typedef struct {Power p;
                VR v;
                HR h;} CannonR;

typedef struct {float x, y, z;
                float yR, xR;} MapMove;


class GameEngine
{
public:
  const int milli = 1000;
  GameEngine();
  ~GameEngine();
  void restart();
  void idle();
  void display(std::vector<GLuint> &textures);
  void camera();
  // void drawHealthBars();
  void checkIfProjectileLandedInWater();
  void checkIfGameOver();
  void checkCannon();
  void checkIfDefenceProjectileExpired();
  void checkIfProjectileHasHitDefence();
  void keyboardCB(unsigned char key, int x, int y);
  void keyboardUpCB(unsigned char key, int x, int y);
  void specialKCB(int key, int x, int y);
  void specialKUpCB(int key, int x, int y);
private:
  Global global = {false, true, true, true, true, 
                  0.0, 0.0, 0.0, 0.0, 128, 0, 0, 0.0, 0.2, 0.0};
  
  // Island cannon rotation booleans
  CannonR cannonR = {{false, false}, {false, false}, {false, false}};

  MapMove mapMove = {0, -0.2, -0.4, 16.8, 17.4};

  // Island
  IslandPtr island = new Island();

  // Ocean 
  Wave wave;

  // Textures
  std::vector<GLuint> textures;

  // Attack projectiles
  std::vector<ProjectileState*> projectilesInAir;

  // Defence projectiles
  std::vector<ProjectileState*> defenceProjectiles;

  // Boats
  std::vector<Boat*> boats;

  // Boat AI
  BoatAi* ai = new BoatAi(global.t);
};

#endif // GAME_ENGINE
