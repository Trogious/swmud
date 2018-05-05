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

#ifndef COMPLAIN_DATA_H_
#define COMPLAIN_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct complain_data
{
	typedef enum { COMPLAIN_BUG, COMPLAIN_TYPO, COMPLAIN_IDEA } Type;

	COMPLAIN_DATA		* next;
	COMPLAIN_DATA		* prev;
	char				* text;
	char				* author;
	char				* date;
	int					location;
	char				* fixedby;
	char				* fixdate;
	Type				type;
};

#endif //COMPLAIN_DATA_H_
