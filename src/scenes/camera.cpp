/**
 * @File camera.cpp
 * @Brief Implementation of the raytracing camera.
 */
#include "camera.h"

#include "utils/util.h"
#include "utils/constants.h"


void Camera::frameBegin()
{
    double x2d = aspectRatio_, y2d = 1;

    double wantedAngle = toRadians(fov_/2);
    double wantedLength = tan(wantedAngle);
    double hypotLength = sqrt(sqr(aspectRatio_) + sqr(1.0));
    double scaleFactor = wantedLength / hypotLength;

    x2d *= scaleFactor*1.5;
    y2d *= scaleFactor*1.5;

    topLeft_ = Vector(-x2d, y2d, 1);
    topRight_ = Vector(x2d, y2d, 1);
    bottomLeft_ = Vector(-x2d, -y2d, 1);

    rotation_ = rotationAroundZ(toRadians(roll_)) *
                rotationAroundX(toRadians(pitch_)) *
                rotationAroundY(toRadians(yaw_));

    topLeft_    *= rotation_;
    topRight_   *= rotation_;
    bottomLeft_ *= rotation_;

    topLeft_    += position_;  // the milimeter list aka the view matrix
    topRight_   += position_;  // is moved around the camera with it
    bottomLeft_ += position_;
}

Ray Camera::getScreenRay(double xScreen, double yScreen)
{
    // the beginning of the view matrix shown in lecture 4
    Vector throughPoint = // startPoint + diff between beg end    * num between 0 1
                                topLeft_ + (topRight_ - topLeft_) * (xScreen / RESX)
                                         + (bottomLeft_ - topLeft_) * (yScreen / RESY);

    Ray ray;
    ray.dir_ = throughPoint - position_;
    ray.dir_.normalize(); // directions must be normalized
    ray.start_ = position_;
    return ray;
}

