/**
 * @File camera.cpp
 * @Brief Implementation of the raytracing camera.
 */
#include "camera.h"

#include "utils/util.h"
#include "utils/constants.h"


void Camera::frameBegin()
{
    double x2d = _aspectRatio, y2d = +1;

    double wantedAngle = toRadians(_fov/2);
    double wantedLength = tan(wantedAngle);
    double hypotLength = sqrt(sqr(_aspectRatio) + sqr(1.0));
    double scaleFactor = wantedLength / hypotLength;

    x2d *= scaleFactor*1.5;
    y2d *= scaleFactor*1.5;

    _topLeft = Vector(-x2d, y2d, 1);
    _topRight = Vector(x2d, y2d, 1);
    _bottomLeft = Vector(-x2d, -y2d, 1);

    _rotation = rotationAroundZ(toRadians(_roll)) *
                rotationAroundX(toRadians(_pitch)) *
                rotationAroundY(toRadians(_yaw));

    _topLeft    *= _rotation;
    _topRight   *= _rotation;
    _bottomLeft *= _rotation;

    _topLeft    += _position;  // the milimeter list aka the view matrix
    _topRight   += _position;  // is moved around the camera with it
    _bottomLeft += _position;
}

Ray Camera::getScreenRay(double xScreen, double yScreen)
{
    // the beginning of the view matrix shown in lecture 4
    Vector throughPoint = // startPoint + diff between beg end    * num between 0 1
                                _topLeft + (_topRight - _topLeft) * (xScreen / RESX)
                                         + (_bottomLeft - _topLeft) * (yScreen / RESY);

    Ray ray;
    ray._dir = throughPoint - _position;
    ray._dir.normalize(); // directions must be normalized
    ray._start = _position;
    return ray;
}

