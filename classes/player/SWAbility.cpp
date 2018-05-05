/*
 * SWAbility.h.cpp
 *
 *  Created on: 2010-07-15
 *      Author: Trog
 */

#include "SWAbility.h"

namespace player
{

#define CREATE_PROXY(methodNamePart)	CREATE_PROXY2(methodNamePart,SWAbility)

SWDbEntityDataMapping SWAbility::getDataMapping() const
{
	SWDbEntityDataMapping mapping;

	mapping.addMapping(getOwnerEntityFKey(), getOwnerEntityId(), OWNER_PROXY);
	mapping.addMapping("no", getNo(), CREATE_PROXY(No));
	mapping.addMapping("level", getLevel(), CREATE_PROXY(Level));
	mapping.addMapping("experience", getXp(), CREATE_PROXY(Xp));

	return mapping;
}

}
