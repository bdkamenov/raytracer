#include <math.h>
#include <vector>

#include "maths/vector.h"
#include "utils/util.h"
#include "render/sdl.h"
#include "color/color.h"
#include "scenes/camera.h"
#include "shaders/shading.h"

Camera camera;
Color vfb[VFB_MAX_SIZE][VFB_MAX_SIZE];
std::vector<Node> nodes;

Vector lightPosition;
double lightIntensity;

void setupScene()
{
    camera._position = Vector(35, 60, 0);
    camera._yaw = 0;
    camera._pitch = 0;
    camera._roll = 0;
    camera._fov = 120;
    camera._aspectRatio = float(RESX) / float(RESY);


    nodes.push_back({ std::make_shared<Plane>(5.0),
                      std::make_shared<CheckerShader>( Color(1.f, 1.f, 1.f),
                                                       Color(0.f, 0.f, 0.f), 7)});


    lightPosition = Vector(35, 90, 100);
    lightIntensity = 10000.0;

    camera.frameBegin();
}

Color raytrace(const Ray& ray)
{
    // we use double for vectors, rays and so on and floats for colors
    const Node* closestNode = nullptr;
    double closestDist = INF;
    IntersectionInfo closestInfo;

    for (const auto& node : nodes)
    {
        IntersectionInfo info;
        if (!node._geometry->intersect(ray, info))
        {
            continue;
        }

        if (info._distance < closestDist)
        {
            closestDist = info._distance;
            closestNode = &node;
            closestInfo = info;
        }
    }

    // check if we hit the sky
    if (closestNode == nullptr)
    {
        return Color(0.f, 0.f, 0.f); // background color
    }
    else
    {
        return closestNode->_shader->shade(ray, closestInfo);
    }
}

void render(int width, int height)
{
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            Ray ray = camera.getScreenRay(x, y);
            vfb[y][x] = raytrace(ray);
        }
}

int main ( int argc, char** argv )
{
    setupScene();
    SdlObject& sdl = SdlObject::instance();
    render(sdl.frameWidth(), sdl.frameHeight());
    sdl.displayVFB(vfb);
    sdl.waitForUserExit();
    return 0;
}
