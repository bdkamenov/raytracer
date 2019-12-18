/**
 * @File geometry.h
 * @Brief Contains declarations of geometry primitives.
 */
#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <memory>
#include <vector>

#include "maths/vector.h"
#include "maths/ray.h"


class Geometry;
struct IntersectionInfo
{
    Vector ip_;
    Vector normal_;
    double distance_;
    double u_, v_;   // u v coords used for texturing
    Geometry* geom_;
};

class Geometry
{
public:
    __device__ virtual ~Geometry() = default;

    __device__ virtual bool intersect(const Ray& ray, IntersectionInfo& info) = 0;

};

class Plane : public Geometry
{
public:
    __device__ Plane() = default;
    __device__ Plane(double y): y_(y) {}
    __device__ ~Plane() = default;


    __device__ bool intersect(const Ray& ray, IntersectionInfo& info) override;

public:
    double y_; // the plane will always be || XZ plane
};

class Sphere : public Geometry
{
public:
    __device__ Sphere() = default;
    __device__ Sphere(const Vector& center, float radius)
    : center_(center)
    , radius_(radius) {}
    __device__ ~Sphere() = default;

    __device__ bool intersect(const Ray& ray, IntersectionInfo& info) override;
private:
    Vector center_;
    float radius_;
};

class Cube : public Geometry
{
public:
    __device__ Cube() = default;
    __device__ Cube(const Vector& center, float halfSide)
    : center_(center)
    , halfSide_(halfSide) {}
    __device__ ~Cube() = default;

    __device__ bool intersect(const Ray& ray, IntersectionInfo& info) override;
    __device__ bool intersectSide(double level, double start, double dir, const Ray& ray, const Vector& normal, IntersectionInfo& info);

private:
    Vector center_;
    float halfSide_;
};

class CsgOp: public Geometry
{
    void findAllIntersections(Ray ray, Geometry* geom, std::vector<IntersectionInfo>& ips);
public:
    __device__ CsgOp() = default;
    __device__ CsgOp(std::unique_ptr<Geometry>& left, std::unique_ptr<Geometry>& right);
    std::unique_ptr<Geometry> left_, right_;
    virtual bool boolOp(bool inA, bool inB) = 0;

    bool intersect(const Ray& ray, IntersectionInfo& info);
};

class CsgAnd: public CsgOp {
public:
    __device__ CsgAnd() = default;
    __device__ CsgAnd(std::unique_ptr<Geometry>& left, std::unique_ptr<Geometry>& right) : CsgOp(left, right) {}
    __device__ bool boolOp(bool inA, bool inB) { return inA && inB; }
};

class CsgPlus: public CsgOp {
public:
    __device__ CsgPlus() = default;
    __device__ CsgPlus(std::unique_ptr<Geometry>& left, std::unique_ptr<Geometry>& right) : CsgOp(left, right) {}
    __device__ bool boolOp(bool inA, bool inB) { return inA || inB; }
};

class CsgMinus: public CsgOp {
public:
    __device__ CsgMinus() = default;
    __device__ CsgMinus(std::unique_ptr<Geometry>& left, std::unique_ptr<Geometry>& right) : CsgOp(left, right) {}
    __device__ bool boolOp(bool inA, bool inB) { return inA && !inB; }
};



#endif // __GEOMETRY_H__
