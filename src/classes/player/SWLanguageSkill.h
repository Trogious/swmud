/*
 * SWLanguageSkill.h
 *
 *  Created on: 2010-07-09
 *      Author: Trog
 */

#ifndef SWLANGUAGESKILL_H_
#define SWLANGUAGESKILL_H_
#include "SWSkill.h"

namespace player
{

class SWLanguageSkill: public SWSkill
{
public:
	SWLanguageSkill()
	{
	}

	SWLanguageSkill(const SWString &name, const SWInt &learned) :
		SWSkill(name, learned)
	{
	}

	SWInt getType()
	{
		return SKILL_TONGUE;
	}

	const SWString getTableName() const
	{
		return "language_pchar";
	}
};

}

#endif /* SWLANGUAGESKILL_H_ */
