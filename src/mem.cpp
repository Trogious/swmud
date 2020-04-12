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
*			   Memory management module     		   *
****************************************************************************/


/*
    W tym pliq maj± byc TYLKO f-cje zaczynaj±ce siê na
    free_... lub
    new_...  inaczej --> zabijê :)            --Thanos
    No, mo¿e jeszcze clean_... byæ ;)
 */

#include <sys/types.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#if !defined( WIN32 )
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <arpa/telnet.h>
#include <netdb.h>
#endif

#include "mud.h"


SHIP_DATA * first_free_ship = NULL;
SHIP_DATA * last_free_ship = NULL;

int	shrec_count;

void free_course( COURSE_DATA *course )
{
    STRFREE( course->stop_name );
    DISPOSE( course );
}

void free_courses( SHIP_DATA *ship )
{
    COURSE_DATA * 	course;
    COURSE_DATA * 	course_next;

    if( !ship || !ship->first_stop )
	return;

    for( course = ship->first_stop; course; course = course_next )
    {
	course_next = course->next;
	UNLINK( course, ship->first_stop, ship->last_stop, next, prev );
	free_course( course );
    }

    ship->first_stop 	= NULL;
    ship->last_stop 	= NULL;
    ship->curr_stop 	= NULL;

    return;
}

EDD *new_ed()
{
	EDD	*ed;

	CREATE( ed, EDD, 1 );
    STRDUP( ed->description, "" );
    STRDUP( ed->keyword, "" );
	top_ed++;
	return ed;
}

void free_ed( EDD *ed )
{
    STRFREE( ed->description );
    STRFREE( ed->keyword );
    DISPOSE( ed );
    top_ed--;
}

ED *new_exit()
{
	ED	*pExit;

	CREATE( pExit, ED, 1 );
	STRDUP( pExit->keyword, "" );
    STRDUP( pExit->description, "" );
    top_exit++;
	return pExit;
}

void free_exit( EXIT_DATA *pExit )
{
    STRFREE( pExit->keyword );
    STRFREE( pExit->description );
    DISPOSE( pExit );
    top_exit--;
}

MPROG_DATA *new_mprog()
{
	MPROG_DATA	*prog;

	CREATE( prog, MPROG_DATA, 1 );
	STRDUP( prog->arglist, "0" );
	STRDUP( prog->comlist, "break" NL );
	prog->type = 0|RAND_PROG;
	prog->next = NULL;
	return prog;
}

void free_mprog( MPROG_DATA *prg )
{
    STRFREE( prg->arglist );
    STRFREE( prg->comlist );
    DISPOSE( prg );
}

AFFECT_DATA *new_affect()
{
	AFFECT_DATA	*pAf;
	CREATE( pAf, AFFECT_DATA, 1 );
	pAf->next		= NULL;
	pAf->prev		= NULL;
	pAf->type		= 0;
	pAf->duration	= 0;
	pAf->location	= 0;
	pAf->modifier	= 0;
	pAf->bitvector	= 0;


	top_affect++;
	return pAf;
}

void free_affect( AFFECT_DATA *pAf )
{
    DISPOSE( pAf );
    top_affect--;
}

RESET_DATA *new_reset()
{
	RESET_DATA	*pReset;

	CREATE( pReset, RESET_DATA, 1 );
    top_reset++;
	return pReset;
}

void free_reset( RESET_DATA *pReset )
{
    DISPOSE( pReset );
    top_reset--;
}

void free_area( AREA_DATA *are )
{
    STRFREE( are->name 		);
    STRFREE( are->filename 	);
    STRFREE( are->author 	);
    STRFREE( are->resetmsg 	);

    DISPOSE( are );
    top_area--;
}

void free_note( NOTE_DATA *pnote )
{
    STRFREE( pnote->text    	);
    STRFREE( pnote->subject 	);
    STRFREE( pnote->to_list 	);
    STRFREE( pnote->date    	);
    STRFREE( pnote->sender  	);
    STRFREE( pnote->yesvotes 	);
    STRFREE( pnote->novotes 	);
    STRFREE( pnote->abstentions );

    DISPOSE( pnote );
}

/*
 * Free a command structure					-Thoric
 */
void free_command( CMDTYPE *command )
{
	STRFREE( command->name );
	if( command->alias )
		STRFREE( command->alias );
	DISPOSE( command );
}


void free_social( SOCIALTYPE *social )
{
    STRFREE( social->name 		);
    STRFREE( social->char_no_arg 	);
    STRFREE( social->others_no_arg 	);
    STRFREE( social->char_found 	);
    STRFREE( social->others_found 	);
    STRFREE( social->vict_found 	);
    STRFREE( social->char_auto 		);
    STRFREE( social->others_auto 	);

    DISPOSE( social );
}



void free_pcdata( PC_DATA *pcdata )
{
    int i;

    STRFREE( pcdata->prefix		);
    STRFREE( pcdata->homepage		);
    STRFREE( pcdata->pwd		);
    STRFREE( pcdata->email		);
    STRFREE( pcdata->bamfin		);
    STRFREE( pcdata->bamfout		);
    STRFREE( pcdata->rank		);
    STRFREE( pcdata->title		);
    STRFREE( pcdata->fake_title 	);/* * * * * * */
    STRFREE( pcdata->fake_name	 	);/* Thanos:   */
    STRFREE( pcdata->fake_desc	 	);/*           */
    STRFREE( pcdata->fake_long	 	);/* Nowy      */
    for( i=0; i<6; i++ )                  /* Disguise  */
    STRFREE( pcdata->fake_infl[i] 	);/* * * * * * */
    STRFREE( pcdata->bestowments 	);
    STRFREE( pcdata->invis_except	); /*Thanos*/
    STRFREE( pcdata->isolated_by	);
    STRFREE( pcdata->silenced_by	); /* Trog */
    STRFREE( pcdata->bio		);
    STRFREE( pcdata->authed_by		);
    STRFREE( pcdata->prompt		);
    STRFREE( pcdata->forbidden_cmd	);
    STRFREE( pcdata->afk_reason     	);
    STRFREE( pcdata->quest_done 	);
    STRFREE( pcdata->editinfo		); /*Thanos*/
    STRFREE( pcdata->ignorelist		); /*Thanos*/
    STRFREE( pcdata->tmp_site 		); /*Thanos*/
    STRFREE( pcdata->host 		); /*Thanos*/
    STRFREE( pcdata->user 		); /*Thanos*/

    if( pcdata->first_alias )
    {
	ALIAS_DATA *	alias;
	ALIAS_DATA * 	alias_next;

	for( alias = pcdata->first_alias; alias; alias=alias_next )
	{
	    alias_next = alias->next;

	    STRFREE( alias->name );
	    STRFREE( alias->sub );
	     UNLINK( alias, pcdata->first_alias,
	    		    pcdata->last_alias, next, prev );
	    DISPOSE( alias );
	}
    }

    if( pcdata->first_last_tell )
    {
	LAST_DATA * last;
	LAST_DATA * last_next;

    	for( last = pcdata->first_last_tell; last; last = last_next )
	{
	    last_next = last->next;

	    STRFREE( last->teller 	);
	    STRFREE( last->text		);
	    STRFREE( last->time		);
	     UNLINK( last,  pcdata->first_last_tell,
			    pcdata->last_last_tell, next, prev );
	    DISPOSE( last );
	}
    }
    DISPOSE( pcdata );
}

