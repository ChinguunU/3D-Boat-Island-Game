#include "boat.h"

Boat::Boat(Vec3f boatPos)
{
    cannon = new Cannon(0, 0);
    this->boatPos = boatPos;
}

Boat::~Boat()
{
    delete cannon;
}

void drawGrid(float x, float y, int tess) {
    glPushMatrix();
        glColor3f(1, 0, 0);
        glTranslatef(-(float)x / 2.0, -(float)y / 2.0, 0);
        
        for(int i = 0; i < tess; ++i) 
        {
        
            float iStep = (float)i * x / (float)tess;
            for(int j = 0; j < tess; ++j)
            {
                float jStep = (float)j * y / (float)tess;
                glBegin(GL_TRIANGLE_STRIP);
                glNormal3f(0, 0, 1);
                glVertex3f(iStep, jStep, 0);
                glNormal3f(0, 0, 1);
                glVertex3f(iStep + x / (float)tess, jStep, 0);
                glNormal3f(0, 0, 1);
                glVertex3f(iStep, jStep + y / (float)tess, 0);
                glNormal3f(0, 0, 1);
                glVertex3f(iStep + x / (float)tess, jStep + y / (float)tess, 0);
                glEnd();
            }
        }
    glPopMatrix();
}

void Boat::drawHull(float length, float height, float width, int tess) 
{
    glPushMatrix();
        glPushMatrix();
            glTranslatef(0, 0, width / 2.0);
            drawGrid(length, height, tess);
        glPopMatrix();
        glPushMatrix();
            glRotatef(90, 0, 1, 0);
            glTranslatef(0, 0, length / 2.0);
            drawGrid(width, height, tess);
        glPopMatrix();
        glPushMatrix();
            glRotatef(180, 0, 1, 0);
            glTranslatef(0, 0, width / 2.0);
            drawGrid(length, height, tess);
        glPopMatrix();
        glPushMatrix();
            glRotatef(270, 0, 1, 0);
            glTranslatef(0, 0, length / 2.0);
            drawGrid(width, height, tess);
        glPopMatrix();
        glPushMatrix();
            glRotatef(90, 1, 0, 0);
            glTranslatef(0, 0, height / 2.0);
            drawGrid(length, width, tess);
        glPopMatrix();
        glPushMatrix();
            glRotatef(270, 1, 0, 0);
            glTranslatef(0, 0, height / 2.0);
            drawGrid(length, width, tess);
        glPopMatrix();
    glPopMatrix();
}

void Boat::drawBoat(bool light, int tess) 
{
    glPushMatrix();
        if(light)
        {
            glEnable(GL_LIGHTING);
            glEnable(GL_LIGHT0);
        
            float ambDiff[4] = {1, 0, 0, 0.5};
        
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff);
        }
        glTranslatef(boatPos.x, boatPos.y, boatPos.z);
        glRotatef(radToDeg(boatZR), 0, 0, 1);
        glRotatef(radToDeg(-boatXR), 1, 0, 0);
        glRotatef(boatYR, 0, 1, 0);
        glScalef(B_SCALE, B_SCALE, B_SCALE);
        glColor3f(1, 0, 0);
        drawHull(B_L, B_H, B_W, tess);
        glPushMatrix();
            glTranslatef(0, Y_DISTANCE, 0);
            glPushMatrix();
                glTranslatef(-0.25, 0, 0);
                drawHull(0.4, 0.4, 0.4, tess);
            glPopMatrix();
            glTranslatef(X_DISTANCE, 0, 0);
            glRotatef(cannon->getCannonAngle(), 0, 0, 1);
            
            // Cannon
            gluSphere(gluNewQuadric(), B_C_RAD, tess, tess);
            glRotatef(90, 0, 1, 0);
            gluCylinder(gluNewQuadric(), B_CANNON_WIDTH, B_CANNON_WIDTH, B_CANNON_L, tess, tess);
        glPopMatrix();
        glDisable(GL_LIGHTING);
        glDisable(GL_LIGHT0);
        drawAxes(2);
    glPopMatrix();
}

void Boat::updateBoatState(Wave wave, float t) 
{
    boatPos.y = wave.getYCor(boatPos.x, boatPos.z, t);
    boatZR = atanf(wave.getDerivativeX(boatPos.x, boatPos.z, t));
    boatXR = atanf(wave.getDerivativeZ(boatPos.x, boatPos.z, t));
    cannon->updateBoatInitCannonPos(boatPos.x, boatPos.z, boatPos.y , boatXR, boatZR, boatYR); 
}

void Boat::fireCannon(float t, std::vector<ProjectileState*>& projectilesInAir)
{
    cannon->fire(t, projectilesInAir);
}

bool Boat::checkIfBoatIsHit(std::vector<ProjectileState*>& projectilesInAir)
{
    bool hit = false;
    unsigned int loop = 0;
    while(loop < projectilesInAir.size() && !hit)
    {
        Vec3f projPos = projectilesInAir[loop]->getPosition();
        float distance = sqrtf(powf(projPos.x - boatPos.x, 2) + 
                            powf(projPos.y - boatPos.y, 2) + 
                            powf(projPos.z - boatPos.z, 2));
        if(distance < PROJ_RAD + boatHitRadius)
        {

            delete projectilesInAir[loop];
            projectilesInAir.erase(projectilesInAir.begin() + loop);
            hit = true;
        }
        else
        {
            ++loop;
        }
    }
    return hit;
}

void Boat::rotateCannonUp()
{
    cannon->setCannonAngle(cannon->getCannonAngle() + 1);
    if(cannon->getCannonAngle() >= 60)
    {
        up = false;
    }
}

void Boat::rotateCannonDown()
{
    cannon->setCannonAngle(cannon->getCannonAngle() - 1);
    if(cannon->getCannonAngle() <= 0)
    {
        up = true;
    }
}

void Boat::turnLeft()
{
    boatYR++;
}

void Boat::turnRight()
{
    boatYR--;
}

Vec3f Boat::getBoatPos()
{
    return boatPos;
}

void Boat::setBoatPos(Vec3f pos)
{
    boatPos = pos;
}

float Boat::getBoatYR()
{
    return boatYR;
}

void Boat::setBoatYR(float yr)
{
    boatYR = yr;
}

float Boat::getBoatHitRadius()
{
    return boatHitRadius;
}

Vec3f Boat::getBoatVelocity()
{
    return cannon->getVelocity();
}

bool Boat::isUp()
{
    return up;
}
