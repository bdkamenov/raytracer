/**
 * @File shading.cpp
 * @Brief Contains implementations of shader classes
 */
#include "shading.h"

extern Vector lightPosition;
extern double lightIntensity;
extern Color ambientLight;

bool visibilityCheck(const Vector& start, const Vector& end);

double getLightContribution(const IntersectionInfo& info)  // calculates the amount of light
{                                                          // that gets to the point

    double distanceToLightSqr = (info.ip_ - lightPosition).lengthSqr();  // the distance to the light ^2

    if(!visibilityCheck(info.ip_ + info.normal_ * 1e-6, lightPosition))
        return 0;
    else
        return lightIntensity/distanceToLightSqr; // by taking away from light the intensity reduces with 1/dist^2


}

Color Lambert::shade(const Ray& ray, IntersectionInfo& info)
{
    Color diffuse = texture_ ? texture_->sample(info) : color_;

    Vector v1 = info.normal_;
    Vector v2 = lightPosition - info.ip_;
    v2.normalize();

    double lambertCoeff = dot(v1, v2);              // take the angle between the light and the normal and the light

    return ambientLight * diffuse + // used for better looking shadow
           diffuse * lambertCoeff * getLightContribution(info);
}

Color Phong::shade(const Ray &ray, IntersectionInfo &info)
{
    Color diffuse = texture_ ? texture_->sample(info) : color_;

    Vector v1 = info.normal_;
    Vector v2 = lightPosition - info.ip_;
    v2.normalize();
    double fromLight = getLightContribution(info);

    double lambertCoeff = dot(v1, v2);              // take the angle between the light and the normal and the light

    Vector r = reflect(info.ip_ - lightPosition, info.normal_);
    Vector toCamera = -ray.dir_;
    double cosGamma = dot(toCamera, r);
    double phongCoeff = cosGamma > 0 ? pow(cosGamma, specularExponent_) :  0;

    return ambientLight * diffuse +
           diffuse * (lambertCoeff * fromLight + phongCoeff * specularMultiplier_ * fromLight);
}

Color CheckerTexture::sample(const IntersectionInfo &info)
{
    int x = (int) floor(info.u_ * scaling_ / 7);
    int y = (int) floor(info.v_ * scaling_ / 7);

    return  ((x + y) % 2 == 0) ? color1_ : color2_;
}
