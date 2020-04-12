/*
 * SWSphere.h
 *
 *  Created on: 04-03-2012
 *      Author: Trog
 */

#ifndef SWSPHERE_H_
#define SWSPHERE_H_
#include "SWIntersectable.h"
#include "SWInt.h"

namespace math
{

class SWSphere : public SWIntersectable
{
	SWVector center;
	SWInt radius;

public:
	SWSphere()
	{
	}

	SWSphere(const SWVector &center, const SWInt &radius) :
		center(center), radius(radius)
	{
	}

	bool intersect(const SWRay &, std::list<SWPoint> * = 0) const;
};

}

#endif /* SWSPHERE_H_ */
