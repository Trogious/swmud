/*
 * SWWeaponSkill.h
 *
 *  Created on: 2010-07-09
 *      Author: Trog
 */

#ifndef SWWEAPONSKILL_H_
#define SWWEAPONSKILL_H_
#include "SWSkill.h"

namespace player
{

class SWWeaponSkill: public SWSkill
{
public:
	SWWeaponSkill()
	{
	}

	SWWeaponSkill(const SWString &name, const SWInt &learned) :
		SWSkill(name, learned)
	{
	}

	SWInt getType()
	{
		return SKILL_WEAPON;
	}

	const SWString getTableName() const
	{
		return "weapon_pchar";
	}
};

}

#endif /* SWWEAPONSKILL_H_ */
