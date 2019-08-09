/**
 * @File shading.h
 * @Brief Contains declarations of shader classes
 */
#ifndef __SHADING_H__
#define __SHADING_H__

#include "maths/ray.h"
#include "geometries/geometry.h"
#include "color/color.h"


class Shader
{
public:
    virtual ~Shader() = default;
    virtual Color shade(const Ray& ray, IntersectionInfo& info) = 0;
};

class CheckerShader : public Shader
{
public:
    CheckerShader(const Color& c1, const Color& c2, size_t size = 5): _color1(c1), _color2(c2), _size(size) {}
    ~CheckerShader() = default;
    Color shade(const Ray& ray, IntersectionInfo& info) override;

private:
    Color _color1;
    Color _color2;
    size_t _size;
};

struct Node
{
    std::shared_ptr<Geometry> _geometry = nullptr;
    std::shared_ptr<Shader> _shader = nullptr;
};


#endif // __SHADING_H__
