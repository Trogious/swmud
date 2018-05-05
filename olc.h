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
*       	     OLC Header Module Added by Thanos			   *
****************************************************************************/

/*
 * Nota: Wiem, ¿e ten olc przypomina zachowaniem i u¿yciem Ilab OLCa
 * Nie jest to jednak Ilab, tylko SW-OLC Madafaka Production By Thanos :)
 * A tak powa¿nie, to nie jest to przeportowany Ilab, tylko co¶, co napisalem
 * na wzór ILaba (pos³uguj±c siê czêsto metodami Copyego & Paste'a) ;)
 */

#include <sys/types.h>
#include <ctype.h>
#include <time.h>
#include "mud.h"

extern AREA_DATA *	shipTmpArea;

void 		reset_planet		args( ( PLANET_DATA *planet ) );
void 		write_area_list 	args( ( ) );
void		write_planet_list	args( ( ) );
int 		get_langflag		args( ( char *flag ) );
void  		pedit_dock		args( ( PLANET_DATA *planet, CHAR_DATA *ch, char *argument ) );
void 		save_ship_index		args( ( SHIP_INDEX_DATA *ship ) );
void 		save_ship_index_list	args( ( ) );
void 		free_srooms		args( ( SHIP_INDEX_DATA *ship ) );


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Dzieki tym makrom objetosc pliku maleje i edycja jest           *
 * stabilniejsza                                        -- Thanos  *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* Trog: dodalem __MAKRO - to pozwoli na szybkie dodefiniowanie potrzebnych (w przyszlosci) makr. */

#define __SET_INT( cmd, darg, dtarget, dval )				\
	if( cmd( arg1, darg ) )									\
	{														\
		dtarget = dval;										\
		ch_printf( ch, "%s set." NL, capitalize( darg ) );	\
		return;												\
	}

#define SET_INT( darg, dtarget, dval ) __SET_INT( !str_cmp, darg, dtarget, dval )
#define PSET_INT( darg, dtarget, dval ) __SET_INT( !str_prefix, darg, dtarget, dval )

/* mo¿na ustaliæ arg inny ni¿ arg1 */
#define __SET_INT_ARG( cmd, darg, dtarget, dval, _arg )			\
	if ( cmd( _arg, darg ) )							\
	{														\
		dtarget = dval;										\
		ch_printf( ch, "%s set." NL, capitalize( darg ) );	\
		return;												\
	}

#define SET_INT_ARG( darg, dtarget, dval, _arg ) __SET_INT_ARG( !str_cmp, darg, dtarget, dval, _arg )
#define PSET_INT_ARG( darg, dtarget, dval, _arg ) __SET_INT_ARG( !str_prefix, darg, dtarget, dval, _arg )

#define __SET_INT_LIMIT( cmd, darg, dtarget, dval, dlow, dhi )	\
	if ( cmd( arg1, darg ) )									\
	{															\
		int	__value = dval;										\
		if( __value < dlow || __value > dhi )					\
		{														\
			ch_printf( ch, "%s range is %d to %d." NL, 			\
			capitalize( darg ), dlow, dhi );					\
			return;												\
		}														\
		dtarget = __value;										\
		ch_printf( ch, "%s set." NL, capitalize( darg ) );		\
		return;													\
	}

#define SET_INT_LIMIT( darg, dtarget, dval, dlow, dhi ) __SET_INT_LIMIT( !str_cmp, darg, dtarget, dval, dlow, dhi )
#define PSET_INT_LIMIT( darg, dtarget, dval, dlow, dhi ) __SET_INT_LIMIT( !str_prefix, darg, dtarget, dval, dlow, dhi )

#define SET_STR_EDIT( darg, dtarget, dval )			\
    if ( !str_prefix( arg1, darg ) )				\
    {								\
        if ( dval[0] == '\0' )					\
        {							\
            string_append( ch, &dtarget );			\
            return;						\
        }							\
								\
        ch_printf( ch, "Syntax: %s  - line edit" NL, darg );	\
        return;							\
    }

