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

#ifndef AFFECT_DATA_H_
#define AFFECT_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct	affect_data
{
	AFFECT_DATA			* next;
	AFFECT_DATA			* prev;
	int					type;
	int					duration;
	int					location;
	int					modifier;
	int64				bitvector;
};

#endif //AFFECT_DATA_H_

