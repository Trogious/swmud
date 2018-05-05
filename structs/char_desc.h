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

#ifndef CHAR_DESC_H_
#define CHAR_DESC_H_
#include "../defines.h"
#include "../typedefs.h"

struct char_desc
{
	desc_type			type;		//do jakiej grupy nalezy ta cecha wygladu
	char				base[23];   //temat nazwy
	desc_end			end;		//rodzaj koncowki fleksyjnej
};

#endif //CHAR_DESC_H_

