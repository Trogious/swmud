/*
 * SWSkill.cpp
 *
 *  Created on: 2010-07-15
 *      Author: Trog
 */

#include "SWSkill.h"

namespace player
{

#define CREATE_PROXY(methodNamePart)	CREATE_PROXY2(methodNamePart,SWSkill)

SWDbEntityDataMapping SWSkill::getDataMapping() const
{
	SWDbEntityDataMapping mapping;

	mapping.addMapping(getOwnerEntityFKey(), getOwnerEntityId(), OWNER_PROXY);
	mapping.addMapping("name", getName(), CREATE_PROXY(Name));
	mapping.addMapping("learned", getLearned(), CREATE_PROXY(Learned));

	return mapping;
}

}
