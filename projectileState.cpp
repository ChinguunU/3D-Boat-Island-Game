#include "projectileState.h"

ProjectileState::ProjectileState()
{
  r = {0.0, 0.0, 0.0};
  v = {0.0, 0.0, 0.0};
}

ProjectileState::ProjectileState(ProjectileState& copy)
{
  r = copy.r;
  v = copy.v;
}

void ProjectileState::setVelocity(Vec3f v)
{
  this->v = v;
}

Vec3f ProjectileState::getVelocity()
{
  return v;
}

void ProjectileState::setPosition(Vec3f r)
{
  this->r = r;
}

void ProjectileState::setRad(float rad)
{
  this->rad = rad;
}

float ProjectileState::getRad()
{
  return rad;
}

Vec3f ProjectileState::getPosition() {
  return r;
}

float ProjectileState::getDefenceStartT()
{
  return defenceStartT;
}

void ProjectileState::setDefenceStartT(float t)
{
  defenceStartT = t;
}

void ProjectileState::updateRadOfDefenceProjectiles(float t)
{
  rad += RAD_GROWTH * t / defenceStartT;
}

void ProjectileState::drawTrajectoryNumerical(float r, float g, float b)
{
  float dt = 0.001;

  Vec3f tempR = this->r;
  Vec3f tempV = this->v;

  glLineWidth(2);
  glColor3f(r, g , b);
  glBegin(GL_LINE_STRIP);
  while(tempR.y > -0.5)
  {
    tempR.x += tempV.x * dt;
    tempR.z += tempV.z * dt;
    tempR.y += tempV.y * dt;
     
    tempV.y += this->g * dt;
    glVertex3f(tempR.x, tempR.y, tempR.z);
  }
  glEnd();
  glLineWidth(1);
}

void ProjectileState::drawDirection(float r, float g, float b)
{
  glLineWidth(2);
  drawVector(this->r.x, this->r.y, this->r.z, 
            v.x, v.y, v.z, 1, false, r, g, b);
  glLineWidth(1);
}

void ProjectileState::updateProjectileStateNumerical(float dt)
{
    // Position
    r.x += v.x * dt;
    r.z += v.z * dt;
    r.y += v.y * dt;
        
    // Velocity
    v.y += g * dt;
}

void ProjectileState::drawDefence(bool light)
{
  if(light)
  {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    float ambDiff[4] = {1, 0.5, 0.5, 0.5};
  
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff);
  }
  glColor3f(1.0, 0.5, 0.5);
  glPushMatrix();
    glTranslatef(r.x, r.y, r.z);
    glutWireSphere(rad, CIRCLE_LAT_LON, CIRCLE_LAT_LON);
  glPopMatrix();
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
}

void ProjectileState::drawProjectile(bool light, int tess)
{
  if(light)
  {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    float ambDiff[4] = {1, 0.5, 0, 0.5};
  
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff);
  }
  glColor3f(1.0, 1.0, 1.0);
  glPushMatrix();
    glTranslatef(r.x, r.y, r.z);
    gluSphere(gluNewQuadric(), PROJ_RAD, tess, tess);
  glPopMatrix();
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
}
