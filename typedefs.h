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


/* This file was generated by Trog. */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_
#include "structs/typedefs.h"

typedef int				ch_ret;
typedef int				obj_ret;

/*
* Function types.
*/
typedef void	DO_FUN			args( ( CHAR_DATA *ch, char *argument ) );
typedef void	CONST_DO_FUN	args( ( CHAR_DATA *ch, const char *argument ) );
typedef bool	SPEC_FUN		args( ( CHAR_DATA *ch ) );
typedef ch_ret	SPELL_FUN		args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
typedef void	CON_FUN			args( ( DESCRIPTOR_DATA *d, char *argument ) );



#endif //TYPEDEFS_H_

