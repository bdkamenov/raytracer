/**
 * @File ray.h
 * @Brief Code to deal rays
 */
#ifndef RAY_H
#define RAY_H

#include "vector.h"

struct Ray {
    Vector _start;
    Vector _dir; //!< normed!
};


#endif //RAY_H
