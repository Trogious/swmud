/*
 * SWException.cpp
 *
 *  Created on: 2009-09-11
 *      Author: Trog
 */

#include "SWException.h"
#include <iostream>

exception *SWException::lastException = 0;

SWException::~SWException() throw()
{
	cerr << "destroying SWException" << endl;
	lastException = 0;
}
