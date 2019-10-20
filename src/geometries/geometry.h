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
    virtual ~Geometry() = default;

    virtual bool intersect(const Ray& ray, IntersectionInfo& info) = 0;

};

class Plane : public Geometry
{
public:
    Plane() = default;
    Plane(double y): y_(y) {}
    ~Plane() = default;


    bool intersect(const Ray& ray, IntersectionInfo& info) override;

public:
    double y_; // the plane will always be || XZ plane
};

class Sphere : public Geometry
{
public:
    Sphere() = default;
    Sphere(const Vector& center, float radius)
    : center_(center)
    , radius_(radius) {}
    ~Sphere() = default;

    bool intersect(const Ray& ray, IntersectionInfo& info) override;
private:
    Vector center_;
    float radius_;
};

class Cube : public Geometry
{
public:
    Cube() = default;
    Cube(const Vector& center, float halfSide)
    : center_(center)
    , halfSide_(halfSide) {}
    ~Cube() = default;

    bool intersect(const Ray& ray, IntersectionInfo& info) override;
    bool intersectSide(double level, double start, double dir, const Ray& ray, const Vector& normal, IntersectionInfo& info);

private:
    Vector center_;
    float halfSide_;
};

class CsgOp: public Geometry
{
    void findAllIntersections(Ray ray, Geometry* geom, std::vector<IntersectionInfo>& ips);
public:
    CsgOp() = default;
    CsgOp(std::unique_ptr<Geometry>& left, std::unique_ptr<Geometry>& right);
    std::unique_ptr<Geometry> left_, right_;
    virtual bool boolOp(bool inA, bool inB) = 0;

    bool intersect(const Ray& ray, IntersectionInfo& info);
};

class CsgAnd: public CsgOp {
public:
    CsgAnd() = default;
    CsgAnd(std::unique_ptr<Geometry>& left, std::unique_ptr<Geometry>& right) : CsgOp(left, right) {}
    bool boolOp(bool inA, bool inB) { return inA && inB; }
};

class CsgPlus: public CsgOp {
public:
    CsgPlus() = default;
    CsgPlus(std::unique_ptr<Geometry>& left, std::unique_ptr<Geometry>& right) : CsgOp(left, right) {}
    bool boolOp(bool inA, bool inB) { return inA || inB; }
};

class CsgMinus: public CsgOp {
public:
    CsgMinus() = default;
    CsgMinus(std::unique_ptr<Geometry>& left, std::unique_ptr<Geometry>& right) : CsgOp(left, right) {}
    bool boolOp(bool inA, bool inB) { return inA && !inB; }
};



#endif // __GEOMETRY_H__
