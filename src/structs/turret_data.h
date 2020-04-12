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

#ifndef TURRET_DATA_H_
#define TURRET_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct turret_data
{
	TURRET_DATA			* next;
	TURRET_DATA			* prev;
	SHIP_DATA			* target;
	CHAR_DATA			* gunner;
	int					vnum;
	int					status;
	int					statet0;
	int					type;
};

#endif //TURRET_DATA_H_
