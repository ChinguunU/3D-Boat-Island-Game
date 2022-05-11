#ifndef WAVE
#define WAVE

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>
#include <vector>
#include <iostream>

#include "utils/drawVector.h"
#include "vec3f.h"

#define M_PI 3.14159265358979323846

typedef struct {float amplitude, kx, kz, w;} SineWaveConst;

class Wave
{
public:
  Wave(float dx, float dz, float amplitudeX, float amplitudeZ, float kx, float kz, float wx, float wz);
  float getYCor(float x, float z, float t);
  float getDerivativeX(float x, float z, float t);
  float getDerivativeZ(float x, float z, float t);
  float getYCor(SineWaveConst sw, float x, float z, float t);
  float getDerivativeX(SineWaveConst sw, float x, float z, float t);
  float getDerivativeZ(SineWaveConst sw, float x, float z, float t);
  void calcSineWave(SineWaveConst sw, float x, float z, float t, float *y, bool dvs, float *dydx, float *dydz);
  void plotSineWave(SineWaveConst sw, int tess, std::vector<Vec3f> &coordinates, std::vector<Vec3f> &normals, float t);
  void updateWave(int tess, float t);
  void drawWave(bool light, int tess);
  void drawNormalsTangents();
private:
  float amplitudeX, amplitudeZ, kx, kz, wx, wz;
  // Wave dimmensions
  float dx, dz;
  std::vector<Vec3f> coordinates;
  std::vector<Vec3f> normals;
};

#endif // WAVE
