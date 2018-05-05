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

#ifndef BOARD_DATA_H_
#define BOARD_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct	board_data
{
	BOARD_DATA			* next;			/* Next board in list		   */
	BOARD_DATA			* prev;			/* Previous board in list	   */
	NOTE_DATA			* first_note;		/* First note on board		   */
	NOTE_DATA			* last_note;		/* Last note on board		   */
	char				* note_file;		/* Filename to save notes to	   */
	char				* read_group;		/* Can restrict a board to a	   */
	char				* post_group;		/* council, clan, guild etc	   */
	char				* extra_readers;	/* Can give read rights to players */
	char				* extra_removers; /* Can give remove rights to players */
	int					board_obj;		/* Vnum of board object		   */
	int					num_posts;		/* Number of notes on this board   */
	int					min_read_level; /* Minimum level to read a note	   */
	int					min_post_level; /* Minimum level to post a note	   */
	int					min_remove_level;/*Minimum level to remove a note  */
	int					max_posts;		/* Maximum amount of notes allowed */
	int					type;		/* Normal board or mail board?	   */
};

#endif //BOARD_DATA_H_

