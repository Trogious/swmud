/*
 * SWAffectItem.h
 *
 *  Created on: 2010-09-15
 *      Author: Trog
 */

#ifndef SWAFFECTITEM_H_
#define SWAFFECTITEM_H_
#include <SWAffect.h>

namespace player
{

class SWAffectItem: public SWAffect
{
public:
	SWAffectItem()
	{
	}

	SWAffectItem(const SWInt &type, const SWInt &duration, const SWInt &modifier, const SWInt &location,
			const SWInt64 &bitvector) :
		SWAffect(type, duration, modifier, location, bitvector)
	{
	}

	SWAffectItem(const SWInt &type, const SWInt &duration, const SWInt &modifier, const SWInt &location,
			const SWInt64 &bitvector, const SWInt &ownerEntityId) :
		SWAffect(type, duration, modifier, location, bitvector, ownerEntityId)
	{
	}

	virtual ~SWAffectItem() = 0;

	virtual const SWString getOwnerEntityFKey() const
	{
		return "item_id";
	}
};

}

#endif /* SWAFFECTITEM_H_ */
