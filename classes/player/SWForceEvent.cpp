/*
 * SWForceEvent.cpp
 *
 *  Created on: 2010-07-15
 *      Author: Trog
 */

#include "SWForceEvent.h"
#include "../SWDbArray.h"

namespace player
{

#define CREATE_PROXY(methodNamePart)	CREATE_PROXY2(methodNamePart,SWForceEvent)

SWDbEntityDataMapping SWForceEvent::getDataMapping() const
{
	SWDbEntityDataMapping mapping;

	mapping.addMapping(getOwnerEntityFKey(), getOwnerEntityId(), OWNER_PROXY);
	mapping.addMapping("trigger", getTrigger(), CREATE_PROXY(Trigger));
	mapping.addMapping("string_attr", getStringAttr(), CREATE_PROXY(StringAttr));
	mapping.addMapping("attributes", attribute, CREATE_PROXY(Attributes));

	return mapping;
}

SWString SWForceEvent::getInsertQuery() const
{
	SWString query("INSERT INTO ");
	query << getTableName() << "(pchar_id,\"trigger\",string_attr,attributes) VALUES(" << getOwnerEntityId() << ","
			<< getTrigger() << ",'" << getStringAttr() << "'," << SWDbArray::toQueryValue(attribute) << ")";

	return query;
}

SWString SWForceEvent::getUpdateQuery() const
{
	SWString query;
	query << "pchar_id=" << getOwnerEntityId() << ", \"trigger\"=" << getTrigger() << ", string_attr='"
			<< getStringAttr() << "', attributes=" << SWDbArray::toQueryValue(attribute);
	addUpdateSuffix(query);
	return query;
}

}
