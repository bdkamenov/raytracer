/**
 * @File geometry.cpp
 * @Brief Contains implementations of geometry primitives' intersection methods.
 */

#include "geometry.h"
#include "utils/constants.h"

#include <algorithm>

bool Plane::intersect(const Ray& ray, IntersectionInfo& info)
{
    if (ray.start_.y_ > y_ && ray.dir_.y_ >= 0)
        return false;
    if (ray.start_.y_ < y_ && ray.dir_.y_ <= 0)
        return false;

    // else we can hit the plane and checking that by calculating how long the
    // vector should be to hit the plane instead of calculating the intersection of the plane
    double scaleFactor = (y_ - ray.start_.y_)/ray.dir_.y_;
    info.ip_ = ray.start_ + ray.dir_ * scaleFactor;
    info.distance_ = scaleFactor;

    // should be _|_ to the plane and when the plane is || XZ -> normal = Y so
    // if we watch from upside the normal is +1 else is -1
    info.normal_ = Vector(0., ray.start_.y_ > y_ ? 1. : -1., 0.);
    info.u_ = info.ip_.x_;
    info.v_ = info.ip_.z_;
    info.geom_ = this;

    return true;
}

bool Sphere::intersect(const Ray &ray, IntersectionInfo &info)
{
    // H = ray.start - center_
    // p^2 * dir.length^2 + p * (2 * dir · H) + (H.length^2 – R^2) = 0

    Vector H = ray.start_ - center_;
    double A = 1; // ray._dir.lengthSqr() -> this is 1 because the direction is normalized so the length is 1s
    double B = 2 * ray.dir_ * H;
    double C = H.lengthSqr() - radius_*radius_;

    double disc = B*B - 4*A*C;

    if (disc < 0) return false;

    double p1 = (-B - sqrt(disc)) / 2*A;
    double p2 = (-B + sqrt(disc)) / 2*A;

    double p;
    bool reverseNormal = false;
    if (p1 > 0)
    {
        p = p1;
    }
    else if (p2 > 0)
    {
        reverseNormal = true;
        p = p2;
    }
    else return false; // if both are negative the sphere is behind the camera

    info.distance_ = p;
    info.ip_ = ray.start_ + p * ray.dir_;
    info.normal_ = info.ip_ - center_;   // this is the continuation of the line from the center to the intersection point
    info.normal_.normalize();
    info.normal_ = reverseNormal ? -info.normal_ : info.normal_;

    Vector posRelative = info.ip_ - center_;    // used for spherical coordinates for u v coords
    info.u_ = atan2(posRelative.z_, posRelative.x_);
    info.v_ = asin(posRelative.y_ / radius_); // it can be between -1 1
    // we want to remap them from [(-PI...PI)x_(-PI/2...PI/2)] -> [(0..1)x_(0..1)] for easier texturing later
    info.u_ = (info.u_ + PI) / (2*PI);
    info.v_ = -(info.v_ + PI/2) / (PI);
    info.geom_ = this;

    return true;
}

bool Cube::intersect(const Ray& ray, IntersectionInfo& info)
{
        info.distance_ = INF;
        intersectSide(center_.x_ - halfSide_, ray.start_.x_, ray.dir_.x_, ray, Vector(-1, 0, 0), info);
        intersectSide(center_.x_ + halfSide_, ray.start_.x_, ray.dir_.x_, ray, Vector(+1, 0, 0), info);
        intersectSide(center_.y_ - halfSide_, ray.start_.y_, ray.dir_.y_, ray, Vector( 0,-1, 0), info);
        intersectSide(center_.y_ + halfSide_, ray.start_.y_, ray.dir_.y_, ray, Vector( 0,+1, 0), info);
        intersectSide(center_.z_ - halfSide_, ray.start_.z_, ray.dir_.z_, ray, Vector( 0, 0,-1), info);
        intersectSide(center_.z_ + halfSide_, ray.start_.z_, ray.dir_.z_, ray, Vector( 0, 0,+1), info);

        return (info.distance_ < INF);
}

bool Cube::intersectSide(double level, double start, double dir, const Ray& ray, const Vector& normal, IntersectionInfo& info)
{
    if (start > level && dir >= 0)
        return false;
    if (start < level && dir <= 0)
        return false;

    double scaleFactor = (level - start) / dir;
    Vector ip = ray.start_ + ray.dir_ * scaleFactor;
    if (ip.y_ > center_.y_ + halfSide_ + 1e-6) return false;
    if (ip.y_ < center_.y_ - halfSide_ - 1e-6) return false;

    if (ip.x_ > center_.x_ + halfSide_ + 1e-6) return false;
    if (ip.x_ < center_.x_ - halfSide_ - 1e-6) return false;

    if (ip.z_ > center_.z_ + halfSide_ + 1e-6) return false;
    if (ip.z_ < center_.z_ - halfSide_ - 1e-6) return false;

    double distance = scaleFactor;
    if (distance < info.distance_) {
        info.ip_ = ip;
        info.distance_ = distance;
        info.normal_ = normal;
        info.u_ = info.ip_.x_ + info.ip_.z_;
        info.v_ = info.ip_.y_;
        info.geom_ = this;
        return true;
    }
    return false;
}

void CsgOp::findAllIntersections(Ray ray, Geometry* geom, std::vector<IntersectionInfo>& ips)
{
    IntersectionInfo info;
    int counter = 30;      // used for safety, if we have a weird geometry it might get in an endless loop
    while (geom->intersect(ray, info) && counter-- > 0)
    {
        ips.push_back(info);
        ray.start_ = info.ip_ + ray.dir_ * 1e-6;
    }
    for (int i = 1; i < (int) ips.size(); i++)
        ips[i].distance_ = ips[i - 1].distance_ + ips[i].distance_ + 1e-6;
}

bool CsgOp::intersect(const Ray& ray, IntersectionInfo& info)
{
    std::vector<IntersectionInfo> leftIPs, rightIPs;
    findAllIntersections(ray, left_.get(), leftIPs);
    findAllIntersections(ray, right_.get(), rightIPs);

    bool inA = leftIPs.size() % 2 ? true : false;
    bool inB = rightIPs.size() % 2 ? true : false;

    std::vector<IntersectionInfo> allIPs;
    allIPs = leftIPs;
    for (auto& ip: rightIPs) allIPs.push_back(ip);

    std::sort(allIPs.begin(), allIPs.end(), [] (const IntersectionInfo& left,
                                                const IntersectionInfo& right) { return left.distance_ < right.distance_; });

    bool predicateNow = boolOp(inA, inB);

    for (auto& ip: allIPs) {
        if (ip.geom_ == left_.get())
            inA = !inA;
        else
            inB = !inB;

        bool predicateNext = boolOp(inA, inB);

        if (predicateNext != predicateNow) {
            info = ip;
            return true;
        }
    }

    return false;
}

CsgOp::CsgOp(std::unique_ptr<Geometry>& left, std::unique_ptr<Geometry>& right)
    : left_(std::move(left))
    , right_(std::move(right))
    { }
