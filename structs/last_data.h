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

#ifndef LAST_DATA_H_
#define LAST_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct last_data
{
	LAST_DATA			* next;
	LAST_DATA			* prev;
	char				* text;
	char				* teller;
	char				* time;
	char				* verb;
	LANG_DATA			* language;
	int					invis_type;
};

#endif //LAST_DATA_H_
