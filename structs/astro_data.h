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

#ifndef ASTRO_DATA_H_
#define ASTRO_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct astro_data
{
	ASTRO_DATA			* next;
	ASTRO_DATA			* prev;
	ASTRO_DATA			* next_in_starsystem;
	ASTRO_DATA			* prev_in_starsystem;
	SPACE_DATA			* starsystem;
	SHIP_DATA			* target0;
	int					type;
	int					value;
	float				ox, oy, oz;
	float				cx, cy, cz;
	int					lasers;
	int					missiles;
	int					vnum;
	char				* filename;
	char				* name;
	char				* home;
	int					energy;
	int					speed;
	int					timer;
};

#endif //ASTRO_DATA_H_

