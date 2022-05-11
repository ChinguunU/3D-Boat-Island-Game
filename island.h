#ifndef ISLAND
#define ISLAND

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <vector>

#include "utils/drawAxes.h"
#include "islandMeasures.h"
#include "vec3f.h"
#include "cannon.h"
#include "boat.h"
#include "projectileState.h"

class Island
{
public:
  Island();
  ~Island();
  void drawLand(bool text, int tess, GLuint texture);
  void drawIsland(bool light, bool text, int tess, GLuint texture);
  void drawCannonTrajectory();
  void updateIslandState();
  void fireCannon(float t, std::vector<ProjectileState*>& projectilesInAir);
  void fireDefence(float t, std::vector<ProjectileState*>& defenceProjectiles);
  void increasePower();
  void decreasePower();
  int checkIfIslandIsHit(std::vector<ProjectileState*>& projectilesInAir, std::vector<Boat*>& boats);
  int checkIfBoatHit(std::vector<Boat*>& v);
  void checkIfProjHit(std::vector<ProjectileState*>& v);
  void rotateCannonLeft();
  void rotateCannonRight();
  void rotateCannonUp();
  void rotateCannonDown();
  float getAlpha();
  float getPhi();
  int getHealth();
  void setHealth(int health);
private:
  int health = MAX_I_HEALTH;
  
  Vec3f islandCenter = {0, -ISLAND_RAD + ABOVE_SEA_LEVEL, 0};
  Vec3f cannonCenter = {0, ABOVE_SEA_LEVEL + CANNON_SCALE * (-(CANNON_BASE_HEIGHT - 2 * CANNON_ELEV))};
  CannonPtr cannon = new Cannon(0.0, 0.0);
};

typedef Island* IslandPtr;

#endif // ISLAND
