/**
 * @File bitmap.h
 * @Brief A class to represent bitmap textures.
 */
#ifndef __BITMAP_H__
#define __BITMAP_H__

#include "color/color.h"

/// @brief a class that represents a bitmap (2d array of colors), e.g. a image
/// supports loading/saving to BMP
class Bitmap {

public:
    Bitmap(); //!< Generates an empty bitmap
    ~Bitmap();
    void freeMem(void); //!< Deletes the memory, associated with the bitmap
    int getWidth(void) const; //!< Gets the width of the image (X-dimension)
    int getHeight(void) const; //!< Gets the height of the image (Y-dimension)
    bool isOK(void) const; //!< Returns true if the bitmap is valid
    void generateEmptyImage(int width, int height); //!< Creates an empty image with the given dimensions
    Color getPixel(int x, int y) const; //!< Gets the pixel at coordinates (x, y). Returns black if (x, y) is outside of the image
    void setPixel(int x, int y, const Color& col); //!< Sets the pixel at coordinates (x, y).

    bool loadBMP(const char* filename); //!< Loads an image from a BMP file. Returns false in the case of an error
    bool saveBMP(const char* filename); //!< Saves the image to a BMP file (with clamping, etc). Returns false in the case of an error (e.g. read-only media)

    bool loadEXR(const char* filename); //!< Loads an EXR file
    bool saveEXR(const char* filename); //!< Saves the image into the EXR format, preserving the dynamic range, using Half for storage.

    virtual bool loadImage(const char* filename); //!< Loads an image (the format is detected from extension)
    virtual bool saveImage(const char* filename); //!< Save the bitmap to an image (the format is detected from extension)

private:
    int width, height;
    Color* data;
};

#endif // __BITMAP_H__
