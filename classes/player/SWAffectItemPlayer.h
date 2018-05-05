/*
 * SWAffectItemPlayer.h
 *
 *  Created on: 2010-09-15
 *      Author: Trog
 */

#ifndef SWAFFECTITEMPLAYER_H_
#define SWAFFECTITEMPLAYER_H_
#include <SWAffectItem.h>

namespace player
{

class SWAffectItemPlayer: public SWAffectItem
{
public:
	SWAffectItemPlayer()
	{
	}

	SWAffectItemPlayer(const SWInt &type, const SWInt &duration, const SWInt &modifier, const SWInt &location,
			const SWInt64 &bitvector) :
		SWAffectItem(type, duration, modifier, location, bitvector)
	{
	}

	SWAffectItemPlayer(const SWInt &type, const SWInt &duration, const SWInt &modifier, const SWInt &location,
			const SWInt64 &bitvector, const SWInt &ownerEntityId) :
		SWAffectItem(type, duration, modifier, location, bitvector, ownerEntityId)
	{
	}

	const SWString getTableName() const
	{
		return isAffectData() ? "affect_data_item_pchar" : "affect_item_pchar";
	}
};

}

#endif /* SWAFFECTITEMPLAYER_H_ */
