/*
 * SWClassFactory.h
 *
 *  Created on: 2009-08-02
 *      Author: Trog
 */

#ifndef SWCLASSFACTORY_H_
#define SWCLASSFACTORY_H_

#include "SWDynamicClass.h"

class SWClassFactory
{
public:
	SWClassFactory()
	{
	}
	virtual ~SWClassFactory()
	{
	}

	virtual SWDynamicClass *createInstance() const = 0;
};

#endif /* SWCLASSFACTORY_H_ */
