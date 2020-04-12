/*
 * SWPoint.cpp
 *
 *  Created on: 17-07-2011
 *      Author: Trog
 */

#include "SWPoint.h"
#include <cmath>
#include "constants.h"

namespace math
{

bool SWPoint::operator==(const SWPoint &p) const
{
	return fabs(x - p.x) < SWEpsilon && fabs(y - p.y) < SWEpsilon && fabs(z - p.z) < SWEpsilon;
}

float SWPoint::distance(const SWPoint &p) const
{
	return sqrtf(powf(x - p.x, 2.f) + powf(y - p.y, 2.f) + powf(z - p.z, 2.f));
}

}
