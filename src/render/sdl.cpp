/**
 * @File sdl.cpp
 * @Brief Implements the interface to SDL singleton class (mainly drawing to screen functions)
 */

#include <stdio.h>
#include "sdl.h"
#include "utils/constants.h"


/// try to create a frame window with the given dimensions
SdlObject::SdlObject(int frameWidth, int frameHeight)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        error_ = SDL_GetError();
        printf("Cannot initialize SDL: %s\n", error_);
        return;
    }

    window_ = SDL_CreateWindow("Raytracer", SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

    if (!window_)
    {
        error_ = SDL_GetError();
        printf("Cannot set video mode %dx%d - %s\n", frameWidth, frameHeight, error_);
        return;
    }

    screen_ = SDL_GetWindowSurface(window_);

    if (!screen_)
    {
        error_ = SDL_GetError();
        printf("Cannot get screen! Error: %s\n", error_);
        return;
    }
}

SdlObject::~SdlObject()
{
    closeGraphics();
    printf("Exited cleanly\n");
}

/// closes SDL graphics
void SdlObject::closeGraphics()
{
    SDL_Quit();
}

/// displays a VFB (virtual frame buffer) to the real framebuffer, with the necessary color clipping
void SdlObject::displayVFB(Color vfb[VFB_MAX_SIZE][VFB_MAX_SIZE])
{
    int rs = screen_->format->Rshift;
    int gs = screen_->format->Gshift;
    int bs = screen_->format->Bshift;
    for (int y = 0; y < screen_->h; y++) {
        Uint32 *row = (Uint32*) ((Uint8*) screen_->pixels + y * screen_->pitch);
        for (int x = 0; x < screen_->w; x++)
            row[x] = vfb[y][x].toRGB32(rs, gs, bs);
    }
    SDL_FreeSurface(screen_);
    SDL_UpdateWindowSurface(window_);
}

/// waits the user to indicate he wants to close the application (by either clicking on the "X" of the window,
/// or by pressing ESC)
void SdlObject::waitForUserExit(void)
{
    SDL_Event ev;
    while (true) {
        while (SDL_WaitEvent(&ev)) {
            switch (ev.type) {
                case SDL_QUIT:
                    return;
                case SDL_KEYDOWN:
                {
                    switch (ev.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            return;
                        default:
                            break;
                    }
                }
                default:
                    break;
            }
        }
    }
}

/// returns the frame width
int SdlObject::frameWidth(void)
{
    if (screen_) return screen_->w;
    return 0;
}

/// returns the frame height
int SdlObject::frameHeight(void)
{
    if (screen_) return screen_->h;
    return 0;
}

SdlObject& SdlObject::instance()
{
    static SdlObject instance(RESX, RESY);
    return instance;
}
