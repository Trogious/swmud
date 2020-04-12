/***************************************************************************
* Star Wars Reality Code Additions and changes from the Smaug Code         *
* copyright (c) 1997 by Sean Cooper                                        *
* Starwars and Starwars Names copyright(c) Lucas Film Ltd.                 *
* SMAUG 1.0 (C) 1994, 1995, 1996 by Derek Snider                           *
* SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus,                    *
* Scryn, Rennard, Swordbearer, Gorog, Grishnakh and Tricops                *
* Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael        *
* Chastain, Michael Quan, and Mitchell Tse.                                *
* Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,          *
* Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.     *
* ------------------------------------------------------------------------ *
*                     ____________   __     ______                         *
*   Aldegard    Jot  (   ___   ___| /  \   |  __  )   Thanos      Trog     *
*        ______________\  \|   |  /  ^   \ |     <_______________          *
*        \________________/|___|/___/"\___\|__|\________________/          *
*                  \   \/ \/   //  \   |  __  )(  ___/`                    *
*                    \       //  ^   \ |     <__)  \                       *
*                      \_!_//___/"\___\|__|\______/TM                      *
* (c) 2001, 2002            M       U        D                Ver 1.1      *
* ------------------------------------------------------------------------ *
*			    Special requests module			   *
****************************************************************************/

#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "mud.h"

int Req;

void init_request_pipe( )
{
#ifdef ReqUESTS
  if ( (Req = open( ReqUEST_PIPE, O_RDONLY | O_NONBLOCK )) == -1 )
  {
    bug ( "ReqUEST pipe not found", 0 );
    exit(1);
  }
#endif
}

void check_requests( )
{
#ifdef ReqUESTS
  char buf[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  int  c;
  if ( read( Req, buf, sizeof( buf ) ) != -1 )
  {
     close( Req );
     init_request_pipe();
     for ( c = 0; c < MAX_STRING_LENGTH; c++ )
	if ( buf[c] == '\n' || buf[c] == '\r' )
	{
	   buf[c] = '\0';
	   break;
	}
     sprintf( buf2, "ReqUEST: %s", buf );
     log_string( buf2 );
     if ( strcmp( buf, "who" ) == 0 )
	do_who( NULL, "" );
     else
     if ( strcmp( buf, "webwho" ) == 0 )
	do_who( NULL, "www" );
  }
#endif
}
