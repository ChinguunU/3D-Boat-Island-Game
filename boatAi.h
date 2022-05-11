#ifndef BOAT_AI
#define BOAT_AI

#include <cmath>
#include <vector>
#include <random>

#include "projectileState.h"
#include "boat.h"
#include "vec3f.h"
#include "island.h"
#include "gameMeasure.h"
#include "utils/radDegConv.h"



class BoatAi {
public:
    BoatAi(float t);
    void moveForward(std::vector<Boat*>& boats);
    void checkToShoot(float t, std::vector<Boat*>& boats, std::vector<ProjectileState*>& projectilesInAir);
    void checkToGenerateBoat(float t, std::vector<Boat*>& boats);
    int checkIfBoatHit(std::vector<Boat*>& boats, std::vector<ProjectileState*>& projectilesInAir);
private:
    float boatGeneratedT = 0;
};

#endif // BOAT_AI