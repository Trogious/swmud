/*
 * SWAffectItemHome.h
 *
 *  Created on: 2010-09-15
 *      Author: Trog
 */

#ifndef SWAFFECTITEMHOME_H_
#define SWAFFECTITEMHOME_H_
#include <SWAffectItem.h>

namespace player
{

class SWAffectItemHome: public SWAffectItem
{
public:
	SWAffectItemHome()
	{
	}

	SWAffectItemHome(const SWInt &type, const SWInt &duration, const SWInt &modifier, const SWInt &location,
			const SWInt64 &bitvector) :
		SWAffectItem(type, duration, modifier, location, bitvector)
	{
	}

	SWAffectItemHome(const SWInt &type, const SWInt &duration, const SWInt &modifier, const SWInt &location,
			const SWInt64 &bitvector, const SWInt &ownerEntityId) :
		SWAffectItem(type, duration, modifier, location, bitvector, ownerEntityId)
	{
	}

	const SWString getTableName() const
	{
		return isAffectData() ? "affect_data_item_home" : "affect_item_home";
	}
};

}

#endif /* SWAFFECTITEMHOME_H_ */
