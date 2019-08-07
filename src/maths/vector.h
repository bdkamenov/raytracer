/**
 * @File vector.h
 * @Brief defines the Vector class (a 3D vector with the usual algebraic operations)
 */ 
#ifndef __VECTOR3D_H__
#define __VECTOR3D_H__

#include <math.h>

class Vector {
public:
	Vector() = default;
	Vector(double x, double y, double z):_x(x), _y(y), _z(z) { }


	void set(double x, double y, double z)
	{
		_x = x;
		_y = y;
		_z = z;
	}

	void makeZero(void)
	{
		_x = _y = _z = 0.0;
	}

	inline constexpr double length(void) const
	{
		return sqrt(_x * _x + _y * _y + _z * _z);
	}

	inline constexpr double lengthSqr(void) const
	{
		return (_x * _x + _y * _y + _z * _z);
	}

	void scale(double multiplier)
	{
		_x *= multiplier;
		_y *= multiplier;
		_z *= multiplier;
	}

	void operator *= (double multiplier)
	{
		scale(multiplier);
	}

	void operator += (const Vector& other)
	{
		_x += other._x;
		_y += other._y;
		_z += other._z;
	}

	void operator /= (double divider)
	{
		scale(1.0 / divider);
	}

	void normalize(void)
	{
		double multiplier = 1.0 / length();

		scale(multiplier);
	}

	void setLength(double newLength)
	{
		scale(newLength / length());
	}

	double _x, _y, _z;

};

inline Vector operator + (const Vector& a, const Vector& b)
{
	return Vector(a._x + b._x, a._y + b._y, a._z + b._z);
}

inline Vector operator - (const Vector& a, const Vector& b)
{
	return Vector(a._x - b._x, a._y - b._y, a._z - b._z);
}

inline Vector operator - (const Vector& a)
{
	return Vector(-a._x, -a._y, -a._z);
}

/// dot product
inline double operator * (const Vector& a, const Vector& b)
{
	return a._x * b._x + a._y * b._y + a._z * b._z;
}

/// dot product (functional form, to make it more explicit):
inline double dot(const Vector& a, const Vector& b)
{
	return a._x * b._x + a._y * b._y + a._z * b._z;
}

/// cross product
inline Vector operator ^ (const Vector& a, const Vector& b)
{
	return Vector(
		a._y * b._z - a._z * b._y,
		a._z * b._x - a._x * b._z,
		a._x * b._y - a._y * b._x
	);
}

inline Vector operator * (const Vector& a, double multiplier)
{
	return Vector(a._x * multiplier, a._y * multiplier, a._z * multiplier);
}

inline Vector operator * (double multiplier, const Vector& a)
{
	return Vector(a._x * multiplier, a._y * multiplier, a._z * multiplier);
}

inline Vector operator / (const Vector& a, double divider)
{
	double multiplier = 1.0 / divider;
	return Vector(a._x * multiplier, a._y * multiplier, a._z * multiplier);
}

#endif // __VECTOR3D_H__