/*
 * Free a character.
 */
void free_char( CHAR_DATA *ch )
{
    OBJ_DATA 		*obj;
    AFFECT_DATA 	*paf;
    TIMER 		*timer;
    MPROG_ACT_LIST 	*mpact, *mpact_next;
    CRIME_DATA 		*crime;
	KNOWN_CHAR_DATA	*_friend; //Tanglor
    int	i;

    if ( !ch )
    {
      bug( "Free_char: null ch!" );
      return;
    }

    if ( ch->desc )
      bug( "Free_char: char (%s) still has descriptor.", ch->name );

    while ( (obj = ch->last_carrying) != NULL )
	extract_obj( obj );
    ch->first_carrying 	= NULL;
    ch->last_carrying 	= NULL;

    while ( (paf = ch->last_affect) != NULL )
	affect_remove( ch, paf );
    ch->first_affect 	= NULL;
    ch->last_affect 	= NULL;

    while ( (crime = ch->last_crime) != NULL )
	free_crime( ch, crime );
    ch->first_crime 	= NULL;
    ch->last_crime 	= NULL;

    while ( (timer = ch->first_timer) != NULL )
	extract_timer( ch, timer );
    ch->first_timer = NULL;

    STRFREE( ch->name		);
    STRFREE( ch->long_descr	);
    STRFREE( ch->description	);
    for( i=0; i<6; i++ )
    STRFREE( ch->przypadki[i]	);

    STRFREE( ch->dest_buf 	);
    STRFREE( ch->dest_buf_2 	);

    STRFREE( ch->s_vip_flags    );
    STRFREE( ch->mob_clan 	);

    // Ratm - zwalnianie pamieci zajetej przez zmienne
    if( ch->variables )
    {
	for( i=0; i<10; i++ )
	{
	    if( ch->variables->global_vars[i] )
		STRFREE( ch->variables->global_vars[i] );
	    if( ch->variables->local_vars[i] )
		STRFREE( ch->variables->local_vars[i] );
	    if( ch->variables->prog_args[i] )
		STRFREE( ch->variables->prog_args[i] );
	}
	DISPOSE( ch->variables );
    }

    stop_hunting( ch );
    stop_hating ( ch );
    stop_fearing( ch );
    free_fight  ( ch );

    if ( ch->pnote )
	free_note( ch->pnote );
    ch->pnote =	 NULL;

    if( ch->pcdata )
	free_pcdata( ch->pcdata );
    ch->pcdata 	= NULL;

    for ( mpact = ch->mpact; mpact; mpact = mpact_next )
    {
	mpact_next = mpact->next;
	free_mpact( mpact );
    }
    ch->mpact 	= NULL;
    ch->inquest = NULL;
	while ( (_friend = ch->last_known) != NULL )
	{
		STRFREE(_friend->name);
		UNLINK(_friend,ch->first_known,ch->last_known,next,prev);
		DISPOSE(_friend);
	};
	ch->first_known	=NULL;
	ch->last_known 	=NULL;

    char_from_room( ch );

    DISPOSE( ch );
    return;
}

CLAN_DATA *new_clan()
{
	CLAN_DATA	*clan;
	int			i;

	CREATE( clan, CLAN_DATA, 1 );
	STRDUP( clan->name,			"" );
	STRDUP( clan->filename,		"" );
	STRDUP( clan->description,	"" );
	STRDUP( clan->vYes,			"" );
//	STRDUP( clan->vNo,			"" );
	STRDUP( clan->tmpstr,		"" );
	clan->next				= NULL;
	clan->prev				= NULL;
	clan->first_member		= NULL;
	clan->last_member		= NULL;
	clan->first_last_ctalk	= NULL;
	clan->last_last_ctalk	= NULL;
	clan->next_suborg		= NULL;
	clan->prev_suborg		= NULL;
	clan->first_suborg		= NULL;
	clan->last_suborg		= NULL;
	clan->mainorg			= NULL;
	clan->vClan				= NULL;
	clan->first_hq			= NULL;
	clan->last_hq			= NULL;
	clan->first_politics	= NULL;
	clan->last_politics		= NULL;
	clan->leader			= NULL;
	clan->first				= NULL;
	clan->second			= NULL;
	clan->diplomat			= NULL;
	clan->type				= CLAN_CONSTRUCTION;
	clan->clan_id			= 0;
	clan->flags				= 0;
	clan->penalty			= 0;
	clan->vehicles			= 0;
	clan->spacecrafts		= 0;
	clan->branches			= 0;
	clan->members			= 0;
	clan->funds				= 0;
	clan->loan				= 0;
	clan->repay_date		= 0;
	clan->pkills			= 0;
	clan->pdeaths			= 0;
	clan->mkills			= 0;
	clan->mdeaths			= 0;
	for( i = 0; i <= CLAN_LEADER; i++ )
		STRDUP( clan->rank[i],	"" );
	top_clan++;

	return clan;
}

void free_clan( CLAN_DATA *clan )
{
	CLAN_DATA	*suborg;
	CLAN_DATA	*suborg_next;
	MEMBER_DATA	*member;
	MEMBER_DATA	*member_next;
	int			i;

	STRFREE( clan->name );
	STRFREE( clan->filename );
	STRFREE( clan->description );
	STRFREE( clan->vYes );
//	STRFREE( clan->vNo );
	STRFREE( clan->tmpstr );
	for( i = 0; i <= CLAN_LEADER; i++ )
		STRFREE( clan->rank[i] );

	for( suborg = clan->first_suborg; suborg; suborg = suborg_next )
	{
		suborg_next = clan->next_suborg;
		UNLINK( suborg, clan->first_suborg, clan->last_suborg, next_suborg, prev_suborg );
		suborg->mainorg = NULL;
		suborg->type = CLAN_ORGANIZATION;
		save_clan( suborg );
	}

	/* free HQ here */

	for( member = clan->first_member; member; member = member_next )
	{
		member_next = member->next;
		UNLINK( member, clan->first_member, clan->last_member, next, prev );
		free_member( member );
	}

	DISPOSE( clan );
	top_clan--;
}

MEMBER_DATA *new_member( int status )
{
	MEMBER_DATA	*member;

	CREATE( member, MEMBER_DATA, 1 );
	STRDUP( member->name,		"" );
	STRDUP( member->bestowments,"" );
	member->status = status;

	return member;
}

