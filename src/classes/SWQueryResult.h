/*
 * SWQueryResult.h
 *
 *  Created on: 2009-09-11
 *      Author: Trog
 */

#ifndef SWQUERYRESULT_H_
#define SWQUERYRESULT_H_
#include <vector>
#include <map>
#include "SWString.h"
#include "SWException.h"
#include "SWInt.h"

class SWDataBase;

class SWQueryResult
{
public:
	class SWQueryResultMap
	{
		typedef map<SWString, SWString> SWResultMapType;
		SWResultMapType resultMap;

	public:
		virtual ~SWQueryResultMap()
		{
		}

		const SWString &operator[](const SWString &columnName)
		{
			return resultMap[columnName];
		}

		const SWString &operator[](unsigned int) const;

		void add(const SWString &columnName, const SWString &cellValue)
		{
			resultMap[columnName] = cellValue;
		}
	};

private:
	typedef vector<SWQueryResultMap> SWResultType;
	SWDataBase *dataBase;
	SWResultType resultEntries;

	void dispose();

	friend class SWDataBase;

public:
	SWQueryResult(SWDataBase *dataBase);
	virtual ~SWQueryResult()
	{
	}

	typedef SWResultType::size_type size_type;

	size_type size() const
	{
		return resultEntries.size();
	}

	SWQueryResultMap &operator[](unsigned int);

	void add(const SWQueryResultMap &resultEntry)
	{
		resultEntries.insert(resultEntries.end(), resultEntry);
	}
};

#endif /* SWQUERYRESULT_H_ */
