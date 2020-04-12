/*
 * SWDbEntityDataMapping.cpp
 *
 *  Created on: 01-01-2011
 *      Author: Trog
 */

#include "SWDbEntityDataMapping.h"

void SWDbEntityDataMapping::addMapping(const SWString &column, const SWString &value,
		SWDbEntityDataMappingProxy::Proxy *proxy)
{
	SWString *val = new SWString(value);
	val->prepend("'");
	val->append("'");
	addMapping(column, val, proxy);
}

const list<SWString> SWDbEntityDataMapping::getNonNullColumns() const
{
	list<SWString> cols;

	for (list<MappingElement>::const_iterator it = mapping.begin(); it != mapping.end(); ++it)
	{
		if (it->second)
		{
			cols.push_back(it->first);
		}
	}

	return cols;
}

const list<SWString> SWDbEntityDataMapping::getAllColumns() const
{
	list<SWString> cols;

	for (list<MappingElement>::const_iterator it = mapping.begin(); it != mapping.end(); ++it)
	{
		cols.push_back(it->first);
	}

	return cols;
}

SWString SWDbEntityDataMapping::getInsertQuery(const SWString &tableName) const
{
	SWString query("INSERT INTO ");
	query << tableName << " (";

	list<MappingElement>::const_iterator it = mapping.begin();
	for (; it != mapping.end(); ++it)
	{
		if (it->second)
		{
			query << it->first;
			break;
		}
	}
	for (++it; it != mapping.end(); ++it)
	{
		if (it->second)
		{
			query << "," << it->first;
		}
	}

	query << ") VALUES(";

	it = mapping.begin();
	for (; it != mapping.end(); ++it)
	{
		if (it->second)
		{
			query << *it->second;
			break;
		}
	}
	for (++it; it != mapping.end(); ++it)
	{
		if (it->second)
		{
			query << "," << *it->second;
		}
	}
	query << ")";

	return query;
}

SWString SWDbEntityDataMapping::getUpdateQuery(const SWString &tableName, const SWInt &id) const
{
	SWString query("UPDATE ");
	query << tableName << " SET ";

	list<MappingElement>::const_iterator it = mapping.begin();
	for (; it != mapping.end(); ++it)
	{
		if (it->second)
		{
			query << it->first << "=" << *it->second;
			break;
		}
	}
	for (++it; it != mapping.end(); ++it)
	{
		if (it->second)
		{
			query << "," << it->first << "=" << *it->second;
		}
	}

	query << " WHERE id=" << id;

	return query;
}

void SWDbEntityDataMapping::setValues(SWQueryResult::SWQueryResultMap &resultMap)
{
	for (list<MappingElement>::iterator it = mapping.begin(); it != mapping.end(); ++it)
	{
		if (it->proxy)
		{
			it->proxy->setValue(resultMap[it->first]);
		}
	}
}
