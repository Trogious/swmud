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
*		        Ship saving & recovery module   		   *
****************************************************************************/

#include <time.h>
#include "mud.h"

SHIP_INDEX_DATA * get_ship_index	args( ( char * ship_index ) );
ROOM_INDEX_DATA * get_sroom		args( ( SHIP_DATA *ship, int svnum ) );
SHIP_DATA * 	  constr_ship		args( ( SHIP_INDEX_DATA *shrec ) );

/*
 * Za³o¿enia zapisu stanu statków:
 * - zapisuj± siê tylko nowe statki (instancje prototypów)
 * - zapisuj± siê tylko statki nale¿±ce do graczy
 * - zapisuj± siê tylko statki bêd±ce w lokacji, lub w systemie gwiezdnym
 * - je¶li statek znajduje siê w innym stanie (nie jest w lokacji, ani
 *   w systemie gwiezdnym, albo jest w stanie startu/l±dowania), do za³adowania
 *   u¿yty zostanie stary save.
 */

void save_ship_state( SHIP_DATA *ship )
{
    FILE *	fp;
    char 	filename	[256];

    if( !ship )
    {
	bug( "null ship pointer!", 0 );
	return;
    }
        
    if( !ship->vnum || !ship->pIndexData )
	return;

//    if( !*ship->owner 
//    ||  !str_cmp( ship->owner, "Public" ) )
//	return;
	
    if( (!ship->starsystem && !ship->in_room)
    ||  (ship->in_room && ship->in_room->ship && ship->in_room->ship->vnum) )
	return;
	 
    sprintf( filename, "%s%s.dat", SHIP_TMP_DIR, ship->transponder );
    
    RESERVE_CLOSE;
    if ( ( fp = fopen( filename, "w" ) ) == NULL )
    {
    	bug( "fopen", 0 );
    	perror( filename );
    }
    else
    {
	fprintf( fp, "#SHIPSTATE   %d\n", 	ship->vnum 		);
	fprintf( fp, "Owner        %s~\n",	ship->owner		);
	fprintf( fp, "End\n\n"	);



	/* TA SEKCJA MA BYÆ ZAWSZE NA KOÑCU PLIKU!			*/
	fprintf( fp, "#LOCATION    "					);
	if( ship->in_room )
	{
	    /* je¶li statek jest zadokowany na innym statku */
	    if( ship->in_room->ship && ship->in_room->ship->pIndexData )
		fprintf( fp, "Ship: %s~ %d\n", ship->in_room->ship->transponder,
					      ship->in_room->svnum	);
	    /* w zwyk³ej lokacji */
	    else
		fprintf( fp, "Room: %d\n", ship->in_room->vnum		);
	}
	else
        /* statek w kosmosie */
        if( ship->starsystem )
    	    fprintf( fp, "Space: %s~ %.0f %.0f %.0f\n",
				ship->starsystem->name,
				ship->vx, ship->vy, ship->vz		);
        /* jakby co to do limbo */
	else
	    fprintf( fp, "Room: %d\n", ROOM_LIMBO_SHIPYARD		);

	fprintf( fp, "#END\n"						);
    }

    fclose( fp );
    RESERVE_OPEN;
    return;
}

void add_ship_to_list_file( FILE *fp, SHIP_DATA *ship )
{
    ROOM_INDEX_DATA *	room;
    SHIP_DATA * 	docked;

    if( !ship->vnum || !ship->pIndexData )
        return;

//    if( !*ship->owner 
//    || !str_cmp( ship->owner, "Public" ) )
//        return;
	    
    fprintf( fp, "%s.dat\n", ship->transponder );	
    SET_BIT( ship->tmp_flags, xSHIP_SAVED );

    for( room = ship->first_location; room; room = room->next_on_ship )
		for( docked = room->first_ship; docked; docked = docked->next_in_room )
	    	if( !IS_SET( docked->tmp_flags, xSHIP_SAVED ) )
				add_ship_to_list_file( fp, docked );
}

