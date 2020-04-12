/*
 * SWOwnerEntityId.h
 *
 *  Created on: 2010-08-05
 *      Author: Trog
 */

#ifndef SWOWNERENTITYID_H_
#define SWOWNERENTITYID_H_
#include "SWInt.h"

class SWOwnerEntityId
{
	SWInt ownerEntityId;

public:
	SWOwnerEntityId()
	{
	}

	SWOwnerEntityId(const SWInt &ownerEntityId) :
		ownerEntityId(ownerEntityId)
	{
	}

	virtual ~SWOwnerEntityId()
	{
	}

	const SWInt &getOwnerEntityId() const
	{
		return ownerEntityId;
	}

	void setOwnerEntityId(const SWInt &ownerEntityId)
	{
		this->ownerEntityId = ownerEntityId;
	}

	virtual const SWString getOwnerEntityFKey() const = 0;
};

#endif /* SWOWNERENTITYID_H_ */
