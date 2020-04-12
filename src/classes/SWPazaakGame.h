/*
 * SWPazaakGame.h
 *
 *  Created on: Feb 21, 2010
 *      Author: Trog
 */

#ifndef SWPAZAAKGAME_H_
#define SWPAZAAKGAME_H_
//#include <sys/time.h>
#include "SWTimeStamp.h"

class SWPazaakGame
{
	unsigned int id;
	SWString aggressor;
	SWString defender;
	long reward;
	SWTimeStamp challenged;

public:
	class Comparator
	{
		bool operator()(const SWPazaakGame *first, const SWPazaakGame *second) const
		{
			return *first < *second;
		}
	};

	SWPazaakGame(unsigned int id, const SWString &aggressor, const SWString &defender, long reward,
			const SWTimeStamp &challenged) :
		id(id), aggressor(aggressor), defender(defender), reward(reward), challenged(challenged)
	{
	}

	const SWString &getAggressor() const
	{
		return aggressor;
	}

	const SWTimeStamp &getChallenged() const
	{
		return challenged;
	}

	const SWString &getDefender() const
	{
		return defender;
	}

	long getReward() const
	{
		return reward;
	}

	bool operator<(const SWPazaakGame &) const;
};

#endif /* SWPAZAAKGAME_H_ */
