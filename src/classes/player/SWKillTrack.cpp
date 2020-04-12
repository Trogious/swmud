/*
 * SWKillTrack.cpp
 *
 *  Created on: 2010-08-22
 *      Author: Trog
 */

#include "SWKillTrack.h"

namespace player
{

const char *SWKillTrack::table_name = "killtrack_pchar";

#define CREATE_PROXY(methodNamePart)	CREATE_PROXY2(methodNamePart,SWKillTrack)

SWDbEntityDataMapping SWKillTrack::getDataMapping() const
{
	SWDbEntityDataMapping mapping;

	mapping.addMapping(getOwnerEntityFKey(), getOwnerEntityId(), OWNER_PROXY);
	mapping.addMapping("vnum", getVnum(), CREATE_PROXY(Vnum));
	mapping.addMapping("killed", getKilled(), CREATE_PROXY(Killed));

	return mapping;
}

}
