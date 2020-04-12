/*
 * SWDbEntity.cpp
 *
 *  Created on: 2010-07-10
 *      Author: Trog
 */

#include "SWDbEntity.h"

SWString SWDbEntity::getDeleteQuery() const
{
	SWString query("DELETE FROM ");
	query << getTableName() << getWhereId();
	return query;
}

SWString SWDbEntity::getInsertQuery() const
{
	return getDataMapping().getInsertQuery(getTableName());
}

SWString SWDbEntity::getUpdateQuery() const
{
	return getDataMapping().getUpdateQuery(getTableName(), getId());
}

SWString SWDbEntity::getWhereId() const
{
	return SWString(" WHERE id=").append(id);
}

SWString SWDbEntity::getUpdateSet() const
{
	SWString query("UPDATE ");
	query << getTableName() << " SET ";
	return query;
}

SWString &SWDbEntity::addUpdateSuffix(SWString &updateQueryInfix) const
{
	return updateQueryInfix.prepend(getUpdateSet()).append(getWhereId());
}

SWInt SWDbEntity::getLastId(SWDataBase *db) const
{
	db->singleQuery(SWString("SELECT last_value FROM ") << getTableName() << "_seq");
	list<SWString> columns;
	columns.push_back("last_value");
	SWQueryResult *result = db->getResult(columns);

	return SWInt::fromString((*result)[0]["last_value"]);
}

bool SWDbEntity::persist()
{
	if (modified)
	{
		SWDataBase db;
		if (id < 1)
		{
			if (!db.singleQuery(getInsertQuery()))
			{
				return false;
			}
			id = getLastId(&db);
		}
		else
		{
			if (!db.singleQuery(getUpdateQuery()))
			{
				return false;
			}
		}

		modified = false;
	}

	return true;
}

bool SWDbEntity::purge()
{
	if (id > 0)
	{
		modified = false;
		return SWDataBase().singleQuery(getDeleteQuery());
	}

	return true;
}

bool SWDbEntity::load(const SWString &query, SWDataBase *db)
{
	bool deleteNeeded = false;
	if (!db)
	{
		db = new SWDataBase;
		deleteNeeded = true;
	}

	db->beginTransaction();
	if (db->query(query))
	{
		SWDbEntityDataMapping mapping = getDataMapping();
		mapping.addMapping("id", SWDbEntityDataMappingProxy::createProxy(&SWDbEntity::setId, this));
		list<SWString> columns = mapping.getAllColumns();
		SWQueryResult *result = db->getResult(columns);
		if (!result || result->size() != 1)
		{
			if (deleteNeeded)
			{
				delete db;
			}
			return false;
		}

		mapping.setValues((*result)[0]);

		bool relatedLoaded = loadRelated(db);
		if (relatedLoaded)
		{
			db->commitTransaction();
		}

		if (deleteNeeded)
		{
			delete db;
		}
		return relatedLoaded;
	}

	if (deleteNeeded)
	{
		delete db;
	}
	return false;
}
