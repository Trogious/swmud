/*
 * SWTimeStamp.cpp
 *
 *  Created on: Feb 22, 2010
 *      Author: Trog
 */

#include "SWTimeStamp.h"
#include <cstring>

SWTimeStamp::operator SWString()
{
	SWString str(getCorrectYear().toString());

	SWString part;

	formatNumber(part, getMonth() + 1);
	str << "-" << part;
	part.clear();

	formatNumber(part, getDayOfMonth());
	str << "-" << part;
	part.clear();

	str << " ";

	formatNumber(part, getHours());
	str << part;
	part.clear();

	formatNumber(part, getMinutes());
	str << ":" << part;
	part.clear();

	formatNumber(part, getSeconds());
	str << ":" << part;

	return str;
}

SWTimeStamp SWTimeStamp::fromString(const SWString &str)
{
	struct tm tm;
	bzero(&tm, sizeof(struct tm));

	if (str.length() > 18)
	{
		SWString dateTime = str.split(".")[0];
		vector<SWString> vDateTime = dateTime.split(" ");
		vector<SWString> vDate = vDateTime[0].split("-");
		vector<SWString> vTime = vDateTime[1].split(":");

		tm.tm_year = SWInt::fromString(vDate[0]) - 1900;
		tm.tm_mon = SWInt::fromString(vDate[1]) - 1;
		tm.tm_mday = SWInt::fromString(vDate[2]);
		tm.tm_hour = SWInt::fromString(vTime[0]) - 1;
		tm.tm_min = SWInt::fromString(vTime[1]);
		tm.tm_sec = SWInt::fromString(vTime[2]);
	}

	return SWTimeStamp(&tm);
}