void free_member( MEMBER_DATA *member )
{
    STRFREE( member->name );
    STRFREE( member->bestowments );
    DISPOSE( member );
}

HELPS_FILE *new_helps_file()
{
	HELPS_FILE 	*fHelp;

	CREATE( fHelp, HELPS_FILE, 1 );
	STRDUP( fHelp->name, DEFAULT_HELP_FILE );

    return fHelp;
}

void free_helps_file( HELPS_FILE *fHelp )
{
	HELP_DATA	*pHelp;
	HELP_DATA	*pHelp_next;

	for( pHelp = fHelp->first_help; pHelp; pHelp = pHelp_next )
	{
		pHelp_next = pHelp->next;
		UNLINK( pHelp, fHelp->first_help, fHelp->last_help, next, prev );
		free_help( pHelp );
	}

	clear_helps_file( fHelp->name );
	STRFREE( fHelp->name );
	DISPOSE( fHelp );
}

void clear_helps_file( char *filename )
{
	char	filepath[MIL];

	sprintf( filepath, "%s%s", HELPS_DIR, filename );
	unlink( filepath );
}

HELP_DATA *new_help( HELPS_FILE *fHelp )
{
    HELP_DATA *	pHelp;

    CREATE( pHelp, HELP_DATA, 1 );
	pHelp->file = fHelp;
    STRDUP( pHelp->keyword,	"" );
    STRDUP( pHelp->syntax, 	"" );
    STRDUP( pHelp->text, 	"" );
    pHelp->type 	= HELP_PHELP;
    pHelp->level	= 0;

    return pHelp;
}

void free_help( HELP_DATA *pHelp )
{
    STRFREE( pHelp->keyword 	);
    STRFREE( pHelp->syntax 	);
    STRFREE( pHelp->text 	);
    DISPOSE( pHelp );
}

SEASON_DATA *new_season( PLANET_DATA *planet, const char *name )
{
    SEASON_DATA *season;

    CREATE( season, SEASON_DATA, 1 );
    STRDUP( season->name, name );
    season->temperature 	= 0;
    season->diff_temperature 	= 0;
    season->mmhg	 	= 1000;
    season->diff_mmhg	 	= 0;
    season->day_length	 	= planet->daylen;
    season->night_length	= planet->nightlen;
    season->length		= 3;
    season->windspeed		= 10;

    if( season->day_length     <= 0)
	season->day_length	= 12;

    if( season->night_length   <= 0)
	season->night_length	= 12;

    return season;
}

void free_who( WHO_DATA *who )
{
    STRFREE( who->text );
    DISPOSE( who );
}

void free_ban( BAN_DATA *pban )
{
    STRFREE(pban->ban_time);
    STRFREE( pban->name );
    DISPOSE( pban );
}

void free_mpact( MPROG_ACT_LIST * mpact )
{
    STRFREE( mpact->buf );
    DISPOSE( mpact );
}

void free_bounty( BOUNTY_DATA *bounty )
{
    STRFREE( bounty->target   );
    DISPOSE( bounty );
}

void free_last( LAST_DATA *last )
{
    STRFREE( last->teller);
    STRFREE( last->text	);
    STRFREE( last->time	);
    STRFREE( last->verb	);
    DISPOSE( last );
}

void free_alias( ALIAS_DATA *alias )
{
    STRFREE( alias->name );
    STRFREE( alias->sub );
    DISPOSE( alias );
}

void free_desc( DESCRIPTOR_DATA *d )
{
#if !defined( WIN32 )
    close( d->descriptor );
#else
    closesocket( d->descriptor );
#endif
    STRFREE( d->host 		);
    DISPOSE( d->outbuf 		);
    STRFREE( d->user 		);    /* identd */
    if ( d->pagebuf )
    DISPOSE( d->pagebuf 	);

    if( d->tmp_ch )
	DISPOSE( d->tmp_ch );

    DISPOSE( d );
    --num_descriptors;
    return;
}

void free_crime( CHAR_DATA *ch, CRIME_DATA *crime )
{
    if ( !ch->first_crime )
    {
	bug( "Free_crime: %s-->no crime.", ch->name );
	return;
    }

    UNLINK( crime, ch->first_crime, ch->last_crime, next, prev );
    STRFREE( crime->planet );
    DISPOSE( crime );
    return;
}

void free_wiz( WIZENT *wiz )
{
    STRFREE(wiz->name);
    DISPOSE(wiz);
}

RID *new_room()
{
	RID	*room;

	CREATE( room, RID, 1 );
	top_room++;
    STRDUP( room->name ,"" );
    STRDUP( room->homename, "" );
    STRDUP( room->description, "" );
    STRDUP( room->nightdesc, "" );
	CREATE( room->variables, VAR_DATA, 1 );
	bzero( room->variables, sizeof( VAR_DATA ) );

	return room;
}

void free_room( RID *room )
{
    RESET_DATA *res, *res_next;

    if (!is_room_unlinked(room))
	{
		bug( "room is being freed, but it's NOT unlinked!" );
		/* Trog: wywalam muda, bo muszae miec core'a */
		res = NULL;
		res->extra = 1;
	}

    if( room->vnum > 0 && room->vnum < MAX_VNUM )
	top_room--;
    else
	top_vroom--;

    for( res = room->first_reset; res; res = res_next )
    {
	res_next=res->next;
    	free_reset( res );
    }

    // Ratm - zwalnianie pamieci zajetej przez zmienne
    if( room->variables )
    {
	int i;

	for( i=0; i<10; i++ )
	{
	    if( room->variables->global_vars[i] )
		STRFREE( room->variables->global_vars[i] );
	    if( room->variables->local_vars[i] )
		STRFREE( room->variables->local_vars[i] );
	    if( room->variables->prog_args[i] )
		STRFREE( room->variables->prog_args[i] );
	}
	DISPOSE( room->variables );
    }


    STRFREE( room->name 	);
    STRFREE( room->homename 	);
    STRFREE( room->description 	);
    STRFREE( room->nightdesc 	);
    DISPOSE( room );
}

void free_hhf( HHF_DATA *hhf )
{
    STRFREE( hhf->name );
    DISPOSE( hhf );
}

OID *new_obj_index()
{
	OID	*obj;
	int	i;

	CREATE( obj, OID, 1 );
    STRDUP( obj->name, "" );
    STRDUP( obj->description, "" );
    STRDUP( obj->action_desc, "" );
    for( i = 0; i < 6; i++ )
		STRDUP( obj->przypadki[i], "" );
	obj->first_affect	= NULL;
	obj->last_affect	= NULL;

	return obj;
}

void free_obj_index( OID *obj )
{
	int	i;

	STRFREE( obj->name );
	STRFREE( obj->description );
	STRFREE( obj->action_desc );

	for( i = 0; i < 6; i++ )
		STRFREE( obj->przypadki[i] );
	DISPOSE( obj );
}

