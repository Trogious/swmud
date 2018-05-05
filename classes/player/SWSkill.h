/*
 * SWSkill.h
 *
 *  Created on: 2010-07-09
 *      Author: Trog
 */

#ifndef SWSKILL_H_
#define SWSKILL_H_
#include <SWDbEntity.h>
#include <SWOwnerEntityId.h>
#include <flags.h>

namespace player
{

class SWSkill: public SWOwnerEntityId, public SWDbEntity
{
	SWString name;
	SWInt learned;

	SWDbEntityDataMapping getDataMapping() const;

public:
	SWSkill()
	{
	}

	SWSkill(const SWString &name, const SWInt &learned) :
		name(name), learned(learned)
	{
	}

	SWSkill(const SWString &name, const SWInt &learned, const SWInt &ownerEntityId) :
		SWOwnerEntityId(ownerEntityId), name(name), learned(learned)
	{
	}

	virtual ~SWSkill()
	{
	}

	const SWString getOwnerEntityFKey() const
	{
		return "pchar_id";
	}

	const SWString getTableName() const
	{
		return "skill_pchar";
	}

	const SWInt &getLearned() const
	{
		return learned;
	}

	const SWString &getName() const
	{
		return name;
	}

	void setLearned(const SWInt &learned)
	{
		this->learned = learned;
	}

	void setName(const SWString &name)
	{
		this->name = name;
	}

	virtual SWInt getType()
	{
		return SKILL_SKILL;
	}
};

}

#endif /* SWSKILL_H_ */
