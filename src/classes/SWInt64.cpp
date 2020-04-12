/*
 * SWInt64.cpp
 *
 *  Created on: 2010-08-07
 *      Author: Trog
 */

#include "SWInt64.h"
#include "SWString.h"
#include <sstream>
#include <cstdlib>

using namespace std;


/**
 * Converts number to string.
 * \return string representing the number
 */
SWString SWInt64::toString() const
{
	stringstream ss;
	ss << value;
	return SWString(ss.str());
}

/**
 * Converts integer to string.
 * \return string representing the number
 */
SWString SWInt64::toString(const SWInt64 &value)
{
	stringstream ss;
	ss << value;
	return SWString(ss.str());
}

int64 SWInt64::fromString(const SWString &value)
{
	return atoi(value.c_str());
}
