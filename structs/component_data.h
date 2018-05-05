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

#ifndef COMPONENT_DATA_H_
#define COMPONENT_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct component_data
{
	COMPONENT_DATA		* next;
	COMPONENT_DATA		* prev;
	int					type;
	int					nr;	// vnum jesli typ == v,
							// numer wear_flagi jesli typ == w
							// numer typu jesli typ == t
};

#endif //COMPONENT_DATA_H_

