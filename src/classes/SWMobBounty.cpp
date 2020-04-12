/*
 * SWMobBounty.cpp
 *
 *  Created on: 2009-08-02
 *      Author: Trog
 */

#include "SWMobBounty.h"
#include "SWInt.h"
//#include "../structs/obj_data.h"
//#include "../structs/obj_index_data.h"
#include "../mud.h"

const SWInt SWMobBounty::MIN_MOB_VNUM_TO_BOUNTY = 11;

/**
 * Sprawdzamy czy wykonano zlecenie zabicia moba.
 * Tutaj robi sie to w ten sposob, ze przy zabiciu moba
 * tworzony jest przedmiot typu ITEM_COPRSE_NPC, do ktorego
 * nazwy dopisywany jest string: m<vnum> moba i te 2 parametry
 * sa kryterium wykonania zlecenia.
 * \sa SWBounty::completed()
 */
bool SWMobBounty::completed(OBJ_DATA *proof)
{
	SWString name(proof->name);
	const SWString &vnum = SWInt::toString(wanted);
	SWString wantedName("m");
	wantedName.append(vnum);
	if ((proof->pIndexData->item_type == ITEM_CORPSE_NPC
			|| proof->pIndexData->item_type == ITEM_DROID_CORPSE) && name.find(
			wantedName) != SWString::npos)
	{
		return true;
	}

	return false;
}

/**
 * Sprawdzamy czy za tego moba wystawione jest bounty.
 * \sa SWBounty::isBountyHunted()
 */
bool SWMobBounty::isBountyHunted(CHAR_DATA *ch)
{
	return IS_NPC( ch ) && wanted == ch->pIndexData->vnum;
}

/**
 * Zwraca imie moba bedacego przedmiotem bounty.
 * \sa SWBounty::getWantedName()
 */
SWString SWMobBounty::getWantedName()
{
	MID *mob = get_mob_index(wanted);

	return SWString(mob->przypadki[0]);
}

void SWMobBounty::setWanted(const SWString &wanted)
{
	MID *mob = get_mob_index(SWInt::fromString(wanted));
	this->wanted = mob->vnum;
}

SWString SWMobBounty::getInsertQuery() const
{
	char buf[MSL];
	sprintf(buf, "'SWMobBounty',%ld,'%s',%d,'%d')", reward, placedBy.c_str(),
			holdBy, wanted);
	return SWBounty::getInsertQuery() + SWString(buf);
}
