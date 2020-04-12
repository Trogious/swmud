/*
 * SWMobBounty.h
 *
 *  Created on: 2009-08-02
 *      Author: Trog
 */

#ifndef SWMOBBOUNTY_H_
#define SWMOBBOUNTY_H_

#include "SWBounty.h"

class SWMobBounty: public SWBounty
{
	int wanted;

	SWDbEntityDataMapping getDataMapping() const
	{
		return SWDbEntityDataMapping();
	}
	bool loadRelated(SWDataBase *db)
	{
		return true;
	}

public:
	static const SWInt MIN_MOB_VNUM_TO_BOUNTY;

	SWMobBounty() :
		SWBounty(), wanted(0)
	{
	}
	SWMobBounty(int wanted, long reward, SWString &placedBy, int holdBy) :
		SWBounty(reward, placedBy, holdBy), wanted(wanted)
	{
	}
	virtual ~SWMobBounty()
	{
	}
	const int getWanted() const
	{
		return wanted;
	}
	void setWanted(const int wanted)
	{
		this->wanted = wanted;
	}
	virtual SWString getUpdateQuery() const
	{
		return "";
	}
	virtual bool completed(OBJ_DATA *);
	virtual bool isBountyHunted(CHAR_DATA *);
	virtual SWString getWantedName();
	virtual void setWanted(const SWString &);
	virtual SWString getInsertQuery() const;

};

MAKE_DYNAMIC( SWMobBounty );

#endif /* SWMOBBOUNTY_H_ */
