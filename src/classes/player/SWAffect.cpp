/*
 * SWAffect.cpp
 *
 *  Created on: 2010-07-15
 *      Author: Trog
 */

#include "SWAffect.h"
#include <sys/time.h>
#include "../../structs/timerset.h"
#include "../../structs/skill_type.h"
#include "../../fun_decls.h"

namespace player
{

SWAffect::~SWAffect()
{
}

#define CREATE_PROXY(methodNamePart)	CREATE_PROXY2(methodNamePart,SWAffect)

SWDbEntityDataMapping SWAffect::getDataMapping() const
{
	SWDbEntityDataMapping mapping;

	mapping.addMapping(getOwnerEntityFKey(), getOwnerEntityId(), OWNER_PROXY);
	mapping.addMapping("type", getType(), CREATE_PROXY(Type));
	mapping.addMapping("duration", getDuration(), CREATE_PROXY(Duration));
	mapping.addMapping("modifier", getModifier(), CREATE_PROXY(Modifier));
	mapping.addMapping("location", getLocation(), CREATE_PROXY(Location));
	mapping.addMapping("bitvector", getBitvector(), CREATE_PROXY(Bitvector));

	return mapping;
}

SWString SWAffect::getInsertQuery() const
{
	SWString query("INSERT INTO ");
	SWString field;
	SWString fieldValue;
	if (isAffectData())
	{
		field = "\"name\"";
		SK *skill = get_skilltype(type);
		fieldValue << "'" << (skill ? skill->name : "unknown") << "'";
	}
	else
	{
		field = "\"type\"";
		fieldValue = getType().toString();

	}
	query << getTableName() << "(" << getOwnerEntityFKey() << ",duration,modifier,\"location\",bitvector," << field
			<< ") VALUES(" << getOwnerEntityId() << "," << getDuration() << "," << getModifier() << ","
			<< getLocation() << "," << fieldValue << ")";
	return query;
}

SWString SWAffect::getUpdateQuery() const
{
	SWString query(" ");
	SWString field;
	SWString fieldValue;
	if (isAffectData())
	{
		field = "\"name\"";
		SK *skill = get_skilltype(type);
		fieldValue << "'" << (skill ? skill->name : "unknown") << "'";
	}
	else
	{
		field = "\"type\"";
		fieldValue = getType().toString();

	}
	query << getOwnerEntityFKey() << "=" << getOwnerEntityId() << ", duration=" << getDuration() << ", modifier="
			<< getModifier() << ", \"location\"=" << getLocation() << ", bitvector=" << getBitvector() << ", " << field
			<< "=" << fieldValue;
	addUpdateSuffix(query);
	return query;
}

}
