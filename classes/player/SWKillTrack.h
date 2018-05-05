/*
 * SWKillTrack.h
 *
 *  Created on: 2010-08-22
 *      Author: Trog
 */

#ifndef SWKILLTRACK_H_
#define SWKILLTRACK_H_
#include <SWDbEntity.h>
#include <SWOwnerEntityId.h>
#include <SWInt64.h>

namespace player
{

class SWKillTrack: public SWOwnerEntityId, public SWDbEntity
{
	static const char *table_name;

	SWInt64 vnum;
	SWInt killed;

	SWDbEntityDataMapping getDataMapping() const;

public:
	SWKillTrack()
	{
	}

	SWKillTrack(const SWInt64 &vnum, const SWInt killed) :
		vnum(vnum), killed(killed)
	{
	}

	SWKillTrack(const SWInt64 &vnum, const SWInt killed, const SWInt &ownerEntityId) :
		SWOwnerEntityId(ownerEntityId), vnum(vnum), killed(killed)
	{
	}

	const SWString getTableName() const
	{
		return "killtrack_pchar";
	}

	const SWInt &getKilled() const
	{
		return killed;
	}

	const SWInt64 &getVnum() const
	{
		return vnum;
	}

	void setKilled(const SWInt &killed)
	{
		this->killed = killed;
	}

	void setVnum(const SWInt64 &vnum)
	{
		this->vnum = vnum;
	}

	const SWKillTrack &operator++()
	{
		killed++;
		return *this;
	}

	const SWKillTrack operator++(int)
	{
		SWKillTrack k(*this);
		killed++;
		return k;
	}

	const SWString getOwnerEntityFKey() const
	{
		return "pchar_id";
	}
};

}

#endif /* SWKILLTRACK_H_ */
