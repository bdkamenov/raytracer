/**
 * @File color.h
 * @Brief Defines the Color class, which we'll use to represent light.
 */
#ifndef __COLOR_H__
#define __COLOR_H__

#include "utils/util.h"

inline constexpr unsigned convertTo8bit(float x)
{
    if (x < 0) x = 0;
    if (x > 1) x = 1;
    return nearestInt(x * 255.0f);
}

/// Represents a color, using floatingpoint components in [0..1]
struct Color {
    float _r, _g, _b;


    Color() = default;
    Color(float r, float g, float b) : _r(r), _g(g), _b(b) { }  //!< Construct a color from floatingpoint values

    explicit Color(unsigned rgbcolor) : //!< Construct a color from R8G8B8 value like "0xffce08"
            _b((rgbcolor & 0xff) / 255.0f),
            _g(((rgbcolor >> 8) & 0xff) / 255.0f),
            _r(((rgbcolor >> 16) & 0xff) / 255.0f)
    { }

    /// convert to RGB32, with channel shift specifications. The default values are for
    /// the blue channel occupying the least-significant byte
    unsigned toRGB32(int redShift = 16, int greenShift = 8, int blueShift = 0)
    {
        unsigned ir = convertTo8bit(_r);
        unsigned ig = convertTo8bit(_g);
        unsigned ib = convertTo8bit(_b);
        return (ib << blueShift) | (ig << greenShift) | (ir << redShift);
    }

    /// make black
    void makeZero(void)
    {
        _r = _g = _b = 0;
    }

    /// set the color explicitly
    void setColor(float r, float g, float b)
    {
        _r = r;
        _g = g;
        _b = b;
    }

    /// get the intensity of the color (direct)
    constexpr float intensity()
    {
        return (_r + _g + _b) / 3;
    }

    /// get the perceptual intensity of the color
    constexpr float intensityPerceptual()
    {
        return (_r * 0.299 + _g * 0.587 + _b * 0.114);
    }

    /// Accumulates some color to the current
    void operator += (const Color& rhs)
    {
        _r += rhs._r;
        _g += rhs._g;
        _b += rhs._b;
    }

    /// multiplies the color
    void operator *= (float multiplier)
    {
        _r *= multiplier;
        _g *= multiplier;
        _b *= multiplier;
    }

    /// divides the color
    void operator /= (float divider)
    {
        _r /= divider;
        _g /= divider;
        _b /= divider;
    }
};

/// adds two colors
inline Color operator + (const Color& a, const Color& _b)
{
    return Color(a._r + _b._r, a._g + _b._g, a._b + _b._b);
}

/// subtracts two colors
inline Color operator - (const Color& a, const Color& _b)
{
    return Color(a._r - _b._r, a._g - _b._g, a._b - _b._b);
}

/// multiplies two colors
inline Color operator * (const Color& a, const Color& _b)
{
    return Color(a._r * _b._r, a._g * _b._g, a._b * _b._b);
}

/// multiplies a color by some multiplier
inline Color operator * (const Color& a, float multiplier)
{
    return Color(a._r * multiplier, a._g * multiplier, a._b * multiplier);
}

/// multiplies a color by some multiplier
inline Color operator * (float multiplier, const Color& a)
{
    return Color(a._r * multiplier, a._g * multiplier, a._b * multiplier);
}

/// divides some color
inline Color operator / (const Color& a, float divider)
{
    return Color(a._r / divider, a._g / divider, a._b / divider);
}

#endif // __COLOR_H__
