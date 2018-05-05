/*
 * SWTimeBase.h
 *
 *  Created on: Feb 22, 2010
 *      Author: Trog
 */

#ifndef SWTIMEBASE_H_
#define SWTIMEBASE_H_
#include <time.h>
#include "SWString.h"
#include "SWInt.h"

class SWTimeBase
{
protected:
	time_t time;
	struct tm tm;

	void init()
	{
		localtime_r(&time, &tm);
	}

public:
	static const int SECONDS_IN_MINUTE = 60;
	static const int MINUTES_IN_HOUR = 60;
	static const int HOURS_IN_DAY = 24;
	static const int DAYS_IN_MONTH = 30; /* ~30 */ //FIXME: make reliable based on the year
	static const int MONTHS_IN_YEAR = 12;
	static const int SECONDS_IN_HOUR = SECONDS_IN_MINUTE * MINUTES_IN_HOUR;
	static const int SECONDS_IN_DAY = SECONDS_IN_HOUR * HOURS_IN_DAY;
	/*
	static const int SECONDS_IN_MONTH = SECONDS_IN_DAY * DAYS_IN_MONTH;
	static const int SECONDS_IN_YEAR = SECONDS_IN_MONTH * MONTHS_IN_YEAR;
	*/

	SWTimeBase() :
		time(::time(0))
	{
		init();
	}
	SWTimeBase(time_t time) :
		time(time)
	{
		init();
	}
	SWTimeBase(struct tm *tm) :
		time(mktime(tm))
	{
		init();
	}
	virtual ~SWTimeBase() = 0;

	static SWString &formatNumber(SWString &str, const SWInt &number)
	{
		if (number < 10)
		{
			str << "0";
		}
		str << number;

		return str;
	}

	time_t getTimeT() const
	{
		return time;
	}
};

#endif /* SWTIMEBASE_H_ */
