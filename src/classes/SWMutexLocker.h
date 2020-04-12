/*
 * SWMutexLocker.h
 *
 *  Created on: Feb 20, 2010
 *      Author: Trog
 */

#ifndef SWMUTEXLOCKER_H_
#define SWMUTEXLOCKER_H_
#include "SWMutex.h"

class SWMutexLocker
{
	SWMutex *mutex;

public:
	SWMutexLocker(SWMutex *mutex) :
		mutex(mutex)
	{
		mutex->lock();
	}
	virtual ~SWMutexLocker()
	{
		mutex->unlock();
	}
};

#endif /* SWMUTEXLOCKER_H_ */
