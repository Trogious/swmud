/*
 * SWForbidden.h
 *
 *  Created on: 2010-07-15
 *      Author: Trog
 */

#ifndef SWFORBIDDEN_H_
#define SWFORBIDDEN_H_
#include "SWCommand.h"

namespace player
{

class SWForbidden: public SWCommand
{
public:
	SWForbidden()
	{
	}

	SWForbidden(const SWString &cmd) :
		SWCommand(cmd)
	{
	}

	SWForbidden(const SWString &cmd, const SWInt &ownerEntityId) :
		SWCommand(cmd, ownerEntityId)
	{
	}

	const SWString getTableName() const
	{
		return "forbid_pdata";
	}
};

}

#endif /* SWFORBIDDEN_H_ */
