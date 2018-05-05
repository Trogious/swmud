/*
 * SWBounty.cpp
 *
 *  Created on: 2009-09-06
 *      Author: Trog
 */

#include <sstream>
#include "SWBounty.h"


const SWInt SWBounty::MIN_REWARD = 20000;

SWString SWBounty::getInsertQuery() const
{
	return SWString("INSERT INTO bounty (class,reward,placed_by,hold_by,wanted) VALUES(");
}
