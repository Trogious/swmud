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

#ifndef EXIT_DATA_H_
#define EXIT_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct	exit_data
{
	EXIT_DATA			* prev;		/* previous exit in linked list */
	EXIT_DATA			* next;		/* next exit in linked list	*/
	EXIT_DATA			* rexit;		/* Reverse exit pointer		*/
	ROOM_INDEX_DATA		* to_room;	/* Pointer to destination room	*/
	char				* keyword;	/* Keywords for exit or door	*/
	char				* description;/* Description of exit		*/
	int					vnum;		/* Vnum of room exit leads to	*/
	int					rvnum;		/* Vnum of room in opposite dir */
	int64				flags;		/* door states & other flags	*/
	int64				orig_flags;	/* door states & other flags	*/
	int					key;		/* Key vnum			*/
	int					vdir;		/* Physical "direction"		*/
	int					distance;	/* how far to the next room	*/
};

#endif //EXIT_DATA_H_

