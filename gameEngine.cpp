#include "gameEngine.h"

GameEngine::GameEngine() : wave(GAME_DIM, GAME_DIM, 0.19, 0.125, 0.3, 0.6, 0.2, 0.2)
{

}

GameEngine::~GameEngine()
{
  delete island;

  delete ai;

  restart();
}

void GameEngine::restart()
{
  for(unsigned int i = 0; i < projectilesInAir.size(); ++i)
  {
    delete projectilesInAir[i];
  }

  for(unsigned int i = 0; i < defenceProjectiles.size(); ++i)
  {
    delete defenceProjectiles[i];
  }

  for(unsigned int i = 0; i < boats.size(); ++i)
  {
    delete boats[i];
  }

  projectilesInAir.clear();
  defenceProjectiles.clear();
  boats.clear();
}

void GameEngine::checkIfProjectileLandedInWater()
{
  unsigned int i = 0;
  while(i < projectilesInAir.size())
  {
    if(projectilesInAir[i]->getPosition().y < -0.5)
    {
      delete projectilesInAir[i];
      projectilesInAir.erase(projectilesInAir.begin() + i);
    }
    else 
    {
      i++;
    }
  }
}

void GameEngine::checkIfGameOver()
{
  if(island->getHealth() == 0)
  {
    global.gameOver = true;
    global.pauseTime = glutGet(GLUT_ELAPSED_TIME) / (float) milli;
    restart();
    island->setHealth(MAX_I_HEALTH);
  }
}

void GameEngine::checkIfDefenceProjectileExpired()
{
  unsigned int loop = 0;
  while(loop < defenceProjectiles.size())
  {
    if(defenceProjectiles[loop]->getDefenceStartT() + 1.9 < global.t)
    {
      delete defenceProjectiles[loop];
      defenceProjectiles.erase(defenceProjectiles.begin() + loop);
    }
    else
    {
      ++loop;
    }
  }
}

void GameEngine::checkIfProjectileHasHitDefence()
{
  unsigned int loop = 0;
  while(loop < defenceProjectiles.size())
  {
    unsigned int inLoop = 0;
    while(inLoop < projectilesInAir.size())
    {
      float radA = projectilesInAir[inLoop]->getRad();
      float radD = defenceProjectiles[loop]->getRad();
      Vec3f posA = projectilesInAir[inLoop]->getPosition();
      Vec3f posD = defenceProjectiles[loop]->getPosition();
      float distance = sqrtf(powf(posA.x - posD.x, 2) + 
                            powf(posA.y - posD.y, 2) + 
                            powf(posA.z - posD.z, 2));
      if(distance < radA + radD)
      {
        delete projectilesInAir[inLoop];
        projectilesInAir.erase(projectilesInAir.begin() + inLoop);
      }
      else
      {
        ++inLoop;
      }
    }

    ++loop;
  }
}

