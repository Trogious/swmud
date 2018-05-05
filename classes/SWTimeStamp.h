/*
 * SWTimeStamp.h
 *
 *  Created on: Feb 22, 2010
 *      Author: Trog
 */

#ifndef SWTIMESTAMP_H_
#define SWTIMESTAMP_H_
#include "SWDate.h"
#include "SWTime.h"

class SWTimeStamp: public SWDate, public SWTime
{
public:
	SWTimeStamp()
	{
	}
	SWTimeStamp(time_t time) :
		SWTimeBase(time), SWDate(time), SWTime(time)
	{
	}
	SWTimeStamp(struct tm *tm) :
		SWTimeBase(tm), SWDate(tm), SWTime(tm)
	{
	}

	operator SWString();

	operator const SWString() const
	{
		return static_cast<SWString> (const_cast<SWTimeStamp&> (*this));
	}
	bool operator<(const SWTimeStamp &ts) const
	{
		return time < ts.time;
	}
	bool operator==(const SWTimeStamp &ts) const
	{
		return time == ts.time;
	}

	SWString getDescriptive() const
	{
		return SWDate::getDescriptive() << ", godzina " << static_cast<const SWTime&> (*this);
	}

	static SWTimeStamp fromString(const SWString &str);
};

#endif /* SWTIMESTAMP_H_ */
