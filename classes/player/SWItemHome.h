/*
 * SWItemHome.h
 *
 *  Created on: Sep 6, 2010
 *      Author: Trog
 */

#ifndef SWITEMHOME_H_
#define SWITEMHOME_H_
#include <SWItem.h>

namespace player
{

class SWItemHome: public SWItem
{
	static const char *table_name;

public:
	SWItemHome()
	{
	}

	SWItemHome(const SWInt64 vnum) :
		SWItem(vnum)
	{
	}

	SWItemHome(const SWInt64 vnum, const SWInt &ownerEntityId) :
		SWItem(vnum, ownerEntityId)
	{
	}

	const SWString getOwnerEntityFKey() const
	{
		return "home_id";
	}

	const SWString getTableName() const
	{
		return "item_home";
	}
};

}

#endif /* SWITEMHOME_H_ */
