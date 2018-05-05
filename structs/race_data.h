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

#ifndef RACE_DATA_H_
#define RACE_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct	race_data
{
	RACE_DATA			* next;
	RACE_DATA			* prev;
	char				* name;
	char				* filename;
	char				* przypadki[6];
	char				* inflectsFemale[6];  // Pixel: przypadkiF
	char				* description;
	int64				flags;
	int64				affected;
	int					str_plus;
	int					dex_plus;
	int					wis_plus;
	int					int_plus;
	int					con_plus;
	int					cha_plus;
	int					lck_plus;
	int					frc_plus;
	int					hp_plus;
	int					force_plus;
	int64				susceptible;
	int64				resistant;
	int64				immune;
	LANG_DATA			*language;
	//tablica bit�w dla poszczegolnych cech wygladu
	long long			desc_restrictions[MAX_DESC_TYPES];
};

#endif //RACE_DATA_H_

