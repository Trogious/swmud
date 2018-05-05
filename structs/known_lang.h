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

#ifndef KNOWN_LANG_H_
#define KNOWN_LANG_H_
#include "../defines.h"
#include "../typedefs.h"

struct	known_lang
{
	KNOWN_LANG			* next;
	KNOWN_LANG			* prev;
	LANG_DATA			* language;
	int					learned;	/* % znajomosci jezyka (1-100) */
/*Trog: przyda sie to?
	int			times_used;
*/
};

#endif //KNOWN_LANG_H_

