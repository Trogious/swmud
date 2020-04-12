/*
 * SWFake.cpp
 *
 *  Created on: Aug 10, 2010
 *      Author: Trog
 */

#include <SWFake.h>
#include <SWDbArray.h>

namespace player
{

#define CREATE_PROXY(methodNamePart)	CREATE_PROXY2(methodNamePart,SWFake)

SWDbEntityDataMapping SWFake::getDataMapping() const
{
	SWDbEntityDataMapping mapping;

	mapping.addMapping("name", getName(), CREATE_PROXY(Name));
	mapping.addMapping("inflect", getInflect(), CREATE_PROXY(Inflect));
	mapping.addMapping("title", getTitle(), CREATE_PROXY(Title));
	mapping.addMapping("long", getLong(), CREATE_PROXY(Long));
	mapping.addMapping("desc", getDesc(), CREATE_PROXY(Desc));

	return mapping;
}

}
