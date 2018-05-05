/*
 * SWAffectPlayer.h
 *
 *  Created on: 2010-09-08
 *      Author: Trog
 */

#ifndef SWAFFECTPLAYER_H_
#define SWAFFECTPLAYER_H_
#include <SWAffect.h>

namespace player
{

class SWAffectPlayer: public SWAffect
{
public:
	SWAffectPlayer()
	{
	}

	SWAffectPlayer(const SWInt &type, const SWInt &duration, const SWInt &modifier, const SWInt &location,
			const SWInt64 &bitvector) :
		SWAffect(type, duration, modifier, location, bitvector)
	{
	}

	SWAffectPlayer(const SWInt &type, const SWInt &duration, const SWInt &modifier, const SWInt &location,
			const SWInt64 &bitvector, const SWInt &ownerEntityId) :
		SWAffect(type, duration, modifier, location, bitvector, ownerEntityId)
	{
	}

	const SWString getOwnerEntityFKey() const
	{
		return "pchar_id";
	}

	const SWString getTableName() const
	{
		return isAffectData() ? "affect_data_pchar" : "affect_pchar";
	}
};

}

#endif /* SWAFFECTPLAYER_H_ */
