/*
 * SWPlayerBounty.h
 *
 *  Created on: 2009-08-02
 *      Author: Trog
 */

#ifndef SWPLAYERBOUNTY_H_
#define SWPLAYERBOUNTY_H_

#include "SWBounty.h"

class SWPlayerBounty: public SWBounty
{
	SWString wanted;

	SWDbEntityDataMapping getDataMapping() const
	{
		return SWDbEntityDataMapping();
	}
	bool loadRelated(SWDataBase *db)
	{
		return true;
	}

public:
	SWPlayerBounty() :
		SWBounty()
	{
	}
	SWPlayerBounty(SWString &wanted, long reward, SWString &placedBy, int holdBy) :
		SWBounty(reward, placedBy, holdBy), wanted(wanted)
	{
	}
	virtual ~SWPlayerBounty()
	{
	}
	const SWString &getWanted() const
	{
		return wanted;
	}
	void setWanted(const SWString &wanted)
	{
		this->wanted = SWString(wanted);
	}
	virtual SWString getUpdateQuery() const
	{
		return "";
	}
	virtual bool completed(OBJ_DATA *);
	virtual bool isBountyHunted(CHAR_DATA *);
	virtual SWString getWantedName();
	virtual SWString getInsertQuery() const;
};

MAKE_DYNAMIC( SWPlayerBounty );

#endif /* SWPLAYERBOUNTY_H_ */