#define SET_STR( darg, dtarget, dval )				\
    if ( !str_prefix( arg1, darg ) )				\
    {								\
        if ( dval[0] == '\0' )					\
        {							\
            ch_printf( ch, "Syntax:  %s <string>" NL, darg );	\
            return;						\
        }							\
        STRDUP( dtarget, dval );				\
        ch_printf( ch, "%s set." NL, capitalize( darg ) );	\
        return;							\
    }

#define SET_STR_ARG( darg, dtarget, dval, _arg )		\
    if ( !str_prefix( _arg, darg ) )				\
    {								\
        if ( dval[0] == '\0' )					\
        {							\
            ch_printf( ch, "Syntax:  %s <string>" NL, darg );	\
            return;						\
        }							\
        STRDUP( dtarget, dval );				\
        ch_printf( ch, "%s set." NL, capitalize( darg ) );	\
        return;							\
    }

/* Trog: wyswietla co innego niz sprawdza */
#define SET_STR_DISP( darg, dtarget, dval, _arg )		\
    if ( !str_prefix( arg1, darg ) )				\
    {								\
        if ( dval[0] == '\0' )					\
        {							\
            ch_printf( ch, "Syntax:  %s <string>" NL, _arg );	\
            return;						\
        }							\
        STRDUP( dtarget, dval );				\
        ch_printf( ch, "%s set." NL, capitalize( _arg ) );	\
        return;							\
    }

/*
 * Robi to co powy¿sze makro, ale pozwala na czyszczenie pola
 * argumentem 'none'
 */
#define SET_STR_NONE( darg, dtarget, dval )			\
    if ( !str_prefix( arg1, darg ) )				\
    {								\
        if ( dval[0] == '\0' )					\
        {							\
            ch_printf( ch, "Syntax:  %s <string>" NL, darg );	\
            return;						\
        }							\
	if ( !str_cmp( dval, "none" ) )				\
    	    STRDUP( dtarget, "" );				\
	else							\
    	    STRDUP( dtarget, dval );				\
        ch_printf( ch, "%s set." NL, capitalize( darg ) );	\
        return;							\
    }

#define SET_FILENAME( darg, dtarget, dval, dext )					\
	if( !str_prefix( arg1, darg ) )									\
	{																\
		if( !VALID_FILENAME( dval, dext ) )							\
		{															\
			ch_printf( ch, "Syntax:  %s <filename>" NL				\
				"filename format:  ^%s%s$" NL, darg, NAMEREGEXP, dext );\
			return;													\
		}															\
		STRDUP( dtarget, dval );									\
		ch_printf( ch, "%s set." NL, capitalize( darg ) );			\
		return;														\
	}

#define SET_VALUE( darg, dch, dtarget, nr, dval )		\
    if ( !str_cmp( arg1, darg ) )				\
    {								\
	if ( dval[0] == '\0' )					\
	{							\
	    set_obj_values( dch, dtarget, -1, (char *)"" );		\
	    return;						\
	}							\
	set_obj_values( dch, dtarget, nr, dval );		\
	return;							\
    }

#define GET_DONE( )						\
    if ( !str_prefix( arg1, "done" ) )				\
    {								\
	edit_done( ch, (char *)"" );					\
        return;							\
    }

#define GET_HELP( darg )					\
    if ( !str_prefix( arg1, "?" ) )				\
    {								\
        show_help( ch, darg );					\
        return;							\
    }

#define GET_CREATE( cmd )				\
    if ( !str_prefix( arg1, "create" ) )\
    {									\
		cmd;							\
		return;							\
    }

#define GET_SAVE( cmd )					\
	if( !str_prefix( arg1, "fold" ) )	\
	{									\
		cmd;							\
		donemsg( ch );					\
		return;							\
	}


