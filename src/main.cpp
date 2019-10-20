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
Color ambientLight = Color(1, 1, 1) * 0.1;

bool wantAA = true; // anti-aliasing

#define COUNT_OF(arr) int((sizeof(arr)) / sizeof(arr[0]))

void setupScene() {
    // camera setup
    camera.position_ = Vector(35, 90, -100);
    camera.yaw_ = 0;
    camera.pitch_ = -20;
    camera.roll_ = 0;
    camera.fov_ = 120;
    camera.aspectRatio_ = float(RESX) / float(RESY);

    // objects setup
    std::unique_ptr csgObj (std::make_unique<CsgMinus>());
    csgObj->left_ = std::make_unique<Cube>(Vector(30, 60.0, -50.0), 15.0);
    csgObj->right_ = std::make_unique<Sphere>(Vector(30, 60.0, -50.0), 20.0);

    std::unique_ptr plane1 (std::make_unique<Plane>(4.0));

    std::unique_ptr cube (std::make_unique<Cube>(Vector(-15.0, 60.0, 0), 20.0));

    // shaders setup
    std::unique_ptr phong (std::make_unique<Phong>(10.0, 30.0, Color(0, 0, 0),
                           std::make_unique<CheckerTexture>(Color(0.4f, 0.2f, 0.1f),
                                                            Color(0.9f, 0.8f, 0.1f), 7)));

    std::unique_ptr lambert1 (std::make_unique<Lambert>(Color(0, 0, 0),
                              std::make_unique<CheckerTexture>(Color(0.1f, 0.8f, 0.6f),
                                                               Color(0.3f, 0.7f, 0.1f))));

    std::unique_ptr lambert2 (std::make_unique<Lambert>(Color(0, 0, 0),
                              std::make_unique<CheckerTexture>(Color(0.f, 1.f, 1.f),
                                                               Color(1.f, 0.f, 1.f), 1)));


    nodes.push_back({std::move(csgObj), std::move(phong)});
    nodes.push_back({std::move(plane1), std::move(lambert1)});
    nodes.push_back({std::move(cube), std::move(lambert2)});

    lightPosition = Vector(40, 100, -130);
    lightIntensity = 25000.0;

    camera.frameBegin();
}

bool visibilityCheck(const Vector &start, const Vector &end) {
    Ray ray;
    ray.start_ = start;
    ray.dir_ = end - start;
    ray.dir_.normalize();

    double targetDist = (end - start).length();

    for (const auto &node : nodes) {
        IntersectionInfo info;
        if (!node.geometry_->intersect(ray, info)) {
            continue;
        }

        if (info.distance_ < targetDist) // check if there is object between the
        {                                // light and the point where it started
            return false;
        }
    }

    return true;
}

Color raytrace(const Ray &ray)
{
    // we use double for vectors, rays and so on and floats for colors
    const Node *closestNode = nullptr;
    double closestDist = INF;
    IntersectionInfo closestInfo;

    for (const auto &node : nodes) {
        IntersectionInfo info;
        if (!node.geometry_->intersect(ray, info))
        {
            continue;
        }

        if (info.distance_ < closestDist)
        {
            closestDist = info.distance_;
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
        return closestNode->shader_->shade(ray, closestInfo);
    }
}

void render(int width, int height)
{

    const double kernel[5][2] = {
            {0.0, 0.0},
            {0.6, 0.0},
            {0.0, 0.6},
            {0.3, 0.3},
            {0.6, 0.6},
    };

    for (int y = 0; y < height; y++)
    {

        for (int x = 0; x < width; x++)
        {
            if (wantAA)
            {
                Color sum(0, 0, 0);
                for (int i = 0; i < COUNT_OF(kernel); i++)
                    sum += raytrace(camera.getScreenRay(x + kernel[i][0], y + kernel[i][1]));
                vfb[y][x] = sum / double(COUNT_OF(kernel));
            } else {
                Ray ray = camera.getScreenRay(x, y);
                vfb[y][x] = raytrace(ray);
            }
        }
    }
}

int main(int argc, char **argv)
{
    setupScene();
    SdlObject &sdl = SdlObject::instance();
    Uint32 startTicks = SDL_GetTicks();
    render(sdl.frameWidth(), sdl.frameHeight());
    Uint32 elapsedMs = SDL_GetTicks() - startTicks;
    printf("Render took %.2lfs\n", elapsedMs / 1000.0);
    sdl.displayVFB(vfb);
    sdl.waitForUserExit();
    return 0;
}
