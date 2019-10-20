/**
 * @File ray.h
 * @Brief Code to deal rays
 */
#ifndef RAY_H
#define RAY_H

#include "vector.h"

struct Ray {
    Vector start_;
    Vector dir_; //!< normed!
};


#endif //RAY_H
