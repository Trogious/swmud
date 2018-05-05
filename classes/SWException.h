/*
 * SWException.h
 *
 *  Created on: 2009-09-11
 *      Author: Trog
 */

#ifndef SWEXCEPTION_H_
#define SWEXCEPTION_H_
#include "SWString.h"

using namespace std;

class SWException: public exception
{
	SWString msg;

	void init()
	{
		lastException = this;
	}
public:
	static exception *lastException;

	SWException()
	{
		init();
	}
	SWException(const SWString &msg) :
		msg(msg)
	{
		init();
	}
	virtual ~SWException() throw();

	virtual const char *what() const throw()
	{
		return msg.c_str();
	}
};

#endif /* SWEXCEPTION_H_ */
