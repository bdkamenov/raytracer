/**
 * @File matrix.cpp
 * @Brief Code to deal with 3x3 matrices
 */

#include <math.h>
#include "matrix.h"

Matrix rotationAroundX(double angle)
{
	double S = sin(angle);
	double C = cos(angle);
	Matrix a(1.0);
	a._m[1][1] = C;
	a._m[2][1] = S;
	a._m[1][2] = -S;
	a._m[2][2] = C;
	return a;
}

Matrix rotationAroundY(double angle)
{
	double S = sin(angle);
	double C = cos(angle);
	Matrix a(1.0);
	a._m[0][0] = C;
	a._m[2][0] = -S;
	a._m[0][2] = S;
	a._m[2][2] = C;
	return a;
}

Matrix rotationAroundZ(double angle)
{
	double S = sin(angle);
	double C = cos(angle);
	Matrix a(1.0);
	a._m[0][0] = C;
	a._m[1][0] = S;
	a._m[0][1] = -S;
	a._m[1][1] = C;
	return a;
}

Matrix operator * (const Matrix& a, const Matrix& b)
{
	Matrix c(0.0);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			for (int k = 0; k < 3; k++)
				c._m[i][j] += a._m[i][k] * b._m[k][j];
	return c;
}

double determinant(const Matrix& a)
{
	return a._m[0][0] * a._m[1][1] * a._m[2][2]
	     - a._m[0][0] * a._m[1][2] * a._m[2][1]
	     - a._m[0][1] * a._m[1][0] * a._m[2][2]
	     + a._m[0][1] * a._m[1][2] * a._m[2][0]
	     + a._m[0][2] * a._m[1][0] * a._m[2][1]
	     - a._m[0][2] * a._m[1][1] * a._m[2][0];
}

double cofactor(const Matrix& m, int ii, int jj)
{
	int rows[2], rc = 0, cols[2], cc = 0;
	for (int i = 0; i < 3; i++)
		if (i != ii) rows[rc++] = i;
	for (int j = 0; j < 3; j++)
		if (j != jj) cols[cc++] = j;
	double t = m._m[rows[0]][cols[0]] * m._m[rows[1]][cols[1]] - m._m[rows[1]][cols[0]] * m._m[rows[0]][cols[1]];
	if ((ii + jj) % 2) t = -t;
	return t;
}

Matrix inverseMatrix(const Matrix& m)
{
	double D = determinant(m);
	if (fabs(D) < 1e-12) return m; // an error; matrix is not invertible
	double rD = 1.0 / D;
	Matrix result;
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			result._m[i][j] = rD * cofactor(m, j, i);
	return result;
}

