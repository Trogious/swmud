/*
 * SWForceSkill.h
 *
 *  Created on: 2010-07-09
 *      Author: Trog
 */

#ifndef SWFORCESKILL_H_
#define SWFORCESKILL_H_
#include "SWSkill.h"

namespace player
{

class SWForceSkill: public SWSkill
{
public:
	SWForceSkill()
	{
	}

	SWForceSkill(const SWString &name, const SWInt &learned) :
		SWSkill(name, learned)
	{
	}

	SWInt getType()
	{
		return SKILL_SPELL;
	}

	const SWString getTableName() const
	{
		return "force_pchar";
	}
};

}

#endif /* SWFORCESKILL_H_ */