/* Zeby statki zapisa³y siê w ³adnej kolejno¶ci, musimy troszkê namieszaæ:
 * Zapisujemy do listy tylko te statki, które NIE S¡ zaparkowane na innych
 * statkach. Dopiero te zapisywane sprawdzaj± swoje pok³ady i je¶li znajdzie
 * siê na nich jaki¶ statek - ka¿± mu siê dopisaæ do listy. Ten z kolei 
 * podczas dopisywania sprawdzi swój pok³ad itd... ¯eby sprawdziæ, czy statek
 * ju¿ by³ zapisany najlepiej jest pos³u¿yæ siê jak±¶ flag± tymczasow±.
 * Dziêki temu statki zapisane s± w kolejno¶ci od zewnêtrznego wg³±b, co
 * znacznie u³atwi sprawê ich wczytywania i umieszczania w lokacjach, kosmosie,
 * a przede wszystkim w innych statkach. */
void save_ship_states_list()
{
    SHIP_DATA * 	ship;
    FILE * 		fp;

    RESERVE_CLOSE;
    if ( ( fp = fopen( SHIP_TMP_LIST, "w" ) ) == NULL )
    {
    	bug( "fopen" );
    	perror( SHIP_TMP_LIST );
    }

    for( ship = first_ship; ship; ship = ship->next )
    {
	/* zapisujemy tylko te statki, które nie stoj± na innych statkach */
	if( IS_SET( ship->tmp_flags, xSHIP_SAVED ) )
	    continue;	

	/* reszta (te zadokowane na statkach), zapisze siê rekursywnie */
	add_ship_to_list_file( fp, ship );
    }

    fprintf( fp, "$\n" );	

    fclose( fp );
    RESERVE_OPEN;

    return;
}

void save_all_ship_states()
{
    SHIP_DATA *		ship;

    for( ship = first_ship; ship; ship = ship->next )
	REMOVE_BIT( ship->tmp_flags, xSHIP_SAVED );
        
    save_ship_states_list();

    for( ship = first_ship; ship; ship = ship->next )
    {
		/* zapisz tylko z listy (ona decyduje co ma byæ zapisane) */
		if( IS_SET( ship->tmp_flags, xSHIP_SAVED ) )
    	{
    	    save_ship_state( ship );
	    	REMOVE_BIT( ship->tmp_flags, xSHIP_SAVED );
		}
    }
    	
    return;
}

void fread_ship_state( SHIP_DATA *ship, FILE *fp )
{
    const char *		word;
    bool 		fMatch;

    for ( ; ; )
    {
	word   = feof( fp ) ? "End" : fread_word( fp );
	fMatch = false;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = true;
	    fread_to_eol( fp );
	    break;

	case 'E':
	    if ( !str_cmp( word, "End" ) )
		return;

        case 'O':
            SKEY( "Owner",            ship->owner,            fread_string( fp ) );
            break;
	}

	if ( !fMatch )
	    bug( "no match: %s (Ship:%d)", word, ship->vnum  );
    }
}

