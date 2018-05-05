/*
 * SWVector.h
 *
 *  Created on: 17-07-2011
 *      Author: Trog
 */

#ifndef SWVECTOR_H_
#define SWVECTOR_H_
#include "SWPoint.h"
#include "constants.h"

namespace math
{

class SWVector: public SWPoint
{
public:
	SWVector()
	{
	}

	SWVector(float x, float y, float z) :
		SWPoint(x, y, z)
	{
	}

	virtual ~SWVector()
	{
	}

	SWVector operator+(const SWVector &) const;
	SWVector operator-(const SWVector &) const;
	SWVector operator*(float) const;
	SWVector operator*(const SWVector &) const;

	float magnitude() const;
	SWVector &normalize();
	inline SWVector getNormalized() const;
	float dot(const SWVector &) const;
	inline bool isOrthogonalTo(const SWVector &) const;
	inline float radiansBetween(const SWVector &) const;
	inline float degreesBetween(const SWVector &) const;

};

inline SWVector operator*(float s, const SWVector &v)
{
	return v * s;
}

inline SWVector SWVector::getNormalized() const
{
	return SWVector(*this).normalize();
}

inline bool SWVector::isOrthogonalTo(const SWVector &v) const
{
	return fabs(dot(v)) < SWEpsilon;
}

inline float SWVector::radiansBetween(const SWVector &v) const
{
	return acos(getNormalized().dot(v.getNormalized()));
}

inline float SWVector::degreesBetween(const SWVector &v) const
{
	return radiansBetween(v) * SW180ByPi;
}

}

#endif /* SWVECTOR_H_ */
