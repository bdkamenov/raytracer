/**
 * @File geometry.h
 * @Brief Contains declarations of geometry primitives.
 */
#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <memory>

#include "maths/vector.h"
#include "maths/ray.h"

struct IntersectionInfo
{
    Vector _ip;
    Vector _normal;
    double _distance;
    double _u, _v;   // u v coords used for texturing
};

class Geometry
{
public:

    virtual ~Geometry() = default;

    virtual bool intersect(const Ray& ray, IntersectionInfo& info) = 0;

};


class Plane : public Geometry
{
public:
    Plane(double y): _y(y) {}
    ~Plane() = default;


    bool intersect(const Ray& ray, IntersectionInfo& info) override;


    double _y; // the plane will always be || XZ plane
};


#endif // __GEOMETRY_H__
