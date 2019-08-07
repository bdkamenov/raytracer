/**
 * @File matrix.h
 * @Brief Code to deal with 3x3 real matrices
 */
#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

struct Matrix {
    double _m[3][3];
    Matrix() = default;
    
    Matrix(double diagonalElement)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                if (i == j) _m[i][j] = diagonalElement;
                else _m[i][j] = 0.0;
    }
};

inline Vector operator * (const Vector& v, const Matrix& m)
{
    return Vector(
        v._x * m._m[0][0] + v._y * m._m[1][0] + v._z * m._m[2][0],
        v._x * m._m[0][1] + v._y * m._m[1][1] + v._z * m._m[2][1],
        v._x * m._m[0][2] + v._y * m._m[1][2] + v._z * m._m[2][2]
    );
}

inline void operator *= (Vector& v, const Matrix& a) { v = v*a; }

Matrix operator * (const Matrix& a, const Matrix& b); //!< matrix multiplication; result = a*b
Matrix inverseMatrix(const Matrix& a); //!< finds the inverse of a matrix (assuming it exists)
double determinant(const Matrix& a); //!< finds the determinant of a matrix

Matrix rotationAroundX(double angle); //!< returns a rotation matrix around the X axis; the angle is in radians
Matrix rotationAroundY(double angle); //!< same as above, but rotate around Y
Matrix rotationAroundZ(double angle); //!< same as above, but rotate around Z

#endif // MATRIX_H
