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

#ifndef Q_COMMAND_DATA_H_
#define Q_COMMAND_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct	q_command_data
{
	QUEST_CMND_DATA		* next;
	QUEST_CMND_DATA		* prev;
	int					command;
	int				arg1;
	int				arg2;
	int				arg3;
	char				* arg4;
	bool				done;
};

#endif //Q_COMMAND_DATA_H_

