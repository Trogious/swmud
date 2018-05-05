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

#ifndef EXTRA_DESCR_DATA_H_
#define EXTRA_DESCR_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct	extra_descr_data
{
	EXTRA_DESCR_DATA	* next; /* Next in list			    */
	EXTRA_DESCR_DATA	* prev; /* Previous in list		    */
	char				* keyword;/* Keyword in look/examine	      */
	char				* description; /* What to see		      */
};

#endif //EXTRA_DESCR_DATA_H_

