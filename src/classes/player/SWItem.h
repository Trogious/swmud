/*
 * SWItem.h
 *
 *  Created on: 2010-08-26
 *      Author: Trog
 */

#ifndef SWITEM_H_
#define SWITEM_H_
#include <SWDbEntity.h>
#include <SWOwnerEntityId.h>
#include <SWInt64.h>
#include <vector>
#include <SWDbEntityDataMapping.h>

namespace player
{

class SWItem: public SWOwnerEntityId, public SWDbEntity
{
	static const SWInt valueMaxElements;

	SWInt64 vnum;
	SWInt64 ownerVnum; /*!< vnum przedmiotu w ktorym ten przedmiot sie znajduje */
	SWInt count;
	SWString name;
	vector<SWString> inflect;
	SWString description;
	SWString actionDescription;
	SWString ownerName;
	SWInt64 inRoom;
	SWInt64 extraFlags;
	SWInt64 wearFlags;
	SWInt wearLoc;
	SWInt itemType;
	SWInt weight;
	SWInt level;
	SWInt timer;
	SWInt cost;
	SWInt gender;
	vector<SWInt> value;
	vector<SWString> strValue;
	SWInt64 inQuest;

	SWDbEntityDataMapping getDataMapping() const;

public:
	SWItem();

	SWItem(const SWInt64 vnum);

	SWItem(const SWInt64 vnum, const SWInt &ownerEntityId);

	virtual ~SWItem() = 0;

	const SWString &getActionDescription() const
	{
		return actionDescription;
	}

	const SWInt &getCost() const
	{
		return cost;
	}

	const SWInt &getCount() const
	{
		return count;
	}

	const SWString &getDescription() const
	{
		return description;
	}

	const SWInt64 &getExtraFlags() const
	{
		return extraFlags;
	}

	const SWInt &getGender() const
	{
		return gender;
	}

	const SWInt64 &getInQuest() const
	{
		return inQuest;
	}

	const SWInt64 &getInRoom() const
	{
		return inRoom;
	}

	const vector<SWString> &getInflect() const
	{
		return inflect;
	}

	const SWInt &getItemType() const
	{
		return itemType;
	}

	const SWInt &getLevel() const
	{
		return level;
	}

	const SWString &getName() const
	{
		return name;
	}

	const SWString &getOwnerName() const
	{
		return ownerName;
	}

	const SWInt64 &getOwnerVnum() const
	{
		return ownerVnum;
	}

	const vector<SWString> &getStrValue() const
	{
		return strValue;
	}

	const SWInt &getTimer() const
	{
		return timer;
	}

	const vector<SWInt> &getValue() const
	{
		return value;
	}

	const SWInt64 &getVnum() const
	{
		return vnum;
	}

	const SWInt64 &getWearFlags() const
	{
		return wearFlags;
	}

	const SWInt &getWearLoc() const
	{
		return wearLoc;
	}

	const SWInt &getWeight() const
	{
		return weight;
	}

	void setActionDescription(const SWString &actionDescription)
	{
		this->actionDescription = actionDescription;
	}

	void setCost(const SWInt &cost)
	{
		this->cost = cost;
	}

	void setCount(const SWInt &count)
	{
		this->count = count;
	}

	void setDescription(const SWString &description)
	{
		this->description = description;
	}

	void setExtraFlags(const SWInt64 &extraFlags)
	{
		this->extraFlags = extraFlags;
	}

	void setGender(const SWInt &gender)
	{
		this->gender = gender;
	}

	void setInQuest(const SWInt64 &inQuest)
	{
		this->inQuest = inQuest;
	}

	void setInRoom(const SWInt64 &inRoom)
	{
		this->inRoom = inRoom;
	}

	void setInflect(const vector<SWString> &inflect)
	{
		this->inflect = inflect;
	}

	void setInflect(const SWInt &no, const SWString &inflect)
	{
		this->inflect[no] = inflect;
	}

	void setItemType(const SWInt &itemType)
	{
		this->itemType = itemType;
	}

	void setLevel(const SWInt &level)
	{
		this->level = level;
	}

	void setName(const SWString &name)
	{
		this->name = name;
	}

	void setOwnerName(const SWString &ownerName)
	{
		this->ownerName = ownerName;
	}

	void setOwnerVnum(const SWInt64 &ownerVnum)
	{
		this->ownerVnum = ownerVnum;
	}

	void setStrValue(const vector<SWString> &strValue)
	{
		this->strValue = strValue;
	}

	void setStrValue(const SWInt &no, const SWString &strValue)
	{
		this->strValue[no] = strValue;
	}

	void setTimer(const SWInt &timer)
	{
		this->timer = timer;
	}

	void setValue(const vector<SWInt> &value)
	{
		this->value = value;
	}

	void setValue(const SWInt &no, const SWInt &value)
	{
		this->value[no] = value;
	}

	void setVnum(const SWInt64 &vnum)
	{
		this->vnum = vnum;
	}

	void setWearFlags(const SWInt64 &wearFlags)
	{
		this->wearFlags = wearFlags;
	}

	void setWearLoc(const SWInt &wearLoc)
	{
		this->wearLoc = wearLoc;
	}

	void setWeight(const SWInt &weight)
	{
		this->weight = weight;
	}

	template<class T>
	static SWItem *removeItem(T &container, const SWInt64 &vnum);
};

template<class T>
SWItem *SWItem::removeItem(T &container, const SWInt64 &vnum)
{
	SWItem *item = 0;
	for (typename T::iterator it = container.begin(); it != container.end(); ++it)
	{
		if ((*it)->getVnum() == vnum)
		{
			item = *it;
			container.erase(it);
			break;
		}
	}

	return item;
}

}

#endif /* SWITEM_H_ */