void free_obj( OBJ_DATA *obj )
{
    int i;
    STRFREE( obj->name        	);
    STRFREE( obj->description 	);
    STRFREE( obj->action_desc 	);
    if( obj->armed_by 	)
    STRFREE( obj->armed_by 	);
    STRFREE( obj->owner_name 	);

    for( i=0; i<6; i++ )
        STRFREE( obj->przypadki[i] );

    // Ratm - zwalnianie pamieci zajetej przez zmienne
    if( obj->variables )
    {
	for( i=0; i<10; i++ )
	{
	    if( obj->variables->global_vars[i] )
		STRFREE( obj->variables->global_vars[i] );
	    if( obj->variables->local_vars[i] )
		STRFREE( obj->variables->local_vars[i] );
	    if( obj->variables->prog_args[i] )
		STRFREE( obj->variables->prog_args[i] );
	}
	DISPOSE( obj->variables );
    }

    DISPOSE( obj );
}


void free_nameslist( NAMESLIST_DATA *nameslist )
{
    STRFREE( nameslist->name );
    DISPOSE( nameslist );
}

void free_season( SEASON_DATA *season )
{
    STRFREE( season->name );
    DISPOSE( season );
}

void free_dock( DOCK_DATA *dock )
{
    STRFREE( dock->name );
    DISPOSE( dock );
}

void free_chapter( CHAPTER_INDEX_DATA *chapt )
{
	QUEST_CMND_DATA *	pCmnd;
	QUEST_CMND_DATA *	pCmnd_next;
	QUEST_ACTION_DATA *	pAction;
	QUEST_ACTION_DATA *	pAction_next;

	for( pCmnd=chapt->first_init_cmd; pCmnd; pCmnd=pCmnd_next )
	{
		pCmnd_next = pCmnd->next;
		UNLINK( pCmnd, chapt->first_init_cmd,
				chapt->last_init_cmd, next, prev );
		free_qcmd( pCmnd );
	}

	for( pCmnd=chapt->first_event; pCmnd; pCmnd=pCmnd_next )
	{
		pCmnd_next = pCmnd->next;
		UNLINK( pCmnd, chapt->first_event,
				chapt->last_event, next, prev );
		free_qcmd( pCmnd );
	}

	for( pAction=chapt->first_action; pAction; pAction=pAction_next )
	{
		pAction_next = pAction->next;
		UNLINK( pAction, chapt->first_action,
				chapt->last_action, next, prev );
		free_qaction( pAction );
	}

	STRFREE( chapt->name );
	DISPOSE( chapt );
}

void free_qcmd( QUEST_CMND_DATA *pCmnd )
{
    STRFREE( pCmnd->arg4 );
    DISPOSE( pCmnd );
}

void free_qaction( QUEST_ACTION_DATA *action )
{
    STRFREE( action->arg4 );
    DISPOSE( action );
}

void free_smaug_aff( SMAUG_AFF *aff )
{
    STRFREE( aff->modifier );
    STRFREE( aff->duration );
    DISPOSE( aff );
}

void free_star( STAR_DATA *star )
{
    STRFREE( star->name );
    DISPOSE( star );
}

void free_moon( MOON_DATA *moon )
{
    STRFREE( moon->name );
    DISPOSE( moon );
}

void free_missile( MISSILE_DATA *missile )
{
    STRFREE( missile->fired_by );
    DISPOSE( missile );
}

void free_starsystem( SPACE_DATA *starsystem )
{
    STRFREE( starsystem->name );
    STRFREE(starsystem->filename);
    DISPOSE( starsystem );
}

void free_transponder( TRANSPONDER_DATA *trans )
{
    STRFREE( trans->number );
    STRFREE( trans->shipname );
    DISPOSE( trans );
}

void free_astro( ASTRO_DATA *astro )
{
    STRFREE(astro->name);
    STRFREE(astro->filename);
    STRFREE(astro->home);
    DISPOSE( astro );
}

void free_suspect( CHAR_DATA *ch, SUSPECT_DATA *sus )
{
    UNLINK( sus, ch->first_suspect, ch->last_suspect, next, prev );
    STRFREE( sus->name );
    DISPOSE( sus );
    return;
}

void free_script( SCRIPT_DATA *script )
{
    STRFREE( script->command );
    DISPOSE( script );
}

void free_mid( MOB_INDEX_DATA *mid )
{
    int i;

    for( i=0; i<6; i++ )
    STRFREE( mid->przypadki[i] 	);
    STRFREE( mid->player_name	);
    STRFREE( mid->long_descr 	);
    STRFREE( mid->description	);
    STRFREE( mid->s_vip_flags	);
    DISPOSE( mid );
}

void free_oid( OBJ_INDEX_DATA *oid )
{
    int i;

    for( i=0; i<6; i++ )
    STRFREE( oid->przypadki[i] 	);

    STRFREE( oid->name 		);
    STRFREE( oid->description 	);
    STRFREE( oid->action_desc 	);

    DISPOSE( oid );
}

void free_inform( INFORM_DATA *inform )
{
    STRFREE( inform->attacker );
    STRFREE( inform->victim );
    DISPOSE( inform );
}

