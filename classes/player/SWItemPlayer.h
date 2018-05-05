/*
 * SWItemPlayer.h
 *
 *  Created on: Sep 6, 2010
 *      Author: Trog
 */

#ifndef SWITEMPLAYER_H_
#define SWITEMPLAYER_H_
#include <SWItem.h>

namespace player
{

class SWItemPlayer: public SWItem
{
public:
	SWItemPlayer()
	{
	}

	SWItemPlayer(const SWInt64 vnum) :
		SWItem(vnum)
	{
	}

	SWItemPlayer(const SWInt64 vnum, const SWInt &ownerEntityId) :
		SWItem(vnum, ownerEntityId)
	{
	}

	const SWString getOwnerEntityFKey() const
	{
		return "pchar_id";
	}

	const SWString getTableName() const
	{
		return "item_pchar";
	}
};

}

#endif /* SWITEMPLAYER_H_ */
