/*
 * SWBestow.h
 *
 *  Created on: 2010-07-15
 *      Author: Trog
 */

#ifndef SWBESTOW_H_
#define SWBESTOW_H_
#include "SWCommand.h"

namespace player
{

class SWBestow: public SWCommand
{
public:
	SWBestow()
	{
	}

	SWBestow(const SWString &cmd) :
		SWCommand(cmd)
	{
	}

	SWBestow(const SWString &cmd, const SWInt &ownerEntityId) :
		SWCommand(cmd, ownerEntityId)
	{
	}

	const SWString getTableName() const
	{
		return "bestow_pdata";
	}
};

}

#endif /* SWBESTOW_H_ */
