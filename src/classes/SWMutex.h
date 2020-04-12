/*
 * SWMutex.h
 *
 *  Created on: Feb 20, 2010
 *      Author: Trog
 */

#ifndef SWMUTEX_H_
#define SWMUTEX_H_
#include <pthread.h>

class SWMutex
{
	pthread_mutex_t mutex;

public:
	SWMutex()
	{
		pthread_mutex_init(&mutex, 0);
	}
	virtual ~SWMutex()
	{
		pthread_mutex_destroy(&mutex);
	}
	bool lock()
	{
		return pthread_mutex_lock(&mutex);
	}
	bool tryLock()
	{
		return pthread_mutex_trylock(&mutex);
	}
	bool unlock()
	{
		return pthread_mutex_unlock(&mutex);
	}
};

#endif /* SWMUTEX_H_ */
