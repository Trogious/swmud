/*
 * SWCommand.h
 *
 *  Created on: 2010-07-15
 *      Author: Trog
 */

#ifndef SWCOMMAND_H_
#define SWCOMMAND_H_
#include "../SWDbEntity.h"
#include "../SWOwnerEntityId.h"

namespace player
{

class SWCommand: public SWDbEntity, public SWOwnerEntityId
{
	SWString cmd;

	SWDbEntityDataMapping getDataMapping() const;

public:
	SWCommand()
	{
	}

	SWCommand(const SWString &cmd) :
		cmd(cmd)
	{
	}

	SWCommand(const SWString &cmd, const SWInt &ownerEntityId) :
		SWOwnerEntityId(ownerEntityId), cmd(cmd)
	{
	}

	virtual ~SWCommand() = 0;

	const SWString getOwnerEntityFKey() const
	{
		return "pdata_id";
	}

	const SWString &getCmd() const
	{
		return cmd;
	}

	void setCmd(const SWString &cmd)
	{
		this->cmd = cmd;
	}
};

}

#endif /* SWCOMMAND_H_ */
