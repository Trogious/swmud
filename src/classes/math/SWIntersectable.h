/*
 * SWIntersectable.h
 *
 *  Created on: 04-03-2012
 *      Author: Trog
 */

#ifndef SWINTERSECTABLE_H_
#define SWINTERSECTABLE_H_
#include "SWRay.h"
#include <list>

namespace math
{

class SWIntersectable
{
public:
	virtual ~SWIntersectable()
	{
	}

	virtual bool intersect(const SWRay &, std::list<SWPoint> * = 0) const = 0;
};

}

#endif /* SWINTERSECTABLE_H_ */
