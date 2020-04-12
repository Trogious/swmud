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

#ifndef TURBOCAR_DATA_H_
#define TURBOCAR_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct turbocar_data /* Trog */
{
	TURBOCAR	*next;
	TURBOCAR	*prev;
	char		*name;
	char		*filename;
	int			vnum;
	TC_STATION	*current_station;
	TC_STATION	*first_station;
	TC_STATION	*last_station;
};

#endif //TURBOCAR_DATA_H_
