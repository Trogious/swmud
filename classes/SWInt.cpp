/*
 * SWInt.cpp
 *
 *  Created on: 2009-08-02
 *      Author: Trog
 */

#include "SWInt.h"
#include "SWString.h"
#include <sstream>
#include <cstdlib>

using namespace std;


/**
 * Converts number to string.
 * \return string representing the number
 */
SWString SWInt::toString() const
{
	stringstream ss;
	ss << value;
	return SWString(ss.str());
}

/**
 * Converts integer to string.
 * \return string representing the number
 */
SWString SWInt::toString(const SWInt &value)
{
	stringstream ss;
	ss << value;
	return SWString(ss.str());
}

int SWInt::fromString(const SWString &value)
{
	return atoi(value.c_str());
}
