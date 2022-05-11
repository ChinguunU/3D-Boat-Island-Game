#include "radDegConv.h"

float radToDeg(float rad)
{

    return (rad * 180.0) / M_PI;
}

float degToRad(float deg)
{

    return (deg * M_PI) / 180.0;
}
