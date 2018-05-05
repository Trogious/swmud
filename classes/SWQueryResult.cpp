/*
 * SWQueryResult.cpp
 *
 *  Created on: 2009-09-11
 *      Author: Trog
 */

#include "SWQueryResult.h"
#include "SWInt.h"
#include "SWDataBase.h"

void SWQueryResult::dispose()
{
	dataBase->results.remove(this);
}

SWQueryResult::SWQueryResult(SWDataBase *dataBase) :
	dataBase(dataBase)
{
	dataBase->results.push_back(this);
}

const SWString &SWQueryResult::SWQueryResultMap::operator[](unsigned int columnNo) const
{
	unsigned int cNo = 0;
	for (SWResultMapType::const_iterator it = resultMap.begin(); it != resultMap.end(); it++)
	{
		if (cNo++ == columnNo)
		{
			return it->second;
		}
	}
	throw new SWException(SWString("no such column number: ") + SWInt::toString(columnNo));
}

SWQueryResult::SWQueryResultMap &SWQueryResult::operator[](unsigned int rowNo)
{
	if (resultEntries.empty() || rowNo >= resultEntries.size())
	{
		throw new SWException(SWString("no such row in result, row: ").append(SWInt::toString(rowNo)));
	}
	return resultEntries[rowNo];
}
