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

#ifndef CLASS_TYPE_H_
#define CLASS_TYPE_H_
#include "../defines.h"
#include "../typedefs.h"

struct	class_type
{
	const char				* name;
	const char				* who_name;
	const char				* przypadki[7];
#if defined( ARMAGEDDON )
	int					attr_mod[7];
	int					thac0_00;
	int					thac0_100;
#endif
	int				attr_prime;
};

#endif //CLASS_TYPE_H_

