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

#ifndef REQUIREMENT_DATA_H_
#define REQUIREMENT_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct requirement_data
{
	REQUIREMENT_DATA	* next;
	REQUIREMENT_DATA	* prev;
	int					location;	/* co potrzebne? */
	int					type;		/* jaki skill/affect */
	int					modifier;	/* ile trzeba */
};

#endif //REQUIREMENT_DATA_H_