void GameEngine::idle()
{
  if(!global.gameOver)
  {
    static float lastT = -1.0;
    float dt;

    // Island cannon state
    island->updateIslandState();

    global.t = glutGet(GLUT_ELAPSED_TIME) / (float) milli - (global.delayTime);

    // Island Cannon rotations
    checkCannon();

    if (lastT < 0.0)
    {
      lastT = global.t;
      return;
    }

    // Update state of Ocean/Wave
    wave.updateWave(global.tess, global.t);

    // Check if boat can should spawn
    ai->checkToGenerateBoat(global.t, boats);

    // Update boat movemen/rotation and cannon state
    for (unsigned int i = 0; i < boats.size(); ++i)
    {
      boats[i]->updateBoatState(this->wave, global.t);
    }

    // Move boats towards Island
    ai->moveForward(boats);

    // Check if boats can shoot
    ai->checkToShoot(global.t, boats, projectilesInAir);
    dt = global.t - lastT;
    
    // Update state of attack projectiles
    for (unsigned int i = 0; i < projectilesInAir.size(); ++i)
    {
      projectilesInAir[i]->updateProjectileStateNumerical(dt);
    }

    // Update state of defence projectiles
    for (unsigned int i = 0; i < defenceProjectiles.size(); ++i)
    {
      defenceProjectiles[i]->updateProjectileStateNumerical(dt);
      defenceProjectiles[i]->updateRadOfDefenceProjectiles(global.t);
    }

    checkIfDefenceProjectileExpired();
    checkIfProjectileHasHitDefence();

    // Check if a boat is hit by attack projectile
    int hits = ai->checkIfBoatHit(boats, projectilesInAir);

    // Check if a Island is hit by attack projectile or boat
    hits += island->checkIfIslandIsHit(projectilesInAir, boats);

    // Score of destroyed boats
    this->global.score += hits;

    // Destroy projectiles landed in water
    checkIfProjectileLandedInWater();

    lastT = global.t;

    // Check if Game Over
    checkIfGameOver();

    // OSD update
    dt = global.t - global.lastFrameRateT;
    if (dt > global.frameRateInterval) {
      global.frameRate = global.frames / dt;
      global.lastFrameRateT = global.t;
      global.frames = 0;
    }
  }
}

void GameEngine::display(std::vector<GLuint> &textures)
{
  int err;
  this->textures = textures;

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (global.polygonMode) 
  {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }
  glEnable(GL_NORMALIZE);
  glEnable(GL_DEPTH_TEST);


  camera();
  
  global.frames++;

  // Check for gl errors
  if ((err = glGetError()) != GL_NO_ERROR)
    fprintf(stderr, "Error: %s\n", gluErrorString(err));
}

void GameEngine::camera()
{
  glPushMatrix();
    glPushMatrix();
      glTranslatef(-3, 2.5, 0.0);
      
      drawHealthBar(island->getHealth(), MAX_I_HEALTH, 1.0, 1.0, 0.0);
    glPopMatrix();
    glTranslatef(mapMove.x, mapMove.y, mapMove.z);
    glRotatef(mapMove.xR, 1, 0, 0);
    glRotatef(mapMove.yR, 0, 1, 0);
    glRotatef(-island->getPhi(), 0, 1, 0);
    float lighting[4] = {1, 2, 1, 0.2};
    float spec[4] = {1, 1, 1, 0};
    float shine = 50;
          
    glLightfv(GL_LIGHT0, GL_POSITION, lighting);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(shine));
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);

    glPushMatrix();
      glTranslatef(0, 2, 0);
      drawBox(global.text, this->textures);
    glPopMatrix();
    drawAxes(2.0);
    island->drawIsland(global.light, global.text, 20, textures[4]);
    for (unsigned int i = 0; i < boats.size(); ++i)
    {
      boats[i]->drawBoat(global.light, 20);
    }
    for (unsigned int i = 0; i < defenceProjectiles.size(); ++i)
    {
      defenceProjectiles[i]->drawDefence(global.light);
    }
    for (unsigned int i = 0; i < projectilesInAir.size(); ++i)
    {
      projectilesInAir[i]->drawProjectile(global.light, 20);
      projectilesInAir[i]->drawTrajectoryNumerical(1, 1, 1);
    }
    glPushMatrix();
      if(global.normTan) 
      {
        wave.drawNormalsTangents();
      }
      wave.drawWave(global.light, global.tess);
    glPopMatrix();
    
    displayOSD(global.frameRate, global.tess);
    displayGameStats(global.score);
    if(global.gameOver)
    {
      displayResume();
    }
        
  glPopMatrix();
}

void GameEngine::checkCannon() 
{
  if(cannonR.p.increase)
  {
    island->increasePower();
  }
  else if(cannonR.p.decrease) 
  {
    island->decreasePower();
  }
  else if(cannonR.v.up) 
  {
    island->rotateCannonUp();
  }
  else if(cannonR.v.down)
  {
    island->rotateCannonDown();
  }
  else if(cannonR.h.left)
  {
    island->rotateCannonLeft();
  }
  else if(cannonR.h.right)
  {
    island->rotateCannonRight();
  }
}

