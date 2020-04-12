/*
 * SWPazaak.h
 *
 *  Created on: Feb 21, 2010
 *      Author: Trog
 */

#ifndef SWPAZAAK_H_
#define SWPAZAAK_H_
#include "../defines.h"
#include "../typedefs.h"
#include "SWPazaakCard.h"
#include "SWDataBase.h"
#include "SWPazaakGame.h"
#include "SWTimeStamp.h"

class SWPazaak
{
public:
	class SWPazaakChallenge
	{
		SWString aggressor;
		SWString defender;
		SWTimeStamp challenged;
		unsigned int reward;

	public:
		class DefenderComparator
		{
			SWString defender;

		public:
			DefenderComparator(const SWString &defender) :
				defender(defender)
			{
			}
			bool operator()(const SWPazaakChallenge *, const SWPazaakChallenge *) const;
		};

		SWPazaakChallenge(const SWString &aggressor, const SWString &defender, unsigned int reward) :
			aggressor(aggressor), defender(defender), reward(reward)
		{
		}

		const SWString &getAggressor() const
		{
			return aggressor;
		}

		const SWString &getDefender() const
		{
			return defender;
		}

		int getReward() const
		{
			return reward;
		}

		const SWTimeStamp &getChallenged() const
		{
			return challenged;
		}

		bool operator<(const SWPazaakChallenge &) const;
	};

private:
	static list<SWPazaakChallenge*> challenges;
	static list<SWPazaakGame*> games;

	static int getPlayerId(SWDataBase &, const SWString &playerName);
	static int getCardId(SWDataBase &, const SWPazaakCard &, int playerId);
	static void clearGames();
	static void clearChallenges();
	static unsigned int countCards(SWDataBase &, int playerId);
public:
	static const unsigned int MIN_CARDS_IN_SIDEDECK = 6; /* must be at least 4 */
	static const unsigned int MIN_REWARD = 10;

	static const list<SWPazaakChallenge*> &getChallenges()
	{
		return challenges;
	}
	static const list<SWPazaakGame*> &getGames()
	{
		return games;
	}

	/**
	 * \return new list of player cards - remember to delete after using
	 */
	static list<SWPazaakCard> *getCards(const SWString &playerName, SWDataBase * = 0);
	static bool addCard(const SWPazaakCard &, const SWString &playerName, SWDataBase * = 0);
	static bool removeCard(const SWPazaakCard &, const SWString &playerName, SWDataBase * = 0);
	static void loadGames(SWDataBase * = 0);
	static bool setPassword(const SWString &playerName, const SWString &password);
	static int withdrawCredits(const SWString &playerName);
	static SWPazaakChallenge *findChallenge(const SWString &aggressor, const SWString &defender);
	static bool challenge(const SWString &aggressor, const SWString &defender, unsigned int reward);
	static bool acceptChallenge(SWPazaakChallenge *);
	static const SWString listCards(const SWString &playerName);
	/**
	 * \return single card by given number from the side deck - remember to delete the card after using
	 */
	static SWPazaakCard *getPlayerCard(const SWString &playerName, const SWInt &cardNo, SWDataBase * = 0);
};

#endif /* SWPAZAAK_H_ */
