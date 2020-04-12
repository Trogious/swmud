/*
 * SWQuest.h
 *
 *  Created on: 2010-08-22
 *      Author: Trog
 */

#ifndef SWQUEST_H_
#define SWQUEST_H_
#include <SWDbEntity.h>
#include <SWOwnerEntityId.h>
#include <SWInt64.h>

namespace player
{

class SWQuest: public SWOwnerEntityId, public SWDbEntity
{
	static const char *table_name;

	SWInt64 questId;

	SWDbEntityDataMapping getDataMapping() const;

public:
	SWQuest()
	{
	}

	SWQuest(const SWInt64 &questId) :
		questId(questId)
	{
	}

	SWQuest(const SWInt64 &questId, const SWInt &ownerEntityId) :
		SWOwnerEntityId(ownerEntityId), questId(questId)
	{
	}

	const SWString getOwnerEntityFKey() const
	{
		return "pchar_id";
	}

	const SWString getTableName() const
	{
		return "quest_done_pchar";
	}

	const SWInt64 &getQuestId() const
	{
		return questId;
	}

	void setQuestId(const SWInt64 &questId)
	{
		this->questId = questId;
		setModified();
	}
};

}

#endif /* SWQUEST_H_ */
