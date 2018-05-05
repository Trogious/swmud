/*
 * SWTime.h
 *
 *  Created on: Feb 22, 2010
 *      Author: Trog
 */

#ifndef SWTIME_H_
#define SWTIME_H_
#include "SWTimeBase.h"

class SWTime: virtual public SWTimeBase
{
public:
	SWTime()
	{
	}
	SWTime(time_t time) :
		SWTimeBase(time)
	{
	}
	SWTime(struct tm *tm) :
		SWTimeBase(tm)
	{
	}

	int getSeconds() const
	{
		return tm.tm_sec;
	}
	int getMinutes() const
	{
		return tm.tm_min;
	}
	int getHours() const
	{
		return tm.tm_hour;
	}
	int getDaylightSavingTime() const
	{
		return tm.tm_isdst;
	}

	SWTime &addSeconds(int seconds)
	{
		time += static_cast<time_t> (seconds);
		return *this;
	}

	SWTime &addMinutes(int minutes)
	{
		time += static_cast<time_t> (minutes * SECONDS_IN_MINUTE);
		return *this;
	}

	SWTime &addHours(int hours)
	{
		time += static_cast<time_t> (hours * SECONDS_IN_HOUR);
		return *this;
	}

	operator SWString();
	operator const SWString() const
	{
		return static_cast<SWString> (const_cast<SWTime&> (*this));
	}

	static SWTime fromString(const SWString &str);
};

#endif /* SWTIME_H_ */
