#include "boatAi.h"

BoatAi::BoatAi(float t)
{
    boatGeneratedT = t;
}

void BoatAi::moveForward(std::vector<Boat*>& boats)
{
    for(unsigned int i = 0; i < boats.size(); ++i)
    {
        Vec3f oPos = boats[i]->getBoatPos();
        Vec3f nPos;
        // Equation to find x, z coordinate after a 0.1 step on the hypotenuse of (x,z)
        float common = 1 - 0.01 / (sqrtf(powf(oPos.z, 2) + powf(oPos.x, 2)));
        
        nPos.z = oPos.z * common;
        nPos.x = oPos.x * common;
        nPos.y = oPos.y;
        boats[i]->setBoatPos(nPos);
    }
}

void BoatAi::checkToShoot(float t, std::vector<Boat*>& boats, std::vector<ProjectileState*>& projectilesInAir)
{
    for(unsigned int i = 0; i < boats.size(); ++i)
    {
        Vec3f v = boats[i]->getBoatVelocity();
        float timeToLand = - 2 * v.y / G_F;
        float landingDist = sqrtf(powf(v.z, 2) + powf(v.x, 2)) * timeToLand;

        Vec3f p = boats[i]->getBoatPos();
        float distFromCenter = sqrtf(powf(p.x, 2) + 
                                powf(p.y, 2) + 
                                powf(p.z, 2));
        if(landingDist < distFromCenter + ISLAND_RAD && 
            landingDist >  distFromCenter - ISLAND_RAD)
        {
            boats[i]->fireCannon(t, projectilesInAir);
        }

        if(boats[i]->isUp())
        {
            boats[i]->rotateCannonUp();
        }
        else 
        {
            boats[i]->rotateCannonDown();
        }
    }
}

void BoatAi::checkToGenerateBoat(float t, std::vector<Boat*>& boats)
{
    if(t - boatGeneratedT > 5) 
    {
        std::random_device randomGen;
        std::uniform_int_distribution<int> cases(0, 1);
        std::uniform_int_distribution<int> boundary(0, 3);
        std::vector<float> bound = {-(float)GAME_DIM / 2.0, 
                                    (float)GAME_DIM / 2.0, 
                                    (float)GAME_DIM / 2.0, 
                                    -(float)GAME_DIM / 2.0};
        std::uniform_real_distribution<float> uniform_dist(-(float)GAME_DIM / 2.0, (float)GAME_DIM / 2.0);
        float random = uniform_dist(randomGen);
        int boolean = cases(randomGen);
        float randZ, randX; 
        if(boolean == 0)
        {
            randZ = random;
            randX = bound[boundary(randomGen)];
        } 
        else 
        {
            randZ = bound[boundary(randomGen)];
            randX = random;
        }
        float angle = asinf(randZ / (sqrtf(powf(randZ, 2) + powf(randX, 2))));
        if(randX > 0) 
        {
            angle = M_PI - angle;
        } 
        Boat *boat = new Boat({randX, 0.0, randZ});
        boat->setBoatYR(radToDeg(angle));
        boats.push_back(boat);
        boatGeneratedT = t;
    }
}

int BoatAi::checkIfBoatHit(std::vector<Boat*>& boats, std::vector<ProjectileState*>& projectilesInAir)
{
    int hits = 0;
    unsigned int loop = 0;
    while(loop < boats.size())
    {
        if(boats[loop]->checkIfBoatIsHit(projectilesInAir))
        {
            hits++;
            delete boats[loop];
            boats.erase(boats.begin() + loop);
        }
        else
        {
            ++loop;
        }
    }
    return hits;
}