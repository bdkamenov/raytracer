/**
 * @File camera.h
 * @Brief Contains declaration of the raytracing camera.
 */
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "maths/vector.h"
#include "maths/matrix.h"
#include "maths/ray.h"

class Camera
{
public:
    void frameBegin(); // called before rendering to setup everything and be ready to render (some caching might be done)
    Ray getScreenRay(double xScreen, double yScreen);


//private:
    Vector _position;
    double _yaw, _pitch, _roll;  // angles of rotation (in degrees)
    double _aspectRatio;       // width:height 4:3, 14:9, 16:9 .. etc
    double _fov;               // angle of view aka field of view (in degrees)

private:
    Vector _topLeft, _topRight, _bottomLeft;  // the view matrix(the milimeter paper) coords, inconvenient to be manually set
    Matrix _rotation;
};

// y is up/down, z is forward/backward and x is left/right as in Maya studio

#endif // __CAMERA_H__
