/*
 * SWItem.cpp
 *
 *  Created on: 2010-08-26
 *      Author: Trog
 */

#include "SWItem.h"
#include <SWPlayerCharacter.h>

namespace player
{

const SWInt SWItem::valueMaxElements = 6;

SWItem::SWItem() :
	inflect(vector<SWString> (SWPlayerCharacter::inflectMaxElements)), value(vector<SWInt> (valueMaxElements)),
			strValue(vector<SWString> (valueMaxElements))
{
}

SWItem::SWItem(const SWInt64 vnum) :
	vnum(vnum), inflect(vector<SWString> (SWPlayerCharacter::inflectMaxElements)), value(vector<SWInt> (
			valueMaxElements)), strValue(vector<SWString> (valueMaxElements))

{
}

SWItem::SWItem(const SWInt64 vnum, const SWInt &ownerEntityId) :
	SWOwnerEntityId(ownerEntityId), vnum(vnum), inflect(vector<SWString> (SWPlayerCharacter::inflectMaxElements)),
			value(vector<SWInt> (valueMaxElements)), strValue(vector<SWString> (valueMaxElements))
{
}

SWItem::~SWItem()
{
}

#define CREATE_PROXY(methodNamePart)	CREATE_PROXY2(methodNamePart,SWItem)

SWDbEntityDataMapping SWItem::getDataMapping() const
{
	SWDbEntityDataMapping mapping;

	mapping.addMapping("vnum", getVnum(), CREATE_PROXY(Vnum));
	mapping.addMapping("owner_vnum", getOwnerVnum(), CREATE_PROXY(OwnerVnum));
	mapping.addMapping("count", getCount(), CREATE_PROXY(Count));
	mapping.addMapping("name", getName(), CREATE_PROXY(Name));
	mapping.addMapping("inflect", getInflect(), CREATE_PROXY(Inflect));
	mapping.addMapping("description", getDescription(), CREATE_PROXY(Description));
	mapping.addMapping("action_desc", getActionDescription(), CREATE_PROXY(ActionDescription));
	mapping.addMapping("owner_name", getOwnerName(), CREATE_PROXY(OwnerName));
	mapping.addMapping("in_room", getInRoom(), CREATE_PROXY(InRoom));
	mapping.addMapping("extra_flags", getExtraFlags(), CREATE_PROXY(ExtraFlags));
	mapping.addMapping("wear_flags", getWearFlags(), CREATE_PROXY(WearFlags));
	mapping.addMapping("wear_loc", getWearLoc(), CREATE_PROXY(WearLoc));
	mapping.addMapping("item_type", getItemType(), CREATE_PROXY(ItemType));
	mapping.addMapping("weight", getWeight(), CREATE_PROXY(Weight));
	mapping.addMapping("level", getLevel(), CREATE_PROXY(Level));
	mapping.addMapping("timer", getTimer(), CREATE_PROXY(Timer));
	mapping.addMapping("cost", getCost(), CREATE_PROXY(Cost));
	mapping.addMapping("gender", getGender(), CREATE_PROXY(Gender));
	mapping.addMapping("value", getValue(), CREATE_PROXY(Value));
	mapping.addMapping("str_value", getStrValue(), CREATE_PROXY(StrValue));
	mapping.addMapping("in_quest", getInQuest(), CREATE_PROXY(InQuest));

	return mapping;
}

}
