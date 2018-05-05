/*
 * SWTime.cpp
 *
 *  Created on: Feb 22, 2010
 *      Author: Trog
 */

#include "SWTime.h"
#include <cstring>

SWTime::operator SWString()
{
	SWString str;
	formatNumber(str, getHours());

	SWString part;

	formatNumber(part, getMinutes());
	str << ":" << part;
	part.clear();

	formatNumber(part, getSeconds());
	str << ":" << part;

	return str;
}

SWTime SWTime::fromString(const SWString &str)
{
	struct tm tm;
	bzero(&tm, sizeof(struct tm));

	if (str.length() > 7)
	{
		SWString sTime = str.split(".")[0];
		vector<SWString> vTime = sTime.split(":");

		tm.tm_hour = SWInt::fromString(vTime[0]) - 1;
		tm.tm_min = SWInt::fromString(vTime[1]);
		tm.tm_sec = SWInt::fromString(vTime[2]);
	}

	return SWTime(&tm);
}
