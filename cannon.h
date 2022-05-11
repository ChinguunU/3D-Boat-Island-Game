#ifndef CANNON
#define CANNON

#include <vector>
#include <iostream>

#include "vec3f.h"
#include "projectileState.h"
#include "islandMeasures.h"
#include "boatMeasure.h"
#include "utils/radDegConv.h"

// // Islands cannon length
// #define I_CANNON_LENGTH  0.25
// // The cannons distance from the x axis
// #define I_CANNONS_DIST_FROM_XAXIS 0.25
// // Scale ratio of the boat
// #define B_SCALE 0.11
// // Cannons length
// #define B_CANNON_LENGTH 0.5
// // The cannons distance from the x axis
// #define B_CANNONS_DIST_FROM_XAXIS 0.25
// // The cannon distance from the y axis
// #define B_CANNONS_DIST_FROM_YAXIS 0.5


class Cannon
{
public:
    Cannon(float cannonAngle, float cannonRotation);
    ~Cannon();
    float getCannonAngle();
    void setCannonAngle(float angle);
    float getCannonRotation();
    void setCannonRotation(float rotation);
    float getFireSpeed();
    void setFireSpeed(float speed); 
    float getLastFiredT();
    void setLastFiredT(float t);
    float getLastDefendT();
    void setLastDefendT(float t);
    Vec3f getVelocity();
    void fire(float t, std::vector<ProjectileState*>& projectilesInAir);
    void fireDefence( float t, std::vector<ProjectileState*>& defenceProjectiles);
    void updateBoatInitCannonPos(float x, float z, float y, float boatXR, float boatZR, float boatYR);
    void updateIslandInitCannonPos();
    void drawIslandCTrajectory();
private:
    float cannonAngle, cannonRotation, pFireSpeed, lastFiredT, lastDefendT;
    // bool fire, defend;
    ProjectilePtr projectile = new ProjectileState();
};

typedef Cannon* CannonPtr;

#endif // CANNON
