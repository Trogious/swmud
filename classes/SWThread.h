/*
 * SWThread.h
 *
 *  Created on: Feb 19, 2010
 *      Author: Trog
 */

#ifndef SWTHREAD_H_
#define SWTHREAD_H_
#include <pthread.h>
#include <set>
#include "SWException.h"

class SWThread
{
	bool finished;
	bool running;
	pthread_t thread;

	static set<SWThread*> threadPool;

	static void *threadFun(void *);
	static void finish(void *);

public:
	SWThread() :
		finished(false), running(false)
	{
	}
	virtual ~SWThread()
	{
	}
	virtual void run() = 0;
	bool isFinished() const
	{
		return finished;
	}
	bool isRunning() const
	{
		return running;
	}
	void start() throw (SWException);
	void join();

	static void addToPool(SWThread *);
	/**
	 * Sprawdza pule watkow i usuwa z niej obiekty watkow, ktore sie zakonczyly.
	 */
	static void updatePool();
};

#endif /* SWTHREAD_H_ */