void free_ship( SHIP_DATA *ship )
{
    COURSE_DATA * 	stop;
    COURSE_DATA * 	stop_next;
    TURRET_DATA	*	turret;
    TURRET_DATA	*	turret_next;
    HANGAR_DATA	*	hanger;
    HANGAR_DATA	*	hanger_next;
    MODULE_DATA	*	module;
    MODULE_DATA	*	module_next;
    TRANSPONDER_DATA *	trans;
    TRANSPONDER_DATA *	trans_next;
    SHIPDOCK_DATA *	dock;
    SHIPDOCK_DATA *	dock_next;
    ROOM_INDEX_DATA *	room;
    ROOM_INDEX_DATA *	room_next;
    char		buf		[MSL];


    sprintf( buf, "Freeing ship: %s", ship->name );
    log_string( buf );

    extract_ship( ship );

    if( ship->starsystem )
	ship_from_starsystem( ship, ship->starsystem );

    UNLINK( ship, first_ship, last_ship, next, prev );

    for( stop = ship->first_stop; stop; stop = stop_next )
    {
	stop_next = stop->next;
	UNLINK( stop, ship->first_stop, ship->last_stop, next, prev );
	DISPOSE( stop );
    }

    for( turret = ship->first_turret; turret; turret = turret_next )
    {
	turret_next = turret->next;
	UNLINK( turret, ship->first_turret, ship->last_turret, next, prev );
	DISPOSE( turret );
    }

    for( hanger = ship->first_hangar; hanger; hanger = hanger_next )
    {
	hanger_next = hanger->next;
	UNLINK( hanger, ship->first_hangar, ship->last_hangar, next, prev );
	DISPOSE( hanger );
    }

/*    for( cargo = ship->first_cargo; cargo; cargo = cargo_next )
    {
	cargo_next = cargo->next;
	UNLINK( cargo, ship->first_cargo, ship->last_cargo, next, prev );
	DISPOSE( cargo );
    }    */

    for( module = ship->first_module; module; module = module_next )
    {
	module_next = module->next;
	UNLINK( module, ship->first_module, ship->last_module, next, prev );
	STRFREE( module->spyname );
	DISPOSE( module );
    }

/*
    for( smap = ship->first_smap; smap; smap = smap_next )
    {
	smap_next = smap->next;
	UNLINK( smap, ship->first_smap, ship->last_smap, next, prev );
	STRFREE( smap->name );
	DISPOSE( smap );
    }
*/
    for( trans = ship->first_trans; trans; trans = trans_next )
    {
	trans_next = trans->next;
	UNLINK( trans, ship->first_trans, ship->last_trans, next, prev );
	free_transponder( trans );
    }

    for( dock = ship->first_dock; dock; dock = dock_next )
    {
	dock_next = dock->next;
	UNLINK( dock, ship->first_dock, ship->last_dock, next, prev );
	DISPOSE( dock );
    }

    for (room = ship->first_location; room; room = room_next)
	{
		EXTRA_DESCR_DATA * ed;
		EXTRA_DESCR_DATA * ed_next;
		EXIT_DATA * xit;
		EXIT_DATA * xit_next;
		MPROG_DATA * mprog;
		MPROG_DATA * mprog_next;
		MPROG_ACT_LIST * mpact;
		MPROG_ACT_LIST * mpact_next;

		room_next = room->next_on_ship;

		if (room->first_person)
		{
			CHAR_DATA * ech;
			CHAR_DATA * ech_next;

			bug( "free_ship: FATAL: room with people (#%d on %s)",
					VNUM(room), ship->name );

			for (ech = room->first_person; ech; ech = ech_next)
			{
				ech_next = ech->next_in_room;

				if (ech->fighting)
					stop_fighting(ech, true);

				if (IS_NPC(ech))
					extract_char(ech, true);
				else
				{
					char_from_room(ech);
					char_to_room(ech, get_room_index(ROOM_VNUM_LIMBO));
				}
			}
		}

		for (ed = room->first_extradesc; ed; ed = ed_next)
		{
			ed_next = ed->next;
			UNLINK( ed, room->first_extradesc, room->last_extradesc, next, prev );
			free_ed(ed);
		}

		for (xit = room->first_exit; xit; xit = xit_next)
		{
			xit_next = xit->next;
			UNLINK( xit, room->first_exit, room->last_exit, next, prev );
			extract_exit(room, xit);
		}

		for (mprog = room->mudprogs; mprog; mprog = mprog_next)
		{
			mprog_next = mprog->next;
			free_mprog(mprog);
		}

		for (mpact = room->mpact; mpact; mpact = mpact_next)
		{
			mpact_next = mpact->next;
			free_mpact(mpact);
		}

		UNLINK( room, ship->first_location, ship->last_location,
				next_on_ship, prev_on_ship );

		unlink_room(room);
		free_room(room);
	}

/* Nie wiem czemu to kurwa nie dziala!
   Jak tylko uzywam ponizszych STRFREE, a nastepnie DISPOSE
   - mud pada na nasepnej probie zaalokowania jakiejkolwiek innej pamieci!
   Ale ok, mam sposob ;)   -- Thanos */
/*
    STRFREE( ship->description 	);
    STRFREE( ship->transponder 	);
    STRFREE( ship->filename 	);
    STRFREE( ship->name 	);
    STRFREE( ship->home 	);
    STRFREE( ship->transponder 	);
    STRFREE( ship->ship_title 	);
    STRFREE( ship->owner 	);
    STRFREE( ship->pilot 	);
    STRFREE( ship->transponder 	);
    STRFREE( ship->copilot 	);
    STRFREE( ship->engineer 	);
    STRFREE( ship->dest 	);
    STRFREE( ship->cargofrom 	);
    STRFREE( ship->sXpos 	);
    STRFREE( ship->sYpos 	);
    STRFREE( ship->sslook 	);
    STRFREE( ship->lock_key 	);


    DISPOSE( ship );    */

/* zrobimy cos ala smietnik - tam beda lezaly statki nieuzywane
   acz ladnie oczyszczone. Teraz jesli bedziemy chcieli stworzyc
   nowy statek nie bedziemy uzywac CREATE, tylko wyciagniemy jakis
   stateczek ze smietnika */
    LINK( ship, first_free_ship, last_free_ship, next, prev );
    return;
}

SHIP_DATA *new_ship( )
{
    SHIP_DATA *	ship;

    if( first_free_ship )
    {
	ship = first_free_ship;
	UNLINK( ship, first_free_ship, last_free_ship, next, prev );
    }
    else
	CREATE( ship, SHIP_DATA, 1 );

    STRDUP( ship->description,  "" );
    STRDUP( ship->transponder, 	"" );
    STRDUP( ship->filename, 	"" );
    STRDUP( ship->name, 	"" );
    STRDUP( ship->home, 	"" );
    STRDUP( ship->transponder, 	"" );
    STRDUP( ship->ship_title, 	"" );
    STRDUP( ship->owner, 	"" );
    STRDUP( ship->pilot, 	"" );
    STRDUP( ship->transponder, 	"" );
    STRDUP( ship->copilot, 	"" );
    STRDUP( ship->engineer, 	"" );
    STRDUP( ship->dest, 	"" );
    STRDUP( ship->cargofrom, 	"" );
    STRDUP( ship->sXpos, 	"" );
    STRDUP( ship->sYpos, 	"" );
    STRDUP( ship->sslook, 	"" );
    STRDUP( ship->lock_key, 	"" );

    ship->pIndexData 		= NULL;
    ship->next			= NULL;
    ship->prev			= NULL;
    ship->next_in_starsystem	= NULL;
    ship->prev_in_starsystem	= NULL;
    ship->next_in_room		= NULL;
    ship->prev_in_room		= NULL;
    ship->first_stop		= NULL;
    ship->last_stop		= NULL;
    ship->curr_stop		= NULL;
    ship->first_turret		= NULL;
    ship->last_turret		= NULL;
    ship->first_hangar		= NULL;
    ship->last_hangar		= NULL;
    ship->in_room		= NULL;
    ship->first_cargo		= NULL;
    ship->last_cargo		= NULL;
    ship->first_module		= NULL;
    ship->last_module		= NULL;
    ship->first_smap		= NULL;
    ship->last_smap		= NULL;
    ship->first_trans		= NULL;
    ship->last_trans		= NULL;
    ship->first_dock		= NULL;
    ship->last_dock		= NULL;
    ship->first_location	= NULL;
    ship->last_location		= NULL;
    ship->cockpit		= NULL;
    ship->location		= NULL;
    ship->lastdoc		= NULL;
    ship->shipyard		= NULL;
    ship->navseat		= NULL;
    ship->pilotseat		= NULL;
    ship->coseat		= NULL;
    ship->gunseat		= NULL;
    ship->target0		= NULL;
    ship->currjump		= NULL;
    ship->killer		= NULL;
    ship->last_dock_with	= NULL;

    ship->flags 		= 0;
    ship->tmp_flags 		= 0;
    return ship;
}

