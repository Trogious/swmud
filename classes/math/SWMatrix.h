/*
 * SWMatrix.h
 *
 *  Created on: 17-07-2011
 *      Author: Trog
 */

#ifndef SWMATRIX_H_
#define SWMATRIX_H_
#include <cstring>

namespace math
{

class SWMatrix
{
protected:
	float m[4][4];

public:
	SWMatrix()
	{
		bzero(m, 16 * sizeof(float));
	}

	SWMatrix(float, float, float, float, float, float, float, float, float, float, float, float, float, float, float,
			float);

	virtual ~SWMatrix()
	{
	}

	SWMatrix operator+(const SWMatrix &) const;
	SWMatrix operator-(const SWMatrix &) const;
	SWMatrix operator*(const SWMatrix &) const;
	SWMatrix operator*(float) const;
	SWMatrix &transpose();
	inline SWMatrix getTransposed() const;
	SWMatrix &inverse();
	inline SWMatrix getInversed() const;

	inline static SWMatrix getIdentity();
};

inline SWMatrix operator*(float s, const SWMatrix &m)
{
	return m * s;
}

inline SWMatrix SWMatrix::getTransposed() const
{
	return SWMatrix(*this).transpose();
}

inline SWMatrix SWMatrix::getInversed() const
{
	return SWMatrix(*this).inverse();
}

inline SWMatrix SWMatrix::getIdentity()
{
	return SWMatrix(1.f, .0f, .0f, .0f, .0f, 1.f, .0f, .0f, .0f, .0f, 1.f, .0f, .0f, .0f, .0f, 1.f);
}

}

#endif /* SWMATRIX_H_ */
