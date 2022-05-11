#include "wave.h"

Wave::Wave(float dx, float dz, float amplitudeX, float amplitudeZ, float kx, float kz, float wx, float wz) 
{
    this->dx = dx;
    this->dz = dz;
    this->amplitudeX = amplitudeX;
    this->amplitudeZ = amplitudeZ;
    this->kx = kx;
    this->kz = kz;
    this->wx = wx;
    this->wz = wz;
}

float Wave::getYCor(float x, float z, float t) 
{
    return getYCor({amplitudeX, kx, 0, wx}, x, z, t) + 
            getYCor({amplitudeZ, 0, kz, wz}, x, z, t) + 
            getYCor({0.1, 0.7, 0.5, 0.1}, x, z, t);
}

float Wave::getDerivativeX(float x, float z, float t)
{
    return getDerivativeX({amplitudeX, kx, 0, wx}, x, z, t) + 
            getDerivativeX({amplitudeZ, 0, kz, wz}, x, z, t) + 
            getDerivativeX({0.1, 0.7, 0.5, 0.1}, x, z, t);
}

float Wave::getDerivativeZ(float x, float z, float t)
{
    return getDerivativeZ({amplitudeX, kx, 0, wx}, x, z, t) + 
            getDerivativeZ({amplitudeZ, 0, kz, wz}, x, z, t) + 
            getDerivativeZ({0.1, 0.7, 0.5, 0.1}, x, z, t);
}

float Wave::getYCor(SineWaveConst sw, float x, float z, float t)
{
    return sinf(M_PI * (2 * x * sw.kx + 2 * z * sw.kz + t * sw.w)) * sw.amplitude;
}

float Wave::getDerivativeX(SineWaveConst sw, float x, float z, float t)
{
  return (2 * M_PI * sw.kx) *
          cosf(M_PI * (2 * x * sw.kx + 2 * z * sw.kz + t * sw.w)) *
          sw.amplitude;
}

float Wave::getDerivativeZ(SineWaveConst sw, float x, float z, float t)
{
  return (2 * M_PI * sw.kz) *
          cosf(M_PI * (2 * x * sw.kx + 2 * z * sw.kz + t * sw.w)) *
          sw.amplitude;
}

void Wave::calcSineWave(SineWaveConst sw, float x, float z, float t, float *y, bool dvs, float *dydx, float *dydz)
{
    *y = getYCor(sw, x, z, t);
    if(dvs) 
    {
        *dydx = getDerivativeX(sw, x, z, t);
        *dydz = getDerivativeZ(sw, x, z, t);
    }
}

void Wave::plotSineWave(SineWaveConst sw, int tess, std::vector<Vec3f> &coordinates, std::vector<Vec3f> &normals, float t)
{
    float x, y, z, dydx, dydz;
    float zStep = this->dz / tess;
    float xStep = this->dx / tess;
    for (int j = 0; j < tess; j++)  
    {
        x = -this->dx / 2.0 + (float) j * xStep;
        for (int i = 0; i <= tess; i++) 
        {
            
            z = -this->dz / 2.0 + (float) i * zStep;
            calcSineWave(sw, x, z, t, &y, true, &dydx, &dydz);
            coordinates.push_back({x, y, z});
            normals.push_back({-dydx, 1, -dydz});
        
            calcSineWave(sw, x + xStep, z, t, &y, true, &dydx, &dydz);
            coordinates.push_back({x + xStep, y, z});
            normals.push_back({-dydx, 1, -dydz});
        }            
    }
}

void Wave::updateWave(int tess, float t)
{
    std::vector<Vec3f> coordinatesX;
    std::vector<Vec3f> normalsX;
    SineWaveConst swX = {amplitudeX, kx, 0, wx};

    std::vector<Vec3f> coordinatesZ;
    std::vector<Vec3f> normalsZ;
    SineWaveConst swZ = {amplitudeZ, 0, kz, wz};

    std::vector<Vec3f> coordinatesXZ;
    std::vector<Vec3f> normalsXZ;
    SineWaveConst swXZ = {0.1, 0.7, 0.5, 0.1};

    plotSineWave(swX, tess, coordinatesX, normalsX, t);
    plotSineWave(swZ, tess, coordinatesZ, normalsZ, t);
    plotSineWave(swXZ, tess, coordinatesXZ, normalsXZ, t);

    
    this->coordinates.clear();
    this->normals.clear();

    for(unsigned int i = 0; i < normalsZ.size(); ++i) 
    {
        this->coordinates.push_back({coordinatesZ[i].x, 
                                coordinatesZ[i].y + coordinatesX[i].y + coordinatesXZ[i].y, 
                                coordinatesZ[i].z});

        this->normals.push_back({normalsZ[i].x + normalsX[i].x + normalsXZ[i].x, 
                            normalsZ[i].y + normalsX[i].y + normalsXZ[i].y, 
                            normalsZ[i].z + normalsX[i].z + normalsXZ[i].z});
    }
}

void Wave::drawWave(bool light, int tess)
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    if(light)
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);

        float ambDiff[4] = {0, 0.5, 1, 0.5};
        
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, ambDiff);
    }  
       
    unsigned int index = 0;
    glColor3f(1, 1, 1);
    for (int j = 0; j < tess; ++j)  
    {
        glBegin(GL_TRIANGLE_STRIP);
        for (int i = 0; i <= tess && index < normals.size(); i++) 
        {
            
            for(int a = 0; a < 2; ++a)
            {
               
                glNormal3f(normals[index].x, normals[index].y, normals[index].z);
                glVertex3f(coordinates[index].x, coordinates[index].y, coordinates[index].z);
                
                index++;
            }
        }
        glEnd();                 
    }
    glDisable(GL_LIGHTING);
    glDisable(GL_LIGHT0);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
}

void Wave::drawNormalsTangents() 
{
    for(unsigned int i = 0; i < coordinates.size(); ++i) 
    {
        // Normal
        drawVector(coordinates[i].x, coordinates[i].y, coordinates[i].z, 
        normals[i].x, normals[i].y, normals[i].z, -1, true, 1, 1, 0);
        
        // x tangent
        drawVector(coordinates[i].x, coordinates[i].y, coordinates[i].z, 
        1, -normals[i].x, 0, -1, true, 0, 1, 1);
        
        // z tangent
        drawVector(coordinates[i].x, coordinates[i].y, coordinates[i].z, 
        0, -normals[i].z, 1, -1, true, 1, 0, 1);        
    }                  
}