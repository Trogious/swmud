/*
 * SWFriend.cpp
 *
 *  Created on: 2010-07-15
 *      Author: Trog
 */

#include "SWFriend.h"
#include "../../defines.h"
#include "../../fun_decls.h"

namespace player
{

#define CREATE_PROXY(methodNamePart)	CREATE_PROXY2(methodNamePart,SWFriend)

SWDbEntityDataMapping SWFriend::getDataMapping() const
{
	SWDbEntityDataMapping mapping;

	mapping.addMapping(getOwnerEntityFKey(), getOwnerEntityId(), OWNER_PROXY);
	mapping.addMapping("name", getName(), CREATE_PROXY(Name));
	mapping.addMapping("sex", getSex(), CREATE_PROXY(Sex));
	mapping.addMapping("race", getRace()->name, CREATE_PROXY(Race));

	return mapping;
}

SWString SWFriend::getInsertQuery() const
{
	SWString query("INSERT INTO ");
	query << getTableName() << "(pchar_id,friend_id) VALUES(" << getOwnerEntityId()
			<< ",(SELECT id FROM pchar WHERE lower(name)=lower('" << getName() << "') ORDER BY cloned DESC LIMIT 1))";
	return query;
}

SWString SWFriend::getUpdateQuery() const
{
	SWString query;
	query << "pchar_id=" << getOwnerEntityId() << ", firend_id=(SELECT id FROM pchar WHERE lower(name)=lower('"
			<< getName() << "') ORDER BY cloned DESC LIMIT 1)";
	addUpdateSuffix(query);
	return query;
}

void SWFriend::setRace(const SWString &name)
{
	find_race(const_cast<char *>(name.c_str()));
}

}
