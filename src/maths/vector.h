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
	Vector(double x, double y, double z):x_(x), y_(y), z_(z) { }


	void set(double x, double y, double z)
	{
		x_ = x;
		y_ = y;
		z_ = z;
	}

	void makeZero()
	{
		x_ = y_ = z_ = 0.0;
	}

	inline double length() const
	{
		return sqrt(x_ * x_ + y_ * y_ + z_ * z_);
	}

	inline constexpr double lengthSqr() const
	{
		return (x_ * x_ + y_ * y_ + z_ * z_);
	}

	void scale(double multiplier)
	{
		x_ *= multiplier;
		y_ *= multiplier;
		z_ *= multiplier;
	}

	void operator *= (double multiplier)
	{
		scale(multiplier);
	}

	void operator += (const Vector& other)
	{
		x_ += other.x_;
		y_ += other.y_;
		z_ += other.z_;
	}

	void operator /= (double divider)
	{
		scale(1.0 / divider);
	}

	void normalize()
	{
		double multiplier = 1.0 / length();

		scale(multiplier);
	}

	void setLength(double newLength)
	{
		scale(newLength / length());
	}

	double x_, y_, z_;

};

inline Vector operator+ (const Vector& a, const Vector& b)
{
	return Vector(a.x_ + b.x_, a.y_ + b.y_, a.z_ + b.z_);
}

inline Vector operator- (const Vector& a, const Vector& b)
{
	return Vector(a.x_ - b.x_, a.y_ - b.y_, a.z_ - b.z_);
}

inline Vector operator- (const Vector& a)
{
	return Vector(-a.x_, -a.y_, -a.z_);
}

/// dot product
inline double operator * (const Vector& a, const Vector& b)
{
	return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_;
}

/// dot product (functional form, to make it more explicit):
inline double dot(const Vector& a, const Vector& b)
{
	return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_;
}

/// cross product
inline Vector operator^ (const Vector& a, const Vector& b)
{
	return Vector(
		a.y_ * b.z_ - a.z_ * b.y_,
		a.z_ * b.x_ - a.x_ * b.z_,
		a.x_ * b.y_ - a.y_ * b.x_
	);
}

inline Vector operator* (const Vector& a, double multiplier)
{
	return Vector(a.x_ * multiplier, a.y_ * multiplier, a.z_ * multiplier);
}

inline Vector operator* (double multiplier, const Vector& a)
{
	return Vector(a.x_ * multiplier, a.y_ * multiplier, a.z_ * multiplier);
}

inline Vector operator/ (const Vector& a, double divider)
{
	double multiplier = 1.0 / divider;
	return Vector(a.x_ * multiplier, a.y_ * multiplier, a.z_ * multiplier);
}

inline Vector reflect(Vector in, const Vector& norm)
{
	in.normalize();
	in += 2 * norm * dot(norm, -in);
	in.normalize();
	return in;
}

inline Vector faceforward(const Vector& ray, const Vector& norm)
{
	if (dot(ray, norm) < 0) return norm;
	else return -norm;
}

#endif // __VECTOR3D_H__
