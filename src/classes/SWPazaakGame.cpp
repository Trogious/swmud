/*
 * SWPazaakGame.cpp
 *
 *  Created on: Feb 25, 2010
 *      Author: Trog
 */

#include "SWPazaakGame.h"

bool SWPazaakGame::operator<(const SWPazaakGame &game) const
{
	if (game.challenged < challenged)
	{
		return true;
	}

	if(game.challenged == challenged)
	{
		if (reward > game.reward)
		{
			return true;
		}

		if (reward == game.reward)
		{
			int cmp = aggressor.compare(game.aggressor);
			if (cmp < 0)
			{
				return true;
			}

			cmp = defender.compare(game.defender);
			if (cmp < 0)
			{
				return true;
			}

			return id > game.id;
		}

		return false;
	}

	return false;
}
