/*
 * SWCommand.cpp
 *
 *  Created on: 2010-07-15
 *      Author: Trog
 */

#include "SWCommand.h"

namespace player
{

SWCommand::~SWCommand()
{
}

#define CREATE_PROXY(methodNamePart)	CREATE_PROXY2(methodNamePart,SWCommand)

SWDbEntityDataMapping SWCommand::getDataMapping() const
{
	SWDbEntityDataMapping mapping;

	mapping.addMapping(getOwnerEntityFKey(), getOwnerEntityId(), OWNER_PROXY);
	mapping.addMapping("cmd", getCmd(), CREATE_PROXY(Cmd));

	return mapping;
}

}
