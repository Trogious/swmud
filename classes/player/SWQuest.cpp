/*
 * SWQuest.cpp
 *
 *  Created on: 2010-08-22
 *      Author: Trog
 */

#include "SWQuest.h"

namespace player
{

const char *SWQuest::table_name = "quest_done_pchar";

#define CREATE_PROXY(methodNamePart)	CREATE_PROXY2(methodNamePart,SWQuest)

SWDbEntityDataMapping SWQuest::getDataMapping() const
{
	SWDbEntityDataMapping mapping;

	mapping.addMapping(getOwnerEntityFKey(), getOwnerEntityId(), OWNER_PROXY);
	mapping.addMapping("quest_id", getQuestId(), CREATE_PROXY(QuestId));

	return mapping;
}

}