SHIP_INDEX_DATA *new_ship_index( )
{
    SHIP_INDEX_DATA *	ship;

    CREATE( ship, SHIP_INDEX_DATA, 1 );
    ship->first_turret		= NULL;
    ship->last_turret		= NULL;
    ship->first_hangar		= NULL;
    ship->last_hangar		= NULL;
    ship->first_cargo		= NULL;
    ship->last_cargo		= NULL;
    ship->first_module		= NULL;
    ship->last_module		= NULL;
    ship->first_smap		= NULL;
    ship->last_smap		= NULL;
    ship->first_dock		= NULL;
    ship->last_dock		= NULL;
    ship->first_room		= NULL;
    ship->last_room		= NULL;
    STRDUP( ship->filename, 	"" );
    STRDUP( ship->name, 	"" );
    STRDUP( ship->builders, 	"" );
    STRDUP( ship->description, 	"" );
    ship->security 		= 2;
    ship->count			= 0;
    shrec_count++;
    return ship;
}

void free_sroom( SHIP_ROOM_DATA * sRoom )
{
    RESET_DATA *res, *res_next;
    EXTRA_DESCR_DATA *	ed;
    EXTRA_DESCR_DATA *	ed_next;
    SHIP_EXIT_DATA *    xit;
    SHIP_EXIT_DATA *    xit_next;
    MPROG_DATA *    	mprog;
    MPROG_DATA *    	mprog_next;

    for( ed = sRoom->first_extradesc; ed; ed = ed_next )
    {
        ed_next = ed->next;
        UNLINK( ed, sRoom->first_extradesc, sRoom->last_extradesc, next, prev );
        free_ed( ed );
    }

    for( xit = sRoom->first_exit; xit; xit = xit_next )
    {
        xit_next = xit->next;
        UNLINK( xit, sRoom->first_exit, sRoom->last_exit, next, prev );
        STRFREE( xit->keyword 		);
        STRFREE( xit->description 	);
	DISPOSE( xit );
    }

    for( mprog = sRoom->mudprogs; mprog; mprog = mprog_next )
    {
        mprog_next = mprog->next;
        free_mprog( mprog );
    }

    for( res = sRoom->first_reset; res; res = res_next )
    {
	res_next=res->next;
    	free_reset( res );
    }

    STRFREE( sRoom->name );
    STRFREE( sRoom->description );
    top_sroom--;
    DISPOSE( sRoom );
}

void free_srooms( SHIP_INDEX_DATA *ship )
{
    SHIP_ROOM_DATA *	sRoom;
    SHIP_ROOM_DATA *	sRoom_next;

    for( sRoom = ship->first_room; sRoom; sRoom = sRoom_next )
    {
	sRoom_next = sRoom->next;
	UNLINK( sRoom, ship->first_room, ship->last_room, next, prev );
	free_sroom( sRoom );
    }

    return;
}

void free_part( PART_DATA *part )
{
    COMPONENT_DATA *	comp;
    COMPONENT_DATA *	comp_next;

    for( comp = part->first_component; comp; comp = comp_next )
    {
        comp_next = comp->next;
        UNLINK( comp, part->first_component, part->last_component,
    		next, prev );
        DISPOSE( comp );
    }
    DISPOSE( part );
    return;
}

void free_project( PROJECT_DATA *pro )
{
    PART_DATA *		part;
    PART_DATA *		part_next;

    for( part = pro->first_part; part; part = part_next )
    {
	part_next = part->next;
	UNLINK( part, pro->first_part, pro->last_part, next, prev );
	free_part( part );
    }
    DISPOSE( pro );
    return;
}

void free_complain( COMPLAIN_DATA *complain )
{
    STRFREE( complain->text    	);
    STRFREE( complain->author 	);
    STRFREE( complain->date 	);
    STRFREE( complain->fixedby    	);
    STRFREE( complain->fixdate  	);

    DISPOSE( complain );
}

void free_skill( SKILLTYPE *skill )
{
	SMAUG_AFF	*aff, *aff_next;

	STRFREE( skill->name );
	STRFREE( skill->noun_damage );
	STRFREE( skill->msg_off );
	if( skill->hit_char )
		STRFREE( skill->hit_char );
	if( skill->hit_vict )
		STRFREE( skill->hit_vict );
	if( skill->hit_room )
		STRFREE( skill->hit_room );
	if( skill->miss_char )
		STRFREE( skill->miss_char );
	if( skill->miss_vict )
		STRFREE( skill->miss_vict );
	if( skill->miss_room )
		STRFREE( skill->miss_room );
	if( skill->die_char )
		STRFREE( skill->die_char );
	if( skill->die_vict )
		STRFREE( skill->die_vict );
	if( skill->die_room )
		STRFREE( skill->die_room );
	if( skill->imm_char )
		STRFREE( skill->imm_char );
	if( skill->imm_vict )
		STRFREE( skill->imm_vict );
	if( skill->imm_room )
		STRFREE( skill->imm_room );
	if( skill->dice )
		STRFREE( skill->dice );
	if( skill->components )
		STRFREE( skill->components );
	if( skill->teachers )
		STRFREE( skill->teachers );
	for( aff = skill->affects; aff; aff = aff_next )
	{
		aff_next = aff->next;
		free_smaug_aff( aff );
	}

	DISPOSE( skill );
}

RACE_DATA *new_race()
{
	RACE_DATA	*race;
	int			i;

	CREATE( race, RACE_DATA, 1 );
	STRDUP( race->name, "" );
	STRDUP( race->filename, "" );
	STRDUP( race->description, "" );
	for( i = 0; i < 6; i++ )
		STRDUP( race->przypadki[i], "" );
        for( i = 0; i < 6; i++ )
		STRDUP( race->inflectsFemale[i], "" );
	race->flags				= FRACE_PROTOTYPE;
	race->affected			= 0;
	race->str_plus			= 0;
	race->dex_plus			= 0;
	race->wis_plus			= 0;
	race->int_plus			= 0;
	race->con_plus			= 0;
	race->cha_plus			= 0;
	race->lck_plus			= 0;
	race->frc_plus			= 0;
	race->hp_plus			= 0;
	race->force_plus		= 0;
	race->frc_plus			= 0;
	race->susceptible		= 0;
	race->resistant			= 0;
	race->immune			= 0;
	race->language			= NULL;
	race->next				= NULL;
	race->prev				= NULL;
	for( i = 0; i < MAX_DESC_TYPES; i++ )
		race->desc_restrictions[i]=0;

	return race;
}

