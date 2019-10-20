/**
 * @File util.h
 * @Brief a few useful short functions
 */
#ifndef __UTIL_H__
#define __UTIL_H__

#include "constants.h"

#include <stdlib.h>
#include <math.h>
#include <string>

inline constexpr double signOf(double x) { return x > 0 ? +1 : -1; }
inline constexpr double sqr(double a) { return a * a; }
inline constexpr double toRadians(double angle) { return angle / 180.0 * PI; }
inline constexpr double toDegrees(double angle_rad) { return angle_rad / PI * 180.0; }
inline constexpr int nearestInt(float x) { return (int) floor(x + 0.5f); }

/// returns a random floating-point number in [0..1).
/// This is not a very good implementation. A better method is to be employed soon.
inline float randomFloat() { return rand() / (float) RAND_MAX; }

std::string upCaseString(std::string s); //!< returns the string in UPPERCASE
std::string extensionUpper(const char* fileName); //!< Given a filename, return its extension in UPPERCASE

/// a simple RAII class for FILE* pointers.
class FileRAII {
    FILE* held;
public:
    FileRAII(FILE* init): held(init) {}
    ~FileRAII() { if (held) fclose(held); held = NULL; }
    FileRAII(const FileRAII&) = delete;
    FileRAII& operator = (const FileRAII&) = delete;
};

#endif // __UTIL_H__
