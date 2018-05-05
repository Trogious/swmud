/*
 * SWDate.cpp
 *
 *  Created on: Feb 22, 2010
 *      Author: Trog
 */

#include "SWDate.h"
#include <cstring>

const SWString SWDate::WEEK_DAY_NAME[MAX_WEEK_DAYS] =
{ "Niedziela", "Poniedzia�ek", "Wtorek", "�roda", "Czwartek", "Pi�tek", "Sobota" };
const SWString SWDate::MONTH_NAME[MAX_MONTHS] =
{ "Stycze�", "Luty", "Marzec", "Kwiecie�", "Maj", "Czerwiec", "Lipiec", "Sierpie�", "Wrzesie�", "Pa�dziernik",
		"Listopad", "Grudzie�" };
const SWString SWDate::MONTH_NAME2[MAX_MONTHS] =
{ "Stycznia", "Lutego", "Marca", "Kwietnia", "Maja", "Czerwca", "Lipca", "Sierpnia", "Wrze�nia", "Pa�dziernika",
		"Listopada", "Grudnia" };

SWDate::operator SWString()
{
	SWString str(SWInt::toString(getYear() + 1900));

	SWString part;

	formatNumber(part, getMonth() + 1);
	str << "-" << part;
	part.clear();

	formatNumber(part, getDayOfMonth());
	str << "-" << part;

	return str;
}

SWDate SWDate::fromString(const SWString &str)
{
	struct tm tm;
	bzero(&tm, sizeof(struct tm));

	if (str.length() > 9)
	{
		vector<SWString> vDate = str.split("-");

		tm.tm_year = SWInt::fromString(vDate[0]) - 1900;
		tm.tm_mon = SWInt::fromString(vDate[1]) - 1;
		tm.tm_mday = SWInt::fromString(vDate[2]);
	}

	return SWDate(&tm);
}