void free_race( RACE_DATA *race )
{
	int	i;

	STRFREE( race->name );
	STRFREE( race->filename );
	STRFREE( race->description );
	STRFREE( race->name );
	for( i = 0; i < 6; i++ )
		STRFREE( race->przypadki[i] );
	for( i = 0; i < 6; i++ )
		STRFREE( race->inflectsFemale[i] );

	DISPOSE( race );
}

LANG_DATA *new_lang()
{
	LANG_DATA	*lang;

	CREATE( lang, LANG_DATA, 1 );
	STRDUP( lang->name, "" );
	STRDUP( lang->alphabet, "" );
	STRDUP( lang->deny_text, "" );
	lang->max_learn		= 99;
	lang->max_practice	= 75;
	lang->min_int		= 0;
	lang->difficulty	= 0;
	lang->next			= NULL;
	lang->prev			= NULL;

	return lang;
}

void free_lang( LANG_DATA *lang )
{
	STRFREE( lang->name );
	STRFREE( lang->alphabet );
	STRFREE( lang->deny_text );

	DISPOSE( lang );
}

KNOWN_LANG *new_known_lang()
{
	KNOWN_LANG	*lang;

	CREATE( lang, KNOWN_LANG, 1 );
	lang->language		= NULL;
	lang->next			= NULL;
	lang->prev			= NULL;
	lang->learned		= 1;
/*
	lang->times_used	= 0;
*/
	return lang;
}

void free_known_lang( KNOWN_LANG *lang )
{
	DISPOSE( lang );
}

BETS_PROGRES * new_bets()
{
	BETS_PROGRES *	pBets;
	CREATE( pBets,BETS_PROGRES,1);
	STRDUP( pBets->name, "" );
	pBets->next			= NULL;
	pBets->prev			= NULL;
	pBets->buy_out		= false;
	pBets->price		= 0;
	pBets->max_price	= 0;
	pBets->quantity		= 1;

	return pBets;
}

void free_bets(BETS_PROGRES *	pBets)
{
	STRFREE( pBets->name );
	DISPOSE( pBets );
}

AUCTION_DATA * new_auction()
{
	struct	tm *	tms;
	AUCTION_DATA *	pAuction;
	CREATE(pAuction,AUCTION_DATA,1);
	STRDUP( pAuction->seller_name, "" );
	STRDUP( pAuction->stock_name, "" );
	STRDUP( pAuction->desc, "" );
	pAuction->next				= NULL;
	pAuction->prev				= NULL;
	pAuction->first				= NULL;
	pAuction->last				= NULL;
	pAuction->planet			= NULL;
	pAuction->item				= NULL;
	pAuction->buy_out_now		= 0;
	pAuction->starting_price	= 0;
	pAuction->bet				= 0;
	pAuction->bidders_count		= 0;
	pAuction->item_collected	= false;
	pAuction->payment_collected	= false;
	pAuction->notes_send		= false;
	pAuction->type				= (auction_type)0;
	pAuction->quantity			= 1;
	pAuction->duration			= 0;

	tms = localtime(&current_time);
	tms->tm_min+=10;
	pAuction->start_time 		= mktime(tms);

	tms = localtime(&current_time);
	tms->tm_mday++;
	tms->tm_min+=10;
	pAuction->end_time			= mktime(tms);

	return pAuction;
}

void free_auction(AUCTION_DATA *	pAuction)
{
	BETS_PROGRES *	pBets;

	STRFREE( pAuction->seller_name );
	STRFREE( pAuction->stock_name );
	STRFREE( pAuction->desc );
	while ( pAuction->first)
	{
		pBets = pAuction->first;
		UNLINK( pBets, pAuction->first, pAuction->last, next, prev );
		free_bets( pBets);
	}
	if (pAuction->item)
	{
		UNLINK( pAuction->item, first_object, last_object,next,prev);
//		extract_obj( pAuction->item );
		free_obj(pAuction->item);
	}
	DISPOSE( pAuction );
}

STOCK_EXCHANGE_DATA * new_stock_exchange()
{
	STOCK_EXCHANGE_DATA * pStock;
	CREATE(pStock, STOCK_EXCHANGE_DATA, 1);
	pStock->next	= NULL;
	pStock->prev	= NULL;
	pStock->pPlanet	= NULL;
	pStock->first	= NULL;
	pStock->last	= NULL;
	pStock->first_new_offert=	NULL;
	pStock->last_new_offert	=	NULL;

	return pStock;
}

void free_stock_exchange(STOCK_EXCHANGE_DATA * pStock)
{
	AUCTION_DATA * pAuction;
	FOREACH( pAuction, pStock->first )
		free_auction( pAuction );
	FOREACH( pAuction, pStock->first_new_offert )
		free_auction( pAuction );

	UNLINK( pStock, first_stock_exchange, last_stock_exchange, next, prev );
	DISPOSE( pStock );
}



MATERIAL_DATA * new_material()
{
	MATERIAL_DATA			* pMaterial;
	CREATE( pMaterial, MATERIAL_DATA, 1 );
	STRDUP( pMaterial->name, "" );
	STRDUP( pMaterial->desc, "(brak)" );
	pMaterial->price			= 0;
	pMaterial->min_price		= 0;
	pMaterial->max_price		= 0;
	pMaterial->type				= U_ORE_FORM;
	pMaterial->rarity			= COMMON_MAT;
	return pMaterial;
}

void free_material( MATERIAL_DATA * pMaterial )
{
	STRFREE( pMaterial->name );
	STRFREE( pMaterial->desc );
	DISPOSE( pMaterial );
}

DIALOG_NODE *new_dialog_node()
{
	DIALOG_NODE *pNode;
	CREATE(pNode,DIALOG_NODE,1);
	STRDUP( pNode->text, "(brak)" );
	STRDUP( pNode->mob_answer, "");
	pNode->pBase			= NULL;
	pNode->first			= NULL;
	pNode->last				= NULL;
	pNode->next				= NULL;
	pNode->prev				= NULL;
	pNode->prog				= NULL;
	pNode->next_in_dialog	= NULL;//etap rozmowy
	pNode->prev_in_dialog	= NULL;	//etap rozmowy
	pNode->nr				= -1;
	pNode->target_nr		= -1;
	pNode->saved			= false;
	return pNode;
}

void free_dialog_node( DIALOG_NODE *pNode )
{
	DIALOG_DATA		* pData = pNode->pBase;
	DIALOG_NODE		* pLeaf = NULL, * pNextLeaf = NULL;

	if ( pData  )
		for( pLeaf= pNode->first; pLeaf	; pLeaf = pNextLeaf )
		{
			pNextLeaf = pLeaf->next_in_dialog;
			UNLINK( pLeaf, pData->first, pData->last, next, prev );
			UNLINK( pLeaf, pNode->first, pNode->last, next_in_dialog,
			prev_in_dialog );
			free_dialog_node( pLeaf );
		}

	if (pNode->prog)
		free_mprog( pNode->prog );

	STRFREE( pNode->text );
	STRFREE( pNode->mob_answer );
	DISPOSE( pNode );
}

