/*
 * SWThread.cpp
 *
 *  Created on: Feb 19, 2010
 *      Author: Trog
 */

#include "SWThread.h"

set<SWThread*> SWThread::threadPool;

void *SWThread::threadFun(void *arg)
{
	pthread_cleanup_push(SWThread::finish, arg);
	SWThread *t = reinterpret_cast<SWThread*> (arg);
	t->run();
	pthread_cleanup_pop(1);
	return 0;
}

void SWThread::finish(void *arg)
{
	SWThread *t = reinterpret_cast<SWThread*> (arg);
	t->running = false;
	t->finished = true;
}

void SWThread::start() throw (SWException)
{
	if (finished)
	{
		throw new SWException("this SWThread instance is already finished");
	}
	if (running)
	{
		throw new SWException("this SWThread instance is already running");
	}

	running = true;
	pthread_create(&thread, 0, SWThread::threadFun, reinterpret_cast<void *> (this));
}

void SWThread::join()
{
	pthread_join(thread, 0);
}

void SWThread::addToPool(SWThread *thread)
{
	if (!thread->isFinished())
	{
		SWThread::threadPool.insert(thread);
	}
}

void SWThread::updatePool()
{
	SWThread *thread;
	for (set<SWThread*>::iterator it = threadPool.begin(); it != threadPool.end();)
	{
		if ((thread = *it)->isFinished())
		{
			threadPool.erase(it++);
			delete thread;
		}
		else
		{
			it++;
		}
	}
}
