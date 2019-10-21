/**
 * @File shading.h
 * @Brief Contains declarations of shader classes
 */
#ifndef __SHADING_H__
#define __SHADING_H__

#include "maths/ray.h"
#include "geometries/geometry.h"
#include "color/color.h"
#include "materials/bitmap.h"

class Texture
{
public:
    virtual ~Texture() = default;
    virtual Color sample(const IntersectionInfo& info) = 0;
};

class CheckerTexture: public Texture
{
public:
    CheckerTexture(const Color& c1, const Color& c2, size_t size = 1): color1_(c1), color2_(c2), scaling_(size) {}
    ~CheckerTexture() override = default;
    Color sample(const IntersectionInfo& info) override;


private:
    Color color1_;
    Color color2_;
    size_t scaling_;
};

class BitmapTexture: public Texture
{
public:
    BitmapTexture(const std::string& filename, double scaling = 1.0);
    ~BitmapTexture() = default;
    Color sample(const IntersectionInfo& info) override;

private:
    std::unique_ptr<Bitmap> bitmap_;
    double scaling_;
};

class Shader
{
public:

    virtual ~Shader() = default;
    virtual Color shade(const Ray& ray, IntersectionInfo& info) = 0;
};

class Lambert : public Shader
{
public:
    Lambert(const Color& color = {0.f, 0.f, 0.f}, std::unique_ptr<Texture> texture = nullptr)
    : color_(color)
    , texture_(std::move(texture))
    {}

    ~Lambert() override = default;
    Color shade(const Ray& ray, IntersectionInfo& info) override;

private:
    Color color_; // used if the texture is null
    std::unique_ptr<Texture> texture_;
};

class Phong : public Shader
{
public:
    Phong(double a, double b, const Color& color = {0.f, 0.f, 0.f}, std::unique_ptr<Texture> texture = nullptr)
    : specularMultiplier_(a)
    , specularExponent_(b)
    , color_(color)
    , texture_(std::move(texture)) {}

    ~Phong() override = default;
    Color shade(const Ray& ray, IntersectionInfo& info) override;

    double specularMultiplier_; // defines how bright the flashes will be
    double specularExponent_;   // defines how fine the flashes will be

private:
    Color color_; // used if the texture is null
    std::unique_ptr<Texture> texture_;
};

struct Node
{
    //Node(std::unique_ptr<Geometry> geometry, std::unique_ptr<Shader> shader): geometry_(geometry.get()), shader_(shader.get()) {}
    std::unique_ptr<Geometry> geometry_;
    std::unique_ptr<Shader> shader_;
};


#endif // __SHADING_H__
