/*
 * SWAbility.h
 *
 *  Created on: 2010-07-09
 *      Author: Trog
 */

#ifndef SWABILITY_H_
#define SWABILITY_H_
#include "../SWDbEntity.h"
#include "../SWInt64.h"
#include "../SWOwnerEntityId.h"
#include "../../defines.h"

namespace player
{

class SWAbility: public SWOwnerEntityId, public SWDbEntity
{
	SWInt no;
	SWInt level;
	SWInt64 xp;

	SWDbEntityDataMapping getDataMapping() const;

public:
	SWAbility()
	{
	}

	SWAbility(const SWInt &no, const SWInt &level, const SWInt64 &xp) :
		level(level), xp(xp)
	{
		setNo(no);
	}

	SWAbility(const SWInt &no, const SWInt & level, const SWInt64 &xp, const SWInt &ownerEntityId) :
		SWOwnerEntityId(ownerEntityId), level(level), xp(xp)
	{
		setNo(no);
	}

	virtual ~SWAbility()
	{
	}

	const SWString getOwnerEntityFKey() const
	{
		return "pchar_id";
	}

	const SWString getTableName() const
	{
		return "ability_pchar";
	}

	const SWInt &getLevel() const
	{
		return level;
	}

	const SWInt &getNo() const
	{
		return no;
	}

	const SWInt64 &getXp() const
	{
		return xp;
	}

	void setLevel(const SWInt &level)
	{
		this->level = level;
	}

	void setNo(const SWInt &no)
	{
		this->no = (no >= 0 && no < MAX_ABILITY) ? no : SWInt(-1);
	}

	void setXp(const SWInt64 &xp)
	{
		this->xp = xp;
	}
};

}

#endif /* SWABILITY_H_ */
