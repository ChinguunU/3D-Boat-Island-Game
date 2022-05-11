#ifndef BOAT
#define BOAT

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <vector>

#include "utils/radDegConv.h"
#include "utils/drawAxes.h"

#include "boatMeasure.h"
#include "projectileState.h"
#include "cannon.h"
#include "wave.h"

static std::vector<Vec3f> normals = {{0, 0, 1}, {1, 0, 0}, {0, 0, -1}, {-1, 0, 0}, {0, -1, 0}, {0, 1, 0}};

class Boat
{
public:
  Boat(Vec3f boatPos);
  ~Boat();
  void drawHull(float length, float height, float width, int tess);
  void drawBoat(bool light, int tess);
  void updateBoatState(Wave wave, float t);
  void fireCannon(float t, std::vector<ProjectileState*>& projectilesInAir);
  bool checkIfBoatIsHit(std::vector<ProjectileState*>& projectilesInAir);
  void rotateCannonUp();
  void rotateCannonDown();
  void turnLeft();
  void turnRight();
  Vec3f getBoatPos();
  void setBoatPos(Vec3f pos);
  float getBoatYR();
  void setBoatYR(float yr);
  float getBoatHitRadius();
  Vec3f getBoatVelocity();
  bool isUp();

private:
  float boatZR = 0.0;
  float boatXR = 0.0;
  float boatYR = 0.0;
  bool up = true;
  Vec3f boatPos;
  CannonPtr cannon;
  float boatHitRadius = (float)B_L / 2.0 * B_SCALE;
};

typedef Boat* BoatPtr;

#endif // BOAT
