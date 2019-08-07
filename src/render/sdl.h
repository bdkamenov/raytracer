/**
 * @File sdl.h
 * @Brief Contains SDL singleton class to create window and display the VFB
 */
#ifndef __SDL_H__
#define __SDL_H__

#include <SDL2/SDL.h>
#include "color/color.h"

class SdlObject
{
public:
    SdlObject(const SdlObject&) = delete;
    SdlObject(SdlObject&&) = delete;
    ~SdlObject();
    SdlObject& operator=(const SdlObject&) = delete;
    SdlObject& operator=(SdlObject&&) = delete;

    static SdlObject& instance();

    void closeGraphics(void);
    void displayVFB(Color vfb[VFB_MAX_SIZE][VFB_MAX_SIZE]); //!< displays the VFB (Virtual framebuffer) to the real one.
    void waitForUserExit(void); //!< Pause. Wait until the user closes the application
    int frameWidth(void); //!< returns the frame width (pixels)
    int frameHeight(void); //!< returns the frame height (pixels)

private:
    SdlObject(int frameWidth, int frameHeight);

    SDL_Window* _window = nullptr;
    SDL_Surface* _screen = nullptr;
    const char* _error = nullptr;
};



#endif // __SDL_H__
