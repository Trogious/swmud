/*
 * SWPlayerBounty.cpp
 *
 *  Created on: 2009-08-02
 *      Author: Trog
 */

#include "SWPlayerBounty.h"
//#include "../structs/obj_data.h"
//#include "../structs/obj_index_data.h"
#include "../mud.h"

/**
 * Sprawdzamy czy wykonano zlecenie zabicia gracza.
 * Tutaj robi sie to w ten sposob, ze przy zabiciu gracza
 * tworzony jest przedmiot typu ITEM_COPRSE_PC, do ktorego
 * nazwy dopisywane jest imie gracza i te 2 parametry
 * tworza kryterium wykonania zlecenia.
 * \sa SWBounty::completed()
 */
bool SWPlayerBounty::completed(OBJ_DATA *proof)
{
	SWString name(proof->name);
	if (proof->pIndexData->item_type == ITEM_CORPSE_PC && name.find(wanted)
			!= SWString::npos)
	{
		return true;
	}

	return false;
}

/**
 * Sprawdzamy czy za tego gracza wystawione jest bounty.
 * \sa SWBounty::isBountyHunted()
 */
bool SWPlayerBounty::isBountyHunted(CHAR_DATA *ch)
{
	return !IS_NPC( ch ) && wanted.find(ch->name) != SWString::npos;
}

SWString SWPlayerBounty::getWantedName()
{
	return wanted;
}

SWString SWPlayerBounty::getInsertQuery() const
{
	char buf[MSL];
	sprintf(buf, "'SWPlayerBounty',%ld,'%s',%d,'%s')", reward,
			placedBy.c_str(), holdBy, wanted.c_str());
	return SWBounty::getInsertQuery() + SWString(buf);
}
