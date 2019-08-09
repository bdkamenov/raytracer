/**
 * @File geometry.cpp
 * @Brief Contains implementations of geometry primitives' intersection methods.
 */

#include "geometry.h"
#include "utils/constants.h"

bool Plane::intersect(const Ray& ray, IntersectionInfo& info)
{
    if (ray._start._y > _y && ray._dir._y >= 0)
        return false;
    if (ray._start._y < _y && ray._dir._y <= 0)
        return false;

    // else we can hit the plane and checking that by calculating how long the
    // vector should be to hit the plane instead of calculating the intersection of the plane
    double scaleFactor = (_y - ray._start._y)/ray._dir._y;
    info._ip = ray._start + ray._dir * scaleFactor;
    info._distance = scaleFactor;

    // should be _|_ to the plane and when the plane is || XZ -> normal = Y so
    // if we watch from upside the normal is +1 else is -1
    info._normal = Vector(0., ray._start._y > _y ? 1. : -1., 0.);
    info._u = info._ip._x;
    info._v = info._ip._z;

    return true;
}
