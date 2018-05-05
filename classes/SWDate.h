/*
 * SWDate.h
 *
 *  Created on: Feb 22, 2010
 *      Author: Trog
 */

#ifndef SWDATE_H_
#define SWDATE_H_
#include "SWTimeBase.h"

class SWDate: virtual public SWTimeBase
{
	static const int MAX_WEEK_DAYS = 7;
	static const SWString WEEK_DAY_NAME[MAX_WEEK_DAYS];
	static const int MAX_MONTHS = 12;
	static const SWString MONTH_NAME[MAX_MONTHS];
	static const SWString MONTH_NAME2[MAX_MONTHS]; /*!< nazwa miesiaca w dopelniaczu */
public:
	static const int MAX_DESCRIPTIVE_LEN = 58;

	SWDate()
	{
	}
	SWDate(time_t time) :
		SWTimeBase(time)
	{
	}
	SWDate(struct tm *tm) :
		SWTimeBase(tm)
	{
	}

	SWInt getMonth() const
	{
		return tm.tm_mon;
	}
	SWInt getYear() const
	{
		return tm.tm_year;
	}
	SWInt getCorrectYear() const
	{
		return tm.tm_year + 1900;
	}
	SWInt getDayOfWeek() const
	{
		return tm.tm_wday;
	}
	SWInt getDayOfMonth() const
	{
		return tm.tm_mday;
	}
	SWInt getDayOfYear() const
	{
		return tm.tm_yday;
	}

	SWDate &addDays(const SWInt &days)
	{
		time += static_cast<time_t> (days * SWTimeBase::SECONDS_IN_DAY);
		init();
		return *this;
	}
	/*
	 SWDate &addMonths(SWInt months)
	 {
	 time += static_cast<time_t> (months * SECONDS_IN_MONTH);
	 init();
	 return *this;
	 }

	 SWDate &addYears(SWInt years)
	 {
	 time += static_cast<time_t> (years * SECONDS_IN_YEAR);
	 init();
	 return *this;
	 }
	 */
	const SWString &getWeekDayName() const
	{
		return WEEK_DAY_NAME[tm.tm_wday];
	}

	const SWString &getMonthName() const
	{
		return MONTH_NAME[tm.tm_mon];
	}

	virtual SWString getDescriptive() const
	{
		return SWString(WEEK_DAY_NAME[tm.tm_wday]) << ", " << SWInt(tm.tm_mday) << " " << MONTH_NAME2[tm.tm_mon] << " "
				<< SWInt(getCorrectYear()) << " roku";
	}

	operator SWString();
	operator const SWString() const
	{
		return static_cast<SWString> (const_cast<SWDate&> (*this));
	}

	static SWDate fromString(const SWString &str);
};

#endif /* SWDATE_H_ */
