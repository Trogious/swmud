/*
 * SWVector.cpp
 *
 *  Created on: 17-07-2011
 *      Author: Trog
 */

#include "SWVector.h"

namespace math
{

SWVector SWVector::operator+(const SWVector &v) const
{
	return SWVector(x + v.x, y + v.y, z + v.z);
}

SWVector SWVector::operator-(const SWVector &v) const
{
	return SWVector(x - v.x, y - v.y, z - v.z);
}

SWVector SWVector::operator*(float s) const
{
	return SWVector(s * x, s * y, s * z);
}

SWVector SWVector::operator*(const SWVector &v) const
{
	return SWVector((y * v.z) - (z * v.y), (z * v.x) - (x * v.z), (x * v.y) - (y * v.x));
}

float SWVector::magnitude() const
{
	return sqrtf(powf(x, 2.f) + powf(y, 2.f) + powf(z, 2.f));
}

SWVector &SWVector::normalize()
{
	float m = magnitude();

	x /= m;
	y /= m;
	z /= m;

	return *this;
}

float SWVector::dot(const SWVector &v) const
{
	return (x * v.x) + (y * v.y) + (z * v.z);
}

}
