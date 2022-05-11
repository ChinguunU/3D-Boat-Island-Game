#include "cannon.h"

Cannon::Cannon(float cannonAngle, float cannonRotation)
{
    this->cannonAngle = cannonAngle;
    this->cannonRotation = cannonRotation;
    pFireSpeed = 1;
    lastFiredT = 0.0;
    lastDefendT = 0.0;
}

Cannon::~Cannon()
{
    delete projectile;
}

float Cannon::getCannonAngle()
{
    return cannonAngle;
}

void Cannon::setCannonAngle(float angle)
{
    cannonAngle = angle;
}

float Cannon::getCannonRotation()
{
    return cannonRotation;
}

void Cannon::setCannonRotation(float rotation)
{
    this->cannonRotation = rotation;
}

float Cannon::getFireSpeed()
{
    return this->pFireSpeed;
}

void Cannon::setFireSpeed(float speed)
{
    if(speed >= 0) 
    {
        this->pFireSpeed = speed;
    }
} 

float Cannon::getLastFiredT()
{
    return lastFiredT;
}

void Cannon::setLastFiredT(float t)
{
    lastFiredT = t;
}

float Cannon::getLastDefendT()
{
    return lastDefendT;
}

void Cannon::setLastDefendT(float t)
{
    lastDefendT = t;
}

Vec3f Cannon::getVelocity()
{
    return projectile->getVelocity();
}

void Cannon::fire(float t, std::vector<ProjectileState*>& projectilesInAir)
{
    if(t - lastFiredT > 1)
    {
        projectilesInAir.push_back(new ProjectileState(*projectile));
        lastFiredT = t;
    }
}

void Cannon::fireDefence(float t, std::vector<ProjectileState*>& defenceProjectiles)
{
    if(t - lastDefendT > 3)
    {
        defenceProjectiles.push_back(new ProjectileState(*projectile));
        defenceProjectiles[defenceProjectiles.size() - 1]->setDefenceStartT(t);
        lastDefendT = t;
    }
}

void Cannon::updateBoatInitCannonPos(float x, float z, float y, float boatXR, float boatZR, float boatYR) 
{
    Vec3f temp;

    temp.x = (X_DISTANCE + B_CANNON_L * cosf(degToRad(cannonAngle))) * 
            cosf(degToRad(-boatYR)) *
            cosf(-boatZR) +
            ((Y_DISTANCE + B_CANNON_L * sinf(degToRad(cannonAngle))) * 
            cosf(boatXR) + 
            (X_DISTANCE + B_CANNON_L * cosf(degToRad(cannonAngle))) * 
            sinf(degToRad(-boatYR)) *
            sinf(boatXR)) *
            sinf(-boatZR);

    temp.z = -(Y_DISTANCE + B_CANNON_L * sinf(degToRad(cannonAngle))) *
            sin(boatXR) + 
            (X_DISTANCE + B_CANNON_L * cosf(degToRad(cannonAngle))) * 
            sinf(degToRad(-boatYR)) *
            cosf(boatXR);

    temp.y = -(X_DISTANCE + B_CANNON_L * cosf(degToRad(cannonAngle))) * 
            cosf(degToRad(-boatYR)) *
            sinf(-boatZR) +
            ((Y_DISTANCE + B_CANNON_L * sinf(degToRad(cannonAngle))) * 
            cosf(boatXR) + 
            (X_DISTANCE + B_CANNON_L * cosf(degToRad(cannonAngle))) * 
            sinf(degToRad(-boatYR)) *
            sinf(boatXR)) *
            cosf(-boatZR); 

    Vec3f r;
    r.x = x + temp.x * B_SCALE;
    r.z = z + temp.z * B_SCALE;
    r.y = y + temp.y * B_SCALE;

    Vec3f v;
    v.x = cosf(degToRad(cannonAngle)) * 
            cosf(degToRad(-boatYR)) *
            cosf(-boatZR) +
            (sinf(degToRad(cannonAngle)) * 
            cosf(boatXR) + 
            cosf(degToRad(cannonAngle)) * 
            sinf(degToRad(-boatYR)) *
            sinf(boatXR)) *
            sinf(-boatZR);

    v.z = -sinf(degToRad(cannonAngle)) *
            sin(boatXR) + 
            cosf(degToRad(cannonAngle)) * 
            sinf(degToRad(-boatYR)) *
            cosf(boatXR);
    
    v.y = -cosf(degToRad(cannonAngle)) * 
            cosf(degToRad(-boatYR)) *
            sinf(-boatZR) +
            (sinf(degToRad(cannonAngle)) * 
            cosf(boatXR) + 
            cosf(degToRad(cannonAngle)) * 
            sinf(degToRad(-boatYR)) *
            sinf(boatXR)) *
            cosf(-boatZR);  
    
    projectile->setVelocity(v);
    projectile->setPosition(r);
}

void Cannon::updateIslandInitCannonPos()
{
    Vec3f v;
    v.x = cosf(degToRad(90 + cannonRotation)) * cosf(degToRad(-cannonAngle)) * pFireSpeed;
    v.z = -sinf(degToRad(90 + cannonRotation)) * cosf(degToRad(-cannonAngle)) * pFireSpeed;
    v.y = sinf(degToRad(-cannonAngle)) * pFireSpeed; 
    
    Vec3f r;
    r.x = v.x / pFireSpeed * CANNON_LENGTH * CANNON_SCALE;
    r.z = v.z / pFireSpeed * CANNON_LENGTH * CANNON_SCALE;
    r.y = v.y / pFireSpeed * CANNON_LENGTH * CANNON_SCALE + 
                        ABOVE_SEA_LEVEL + CANNON_SCALE * CANNON_ELEV;
    projectile->setVelocity(v);
    projectile->setPosition(r);
}

void Cannon::drawIslandCTrajectory()
{
    projectile->drawDirection(1, 0.7, 0.8);
    projectile->drawTrajectoryNumerical(1, 1, 0);
}