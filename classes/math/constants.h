/*
 * constants.h
 *
 *  Created on: 17-07-2011
 *      Author: Trog
 */

#ifndef CONSTANTS_H_
#define CONSTANTS_H_
#include <cmath>

namespace math
{

const float SWEpsilon = .00001f;

#ifndef M_PI
const float SWPi = 3.14159265358979323846f;
#else
const float SWPi = M_PI;
#endif

const float SW180ByPi = 180.f / SWPi;

}

#endif /* CONSTANTS_H_ */
