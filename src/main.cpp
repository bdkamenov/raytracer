#include <math.h>
#include "utils/util.h"
#include "render/sdl.h"
#include "color/color.h"

Color vfb[VFB_MAX_SIZE][VFB_MAX_SIZE];

void render(int width, int height)
{
    for (int y = 0; y < height; y++)
        for (int x = 0; x < width; x++) {
            vfb[y][x] = Color(float(x) / width, float(y) / height, 0.0f);
        }
}

int main ( int argc, char** argv )
{
    SdlObject& sdl = SdlObject::instance();
    render(sdl.frameWidth(), sdl.frameHeight());
    sdl.displayVFB(vfb);
    sdl.waitForUserExit();
    return 0;
}
