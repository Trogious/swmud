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

#ifndef FOR_DATA_H_
#define FOR_DATA_H_
#include "../defines.h"
#include "../typedefs.h"

struct for_data
{
	CHAR_DATA			* fOwner;		// wska�nik na wykonawc� 'for'
	CHAR_DATA			* fVictim;	// wska�nik na moba/gracza aktualnie 'obs�ugiwanego'
	AREA_DATA			* fArea;		// wska�nik na kraink� aktualnie 'obs�ugiwan�'
	int					fRoom;		// vnum pokoju aktualnie 'obs�ugiwanego'
	char				* command;	// komenda
	bool				fMobs;		// czy for dotyczy mob�w
	bool				fMortals;	// czy for dotyczy graczy
	bool				fGods;		// czy for dotyczy immos�w
	bool				fEverywhere;// czy mo�e wszystkich lokacji muda
	bool				fAreaMobs;	// albo mob�w w krainie
	bool				fAreaRooms;	// albo lokacji w krainie
	bool				fStopped;	// czy zatrzymana
	int					fCount;		// ilo�� powt�rze�
};

#endif //FOR_DATA_H_

