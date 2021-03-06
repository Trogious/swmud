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

#ifndef HQ_ROOM_DESC_H_
#define HQ_ROOM_DESC_H_
#include "../defines.h"
#include "../typedefs.h"

struct hq_room_desc
{
	HQ_ROOM_DESC *		next;
	HQ_ROOM_DESC *		prev;
	hq_room_type		type;		/*! type of room */
	char *				room_name;	/*! clan specific room name */
	char *				room_desc;	/*! clan specific room desc */
};

#endif //HQ_ROOM_DESC_H_

