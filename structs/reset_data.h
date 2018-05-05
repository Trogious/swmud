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

#ifndef RESET_DATA_H_
#define RESET_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct	reset_data
{
	RESET_DATA			* next;
	RESET_DATA			* prev;
	char				command;
	int					extra;
	int64				arg1;
	int64				arg2;
	int64				arg3;
	int64				arg4;		// tylko w space resets! -- Thanos
};

#endif //RESET_DATA_H_
