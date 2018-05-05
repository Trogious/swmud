/***************************************************************************
* swmud.pl
* ------------------------------------------------------------------------ *
*                     ____________   __     ______                         *
*   Aldegard    Jot  (   ___   ___| /  \   |  __  )   Thanos      Trog     *
*        ______________\  \|   |  /  ^   \ |     <_______________          *
*        \________________/|___|/___/"\___\|__|\________________/          *
*                  \   \/ \/   //  \   |  __  )(  ___/`                    *
*                    \       //  ^   \ |     <__)  \                       *
*                      \_!_//___/"\___\|__|\______/TM                      *
* (c) 2009                  M       U        D                Ver 1.0      *
* ------------------------------------------------------------------------ *
*                               <module_name>                              *
****************************************************************************/


/* This file was generated by Trog's granulate_structs.pl. */

#ifndef BETS_PROGRES_H_
#define BETS_PROGRES_H_
#include "../defines.h"
#include "../typedefs.h"

struct bets_progres	//jak obstawiaja gracze
{
	BETS_PROGRES		* next;
	BETS_PROGRES		* prev;
	char				* name;		/*!< nazwa licytanta */
	bool				buy_out;	/*!< czy to on dokona� wykupu natychmiastowego */
	int64				price;		/*!< aktualna stawka */
	int64				max_price;	/*!< maskymalna kwita jaka jest gotowy zaplacic */
	int64				quantity;	/*!< ilosc kupowanych elementow - domyslnie 1 */
};

#endif //BETS_PROGRES_H_

