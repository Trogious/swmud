/*
 * SWBounty.h
 *
 *  Created on: 2009-08-02
 *      Author: Trog
 */

#ifndef SWBOUNTY_H_
#define SWBOUNTY_H_

#include <list>
#include "SWString.h"
#include "SWClassLoader.h"
#include "SWInt.h"
#include "SWDbEntity.h"
#include "../defines.h"
#include "../typedefs.h"

using namespace std;

/**
 * Baza dla bounty.
 * Przechowuje podstawowe wlasciwosci dla bounty.
 */
class SWBounty: public SWDynamicClass, public SWDbEntity
{
protected:
	long reward; /*!< ilosc kredytek nagrody */
	SWString placedBy; /*!< imie gracza, ktory wystawil bounty */
	int holdBy; /*!< vnum moba, ktory przyjal zlecenie, i ktory wyplaca nagrode */

public:
	static const SWInt MIN_REWARD;

	SWBounty() :
		reward(0), holdBy(0)
	{
	}
	SWBounty(long reward, SWString &placedBy, int holdBy) :
		reward(reward), placedBy(placedBy), holdBy(holdBy)
	{
	}
	virtual ~SWBounty()
	{
	}

	const SWString getTableName() const
	{
		return "bounty";
	}

	long getReward() const
	{
		return reward;
	}

	void setReward(long reward)
	{
		this->reward = reward;
	}

	const SWString &getPlacedBy() const
	{
		return placedBy;
	}

	void setPlacedBy(const SWString &placedBy)
	{
		this->placedBy = placedBy;
	}

	int getHoldBy() const
	{
		return holdBy;
	}

	void setHoldBy(int holdBy)
	{
		this->holdBy = holdBy;
	}

	virtual SWString getInsertQuery() const;

	virtual void setWanted(const SWString &wanted) = 0;

	/*!
	 * Sprawdza czy dostarczony przedmiot jest dowodem wykonania zadania.
	 *
	 * \param proof przedmiot, ktory jest (badz nie), dowodem na to,
	 * ze zadanie zostalo wykonane
	 * \return true w przypadku gdy przedmiot jest dowodem wykonania zlecenia.
	 */
	virtual bool completed(OBJ_DATA *proof) = 0;

	/*!
	 * Sprawdza czy istota jest podmiotem tego bounty.
	 *
	 * \param victim istota ktora jest sprawdzana
	 * \return true w przypadku gdy istota sprawdzana jest podmiotem tego bounty.
	 */
	virtual bool isBountyHunted(CHAR_DATA *victim) = 0;

	/*!
	 * Podaje imie istoty ktora jest przemiotem bounty.
	 *
	 * \return imie istoty bedacej przemiotem bounty.
	 */
	virtual SWString getWantedName() = 0;
};

extern list<SWBounty *> bounties;

#endif /* SWBOUNTY_H_ */
