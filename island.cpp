#include "island.h"

Island::Island()
{

}

Island::~Island()
{
  delete cannon;
}

void Island::drawLand(bool text, int tess, GLuint texture) 
{
  glPushMatrix();
    GLUquadricObj *cylinder = nullptr;
    cylinder = gluNewQuadric();
    if(text)
    {
      glEnable(GL_TEXTURE_2D);
      glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
      glBindTexture(GL_TEXTURE_2D, texture);
      gluQuadricTexture(cylinder, true);
    }
    gluQuadricNormals(cylinder, GLU_SMOOTH);
    glTranslatef(0, - (ISLAND_RAD - ABOVE_SEA_LEVEL), 0);
    glPushMatrix();
      glTranslatef(0, -(HEIGHT), 0);
      glRotatef(-90, 1, 0, 0);
      gluCylinder(cylinder, ISLAND_RAD + 0.5, ISLAND_RAD, HEIGHT, tess, tess);
    glPopMatrix();
    glRotatef(-90, 0, 0, 1);
    gluSphere(cylinder, ISLAND_RAD, tess, tess);
    glDisable(GL_TEXTURE_2D);
    // delete cylinder;
  glPopMatrix();
}

void Island::drawIsland(bool light, bool text, int tess, GLuint texture)
{
  glPushMatrix();
    if(light) 
    {
      glEnable(GL_LIGHTING);
      glEnable(GL_LIGHT0);
    
      float ambDiff[4] = {0, 0, 1, 0.5};
    
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff);
    }
    drawLand(text, tess, texture);   
    glPushMatrix();  
      //glRotatef(phi, 0, 1, 0);
      glPushMatrix();
        glTranslatef(0, ABOVE_SEA_LEVEL, 0);
        glPushMatrix();
          glScalef(CANNON_SCALE, CANNON_SCALE, CANNON_SCALE);  
          glPushMatrix();
            glTranslatef(0, -(CANNON_BASE_HEIGHT - CANNON_ELEV), 0);
            glRotatef(-90, 1, 0, 0);
            gluCylinder(gluNewQuadric(), 0.9, 0.7, CANNON_BASE_HEIGHT, tess, tess);
          glPopMatrix();
          glRotatef(cannon->getCannonRotation(), 0, 1, 0);
          glPushMatrix();
            glTranslatef(0, CANNON_ELEV, 0);
            glPushMatrix();
              glRotatef(-90, 1, 0, 0);
              gluSphere(gluNewQuadric(), CANNON_BODY_RAD, tess, tess);
            glPopMatrix();
            glRotatef(180, 0, 1, 0);
            
            // Cannon
            glPushMatrix();
              glRotatef(cannon->getCannonAngle(), 1, 0, 0);
              gluCylinder(gluNewQuadric(), CANNON_WIDTH, CANNON_WIDTH, CANNON_LENGTH, tess, tess);
            glPopMatrix();
          glPopMatrix();
        glPopMatrix();
      glPopMatrix();
    glPopMatrix();
  glPopMatrix();
  glDisable(GL_LIGHTING);
  glDisable(GL_LIGHT0);
  drawCannonTrajectory();
}

void Island::drawCannonTrajectory()
{
  cannon->drawIslandCTrajectory();
}

void Island::updateIslandState() 
{
  cannon->updateIslandInitCannonPos();
}

void Island::fireCannon(float t, std::vector<ProjectileState*>& projectilesInAir)
{
  cannon->fire(t, projectilesInAir);
}

void Island::fireDefence(float t, std::vector<ProjectileState*>& defenceProjectiles)
{
  cannon->fireDefence(t, defenceProjectiles);
}

int Island::checkIfIslandIsHit(std::vector<ProjectileState*>& projectilesInAir, std::vector<Boat*>& boats)
{
  checkIfProjHit(projectilesInAir);
  return checkIfBoatHit(boats);
}

int Island::checkIfBoatHit(std::vector<Boat*>& v)
{
  int hits = 0;
  unsigned int loop = 0;
  while(loop < v.size())
  {
    Vec3f pos = v[loop]->getBoatPos();
    float distance = sqrtf(powf(pos.x - islandCenter.x, 2) + 
                        powf(pos.y - islandCenter.y, 2) + 
                        powf(pos.z - islandCenter.z, 2));
    if(distance < v[loop]->getBoatHitRadius() + ISLAND_RAD)
    {

      if(health > 0) 
      {
        --health;
      }
      hits++;
      delete v[loop];
      v.erase(v.begin() + loop);
    }
    else
    {
      ++loop;
    }
  }
  return hits;
}

void Island::checkIfProjHit(std::vector<ProjectileState*>& v)
{
  unsigned int loop = 0;
  while(loop < v.size())
  {
    Vec3f pos = v[loop]->getPosition();
    float distance = sqrtf(powf(pos.x - islandCenter.x, 2) + 
                        powf(pos.y - islandCenter.y, 2) + 
                        powf(pos.z - islandCenter.z, 2));

    float distancefromC = sqrtf(powf(pos.x - cannonCenter.x, 2) + 
                        powf(pos.y - cannonCenter.y, 2) + 
                        powf(pos.z - cannonCenter.z, 2));
    if(distance < v[loop]->getRad() + ISLAND_RAD ||  
      distancefromC < v[loop]->getRad() + CANNON_BODY_RAD)
    {

      if(health > 0) 
      {
        --health;
      }
      delete v[loop];
      v.erase(v.begin() + loop);
    }
    else
    {
      ++loop;
    }
  }
}

void Island::increasePower()
{
  cannon->setFireSpeed(cannon->getFireSpeed() + 0.05);
}

void Island::decreasePower()
{
  cannon->setFireSpeed(cannon->getFireSpeed() - 0.05);
}

void Island::rotateCannonLeft()
{
  cannon->setCannonRotation(cannon->getCannonRotation() + 2);
}

void Island::rotateCannonRight()
{
  cannon->setCannonRotation(cannon->getCannonRotation() - 2);
}

void Island::rotateCannonUp()
{
  if(cannon->getCannonAngle() > -180)
  {
    cannon->setCannonAngle(cannon->getCannonAngle() - 2);
  }
}

void Island::rotateCannonDown()
{
  if(cannon->getCannonAngle() < 0)
  {
    cannon->setCannonAngle(cannon->getCannonAngle() + 2);
  }
}

float Island::getPhi()
{
  return cannon->getCannonRotation();
}

float Island::getAlpha()
{
  return cannon->getCannonAngle();
}

int Island::getHealth()
{
  return health;
}

void Island::setHealth(int health)
{
  this->health = health;
}