DIALOG_DATA * new_dialog_data()
{
	DIALOG_DATA			* pData;	//pierwszy tekst jaki gracz moze wystosowac do moba
	DIALOG_NODE			* pNode;
	CREATE( pData,DIALOG_DATA,1);
	STRDUP( pData->name, "" );
	pData->next			= NULL;
	pData->prev			= NULL;
	pData->dialog_ID	= free_dialog_ID();

	pNode 				= new_dialog_node();
	LINK( pNode, pData->first, pData->last, next, prev );
	pNode->pBase		= pData;

	return pData;
}

void free_dialog_data( DIALOG_DATA * pData )
{
	DIALOG_NODE			* pNode;
	while ( pData->first)
	{
		pNode = pData -> first;
		UNLINK( pNode, pData->first, pData->last, next, prev );
		free_dialog_node( pNode );
	}
	STRFREE( pData->name );
	DISPOSE( pData );
}
DEPOSIT_DATA * new_deposit()
{
	DEPOSIT_DATA	* pDeposit;
	CREATE( pDeposit, DEPOSIT_DATA, 1);
	pDeposit->next = NULL;
	pDeposit->prev = NULL;
	STRDUP( pDeposit->material_name,	"" );
	pDeposit->daily_mining = 0;
	return pDeposit;
}

void free_deposit( DEPOSIT_DATA * pDeposit )
{
	STRFREE( pDeposit->material_name );
	DISPOSE( pDeposit )	;
}

CARGO_DATA * new_cargo()
{
	CARGO_DATA			* pCargo;
	CREATE( pCargo, CARGO_DATA, 1);
	pCargo->pMaterial	= NULL;
	return pCargo;
}

void free_cargo( CARGO_DATA * pCargo )
{
	if ( pCargo->pMaterial )
		free_material( pCargo->pMaterial );
	DISPOSE( pCargo );
}

HQ_DATA * new_hq_data()
{
	HQ_DATA * pHQ=NULL;

	CREATE( pHQ, HQ_DATA, 1);
	pHQ->next=NULL;
	pHQ->prev=NULL;
	pHQ->first_room=NULL;
	pHQ->last_room=NULL;
	pHQ->main=true;

	return pHQ;
}

TURBOCAR *new_turbocar()
{
	TURBOCAR	*tc;

	CREATE( tc, TURBOCAR, 1 );
	STRDUP( tc->name, "" );
	STRDUP( tc->filename, "" );
	tc->vnum = 0;
	tc->next = NULL;
	tc->prev = NULL;
	tc->current_station = NULL;
	tc->first_station = NULL;
	tc->last_station = NULL;

	return tc;
}

void free_turbocar( TURBOCAR *tc )
{
	TC_STATION	*station;
	TC_STATION	*station_next;

	STRFREE( tc->name );
	STRFREE( tc->filename );
	for( station = tc->first_station; station; station = station_next )
	{
		station_next = station->next;
		UNLINK( station, tc->first_station, tc->last_station, next, prev );
		free_station( station );
	}
	DISPOSE( tc );
}

TC_STATION *new_station()
{
	TC_STATION	*station;

	CREATE( station, TC_STATION, 1 );
	STRDUP( station->name, "" );
	station->vnum = 0;
	station->next = NULL;
	station->prev = NULL;

	return station;
}

void free_station( TC_STATION *station )
{
	STRFREE( station->name );
	DISPOSE( station );
}

FEVENT_DATA	*new_fevent()
{
	FEVENT_DATA	*fevent;

	CREATE( fevent, FEVENT_DATA, 1 );
	STRDUP( fevent->sattr, "" );
	fevent->trigger = (fe_trigger)0;
	memset( &fevent->attr, 0, FE_MAX_ATTR*sizeof(int64) );
	fevent->next = NULL;
	fevent->prev = NULL;

	return fevent;
}

void free_fevent( FEVENT_DATA *fevent )
{
	STRFREE( fevent->sattr );
	DISPOSE( fevent );
}

MID	*new_mob()
{
	MID	*pMobIndex;

	CREATE( pMobIndex, MID, 1 );
	clean_mob( pMobIndex );
	return pMobIndex;
}

void free_mob( MID *mob )
{
	MPROG_DATA	*mprog;
	MPROG_DATA	*mprog_next;
	int			i;

	STRFREE( mob->player_name );
	for( i = 0; i < 6; i++ )
		STRFREE( mob->przypadki[i] );
	STRFREE( mob->s_vip_flags );
	STRFREE( mob->long_descr );
	STRFREE( mob->description );
	STRFREE( mob->dialog_name );

	for( mprog = mob->mudprogs; mprog; mprog = mprog_next )
	{
		mprog_next = mprog->next;
		free_mprog( mprog );
	}
}

REQ *new_requirement()
{
	REQ	*req;

	CREATE( req, REQ, 1 );
	req->type = 0;
	req->location = 0;
	req->modifier = 0;
	top_req++;
	return req;
}

void free_requirement( REQ *req )
{
	DISPOSE( req );
}

SHD	*new_shop()
{
	SHD	*shop;

	CREATE( shop, SHD, 1 );
	top_shop++;
	return shop;
}

void free_shop( SHD *shop )
{
	top_shop--;
	DISPOSE( shop );
}

RHD *new_repair()
{
	RHD	*repair;

	CREATE( repair, RHD, 1 );
	return repair;
}

void free_repair( RHD *repair )
{
	DISPOSE( repair );
}

AREA_TMP_DATA *new_area_tmp()
{
	AREA_TMP_DATA	*area_tmp;

	CREATE( area_tmp, AREA_TMP_DATA, 1 );
	area_tmp->first_mob		= NULL;
	area_tmp->last_mob		= NULL;
	area_tmp->first_obj		= NULL;
	area_tmp->last_obj		= NULL;
	area_tmp->first_room	= NULL;
	area_tmp->last_room		= NULL;
	area_tmp->first_shop	= NULL;
	area_tmp->last_shop		= NULL;
	area_tmp->first_repair	= NULL;
	area_tmp->last_repair	= NULL;
	return area_tmp;
}

void free_area_tmp( AREA_TMP_DATA *area_tmp )
{
	DISPOSE( area_tmp );
}

ILD *new_ild()
{
	ILD	*ild;

	CREATE( ild, ILD, 1 );
	STRDUP( ild->data, "" );
	return ild;
}

void free_ild( ILD *ild )
{
	STRFREE( ild->data );
	DISPOSE( ild );
}

POLITICS_DATA *new_politics()
{
	POLITICS_DATA	*politics;

	CREATE( politics, POLITICS_DATA, 1 );
	politics->clan			= NULL;
	politics->relations		= 0;

	return politics;
}

void free_politics( POLITICS_DATA *politics )
{
	DISPOSE( politics );
}

