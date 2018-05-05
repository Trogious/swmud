/*
 * SWFriend.h
 *
 *  Created on: 2010-07-08
 *      Author: Trog
 */

#ifndef SWFRIEND_H_
#define SWFRIEND_H_
#include "../SWDbEntity.h"
#include "../SWOwnerEntityId.h"
#include "../../structs/typedefs.h"

namespace player
{

class SWFriend: public SWOwnerEntityId, public SWDbEntity
{
	SWString name;
	RACE_DATA *race;
	SWInt sex;
	SWInt remembered;

	SWDbEntityDataMapping getDataMapping() const;
	void setRace(const SWString &name);

public:
	SWFriend() :
		remembered(-1)
	{
	}

	SWFriend(const SWString &name, RACE_DATA * const race, const SWInt &sex, const SWInt &remembered = -1) :
		name(name), race(race), sex(sex), remembered(remembered)
	{
	}

	SWFriend(const SWString &name, RACE_DATA * const race, const SWInt &sex, const SWInt &ownerEntityId,
			const SWInt &remembered = -1) :
		SWOwnerEntityId(ownerEntityId), name(name), race(race), sex(sex), remembered(remembered)
	{
	}

	virtual ~SWFriend()
	{
	}

	const SWString getOwnerEntityFKey() const
	{
		return "pchar_id";
	}

	const SWString getTableName() const
	{
		return "friend_pchar";
	}

	const SWString &getName() const
	{
		return name;
	}

	const RACE_DATA *getRace() const
	{
		return race;
	}

	const SWInt &getSex() const
	{
		return sex;
	}

	void setName(const SWString &name)
	{
		this->name = name;
	}

	void setRace(RACE_DATA * const race)
	{
		this->race = race;
	}

	void setSex(const SWInt &sex)
	{
		this->sex = sex;
	}

	virtual SWString getInsertQuery() const;
	virtual SWString getUpdateQuery() const;
};

}

#endif /* SWFRIEND_H_ */
