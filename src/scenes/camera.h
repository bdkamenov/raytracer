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
    Vector position_;
    double yaw_, pitch_, roll_;  // angles of rotation (in degrees)
    double aspectRatio_;       // width:height 4:3, 14:9, 16:9 .. etc
    double fov_;               // angle of view aka field of view (in degrees)

private:
    Vector topLeft_, topRight_, bottomLeft_;  // the view matrix(the milimeter paper) coords, inconvenient to be manually set
    Matrix rotation_;
};

// y is up/down, z is forward/backward and x is left/right as in Maya studio

#endif // __CAMERA_H__