void fread_ship_location( SHIP_DATA *ship, FILE *fp )
{
    char *	word;
    
    word = fread_word( fp );

    if( !str_cmp( word, "Room:" ) )
    {
	ROOM_INDEX_DATA *	room;
	int			toRoomVnum;
        
	toRoomVnum = fread_number( fp );
	if( !(room=get_room_index( toRoomVnum )) )
	{
	    bug( "location %d doesn't exist.", toRoomVnum );
	    place_ship_in_room( ship, get_room_index( ROOM_LIMBO_SHIPYARD ) );
	    return;
	}

        place_ship_in_room( ship, room );
	return;
    }
    else
    if( !str_cmp( word, "Space:" ) )
    {
	SPACE_DATA *	starsystem;
	char *		starsystemname;
	float		tmpX, tmpY, tmpZ;
		
        starsystemname = st_fread_string( fp );
        tmpX = atof( fread_word( fp ) );
        tmpY = atof( fread_word( fp ) );
        tmpZ = atof( fread_word( fp ) );

	if( !(starsystem=starsystem_from_name( starsystemname )) )
	{
	    bug( "Starsystem '%s' doesn't exist.", starsystemname );
	    place_ship_in_room( ship, get_room_index( ROOM_LIMBO_SHIPYARD ) );
	    return;
	}
	
	place_ship_in_space( ship, starsystem, tmpX, tmpY, tmpZ );
	return;
    }
    else
    if( !str_cmp( word, "Ship:" ) )
    {
	ROOM_INDEX_DATA *	room;
	SHIP_DATA *		dock;
	char *			tmpTransponder;
	int			toRoomVnum;
	
        tmpTransponder = st_fread_string( fp );
        toRoomVnum = fread_number( fp );

	for ( dock = first_ship; dock; dock = dock->next )
    	    if ( !str_cmp( tmpTransponder, dock->transponder ) )
		break;
	if( !dock )
	{
	    bug( "Ship '%s' doesn't exist.", tmpTransponder );
	    place_ship_in_room( ship, get_room_index( ROOM_LIMBO_SHIPYARD ) );
	    return;
	}

	if( (room=get_sroom( dock, toRoomVnum )) == NULL )
	{
	    bug( "Room %d on ship '%s' doesn't exist.", toRoomVnum, tmpTransponder );
    	    place_ship_in_room( ship, get_room_index( ROOM_LIMBO_SHIPYARD ) );
	    return;
	}

	place_ship_in_room( ship, room );
	return;
    }
    else
    {
        bug( "unknown location. Ship: %s", ship->transponder );
	place_ship_in_room( ship, get_room_index( ROOM_LIMBO_SHIPYARD ) );
        fread_to_eol( fp );
	return;
    }
}

bool load_ship_state( const char *shipfile )
{
    char 		filename	[256];
    char 		tmpTransponder	[256];
    SHIP_INDEX_DATA *	shrec;
    SHIP_DATA *		ship = NULL;
    FILE *		fp;

    if( str_suffix( ".dat", shipfile ) )
    {
	bug( "%s bad suffix (must be .dat)", shipfile );
	return false;
    }
    sprintf( tmpTransponder, "%s", shipfile );
    tmpTransponder[strlen(tmpTransponder)-4] = '\0';

    sprintf( filename, "%s%s", SHIP_TMP_DIR, shipfile );

    if ( ( fp = fopen( filename, "r" ) ) != NULL )
    {
	for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( "# not found.", 0 );
		break;
	    }

	    word = fread_word( fp );
	    if ( !str_cmp( word, "END"	) )
	        break;
	    else
	    if ( !str_cmp( word, "SHIPSTATE" ) )
	    {
		int	vnum = fread_number( fp );
		
		if( (shrec=get_ship_index( itoa( vnum ) ) )==NULL )
		{
		    bug( "Bad vnum. (%s)", shipfile );
		    fclose( fp );
		    return false;
		}

		if( (ship=constr_ship( shrec ))==NULL )
		{
		    bug( "Cannot constr. (%s)", shipfile );
		    fclose( fp );
		    return false;
		}

		STRDUP( ship->transponder, tmpTransponder );
	    	fread_ship_state( ship, fp );
	    }
	    else
	    if ( !str_cmp( word, "LOCATION" ) )
	    {
		if( !ship )
		{
		    bug( "LOCATION found, but no ship. (%s)", shipfile );
		    fclose( fp );
		    return false;
		}
		fread_ship_location( ship, fp );
	    }
	    else
	    {
		bug( "bad section: %s.", word );
		break;
	    }
	}
	fclose( fp );
    }
    else
    {
    	bug( "fopen (%s)", shipfile );
    	perror( shipfile );
	return false;
    }

    return true;
}


/*
 * Load in all the ship files.
 */
void restore_ship_states( )
{
    FILE *	fpList;
    const char *	filename;


    RESERVE_CLOSE;

    if ( ( fpList = fopen( SHIP_TMP_LIST, "r" ) ) == NULL )
    {
	perror( SHIP_TMP_LIST );
	exit( 1 );
    }

    for ( ; ; )
    {
	filename = feof( fpList ) ? "$" : fread_word( fpList );

	if ( filename[0] == '$' )
	  break;

	if ( !load_ship_state( filename ) )
	  bug( "Cannot load ship state: %s", filename );
    }
    fclose( fpList );

    RESERVE_OPEN;
    return;
}

