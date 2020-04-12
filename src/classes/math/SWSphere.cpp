/*
 * SWSphere.cpp
 *
 *  Created on: 04-03-2012
 *      Author: Trog
 */

#include "SWSphere.h"
#include <algorithm>

namespace math
{

bool SWSphere::intersect(const SWRay &ray, std::list<SWPoint> *points) const
{
	SWVector d = ray.getDirection();
	SWVector oc = ray.getOrigin() - center;

	float a = d.dot(d);
	float b = (2 * oc).dot(d);
	float c = oc.dot(oc) - float(radius * radius);

	float bb = b * b;
	float four_ac = 4 * a * c;
	float discriminant = bb - four_ac;

	if (discriminant < 0.f) /* promien nie przecina sfery */
	{
		return false;
	}

	float discSqrt = sqrtf(bb - four_ac);
	float q = (b < 0.f) ? ((discSqrt - b) * .5f) : ((-b - discSqrt) * .5f);

	float t0 = q / a;
	float t1 = c / q;

	if (t0 > t1)
	{
		std::swap(t0, t1);
	}

	if (t1 < 0) /* sfera jest w przeciwnym kierunku do promienia => brak przeciecia */
	{
		return false;
	}

	if (points)
	{
		SWVector o = ray.getOrigin();

		if (t0 < 0.f)
		{
			points->push_back(SWPoint(o.getX() + d.getX() * t1, o.getY() + d.getY() * t1, o.getZ() + d.getZ() * t1));
		}
		else
		{
			points->push_back(SWPoint(o.getX() + d.getX() * t0, o.getY() + d.getY() * t0, o.getZ() + d.getZ() * t0));

			if (discriminant > 0.f)
			{
				points->push_back(SWPoint(o.getX() + d.getX() * t1, o.getY() + d.getY() * t1, o.getZ() + d.getZ() * t1));
			}
		}
	}

	return true;
}

}
