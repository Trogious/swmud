/*
 * SWHome.cpp
 *
 *  Created on: 2010-07-15
 *      Author: Trog
 */

#include "SWHome.h"

namespace player
{

SWDbEntityDataMapping SWHome::getDataMapping() const
{
	SWDbEntityDataMapping mapping;

	//TODO: implement

	return mapping;
}

bool SWHome::loadRelated(SWDataBase *db)
{
	return true;//TODO: implement
}

SWString SWHome::getInsertQuery() const
{
	SWString query("INSERT INTO ");
	query << getTableName() << "(pdata_id,vnum) VALUES(" << getOwnerEntityId() << "," << getVnum() << ")";
	return query;
}

SWString SWHome::getUpdateQuery() const
{
	SWString query;
	query << "pdata_id=" << getOwnerEntityId() << ", " << "vnum=" << getVnum();
	addUpdateSuffix(query);
	return query;
}

}
