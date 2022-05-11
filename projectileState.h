#ifndef PROJECTILE_STATE
#define PROJECTILE_STATE

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <string>
#include "utils/drawVector.h"
#include "vec3f.h"
#include "wave.h"

#define PROJ_RAD 0.05
#define CIRCLE_LAT_LON 32
#define RAD_GROWTH 0.1
#define G_F -0.3

class ProjectileState
{
public:
  ProjectileState();
  ProjectileState(ProjectileState& copy);
  void setVelocity(Vec3f v);
  Vec3f getVelocity();
  void setPosition(Vec3f r);
  void setRad(float rad);
  float getRad();
  Vec3f getPosition();
  float getDefenceStartT();
  void setDefenceStartT(float t);
  void updateRadOfDefenceProjectiles(float t);
  void drawDefence(bool light);
  void drawProjectile(bool light, int tess);
  void updateProjectileStateNumerical(float dt);
  void drawTrajectoryNumerical(float r, float g, float b);
  void drawDirection(float r, float g, float b);
private:
  Vec3f r, v;
  float rad = PROJ_RAD;
  float defenceStartT = 0.0;
  float g = G_F;
};

typedef ProjectileState* ProjectilePtr;

#endif // PROJECTILE_STATE
