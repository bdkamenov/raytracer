/**
 * @File shading.cpp
 * @Brief Contains implementations of shader classes
 */
#include "shading.h"

extern Vector lightPosition;
extern double lightIntensity;

Color CheckerShader::shade(const Ray& ray, IntersectionInfo& info)
{
    int x = (int) floor(info._u / _size);
    int y = (int) floor(info._v / _size);

    Color checkerColor = ((x + y) % 2 == 0) ? _color1 : _color2;

    Vector v1 = info._normal;
    Vector v2 = lightPosition - info._ip;
    double distToLightSqr = v2.lengthSqr();         // the distance to the light ^2
    v2.normalize();


    double lambertCoeff = dot(v1, v2);              // take the angle between the light and the normal and the light
    double attenuationCoeff = 1.0 / distToLightSqr; // by taking away from light the intensity reduces with 1/dist^2


    return checkerColor * lambertCoeff * attenuationCoeff * lightIntensity;
}