void GameEngine::keyboardCB(unsigned char key, int x, int y)
{
  if(!global.gameOver)
  {
    switch (key)
    {
    case 27:
      exit(EXIT_SUCCESS);
      break;
    case 'p': 
      global.polygonMode = !global.polygonMode;
      break;
    case 'l':
      global.light = !global.light;
      break;
    case 'n':
      global.normTan = !global.normTan;
      break;
    case 't':
      global.text = !global.text;
      break;
    case 'h':
      global.gameOver = !global.gameOver;
      global.pauseTime = glutGet(GLUT_ELAPSED_TIME) / (float) milli;
      break;
    case '+':
      global.tess *= 2;
      break;
    case '-':
      if(global.tess > 4)
      {
        global.tess /= 2;
      }
      break;
    case 'v':
      island->fireDefence(global.t, defenceProjectiles);
      break;
    case ' ':
      island->fireCannon(global.t, projectilesInAir);
      break;
    case 'w':
      cannonR.p.increase = true;
      break;
    case 's':
      cannonR.p.decrease = true;
      break;
    case 'a':
      cannonR.v.up = true;
      break;
    case 'd':
      cannonR.v.down = true;
      break;
    case 55:
      mapMove.x -= 0.2;
      break;
    case 52:
      mapMove.x += 0.2;
      break;
    case 56:
      mapMove.y -= 0.2;
      break;
    case 53:
      mapMove.y += 0.2;
      break;
    case 57:
      mapMove.z -= 0.2;
      break;
    case 54:
      mapMove.z += 0.2;
      break;
    case 48:
      mapMove.yR -= 0.2;
      break;
    case 49:
      mapMove.yR += 0.2;
      break;
    case 50:
      mapMove.xR -= 0.2;
      break;
    case 51:
      mapMove.xR += 0.2;
      break;
    default:
      break;
    }
  }
  else
  {
    switch (key)
    {
    case 27:
      exit(EXIT_SUCCESS);
      break;
    case 'h':
      global.gameOver = !global.gameOver;
      global.startTime = glutGet(GLUT_ELAPSED_TIME) / (float) milli;
      global.delayTime += global.startTime - global.pauseTime;
      break;
    case 55:
      mapMove.x -= 0.2;
      break;
    case 52:
      mapMove.x += 0.2;
      break;
    case 56:
      mapMove.y -= 0.2;
      break;
    case 53:
      mapMove.y += 0.2;
      break;
    case 57:
      mapMove.z -= 0.2;
      break;
    case 54:
      mapMove.z += 0.2;
      break;
    case 48:
      mapMove.yR -= 0.2;
      break;
    case 49:
      mapMove.yR += 0.2;
      break;
    case 50:
      mapMove.xR -= 0.2;
      break;
    case 51:
      mapMove.xR += 0.2;
      break;
    default:
      break;
    }
  }
}

void GameEngine::keyboardUpCB(unsigned char key, int x, int y)
{
  switch(key)
  {
    case 'w':
      cannonR.p.increase = false;
      break;
    case 's':
      cannonR.p.decrease = false;
      break;
    case 'a':
      cannonR.v.up = false;
      break;
    case 'd':
      cannonR.v.down = false;
      break;
    default:
      break;
  }
}

void GameEngine::specialKCB(int key, int x, int y) 
{
  if(!global.gameOver)
  {
    switch(key)
    {
      case GLUT_KEY_LEFT:
        cannonR.h.left = true;
        break;
      case GLUT_KEY_RIGHT:
        cannonR.h.right = true;
        break;
      default:
        break;
    }
  }
}

void GameEngine::specialKUpCB(int key, int x, int y) 
{
  switch(key)
  {
    case GLUT_KEY_LEFT:
      cannonR.h.left = false;
      break;
    case GLUT_KEY_RIGHT:
      cannonR.h.right = false;
      break;
    default:
      break;
  }
}