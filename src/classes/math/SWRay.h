/*
 * SWRay.h
 *
 *  Created on: 04-03-2012
 *      Author: Trog
 */

#ifndef SWRAY_H_
#define SWRAY_H_
#include "SWVector.h"

namespace math
{

class SWRay
{
	SWVector origin;
	SWVector direction;

public:
	SWRay()
	{
	}

	SWRay(const SWVector &origin, const SWVector &direction) :
		origin(origin), direction(direction)
	{
	}

	const SWVector &getOrigin() const
	{
		return origin;
	}

	const SWVector &getDirection() const
	{
		return direction;
	}
};

}

#endif /* SWRAY_H_ */
