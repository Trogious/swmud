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

#ifndef PLANET_DATA_H_
#define PLANET_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct	planet_data
{
	PLANET_DATA			* next;
	PLANET_DATA			* prev;
	PLANET_DATA			* next_in_system;
	PLANET_DATA			* prev_in_system;
	GUARD_DATA			* first_guard;
	GUARD_DATA			* last_guard;
	SPACE_DATA			* starsystem;
	AREA_DATA			* first_area;
	AREA_DATA			* last_area;
	CLAN_DATA			* governed_by;
	DOCK_DATA			* first_dock;
	DOCK_DATA			* last_dock;
	char				* name;
	char				* filename;
	long				base_value;
	int					population;
	int64				flags;
	float				pop_support;
	int					first_jail;//added by Thanos (wi�zienia na planetach)
	int					last_jail;
	int					import;
	int					_export;
	int					prisei;
	int					prisee;
	int					stock;
	int					maxstock;
	float				xpos;//added by Aldegard (zamieniamy all na listy)
	float				ypos;
	float				zpos;
	float				gravity;
	int					version;//
	char				* description;
	char				* description2;
	SEASON_DATA			* first_season; /* Thanos */
	SEASON_DATA			* last_season;	/* Thanos */
	SEASON_DATA			* curr_season;	/* Thanos */
	int					daylen;		/* domy�lna d�ugo�� dnia */
	int					nightlen;	/* domy�lna d�ugo�� nocy */
	int					monthlen;	/* domy�lna d�ugo�� miesi�ca */
	int					hour;		/* aktualna godzina */
	int					day;		/* aktualny dzien */
	int					month;		/* aktualny miesiac */

	int					mmhg;		/* cisnienie */
	int					change;		/* zmiana cisnienia */
	int					sky;		/* jakie niebo */
	int					sunlight;	/* jakie slonce */
	int					windspeed;	/* predkosc wiatru */
	int					winddir;	/* kierunek wiatru */
	int					temperature;	/* done */
	float				radius;
	float				star_radius;
	float				star_radius2;
	DEPOSIT_DATA		* first_deposit;
	DEPOSIT_DATA		* last_deposit;
	WAREHOUSE_DATA		* pWarehouse;
};

#endif //PLANET_DATA_H_

