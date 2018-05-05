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
*       		 Quest Module Added by Thanos			   *
****************************************************************************/


#include <sys/types.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include "mud.h"


CHAR_DATA *	Quest_Master;
extern int 	top_quest;

#define CHAPTER_NEXT	       -1

#define STOP_RESIGN 		-4
#define STOP_WHO_DID_IT 	-3
#define STOP_TOO_LATE	 	-2
#define STOP_VERY_OK		-1
#define STOP_OK				 0
#define STOP_BAD	 		 1


#if defined(KEY)
#undef KEY
#endif
#define KEY( literal, field, value )				\
			if ( !str_cmp( word, literal ) )		\
{										\
			    field  = value;						\
			    fMatch = true;						\
			    break;								\
}


void quest_stop      args( ( CHAR_DATA *owner, int type) );
void mprog_driver    args( ( char* com_list, CHAR_DATA* mob, CHAR_DATA* actor,
							 OBJ_DATA* obj,  void* vo, bool single_step ) );

bool init_chapter    args( ( QUEST_DATA *quest, int chaptnr ) );

/*
 * f-cje robocze
 */
void q_log( const char *str, ... )
{
	char 	buf	[MAX_STRING_LENGTH];

	strcpy( buf, ": " );
	{
		va_list param;

		va_start(param, str);
		vsprintf( buf + strlen(buf), str, param );
		va_end(param);
	}
	to_channel( buf,CHANNEL_MONITOR, FB_BLUE "Quest", LEVEL_QUESTSEE );

	return;
}

void q_bug( const char *str, ... )
{
	char 	buf	[MAX_STRING_LENGTH];
	FILE *	fp;

	strcpy( buf, "[*****]: " );
	{
		va_list param;

		va_start(param, str);
		vsprintf( buf + strlen(buf), str, param );
		va_end(param);
	}

	to_channel( buf, CHANNEL_LOG, FB_WHITE "QUESTBUG ", LEVEL_LOG );

	LOG_CLOSE;
	if ( ( fp = fopen( QBUG_FILE, "a" ) ) != NULL )
	{
		fprintf( fp, "%s\n", buf );
		fclose( fp );
	}
	LOG_OPEN;

	return;
}

/* do mobkomend mpqoload i mpqmload oraz do can_see_obj */
bool belongs_to_quest( CHAR_DATA *ch, QUEST_DATA *quest )
{
	QUEST_MOB_DATA *qMob;

	if( ch->inquest == quest )
		return true;

	/* moby zaproszone */
	for( qMob = quest->first_mob; qMob; qMob = qMob->next )
	{
		if( qMob->vnum == ch->pIndexData->vnum )
			return true;
	}

	return false;
}


/* wbrew pozorom potrzebne */
void restore_quest_master()
{
	MOB_INDEX_DATA *	pMobIndex;
	CHAR_DATA *		masta;

	q_bug( "Quest Master not found. Trying to restore." );

	if ( ( pMobIndex = get_mob_index( MOB_VNUM_QUEST_MASTER ) ) == NULL )
	{
		q_bug( "Restore_Quest_Master: FATAL!!! No QuestMasterIndex found !!!" );
		return;
	}

	masta = create_mobile( pMobIndex );
	char_to_room( masta, get_room_index(3) );
	Quest_Master = masta;
	return;
}

bool done_that_quest( CHAR_DATA *ch, int id )
{
	QUEST_INDEX_DATA *	pQuest;
	char				buf	[MSL];

	if( IS_NPC( ch ) )
		return true;

	if( get_trust( ch ) >= LEVEL_QUESTSEE
		   && IS_SET( ch->act, PLR_HOLYLIGHT 	) )
		return false;

	// powtarzalne questy mimo, ¿e dodane s± do listy wykonanych,
	// uznane s± za niewykonywane dot±d.
	for( pQuest = first_quest_index; pQuest; pQuest = pQuest->next )
		if( pQuest->quest_id == id )
		{
			if( IS_SET( pQuest->flags, QUEST_REPEATABLE ) )
				return false;

			break;
		}

	sprintf( buf, "%d", id );

	if( is_name( buf, ch->pcdata->quest_done ) )
		return true;

	return false;
}

/* zaznaczamy, ¿e gracz ju¿ robi³ taki quest */
void add_to_done_quests( CHAR_DATA *ch, QUEST_DATA *quest )
{
	char	buf	[MSL];

	if( IS_NPC( ch ) )
		return;

	if( get_trust( ch ) >= LEVEL_QUESTSEE
		   && IS_SET( ch->act, PLR_HOLYLIGHT 	) )
		return;

	if( is_name( itoa( quest->pIndexData->quest_id ), ch->pcdata->quest_done ) )
		return;

	strcpy( buf, ch->pcdata->quest_done );
	strcat ( buf, " " );
	strcat ( buf, itoa( quest->pIndexData->quest_id ) );

	STRDUP( ch->pcdata->quest_done, buf );

	return;
}

/* ile czasu jeszcze zosta³o nam na zrobienie questa? */
int calc_time( QUEST_DATA *quest )
{
	int	time;

	time = quest->pIndexData->time  		* 60
			- (int)(current_time - quest->logon)	/ 60
			- quest->time_took;

	return time;
}


/* przetwarza akcjê(e) (je¶li jest zdefiniowana) PO zakoñczeniu chapteru */
bool process_action(  QUEST_DATA *quest, CHAPTER_DATA *chapter )
{
	QUEST_ACTION_DATA *	action;
	bool		done 	= true;

    for( action = chapter ? chapter->first_action :
		 quest->first_action;
		 action;
		 action = action->next )
	{
		if (action->done)
		{
			continue;
		}

		done = false; /*   <-----.                                       */
		/* to znaczy, ¿e znale¼li¶my akcjê, teraz: czy zostanie wykonana */

		switch ( action->command )
		{
			default:
				q_bug( "Process_action: unknown action type" );
				action->done = true;
				return 		false; /* nie zosta³a wykonana */

			case ACTION_STOPQUEST:
				action->done = true;
				if( !quest->finished )
					quest_stop( quest->player, action->arg1 );
				return 		true; /* zosta³a wykonana */

			case ACTION_NONE:
				action->done = true;
				done = 		true;
				break;

			case ACTION_ECHO:
				action->done = true;
				send_to_char( action->arg4, quest->player );
				send_to_char( EOL, quest->player );
				done = 		true;
				break;

			case ACTION_TELL:
			{
				QUEST_MOB_DATA * 	qMob;
				bool		found = false;

				for( qMob = quest->first_mob; qMob; qMob=qMob->next )
					if( qMob->mob && !char_died( qMob->mob ) && qMob->vnum == action->arg1 )
				{
					found = true;
					break;
				}

				action->done = true;
				if( !found )
					q_bug( "Process_action: Couldn't find mob! Vnum: %d (dead?)",
						   action->arg1 );
				else
					ch_tell( qMob->mob, quest->player, action->arg4 );
			}
			done = 		true;
			break;

			case ACTION_PROG:
			{
				MPROG_DATA *	mprg;
				MOB_INDEX_DATA *	pMobIndex;
				QUEST_MOB_DATA * 	qMob;
				bool		found = false;

				action->done = true;
				for( qMob = quest->first_mob; qMob; qMob=qMob->next )
					if( qMob->mob->pIndexData->vnum == action->arg1 )
				{
					found = true;
					break;
				}


				if( found )
				{
					int prog_nr=1;
					int old_prog_line = current_prog_line;
					int old_prog_nr = current_prog_number;

					pMobIndex = qMob->mob->pIndexData;

					for ( mprg = pMobIndex->mudprogs; mprg; mprg = mprg->next, prog_nr++ )
					{
						/* tylko quest_progi */
						if( mprg->type != QUEST_PROG )
							continue;

						if( !str_cmp( mprg->arglist, action->arg4 ) )
						{
							current_prog_line = 0;
							current_prog_number = prog_nr;

							mprog_driver(   mprg->comlist, qMob->mob,
											quest->player, NULL, NULL, false );
							current_prog_line = old_prog_line;
							current_prog_number = old_prog_nr;
							break;
						}
					}
				}
				else
					q_bug( "Process_action: ACTION_PROG. Couldn't find mob vnum: %d",
						   action->arg1 );

				done = 		true;
				break;
			}

			/* EXTREMELY DANGEROUS :P USE WITH CARE AND RESPECT ;) */
			/* Wszystkie ewentualne bledy sa wasze nie moje ;)))   */
			case ACTION_RUNCHPT:
			{
				CHAPTER_DATA * chapter;

				for( chapter = quest->first_chapter; chapter; chapter = chapter->next )
				{
					if( !str_cmp( action->arg4, chapter->pIndexData->name ) )
						break;
				}

				if( !chapter )
				{
					q_bug( "Process_action: ACION_RUNCHAPT: Chapter not found" );
				}

				action->done = true;
				init_chapter( quest, chapter->nr );
				quest->curr_chapter->completed = false;
				/* Nie break, bo skaczemy od razu do chaptera */
				return true;
			}
			case ACTION_REWARD:
			{
				QUEST_MOB_DATA * 	qMob;
				OBJ_INDEX_DATA *	pObjIndex;
				OBJ_DATA *		pObj;
				bool		found 		= false;
				int			lev 		= quest->player->top_level;
				int			ovnum 		= action->arg2;
				int			mvnum 		= action->arg1;
//		    int			immediate 	= action->arg3;

				for( qMob = quest->first_mob; qMob; qMob=qMob->next )
					if( qMob->mob->pIndexData->vnum == mvnum )
				{
					found = true;
					break;
				}

				if( !found )
				{
					q_bug( "Process_action: Couldn't find mob! Vnum: %d",
						   mvnum );
					break;
				}

				if ( !( pObjIndex = get_obj_index( ovnum ) ) )
				{
					q_bug( "Action_Reward: bad obj vnum %d.",
						   ovnum );
					break;
				}

				if( (pObj = create_object( pObjIndex, lev )) == NULL )
				{
					q_bug( "Action_Reward: Couldn't create an obj. Vnum: %d",
						   ovnum );
					break;
				}

				action->done = true;
				obj_to_char( pObj, quest->player );
				act( PLAIN, "$N daje ci $p$3.",
					 quest->player, pObj, qMob->mob, TO_CHAR );
				ch_printf( quest->player, FB_WHITE
						"Dosta³%s¶ przedmiot Questowy!" EOL,
						SEX_SUFFIX_EAE( quest->player ) );

				/*  FIXME !!! */
		/*
				*	Mozliwe sytuacje
				*		mob->in_room == ch->in_room	(do_give)
				*		mob->in_room != ch->in_room	(do_at do_give)
				*		!immediate			wait & give
		*/
				done = 	true;
				break;
			}
		}
	}
	return done;
}

QUEST_MOB_DATA *add_to_qmobs( CHAR_DATA *pMob, QUEST_DATA *quest, bool invited )
{
	QUEST_MOB_DATA * 	qMob;

	CREATE( qMob, QUEST_MOB_DATA, 1 );
	LINK( qMob, quest->first_mob, quest->last_mob, next, prev );

	qMob->mob 		= pMob;
	qMob->vnum 		= pMob->pIndexData->vnum;
	qMob->invited 	= invited;
	if( !invited )
		pMob->inquest 	= quest;

	return qMob;
}

/*
 * ³aduje moba a ³aduj±c dodaje go do listy struktur QUEST_MOB_DATA
 * ta struktura jest potrzebna do save'owania mobow questowych i
 * ogólnie przyspiesza wyszukiwanie mobkoff
 */
QUEST_MOB_DATA *qmob_invoke( int pIndex, QUEST_DATA *quest )
{
	MOB_INDEX_DATA *	pMobIndex;
	CHAR_DATA *		pMob;

	if ( !( pMobIndex = get_mob_index( pIndex ) ) )
	{
		q_bug( "QMob_invoke: bad vnum %d.", pIndex );
		return NULL;
	}


	if( (pMob = create_mobile( pMobIndex )) == NULL )
	{
		q_bug( "QMob_invoke: Couldn't create a mobile vnum: %d", pIndex );
		return NULL;
	}

	SET_BIT( pMob->act, ACT_QUEST );

	return add_to_qmobs( pMob, quest, false );
}


/*
 * ³aduje obj a ³aduj±c dodaje go do listy struktur QUEST_OBJ_DATA
 * ta struktura jest potrzebna do save'owania obj questowych i
 * ogólnie przyspiesza wyszukiwanie przedmiotów
 */
QUEST_OBJ_DATA *qobj_invoke( int pIndex, QUEST_DATA *quest, int level )
{
	QUEST_OBJ_DATA * 	qObj;
	OBJ_DATA *		pObj;
	OBJ_INDEX_DATA *	pObjIndex;

	if ( !( pObjIndex = get_obj_index( pIndex ) ) )
	{
		q_bug( "QObj_invoke: bad vnum %d.", pIndex );
		return NULL;
	}

	if( (pObj = create_object( pObjIndex, level )) == NULL )
	{
		q_bug( "QObj_invoke: Couldn't create an obj vnum: %d", pIndex );
		return NULL;
	}


	CREATE( qObj, QUEST_OBJ_DATA, 1 );
	LINK( qObj, quest->first_obj, quest->last_obj, next, prev );

	qObj->obj 		= pObj;
	pObj->inquest	= quest;
	pObj->cost		= 0;

	SET_BIT( qObj->obj->extra_flags, ITEM_PERSONAL );
	STRDUP( qObj->obj->owner_name, quest->player->name );

	return qObj;
}

/*
   Funkcja inicjuje jeden rozdzia³ questa:
   ³aduje moby, przedmioty etc.

   Je¶li chaptnr ==  0 : inicjowany jest nag³owek questa
   Je¶li chaptnr == -1 : przechodzimy do nastêpnego rozdzia³u
   Zwraca true je¶li siê uda.
 */
bool init_chapter( QUEST_DATA *quest, int chaptnr )
{
	CHAPTER_DATA *	pChapter = NULL;
	QUEST_CMND_DATA * 	pComD;

	if( chaptnr != -1 && chaptnr != 0 )
	{
		for( pChapter = quest->first_chapter; pChapter; pChapter = pChapter->next )
			if( pChapter->nr == chaptnr )
				break;
	}
	else if( chaptnr == -1 )
	{
		if( !quest->curr_chapter )
			return false;
		pChapter = quest->curr_chapter->next;
	}

	if( !pChapter && chaptnr == CHAPTER_NEXT )
		return false; /* to znaczy, ¿e skakali¶my z ostatniego chaptera */

	if( !pChapter && chaptnr != 0 )
	{
		q_bug( "Init_chapter: Chapter nr: %d not found.", chaptnr );
		return false;
	}

    for( pComD = ( chaptnr == 0 ? quest->first_init_cmd :
		 pChapter->first_init_cmd );
		 pComD; pComD = pComD->next )
	{
		ROOM_INDEX_DATA *	pRoomIndex;
		CHAR_DATA *		pMob;
		QUEST_MOB_DATA *	qMob;
		QUEST_OBJ_DATA *	qObj;
		int			vnum;

		if (pComD->done)
			continue;

		switch( pComD->command )
		{
			default:
				q_bug( "Init_chapter: bad Init command %c.", pComD->command );
				break;

			case INIT_LOAD_MOB:
				if( pComD->arg2 > pComD->arg3
								||  pComD->arg2 >= MAX_VNUM
								||  pComD->arg3 >= MAX_VNUM )
				{
					q_bug( "Init_chapter: INIT_LOAD_MOB bad Room Vnums (mob:%d).", pComD->arg1 );
					continue;
				}

				{ /*  Losujemy 1 pokój z zakresu */
					int loop=0;

					while( loop < 99 )
					{
						vnum = number_range( pComD->arg2, pComD->arg3 );
						if ( ( pRoomIndex = get_room_index( vnum ) ) != NULL
												 && ( !IS_SET( pRoomIndex->room_flags, ROOM_PRIVATE  ) ) )
							break;
						loop++;
					}
				}

				if( !pRoomIndex )
				{
					q_bug( "Init_chapter: INIT_LOAD_MOB No valid rooms found in range (mob:%d) Quest: %s.",
						   pComD->arg1, quest->pIndexData->name );
					continue;
				}

				if( (qMob = qmob_invoke( pComD->arg1, quest )) == NULL )
					continue;

				if ( room_is_dark( pRoomIndex ) )
					SET_BIT( qMob->mob->affected_by, AFF_INFRARED );

				pComD->done = true;
				char_to_room( qMob->mob, pRoomIndex );
				break;

			case INIT_LOAD_OBJ:
				if ( !( pRoomIndex = get_room_index( pComD->arg3 ) ) )
				{
					q_bug( "Init_chapter: INIT_LOAD_OBJ bad vnum %d.", pComD->arg3 );
					continue;
				}

				if( (qObj = qobj_invoke( pComD->arg1,
					 quest,
					 quest->pIndexData->min_lev )) == NULL )
					continue;

				if (pComD->arg2 == 1)
					SET_BIT( qObj->obj->extra_flags, ITEM_QUEST );
				else
					qObj->obj->timer	= pComD->arg2;

				pComD->done = true;
				obj_to_room( qObj->obj, pRoomIndex );
				break;

			case INIT_PUT_OBJ:
			{
				OBJ_DATA * 		ToObj;
				OBJ_INDEX_DATA *	ToObjIndexData;

				if ( !( ToObjIndexData = get_obj_index( pComD->arg3 ) ) )
				{
					q_bug( "Init_chapter: INIT_PUT_OBJ bad objTo vnum %d.", pComD->arg3 );
					continue;
				}

				if (!(ToObj = get_obj_type( ToObjIndexData ) ))
				{
					bug( "INIT_PUT_OBJ did not find %d.", pComD->arg3 );
					continue;
				}

				if( (qObj = qobj_invoke( pComD->arg1,
					 quest,
					 quest->last_obj->obj->level )) == NULL )
					continue;

				if (pComD->arg2 == 1)
					SET_BIT( qObj->obj->extra_flags, ITEM_QUEST );
				else
					qObj->obj->timer = pComD->arg2;

				pComD->done = true;
				obj_to_obj( qObj->obj, ToObj );
			}
			break;

			case INIT_EQUIP_MOB:
				if ( !quest->last_mob || !quest->last_mob->mob )
				{
					q_bug( "Init_chapter: INIT_EQUIP_MOB No last Mob: vnum %d.",
						   pComD->arg1 );
					continue;
				}

				if( (qObj = qobj_invoke( pComD->arg1,
					 quest,
					 quest->last_mob->mob->top_level )) == NULL )
					continue;

				if (pComD->arg2 == 1)
					SET_BIT( qObj->obj->extra_flags, ITEM_QUEST );
				else
					qObj->obj->timer = pComD->arg2;

				pComD->done = true;
				obj_to_char( qObj->obj, quest->last_mob->mob );

				if( pComD->arg3 != -1 )
					equip_char( quest->last_mob->mob, qObj->obj, pComD->arg3 );
				break;

			case INIT_PURGE:
				if( ( is_number(pComD->arg4 )
								  && (vnum = atoi( pComD->arg4 ) )!=0 ) )
				{
					if( (pRoomIndex = get_room_index( vnum ) ) )
					{
						char_to_room( supermob, pRoomIndex );
						do_mppurge( supermob, (char *)"" );
						char_from_room( supermob );
						pComD->done = true;
					}
					else
						q_bug( "Init_chapter: INIT_PURGE no such place: %d!",
							   atoi( pComD->arg4 ) );
				}
				else if( pComD->arg4[0] != '\0' )
				{
					char buf[MAX_STRING_LENGTH];

					char_to_room( supermob, get_room_index( ROOM_VNUM_LIMBO ) );
					sprintf( buf, "%s mppurge %s", pComD->arg4, pComD->arg4 );
					do_mpat( supermob, buf );
					char_from_room( supermob );
					pComD->done = true;
				}
				break;

				case INIT_HINT:	/* tu olewamy - quest info to za nas za³atwi */
					break;

			case INIT_ECHO:
				send_to_char( pComD->arg4, quest->player );
				send_to_char( PLAIN, quest->player );
				pComD->done = true;
				break;

			case INIT_TRANSFER:
				if( !(pRoomIndex = get_room_index( atoi( pComD->arg4 ) ) ) )
					if( !(pRoomIndex = find_location( Quest_Master, pComD->arg4 )))
				{
					q_bug( "Init_chapter: INIT_TRANSFER No such place: %s",
						   pComD->arg4 );
					continue;
				}

				pMob = quest->player;
				pComD->done = true;
				char_from_room( pMob );
				char_to_room  ( pMob, pRoomIndex );
				send_to_char(
						FB_BLUE "Zaczynasz czuæ siê bardzo dziwnie i... " EOL
						FB_BLUE "nagle jeste¶ w zupe³nie innym miejscu ni¿ przed chwil±!" EOL, pMob );
				do_look( pMob, (char *)"auto" );
				break;
			case INIT_INVITE_MOB:
				/* zrobione w quest_start */
				break;
			case INIT_GIVE_OBJ: //Tanglor
				if( (qObj = qobj_invoke( pComD->arg1,quest,
					 quest->pIndexData->min_lev )) == NULL )
					continue;

				if (pComD->arg2 == 1)
					SET_BIT( qObj->obj->extra_flags, ITEM_QUEST );
				else
					qObj->obj->timer	= pComD->arg2;
				pComD->done = true;
				obj_to_char(qObj->obj, quest->player);
				break;

		}/*switch*/
	}/*for*/

	if( chaptnr == 0 )
		process_action( quest, NULL );
	else
	{
		quest->curr_chapter 	= pChapter;

		q_log( "%s Chapter initiated: %s (%d)",
			   bit_name( quest_chapter_types, pChapter->pIndexData->type ),
			   pChapter->pIndexData->name,
			   pChapter->nr );

		/* je¶li chapter nie ma eventów to znaczy ¿e wszystko zrobione nie? */
		if(!pChapter->first_event )
			pChapter->completed = true;


	/* Initujemy chaptery a¿ do pierwszego napotkanego criticala
		Potem bêdziemy czekaæ */
		if( pChapter->pIndexData->type != CHAPTER_CRITICAL
				  &&  pChapter->pIndexData->type != CHAPTER_VERY_CRITICAL )
			init_chapter( quest, CHAPTER_NEXT );
	}
	return true;
}

/* najbardzie lamerski styl wyszukiwania questora:
    NIE PRÓBUJCIE TEGO W DOMU!!! ;)  */
CHAR_DATA *get_questor( QUEST_DATA *quest )
{
	CHAR_DATA *		questor;

	if( quest->player && quest->player->in_room )
		for( questor = quest->player->in_room->first_person;
				   questor; questor = questor->next_in_room )
			if( IS_NPC( questor )
						 && quest->pIndexData->questor == questor->pIndexData->vnum )
				return questor;

	/* No a jak nie ma obok gracza.. */
	for( questor = first_char; questor; questor = questor->next )
		if( IS_NPC( questor )
				  && quest->pIndexData->questor == questor->pIndexData->vnum )
			return questor;

	return NULL;
}

/*
 * zgadnij co ta f-cja robi ;)
 * Je¶li fromfile == true to znaczy ze quest jest ReStartowany czyli
 * gracz wchodzi do gry z zapisanym wczesniej questem
 * Nic wtedy nie jest inicjowane a i moby i objs siê nie ³aduj±
 * (bo s± ³adowane z 'load_quest_data')
 */
QUEST_DATA *quest_start( CHAR_DATA *ch, int quest_id, bool fromfile )
{
	QUEST_INDEX_DATA *	pQuest;
	CHAPTER_INDEX_DATA*	ChaptIndex;
	CHAPTER_DATA *	pChapter;
	QUEST_DATA *	quest;
	EVENT_DATA *	pEvent;
	QUEST_CMND_DATA *	pCmnd;
	QUEST_ACTION_DATA *	action;
	QUEST_CMND_DATA * 	cmd;

	int			i = 1;

	for( pQuest = first_quest_index; pQuest; pQuest = pQuest->next )
		if( pQuest->quest_id == quest_id )
			break;

	if( !pQuest )
	{
		q_bug( "No such quest %d for %s." NL, quest_id, ch->name );
		return NULL;
	}

	CREATE( quest, QUEST_DATA, 1 );
	quest->pIndexData 	= pQuest;
	quest->player 	= ch;

	{
		CHAR_DATA      *	questor;

		/* Wciskamy questora na listê questmobów questa*/
		if( !(questor=get_questor( quest ) ) )
		{
			q_bug( "Propably dead questor: %s QUEST: %s",
				   quest->player->name, quest->pIndexData->name );
		}
		else
			add_to_qmobs( questor, quest, true );

		/* Trog: alokujemy komendy */
		FOREACH( cmd, pQuest->first_init_cmd )
		{
			CREATE( pCmnd, QUEST_CMND_DATA, 1 );
			pCmnd->arg1 = cmd->arg1;
			pCmnd->arg2 = cmd->arg2;
			pCmnd->arg3 = cmd->arg3;
			STRDUP( pCmnd->arg4, cmd->arg4 );
			pCmnd->command = cmd->command;
			pCmnd->done = false;
			LINK( pCmnd, quest->first_init_cmd, quest->last_init_cmd, next, prev );
		}


		/* a teraz specyficzny init (invite_mob) */
		for( pCmnd = quest->first_init_cmd;
				   pCmnd; pCmnd = pCmnd->next )
		{
			if ( pCmnd->command == INIT_INVITE_MOB )
			{
				MOB_INDEX_DATA *	pMobIndex;
				CHAR_DATA *		pMob;
				CHAR_DATA *		mob;
				int			pIndex = pCmnd->arg1;

				if ( !( pMobIndex = get_mob_index( pIndex ) ) )
				{
					q_bug( "INIT_INVITE_MOB: bad vnum %d. Q:%s P:%s", pIndex, quest->pIndexData->name, ch->name );
					break;
				}
				else
				{
					pMob = NULL;
					for( mob = first_char; mob; mob = mob->next )
						if ( IS_NPC(mob) && pIndex == mob->pIndexData->vnum )
					{
						pMob = mob;
						break;
					}
				}

				if( !pMob )
				{
					q_bug( "INIT_INVITE_MOB: Couldn't find a mobile vnum: %d (dead?) Q:%s P:%s",
						   pIndex, quest->pIndexData->name, ch->name );
					continue;
				}
				add_to_qmobs( pMob, quest, true );
				pCmnd->done = true;
			}
		}

		/* Trog: allokujemy akcje */
		FOREACH( action, pQuest->first_action )
		{
			QUEST_ACTION_DATA *pAction;
			CREATE( pAction, QUEST_ACTION_DATA, 1 );
			pAction->arg1 = action->arg1;
			pAction->arg2 = action->arg2;
			pAction->arg3 = action->arg3;
			STRDUP( pAction->arg4, action->arg4 );
			pAction->command = action->command;
			pAction->done = false;
			LINK( pAction, quest->first_action, quest->last_action, next, prev );
		}
	}

	if( !fromfile )
		init_chapter( quest, 0 );

	for( ChaptIndex = pQuest->first_chapter; ChaptIndex; ChaptIndex = ChaptIndex->next, i++ )
	{
		int		x = 1;

		CREATE( pChapter, CHAPTER_DATA, 1 );
		pChapter->pIndexData	= ChaptIndex;
		pChapter->completed 	= false;
		pChapter->nr		= i;
		LINK( pChapter, quest->first_chapter, quest->last_chapter, next, prev );

		/* Trog: alokujemy komendy */
		FOREACH( cmd, ChaptIndex->first_init_cmd )
		{
			CREATE( pCmnd, QUEST_CMND_DATA, 1 );
			pCmnd->arg1 = cmd->arg1;
			pCmnd->arg2 = cmd->arg2;
			pCmnd->arg3 = cmd->arg3;
			STRDUP( pCmnd->arg4, cmd->arg4 );
			pCmnd->command = cmd->command;
			pCmnd->done = false;
			LINK( pCmnd, pChapter->first_init_cmd, pChapter->last_init_cmd, next, prev );
		}

		/* Trog: allokujemy akcje */
		FOREACH( action, ChaptIndex->first_action )
		{
			QUEST_ACTION_DATA *pAction;
			CREATE( pAction, QUEST_ACTION_DATA, 1 );
			pAction->arg1 = action->arg1;
			pAction->arg2 = action->arg2;
			pAction->arg3 = action->arg3;
			STRDUP( pAction->arg4, action->arg4 );
			pAction->command = action->command;
			pAction->done = false;
			LINK( pAction, pChapter->first_action, pChapter->last_action, next, prev );
		}

		/* allokujemy eventy */
		for( pCmnd = ChaptIndex->first_event; pCmnd; pCmnd = pCmnd->next, x++ )
		{
			CREATE( pEvent, EVENT_DATA, 1 );
			pEvent->done 	= false;
			pEvent->cmd 	= pCmnd;
			pEvent->nr		= x;
			LINK( pEvent, pChapter->first_event, pChapter->last_event, next, prev );
		}
	}

	if( !fromfile )
		init_chapter( quest, 1 );

	LINK( quest, first_quest, last_quest, next, prev );

	q_log( "Player: %s %starting Quest: %s (%d)",
		   ch->name, fromfile ? "Res" : "S",
		   quest->pIndexData->name, quest->pIndexData->quest_id );

	if( !fromfile )
		quest->time_took	= 0;

	quest->logon 	= current_time;
	quest->finished 	= false;
	quest->failed 	= false;
	top_quest++;

	return quest;
}

/*
 * sprz±ta po que¶cie.
 * uruchamia siê zawsze gdy gracz
 * wychodzi z gry i/lub na koñcu questu
 */
void quest_cleanup( CHAR_DATA *ch, bool end )
{
	QUEST_DATA *	quest = ch->inquest;
	QUEST_MOB_DATA *	qMob, 		 *	qMob_next;
	QUEST_OBJ_DATA *	qObj, 		 *	qObj_next;
	QUEST_ACTION_DATA *	pAction, 	 *	pAction_next;
	EVENT_DATA *	pEvent, 	 *	pEvent_next;
	CHAPTER_DATA *	pChapter, 	 *	pChapter_next;
	QUEST_CMND_DATA 	*pCmnd, *pCmnd_next;

	if( !quest )
		return;

	for( qMob = quest->first_mob; qMob; qMob = qMob_next )
	{
		qMob_next = qMob->next;

		if( !qMob->invited ) /* moby wci±gniête do questu oszczêdzamy */
			extract_char( qMob->mob, true );
		else
		{
			UNLINK( qMob, quest->first_mob, quest->last_mob, next, prev );
			DISPOSE( qMob );
		}
	}

	for( qObj = quest->first_obj; qObj; qObj = qObj_next )
	{
		qObj_next = qObj->next;

//	if( IS_OBJ_STAT( qObj->obj, ITEM_QUEST ) )
		extract_obj( qObj->obj );
	}

	/* Trog */
	for( pCmnd = quest->first_init_cmd; pCmnd; pCmnd = pCmnd_next )
	{
		pCmnd_next = pCmnd->next;

		UNLINK( pCmnd, quest->first_init_cmd, quest->last_init_cmd, next, prev );
		free_qcmd( pCmnd );
	}

	/* Trog */
	for( pAction = quest->first_action; pAction; pAction = pAction_next )
	{
		pAction_next = pAction->next;

		UNLINK( pAction, quest->first_action, quest->last_action, next, prev );
		free_qaction( pAction );
	}

	for( pChapter = quest->first_chapter; pChapter; pChapter = pChapter_next )
	{
		pChapter_next = pChapter->next;

		/* Trog */
		FOREACH( pCmnd, pChapter->first_init_cmd )
		{
			pCmnd_next = pCmnd->next;

			UNLINK( pCmnd, pChapter->first_init_cmd, pChapter->last_init_cmd, next, prev );
			free_qcmd( pCmnd );
		}

		/* Trog */
		FOREACH( pAction, pChapter->first_action )
		{
			pAction_next = pAction->next;

			UNLINK( pAction, pChapter->first_action, pChapter->last_action, next, prev );
			free_qaction( pAction );
		}

		for( pEvent = pChapter->first_event; pEvent; pEvent = pEvent_next )
		{
			pEvent_next = pEvent->next;

			UNLINK( pEvent, pChapter->first_event, pChapter->last_event, next, prev );
			DISPOSE( pEvent );
		}

		UNLINK( pChapter, quest->first_chapter, quest->last_chapter, next, prev );
		DISPOSE( pChapter );
	}

	ch->inquest	= NULL;
	UNLINK( quest, first_quest, last_quest, next, prev );
	DISPOSE( quest );
	top_quest--;

	return;
}

/*
 * uruchamia siê tylko, je¶li gracz skoñczy quest
 * (albo kto¶ mu go wy³±czy)
 */
void quest_stop( CHAR_DATA * owner, int type )
{
	QUEST_DATA * quest = owner->inquest;

	if( !quest )
		return;

    /* je¶li to by³o normalne zakoñczenie dodajemy jeszcze jeden
	* rozdzia³ -- powrót po nagrodê */
	if( type == STOP_OK )
	{

		CHAPTER_DATA *	chapter;
		EVENT_DATA *	event;

		quest->finished 	= true;
		//	quest_cleanup( owner, false );
		quest->failed 		= false;
		q_log( "Player: %s, Last Event completed %d (%s) OK.",
			   owner->name,
			   quest->pIndexData->quest_id,
			   quest->pIndexData->name );
		send_to_char( "Doskona³a robota. Wracaj do zleceniodawcy po nagrodê!" NL,
					  owner );

		/* Sprawd¼my, czy gracz powstrzyma³ siê od wykonania bardzo
		niekrytycznych eventów, je¶li tak - nagród¼my go */
		for( chapter = owner->inquest->first_chapter;
				   chapter;
				   chapter = chapter->next )
		{
			if( chapter->pIndexData->type == CHAPTER_VERY_UNCRITICAL
						 &&  !chapter->completed )
				for( event = chapter->first_event; event; event = event->next )
					owner->inquest->curr_qp += event->cmd->arg3;
		}

		save_char_obj( owner );

		/* moze juz jestesmy u questora? */
		quest_trigger( owner, EVENT_VISIT_MOB, NULL, NULL );
		return;
	}

	/* tu tak samo, ale bez qp... */
	if( type == STOP_TOO_LATE )
	{
		quest->finished = true;
	//	quest_cleanup( owner, false );
		quest->failed 	= true;
		q_log( "Player: %s, Quest Aborted %d (%s) TooLate.",
			   owner->name,
			   quest->pIndexData->quest_id,
			   quest->pIndexData->name );

		send_to_char( FB_WHITE
				"W³a¶nie min±³ czas na wykonanie twojego zadania!" EOL
				"Ciekawe jak ty siê z tego wyt³umaczysz swojemu zleceniodawcy..." NL,
		owner );
		save_char_obj( owner );
		/* moze juz jestesmy u questora? */
		quest_trigger( owner, EVENT_VISIT_MOB, NULL, NULL );
		return;
	}

	/* Rezygnacja: tak samo jak spó¼nienie, ale bez komunikatu */
	if( type == STOP_RESIGN )
	{
		quest->finished = true;
	//	quest_cleanup( owner, false );
		quest->failed 	= true;
		q_log( "Player: %s, Quest Aborted %d (%s) Resigned.",
			   owner->name,
			   quest->pIndexData->quest_id,
			   quest->pIndexData->name );

		send_to_char( FB_WHITE
				"Rozk³adasz bezradnie rêce i ze ³zami w oczach rezygnujesz." EOL
				"Ciekawe co na to twój zleceniodawca..." NL,
		owner );
		save_char_obj( owner );
		/* moze juz jestesmy u questora? */
		quest_trigger( owner, EVENT_VISIT_MOB, NULL, NULL );
		return;
	}

	/* ustawiane przez ACTION_STOP_QUEST */
	if( type == STOP_BAD )
	{
		q_log( "Player: %s, done quest %d (%s) BAD.",
			   owner->name, quest->pIndexData->quest_id, quest->pIndexData->name );

		add_to_done_quests( owner, quest );
		quest_cleanup( owner, true );

		ch_printf( owner,NL FB_RED
				"Spapra³%s¶ wszystko. Nici z twojego zadania." EOL NL,
				SEX_SUFFIX_EAE( owner ) );

		{
			char	filename	[256];

			sprintf( filename, "%s%s", QDATA_DIR, capitalize(owner->name) );
			unlink ( filename );
		}

		ch_printf( owner, FG_BLUE
				"Tracisz " FB_BLUE "%d" FG_BLUE " punktów do¶wiadczenia za niewykonanie zadania!" EOL,
				quest->pIndexData->penalty );
		owner->experience[owner->main_ability] -= quest->pIndexData->penalty;
		if( owner->experience[owner->main_ability] < 0 )
			owner->experience[owner->main_ability] = 0;
		save_char_obj( owner );
		/* moze juz jestesmy u questora? */
		quest_trigger( owner, EVENT_VISIT_MOB, NULL, NULL );
		return;
	}

	/* definitywnie koñczymy zabawê w questa */
	if( type == STOP_VERY_OK )
	{
		MOB_INDEX_DATA *	questor;
		char *			qmname;

		if( !Quest_Master ) restore_quest_master();

		qmname = Quest_Master->przypadki[0];

		if( (questor = get_mob_index( quest->pIndexData->questor ) ) )
			Quest_Master->przypadki[0] = questor->przypadki[0];
		else
			q_bug( "Stop_quest: Couldn't find questor!" );

		q_log( "Player: %s, done quest %d (%s) OK.",
			   owner->name, quest->pIndexData->quest_id, quest->pIndexData->name );

	//if( calc_time( quest ) >= 0 )
		if( !quest->failed )
		{
			ch_tell( Quest_Master, owner, "Widzê, ¿e zadanie zosta³o ukoñczone." );
			ch_tell( Quest_Master, owner, "¦wietnie, wielkie dziêki." 		  );
			ch_tell( Quest_Master, owner, "Wspó³praca z tob± to przyjemno¶æ." 	  );


			if( owner->inquest->curr_qp )
			{
				ch_printf( owner, EOL FG_GREEN
						"W nagrodê za ukoñczenie zadania dostajesz " FB_GREEN "%d" FG_GREEN " qp." EOL,
						owner->inquest->curr_qp );

				owner->pcdata->quest_points    += owner->inquest->curr_qp;
			}
			fevent_trigger( owner, FE_FINISH_QUEST, owner->inquest->pIndexData );
		}
		else /* auæ */
		{
			ch_tell( Quest_Master, owner, "No tak..." );
			ch_tell( Quest_Master, owner, "Nie uda³o ci siê..."   );
			ch_tell( Quest_Master, owner, "..."   );
			ch_tell( Quest_Master, owner, "NIE POKAZUJ MI SIÊ WIÊCEJ NA OCZY !!!"   );

			ch_printf( owner,EOL FG_BLUE
					"Tracisz " FB_BLUE "%d" FG_BLUE " punktów do¶wiadczenia za niewykonanie zadania!" EOL,
					quest->pIndexData->penalty );
			owner->experience[owner->main_ability] -= quest->pIndexData->penalty;
			if( owner->experience[owner->main_ability] < 0 )
				owner->experience[owner->main_ability] = 0;
			fevent_trigger( owner, FE_FAIL_QUEST, owner->inquest->pIndexData );
		}

		Quest_Master->przypadki[0] = qmname;

		add_to_done_quests( owner, quest );
		quest_cleanup( owner, true );

		{
			char	filename	[256];

			sprintf( filename, "%s%s", QDATA_DIR, capitalize(owner->name) );
			unlink ( filename );
		}

		save_char_obj( owner );
		return;
	}

	/* a tu koñczymy zabawê jeszcze definitywniej */
	if( type == STOP_WHO_DID_IT )
	{
		send_to_char( "Twój Quest zosta³ zatrzymany!" NL, owner );
		quest_cleanup( owner, true );

		{
			char	filename	[256];

			sprintf( filename, "%s%s", QDATA_DIR, capitalize(owner->name) );
			unlink ( filename );
		}

		save_char_obj( owner );
		return;
	}
}


/* komunikacja gracza z questem */
DEF_DO_FUN( quest )
{
	QUEST_DATA *	quest;
	QUEST_INDEX_DATA *	pQuest;
	char		arg [MAX_INPUT_LENGTH];

	if( IS_NPC( ch ) )
	{
		send_to_char( "Eee tam questy... Po³a¼ sobie lepiej bez celu." NL, ch );
		return;
	}

	argument = one_argument( argument, arg );

	if ( arg[0] == '\0' || !str_prefix( arg, "info" ) )
	{
		QUEST_CMND_DATA * 	pCmnd;
		CHAPTER_DATA *		chapt;
		bool			found;

		if (!first_quest || !ch->inquest )
		{
			send_to_char ("Nie wykonujesz ¿adnego zadania w tej chwili." NL, ch);
			return;
		}

		quest = ch->inquest;

		pager_printf(ch,
					 "Bierzesz udzia³ w que¶cie: " FB_WHITE "%s" PLAIN ", dla graczy na poziomie: "
							 FB_WHITE "%d" PLAIN " do " FB_WHITE "%d" PLAIN "." EOL EOL
							 "%s" EOL,
					 quest->pIndexData->name,
					 quest->pIndexData->min_lev,
					 quest->pIndexData->max_lev,
					 quest->pIndexData->description);

		if( quest->finished )
		{
			if( quest->failed )
			{
				pager_printf( ch,
							  "Wzkazówka:" NL
									  "Nie uda³o ci siê wykonaæ zadania. Wracaj do zleceniodawcy. Ale bêd± ciêgi." NL );
			}
			else
			{
				pager_printf( ch,
							  "Wzkazówka:" NL
									  "Wracaj po nagrodê. Zadanie ukoñczone." NL );
			}
			return;
		}

		pager_printf( ch, "Wzkazówka:" NL );

		chapt = quest->curr_chapter;

		if( !chapt )
		{
			pager_printf( ch, "Zdecydowanie musisz co¶ wykombinowaæ..." NL );
			q_bug( "No curr_chapter found in %s's quest (%d:%s)",
				   ch->name,
				   ch->inquest->pIndexData->quest_id,
				   ch->inquest->pIndexData->name );
			return;
		}

		found = false;
		for( pCmnd = chapt->pIndexData->last_init_cmd; pCmnd; pCmnd = pCmnd->prev )
		{
			if( pCmnd->command == INIT_HINT && *pCmnd->arg4 )
			{
				found = true;
				break;
			}
		}

		if( found )
			pager_printf( ch, "%s" EOL, pCmnd->arg4 );
		else
			pager_printf( ch, "Kombinuj, kombinuj..." NL );

		/* Ostrzegamy na 15 minut przed koñcem */
		if( calc_time( quest ) <= 15 )
			pager_printf( ch, NL FB_YELLOW
					"Uwaga! Zosta³o ci ju¿ ma³o czasu na wykonanie zadania." EOL );

		if( get_trust( ch ) > LEVEL_QUESTSEE )
		{
			ch_printf( ch, NL
					"You have still %d/%d minutes to finish that quest." NL,
					calc_time( quest ),
					quest->pIndexData->time*60 );
		}
		return;
	}

	if( !str_cmp( arg, "giveup" ) )
	{
		if (!first_quest || !ch->inquest )
		{
			send_to_char ("Nie wykonujesz ¿adnego zadania w tej chwili." NL, ch);
			return;
		}

		if( IS_SET( ch->inquest->pIndexData->flags, QUEST_NORESIGN ) )
		{
			send_to_char ("Nie mo¿esz zrezygnowaæ z tego zadania." NL, ch);
			send_to_char ("Nie poddawaj siê, walcz!" NL, ch);
			return;
		}

		fevent_trigger( ch, FE_FAIL_QUEST, ch->inquest->pIndexData );
		quest_stop( ch, STOP_RESIGN );
		return;
	}

	if( !str_prefix( arg, "list" ) )
	{
		int 			i = 1;
		QUEST_INDEX_DATA *	pQuest;

		ch_printf( ch,
				   "                              Spis Questów:" NL FB_WHITE
						   " Nazwa                            Autor             Poziomy" EOL );

		for( pQuest = first_quest_index; pQuest; pQuest = pQuest->next,i++ )
		{

			if( IS_SET( pQuest->flags, QUEST_HIDDEN )
						 &&  get_trust( ch ) < LEVEL_QUESTSEE )
				continue;

			ch_printf( ch, " %-32s" PLAIN " %-15s" PLAIN "   %3d-%-3d",
					   pQuest->name,
					   pQuest->author,
					   pQuest->min_lev,
					   pQuest->max_lev  );

			if(	IS_IMMORTAL( ch ) )
				ch_printf( ch,
						   "    " FB_WHITE "{" PLAIN " %5d" FB_WHITE "}" EOL,
						   pQuest->quest_id );
			else
				ch_printf( ch, EOL );
		}
		return;
	}

	if( !str_prefix( arg, "ask" ) )
	{
		CHAR_DATA *	questor = 0;
		bool		found 	= false;

		if( ch->inquest )
		{
			send_to_char(
					"A czy ty przypadkiem nie bierzesz ju¿ udzia³u w przygodzie?" NL,
			ch );
			return;
		}

		for( pQuest = first_quest_index; pQuest; pQuest = pQuest->next )
		{
			if( get_trust(ch) < LEVEL_QUESTSEE )
				if((IS_SET( pQuest->flags, QUEST_NF_COMBAT		)
								&& ch->main_ability == COMBAT_ABILITY		)
								|| (IS_SET( pQuest->flags, QUEST_NF_PILOT   	)
								&& ch->main_ability == PILOTING_ABILITY		)
								|| (IS_SET( pQuest->flags, QUEST_NF_ENGINEER	)
								&& ch->main_ability == ENGINEERING_ABILITY	)
								|| (IS_SET( pQuest->flags, QUEST_NF_BOUNTY   	)
								&& ch->main_ability == HUNTING_ABILITY 		)
								|| (IS_SET( pQuest->flags, QUEST_NF_SMUGGLER 	)
								&& ch->main_ability == SMUGGLING_ABILITY 	)
								|| (IS_SET( pQuest->flags, QUEST_NF_DIPLOMAT 	)
								&& ch->main_ability == DIPLOMACY_ABILITY 	)
								|| (IS_SET( pQuest->flags, QUEST_NF_LEADER   	)
								&& ch->main_ability == LEADERSHIP_ABILITY 	)
								|| (IS_SET( pQuest->flags, QUEST_NF_FORCEUSER	)
								&& ch->main_ability == FORCE_ABILITY 		) )
					continue;

			for( questor = ch->in_room->first_person;
						  questor;
						  questor = questor->next_in_room )
			{

				if ( IS_NPC( questor )
								 && pQuest->questor == questor->pIndexData->vnum )
				{
					if( !str_cmp(pQuest->author, ch->name) )
					{
						if( IS_SET( pQuest->flags, QUEST_PROTOTYPE )
											  || !done_that_quest( ch, pQuest->quest_id ) )
						{
							found = true;
							break;
						}
					}
					else
						if( !done_that_quest( ch, pQuest->quest_id )
											   && ( !IS_SET( pQuest->flags, QUEST_PROTOTYPE )
													   || ( get_trust( ch ) >= LEVEL_QUESTSEE
															   && IS_SET( ch->act, PLR_HOLYLIGHT ) ) ) )
					{
						found = true;
						break;
					}
				}
			}
			if( found )
				break;
		}

		if( !questor
				   ||  !found
				   ||  !pQuest  )
		{
			send_to_char(
					"Nie mo¿esz znale¼æ nikogo, kto móg³by daæ ci odpowiednie zadanie." NL,
			ch );
			return;
		}

		if( !can_see(questor,ch) )
		{
			send_to_char( "Przykro mi, ale on ciebie nie widzi" NL, ch );
			return;
		}

		act( PLAIN, "$n szepcze co¶ $N$2 na ucho.", ch, NULL, questor, TO_ROOM );

		if( !knows_language( questor, ch->speaking, ch ) )
		{
			check_social( questor, "marszcz", ch->name );
			ch_printf( ch, "Wygl±da na to, ¿e %s nie mo¿e ciê zrozumieæ." NL,
					   PERS( questor, ch, 0 ) );
			return;
		}

		if( pQuest->min_lev > ch->top_level
				  && get_trust( ch ) < LEVEL_IMMORTAL )
		{
			ch_tell( questor, ch,
					 "Jeszcze nie mam dla ciebie zadania. Ale mo¿e ju¿ wkrótce..." );
			do_emote( questor, (char *)"bezradnie za³amuje rêce" );
			return;
		}

		if( pQuest->max_lev < ch->top_level
				  && get_trust( ch ) < LEVEL_IMMORTAL )
		{
			ch_tell( questor, ch,
					 "Nie dla ciebie takie drobnostki. Nie bêdê ci zawracaæ g³owy." );
			do_emote( questor, (char *)"wraca do swoich zajêæ" );
			return;
		}

		ch_tell( questor, ch,"A tak, mam tu co¶ akurat dla ciebie:" );
		ch_tell( questor, ch, pQuest->name );

		quest = quest_start( ch, pQuest->quest_id, false );

		if( !quest ) /* Oops */
		{
			ch_tell( questor, ch,
					 "Albo mo¿e jednak nie, wróæ do mnie kiedy indziej." );
			ch_tell( questor, ch,
					 "Ja muszê teraz napisaæ notkê. ;-)" );
			q_bug( "Questor %d (%s) couldn't start quest %d for %s",

				   questor->pIndexData->vnum, questor->przypadki[0],
				   pQuest->quest_id, ch->name );

			return;
		}

		ch->inquest 		= quest;
		quest->curr_qp 		= 0;

		send_to_char ( NL, ch);
		ch_tell( questor, ch, "Wróæ do mnie jak ju¿ skoñczysz." );

		send_to_char ( NL "Przygoda siê zaczyna !!!" EOL,ch);
		fevent_trigger( ch, FE_START_QUEST, pQuest );
		save_char_obj( ch );
		return;
	}


	if( get_trust( ch ) >= LEVEL_QUESTSEE )
	{
		if( !str_prefix( arg, "stop" ))
		{
			CHAR_DATA *	victim;

			if(!first_quest)
			{
				send_to_char("No quest started." NL, ch );
				return;
			}

			if(argument[0] == '\0')
			{
				send_to_char("Syntax: quest stop <player_name>" NL, ch );
				return;
			}
			if ( ( victim = get_char_room( ch, argument ) ) == NULL )
			{
				send_to_char("Can't see them here." NL, ch );
				return;
			}
			if( !victim->inquest )
			{
				send_to_char("This person doesn't seem to do a quest right now." NL, ch );
				return;
			}
			if( IS_NPC( victim ) )
			{
				send_to_char("That is not a player." NL, ch );
				return;
			}

			/* Abnormal Termination of Quest */
			quest_stop( victim, STOP_WHO_DID_IT );

			q_log( "%s has stopped %s's quest.", ch->name, victim->name );

			send_to_char("Ok. Quest Stopped." NL, ch );
			return;
		}

		if( !str_prefix( arg, "moblist" ))
		{
			QUEST_MOB_DATA * 	qMob;

			if (!first_quest || !ch->inquest )
			{
				send_to_char ("Nie wykonujesz ¿adnego zadania w tej chwili." NL, ch);
				return;
			}

			quest = ch->inquest;

			for( qMob = quest->first_mob; qMob; qMob=qMob->next )
				ch_printf( ch,
						   "[%5d] [" FB_CYAN "%s" PLAIN "] " FB_CYAN "[%25.25s]" PLAIN " at:" FG_CYAN "[%25.25s]" PLAIN " [%5d]" NL,
						   qMob->mob->pIndexData->vnum,
						   qMob->invited ? "I" : " ",
						   qMob->mob->przypadki[0],
						   qMob->mob->in_room ? qMob->mob->in_room->name :"XXX",
						   qMob->mob->in_room ? qMob->mob->in_room->vnum : 0 );
			return;
		}

		if( !str_prefix( arg, "objlist" ))
		{
			QUEST_OBJ_DATA * 	qObj;

			if (!first_quest || !ch->inquest )
			{
				send_to_char ("Nie wykonujesz ¿adnego zadania w tej chwili." NL, ch);
				return;
			}

			quest = ch->inquest;

			for( qObj = quest->first_obj; qObj; qObj=qObj->next )
			{
				OBJ_DATA *	obj = qObj->obj;
				ch_printf( ch,
						   "[%5d] " FB_CYAN "[%25.25s]" PLAIN,
						   obj->pIndexData->vnum,
						   obj->przypadki[0]	);
				if( obj->in_room )
					ch_printf( ch," InRoom: [%5d] [%25.25s]" EOL,
							   obj->in_room->vnum,
							   obj->in_room->name );
				else
					if( obj->in_obj )
						ch_printf( ch," InObj:  [%5d] [%25.25s]" EOL,
								   obj->in_obj->pIndexData->vnum,
								   obj->in_obj->przypadki[0] );
				else
					if( obj->carried_by )
						ch_printf( ch," CarrBy: [%5d] [%25.25s]" EOL,
								   IS_NPC(obj->carried_by) ? obj->carried_by->pIndexData->vnum : 0,
								   obj->carried_by->name );
			}
			return;
		}
	}

	send_to_char( "Co? Wpisz 'help quest'." NL, ch );
	return;
}


/* updatuje wszystkie questy */
void quest_update( )
{
	QUEST_DATA *	quest;
	QUEST_DATA *	quest_next;

	for( quest = first_quest; quest; quest = quest_next )
	{
		CHAPTER_DATA *	chapter = quest->curr_chapter;

		quest_next = quest->next;

		if( calc_time( quest ) < 0 )
		{
			if( !quest->finished )
			{
				quest_stop( quest->player, STOP_TOO_LATE );
				continue;
			}
		}

		/* Na wypadek, gdy pan Builder zapomni questa zakoñczyæ */
		if( chapter && chapter->completed )
		{
//	    process_action( quest, chapter );
			if( chapter && chapter->completed )
				if( !init_chapter( quest, CHAPTER_NEXT ) )
			{
				if( !quest->finished )
				{
					quest_stop( quest->player, STOP_OK );
					continue;
				}
			}
		}
	}
	return;
}


void interpret_trigger( CHAR_DATA *ch, CHAPTER_DATA *chapter, int cmd, OBJ_DATA *obj, CHAR_DATA *victim )
{
	CHAR_DATA *		fch;
	QUEST_CMND_DATA *   pCmnd;
	EVENT_DATA *	pEvent;
	bool 		found = false;
	bool		Done;

	if( !chapter )
		return;

	for( pEvent = chapter->first_event; pEvent; pEvent = pEvent->next )
	{
		pCmnd 	= pEvent->cmd;
		if( pCmnd->command == cmd && !pEvent->done )
		{
			switch( cmd )
			{
				default:
					q_bug( "Quest_trigger: unknown event!" );
					break;

				case EVENT_FIND_OBJ:
					if( !obj )
					{
						q_bug( "quest_trigger: EVENT_FIND_OBJ - no obj!" );
						break;
					}

					if( pCmnd->arg1 == obj->pIndexData->vnum )
					{
						pEvent->done	= true;
						found		= true;
					}
					break;

				case EVENT_GIVE_OBJ:
					if( !obj )
					{
						q_bug( "quest_trigger: EVENT_GIVE_OBJ - no obj!" );
						break;
					}

					if( !victim )
					{
						q_bug( "quest_trigger: EVENT_GIVE_OBJ - no victim!" );
						break;
					}

					if( pCmnd->arg1 == obj->pIndexData->vnum
									   &&  pCmnd->arg2 == victim->pIndexData->vnum )
					{
						pEvent->done 	= true;
						found 		= true;
					}
					break;

				case EVENT_KILL_MOB:
					if( !victim )
					{
						q_bug( "quest_trigger: EVENT_KILL_MOB - no victim!" );
						break;
					}

					if( pCmnd->arg1 == victim->pIndexData->vnum )
					{
						pEvent->done 	= true;
						found		= true;
					}
					break;

				case EVENT_VISIT_MOB:
					for ( fch = ch->in_room->first_person; fch; fch = fch->next_in_room )
					{
						if( !IS_NPC( fch ) )
							continue;

						if( pCmnd->arg1 == fch->pIndexData->vnum )
						{
							pEvent->done 	= true;
							found		= true;
							break;
						}
					}
					break;

				case EVENT_VISIT_ROOM:
					if( ch->in_room->vnum == pCmnd->arg1 )
					{
						pEvent->done	= true;
						found		= true;
						break;
					}
					break;

				case EVENT_BRING_OBJ:
					for ( fch = ch->in_room->first_person; fch; fch = fch->next_in_room )
					{
						if (!IS_NPC( fch ) )
							continue;

						if( pCmnd->arg2 == fch->pIndexData->vnum )
						{
							OBJ_DATA	*obj;

							for( obj = ch->first_carrying;
													  obj;
													  obj = obj->next_content )
							{
								if( pCmnd->arg1 == obj->pIndexData->vnum )
								{
									pEvent->done 	= true;
									found		= true;
									obj_from_char( obj );
									obj_to_char( obj, fch );
								}
							}
						}
						if( found )
							break;
					}
					break;
				case EVENT_LOOK_OBJ:
					if( !obj )
					{
						q_bug( "quest_trigger: EVENT_LOOK_OBJ - no obj!" );
						break;
					}
					if( pCmnd->arg1 == obj->pIndexData->vnum )
					{
						if ( pCmnd->arg2 )
						{
							if ( obj->in_room->vnum == pCmnd->arg2 )
							{
								pEvent->done	= true;
								found			= true;
							}
						}
						else
						{
							pEvent->done	= true;
							found			= true;
						}
					}
					break;
				case EVENT_LOOK_MOB:
					if( !obj )
					{
						q_bug( "quest_trigger: EVENT_LOOK_MOB - no obj!" );
						break;
					}
					break;
			}

			if( found )
			{
				q_log( "%s: Completed Event", ch->name );
		/* bardzo niekrytyczne chaptery nagradzaj± qpointami za
				NIE WYKONANIE eventów */
				if( chapter->pIndexData->type != CHAPTER_VERY_UNCRITICAL )
					ch->inquest->curr_qp += pCmnd->arg3;
				break;
			}
		}

	}

	/* je¶li chapter jest zwyk³y krytyczny - wystarczy spe³niæ jeden event */
	if( chapter->pIndexData->type == CHAPTER_CRITICAL
		   ||  chapter->pIndexData->type == CHAPTER_VERY_UNCRITICAL )
	{
		if( found )
		{
			chapter->completed = true;
			process_action( ch->inquest, chapter );
		}
	}
	else
		/* teraz sprawdzamy czy wszystkie eventy z chaptera ju¿ wykonane */
		if( chapter->pIndexData->type == CHAPTER_VERY_CRITICAL
				  ||  chapter->pIndexData->type == CHAPTER_UNCRITICAL )
	{
		Done = true;
		for( pEvent = chapter->first_event; pEvent; pEvent = pEvent->next )
			if( !pEvent->done )
				Done = false;

		if( Done )
		{
			chapter->completed = true;
			process_action( ch->inquest, chapter );
		}
	}

	return;
}



/* reakcja questu na wykonanie eventu */
void quest_trigger( CHAR_DATA *ch, int cmd, OBJ_DATA *obj, CHAR_DATA *victim )
{
	CHAPTER_DATA *	chapter;
	QUEST_DATA *	quest;


	if( !ch->inquest || !first_quest || IS_NPC( ch )
			|| ( victim && !IS_NPC( victim ) ) )
		return;


	quest = ch->inquest;

	/* wracaj do questora */
	if( quest->finished && cmd == EVENT_VISIT_MOB )
	{
		CHAR_DATA *	fch;

		for ( fch = ch->in_room->first_person; fch; fch = fch->next_in_room )
		{
			if( !IS_NPC( fch ) )
				continue;

			if( fch->pIndexData->vnum == quest->pIndexData->questor )
			{
				quest_stop( ch, STOP_VERY_OK );
				q_log( "Quest finished (player: %s)", ch->name );
				return;
			}
		}
		return;
	}

    /* sprawdzamy, czy trigger nie odnosi siê
	do aktualnego (krytycznego) chapteru */
	chapter 	= ch->inquest->curr_chapter;
	interpret_trigger( ch, chapter, cmd, obj, victim );

	if( chapter && chapter->completed && !quest->finished )
	{
		q_log( "%s Chapter Completed",
			   bit_name( quest_chapter_types, chapter->pIndexData->type ) );
		process_action( ch->inquest, chapter );
	}
	else
    /* sprawdzamy jeszcze czy nie by³ to który¶
		z poprzednich niekrytycznych chapterów */
		if( ch->inquest
				  &&  ch->inquest->curr_chapter && ch->inquest->curr_chapter->prev )
	{
		for( chapter = ch->inquest->curr_chapter->prev;
				   chapter;
				   chapter = chapter->prev )
		{
			if( chapter->pIndexData->type == CHAPTER_CRITICAL
						 ||  chapter->pIndexData->type == CHAPTER_VERY_CRITICAL
						 ||  chapter->completed )
				continue;

			interpret_trigger( ch, chapter, cmd, obj, victim );

			if( chapter->completed && !quest->finished )
			{
				q_log( "%s Chapter Completed",
					   bit_name( quest_chapter_types, chapter->pIndexData->type ) );
				/* akcja (je¶li jest) */
//    		process_action( ch->inquest, chapter );
			}

		}
	}

	if( ch->inquest &&
		   (!ch->inquest->curr_chapter || ch->inquest->curr_chapter->completed) )
	/* Teraz musimy sprawdziæ, czy przypadkiem nie skoñczli¶my
		__t±_czynno¶ci±__ jakiego¶ zab³±kanego
		chaptera i szybciutko zainicjowaæ drugi */
		if( !init_chapter( ch->inquest, CHAPTER_NEXT ) )
	{
		if( !quest->finished )
			quest_stop( ch, STOP_OK );
	}
	return;
}


/* zwraca wska¼nik na chapter o numerze nr */
CHAPTER_DATA *get_chapter( QUEST_DATA *quest, int nr )
{
	CHAPTER_DATA *	chapter = NULL;

	for( chapter = quest->first_chapter; chapter; chapter = chapter->next )
		if( chapter->nr == nr )
			break;

	return chapter;
}

/* i na event w danym chapterze */
EVENT_DATA *get_event( CHAPTER_DATA *chapter, int nr )
{
	EVENT_DATA * 	event = NULL;

	for( event = chapter->first_event; event; event = event->next )
		if( event->nr == nr )
			break;

	return event;
}

/* zapisuje moba questowego do pliq */
void fwrite_quest_mob( CHAR_DATA *ch, QUEST_MOB_DATA *qMob, FILE *fp )
{
	CHAR_DATA *		mob=qMob->mob;
	AFFECT_DATA *	paf;
	SKILLTYPE *		skill = 0;
	int			roomvnum;

	if( qMob->invited )
		return;

	if( !mob || !mob->in_room )
	{
		q_bug( "Fwrite_quest_mob: No mob for qMob (player:%s)", ch->name );
		return;
	}

	if( mob->in_room->vnum > MAX_VNUM )
		roomvnum = mob->was_in_room->vnum;
	else
		roomvnum = mob->in_room->vnum;

	de_equip_char( mob );

	fprintf( fp, "#QUESTMOB  %d\n", mob->pIndexData->vnum );
	fprintf( fp, " Room      %d\n", roomvnum );
	fprintf( fp, " Act       %lld\n", mob->act );
	fprintf( fp, " Position  %d\n",
        mob->position == POS_FIGHTING ? POS_STANDING :
				mob->position == POS_MOUNTED  ? POS_STANDING : mob->position );
	fprintf( fp, " HpFrMv    %d %d %d %d %d %d\n",
			 mob->hit,  mob->max_hit,
			 mob->mana, mob->max_mana,
			 mob->move, mob->max_move );
	fprintf( fp, " Credits   %d\n", mob->gold );

	for ( paf = mob->first_affect; paf; paf = paf->next )
	{
		if ( paf->type >= 0 && (skill=get_skilltype(paf->type)) == NULL )
			continue;

		if ( paf->type >= 0 && paf->type < TYPE_PERSONAL )
			fprintf( fp, " AffectData   '%s' %3d %3d %3d %10lld\n",
					 skill->name,    paf->duration,  paf->modifier,
					 paf->location,  paf->bitvector	    	);
		else
			fprintf( fp, " Affect       %3d %3d %3d %3d %10lld\n",
					 paf->type,      paf->duration,  paf->modifier,
					 paf->location,  paf->bitvector    		);
	}

	if ( mob->first_carrying )
		fwrite_obj( mob, mob->last_carrying, fp, 0, OS_CARRY );

	fprintf( fp, "End\n\n" );

	re_equip_char( mob );
	return;
}

/* zapisuje wszystkie quest_przedmioty le¿±ce na ziemi (oraz ich zawarto¶ci)
   te w inventory (moba jaki i gracza) zapisuja sie w locie u nich.
 */
void fwrite_quest_obj( CHAR_DATA *ch, QUEST_OBJ_DATA *qObj, FILE * fp )
{
	OBJ_DATA *		obj=qObj->obj;
	REQUIREMENT_DATA *	req;
	int			roomvnum;

	if( !obj )
	{
		q_bug( "Fwrite_quest_obj: No obj for qObj (player:%s)", ch->name );
		return;
	}

	if( !obj->in_room )
		return;

	if( obj->in_room->vnum > MAX_VNUM )
	{
		q_bug( "Fwrite_quest_obj: Obj found in VirtualRoom!!! (player: %s) Putting Obj at QuestMaster's Office", ch->name );
		roomvnum = Quest_Master->in_room->vnum;
	}
	else
		roomvnum = obj->in_room->vnum;

	fprintf( fp, "#QUESTOBJ     %d %d\n", obj->pIndexData->vnum, obj->level   );
	fprintf( fp, " Room         %d\n", 	roomvnum	        );
	fprintf( fp, " Name         %s~\n",	obj->name	     	);
	fprintf( fp, " ShortDescr   %s~ %s~ %s~ %s~ %s~ %s~\n",
			 obj->przypadki[0], obj->przypadki[1], obj->przypadki[2],
			 obj->przypadki[3], obj->przypadki[4], obj->przypadki[5] );
	if ( str_cmp( obj->description, obj->pIndexData->description ) )
		fprintf( fp, " Description  %s~\n",	obj->description	);
	fprintf( fp, " Owner        %s~\n",	obj->owner_name       	);
	fprintf( fp, " Weight       %d\n",	obj->weight		);
	fprintf( fp, " Timer        %d\n",	obj->timer		);
	if ( obj->value[0] || obj->value[1] || obj->value[2]
			||   obj->value[3] || obj->value[4] || obj->value[5] )
		fprintf( fp, " Values       %d %d %d %d %d %d\n",
				 obj->value[0], obj->value[1], obj->value[2],
				 obj->value[3], obj->value[4], obj->value[5]     	);

	for ( req=obj->first_requirement; req; req = req->next )
	{
		char _buf		[MSL];

		if( req->location == REQ_SKILL && IS_VALID_SN( req->type ) )
		{
			sprintf( _buf, "%s", skill_table[req->type]->name );
		}
		else
			_buf[0]='\0';

		fprintf( fp, " Req          %d 0 %d 0 %s~\n",
				 req->location, req->modifier,
				 ( req->location == REQ_SKILL && IS_VALID_SN(req->type) ) ?
						 skill_table[req->type]->name : " " );
	}
	if( obj->last_content )
		fwrite_obj( ch, obj->last_content, fp, 0, OS_CARRY );

	fprintf( fp, "End\n\n" );
}

/* zapisuje stan questu w pliq */
void save_quest_data( CHAR_DATA *ch )
{
	FILE *		fp;
	QUEST_DATA *	quest = ch->inquest;
	QUEST_OBJ_DATA * 	qObj;
	QUEST_MOB_DATA * 	qMob;
	CHAPTER_DATA *	chptr;
	char 		filename	[256];


	if( !quest )
		return;

	sprintf( filename, "%s%s", QDATA_DIR, capitalize(ch->name) );

	if( (fp = fopen( filename, "w" )) == NULL )
	{
		q_bug( "Fwrite_quest_data: Could not open file %s for %s",
			   filename, ch->name );
		perror( filename );
		return;
	}

	fprintf( fp, "#QUEST          %d\n", quest->pIndexData->quest_id 	);
	fprintf( fp, " CurrQp         %d\n", quest->curr_qp    		);

	if( !quest->finished )
	{
		EVENT_DATA *	pEvent;

		for( chptr = quest->first_chapter; chptr; chptr = chptr->next )
		{
			if( !chptr->completed )
			{
				fprintf( fp, " ChapToDo       %d     ", chptr->nr );

				for( pEvent = chptr->first_event; pEvent; pEvent = pEvent->next )
					if( !pEvent->done )
						fprintf( fp, " %d", pEvent->nr );

				fprintf( fp, " 0\n" );
			}
		}

		if( quest->curr_chapter )
			fprintf( fp, " CurrChptr      %d\n", quest->curr_chapter->nr 	);
		else
			fprintf( fp, " CurrChptr     -1\n" );
	}
	else
		fprintf( fp, " CurrChptr     -1\n" );
	fprintf( fp, " TimeTook       %d\n",
			 quest->time_took +
					 (int)(current_time - quest->logon)/60 );
	fprintf( fp, "End\n\n" );

	for( qMob = ch->inquest->last_mob; qMob; qMob = qMob->prev )
		fwrite_quest_mob( ch, qMob, fp );

	for( qObj = ch->inquest->last_obj; qObj; qObj = qObj->prev )
		if( qObj->obj && qObj->obj->in_room )
			/* czyli przedmiot nie zosta³ jeszcze zapisany */
			fwrite_quest_obj( ch, qObj, fp );

	fprintf( fp, "#END\n\n" );
	fclose( fp );
	return;
}



/* zczytujemy z pliq  -- moba*/
void fread_quest_data_mob( CHAR_DATA *ch, FILE *fp )
{
	QUEST_MOB_DATA *	qMob;
	CHAR_DATA *		mob;
	const char *		word;
	bool		fMatch = false;

	if( !ch->inquest )
	{
		q_bug( "Couldn't find a quest. Wont read mobs" );
		return;
	}

	if( (qMob = qmob_invoke( fread_number( fp ), ch->inquest )) == NULL )
	{
		q_bug( "Could not create a qMob data. Wont read mobs" );
		return;
	}

	mob = qMob->mob;

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

			case '#':
				if ( !str_cmp( word, "#OBJECT" ) )
				{
					fread_obj  ( mob, fp, OS_CARRY );
					fMatch = true;
					break;
				}
				break;

			case 'A':
				KEY( "Act", 	mob->act, 		fread_number( fp ) );
				KEY( "Affected",	mob->affected_by,	fread_number( fp ) );
				if ( !str_cmp( word, "Affect" ) || !str_cmp( word, "AffectData" ) )
				{
					AFFECT_DATA *paf;

					CREATE( paf, AFFECT_DATA, 1 );
					if ( !str_cmp( word, "Affect" ) )
					{
						paf->type	= fread_number( fp );
					}
					else
					{
						int sn;
						char *sname = fread_word(fp);

						if ( (sn=skill_lookup(sname)) < 0 )
						{
							if ( (sn=herb_lookup(sname)) < 0 )
								q_bug( "Fread_quest_data_mob: unknown skill.", 0 );
							else
								sn += TYPE_HERB;
						}
						paf->type = sn;
					}

					paf->duration	= fread_number( fp );
					paf->modifier	= fread_number( fp );
					paf->location	= fread_number( fp );
					paf->bitvector	= fread_number( fp );
					LINK(paf, mob->first_affect, mob->last_affect, next, prev );
					fMatch = true;
					break;
				}
				break;

			case 'C':
				KEY( "Credits",	mob->gold,		fread_number( fp ) );
				break;

			case 'E':
				if( !str_cmp( word, "End" ) )
					return;
				break;

			case 'H':
				if( !str_cmp( word, "HpFrMv" ) )
				{
					mob->hit 	= fread_number( fp );
					mob->max_hit 	= fread_number( fp );
					mob->mana 	= fread_number( fp );
					mob->max_mana 	= fread_number( fp );
					mob->move 	= fread_number( fp );
					mob->max_move 	= fread_number( fp );
					fMatch 		= true;
					break;
				}
				break;

			case 'R':
				if( !str_cmp( word, "Room" ) )
				{
					ROOM_INDEX_DATA *	room;

					if( (room = get_room_index( fread_number( fp ) )) != NULL )
						char_to_room( mob, room );
					else
						char_to_room( mob, Quest_Master->in_room );

					fMatch = true;
					break;
				}
				break;

			case 'P':
				KEY( "Position",	mob->position,	fread_number( fp ) );
				break;
		}

		if ( !fMatch )
		{
			q_bug( "Fread_quest_data_mob: no match: %s", word );
			fread_to_eol( fp );
		}
	}

	return;
}


/* zczytujemy z pliq  -- obj */
void fread_quest_data_obj( CHAR_DATA *ch, FILE *fp )
{
	QUEST_OBJ_DATA *	qObj;
	OBJ_DATA *		obj;
	const char *		word;
	bool		fMatch = false;
	int			vnum;
	int			level;

	if( !ch->inquest )
	{
		q_bug( "Couldn't find a quest. Wont read objs" );
		return;
	}
	vnum  = fread_number( fp );
	level = fread_number( fp );

	if( (qObj = qobj_invoke( vnum, ch->inquest, level )) == NULL )
	{
		q_bug( "Could not create a qObj data. Wont read objs" );
		return;
	}

	obj = qObj->obj;

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

			case '#':
				if ( !str_cmp( word, "#OBJECT" ) )
				{
					OBJ_DATA *	tobj;
					OBJ_DATA *	tobj_next;

					set_supermob( obj );
					fread_obj ( supermob, fp, OS_CARRY );
					for ( tobj = supermob->first_carrying; tobj; tobj = tobj_next )
					{
						tobj_next = tobj->next_content;
						obj_from_char( tobj );
						obj_to_obj( tobj, obj );
					}
					release_supermob(); /* panu ju¿ podziêkujemy :P */
					fMatch = true;
					break;
				}
				break;

			case 'D':
				SKEY( "Description",obj->description,	fread_string( fp ) );
				break;

			case 'E':
				if( !str_cmp( word, "End" ) )
					return;
				break;

			case 'N':
				SKEY( "Name",	obj->name,		fread_string( fp ) );
				break;

			case 'O':
				SKEY( "Owner",	obj->owner_name,	fread_string( fp ) );
				break;

			case 'R':
				if( !str_cmp( word, "Room" ) )
				{
					ROOM_INDEX_DATA *	room;

					if( (room = get_room_index( fread_number( fp ) )) != NULL )
						obj_to_room( obj, room );
					else
						obj_to_room( obj, Quest_Master->in_room );

					fMatch = true;
					break;
				}
				/* Thanos: requirements */
				if ( !str_cmp( word, "Req" ) )
				{
					REQUIREMENT_DATA *	pReq;
					int			loc;
					int			mod;
					int			type;

					loc 		= fread_number( fp );
					fread_number( fp );
					mod 		= fread_number( fp );
					fread_number( fp );
					type		= skill_lookup( st_fread_string( fp ) );
					if( loc < MAX_REQ )
					{
						CREATE( pReq, REQUIREMENT_DATA, 1 );
						pReq->location = loc;
						pReq->type	   = type;
						pReq->modifier = mod;
						LINK( pReq, obj->first_requirement,
							  obj->last_requirement, next, prev );
					}
					fMatch = true;
					break;
				}
				/* done requirements */
				break;

			case 'S':
				if( !str_cmp( word, "ShortDescr" ) )
				{
					STRDUP( obj->przypadki[0], st_fread_string( fp ) );
					STRDUP( obj->przypadki[1], st_fread_string( fp ) );
					STRDUP( obj->przypadki[2], st_fread_string( fp ) );
					STRDUP( obj->przypadki[3], st_fread_string( fp ) );
					STRDUP( obj->przypadki[4], st_fread_string( fp ) );
					STRDUP( obj->przypadki[5], st_fread_string( fp ) );
					fMatch 			= true;
					break;
				}
				break;

			case 'T':
				KEY( "Timer",	obj->timer,		fread_number( fp ) );
				break;

			case 'V':
				if ( !str_cmp( word, "Values" ) )
				{
					obj->value[0]		= fread_number( fp );
					obj->value[1]		= fread_number( fp );
					obj->value[2]		= fread_number( fp );
					obj->value[3]		= fread_number( fp );
					obj->value[4]		= fread_number( fp );
					obj->value[5]		= fread_number( fp );
					fMatch			= true;
					break;
				}
				break;

			case 'W':
				KEY( "Weight",	obj->weight,		fread_number( fp ) );
				break;
		}

		if ( !fMatch )
		{
			q_bug( "Fread_quest_data_obj: no match: %s", word );
			fread_to_eol( fp );
		}
	}

	return;
}


/* zczytujemy z pliq -- nag³ówek */
void fread_quest_data_header( CHAR_DATA *ch, FILE *fp )
{
	QUEST_DATA * 	quest;
	const char *		word;
	bool 		fMatch = false;

	quest = quest_start( ch, fread_number( fp ), true );

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

			case 'C':
				KEY( "CurrQp",	quest->curr_qp, 	fread_number( fp ) );
				if( !str_cmp( word, "CurrChptr" ) )
				{
					int	nr;

					nr = fread_number( fp );
					if( nr == -1 ) /* je¶li -1 to quest ju¿ w³a¶ciwie zakoñczony */
					{
						quest->finished = true;
						quest->curr_chapter = quest->last_chapter;
					}
					else
						quest->curr_chapter = get_chapter( quest, nr );

					if( !quest->curr_chapter )
					{
						q_bug( "Fread_quest_data_header: FATAL!!! Couldn't find curr_chapter!" );
						quest_cleanup( ch, true );
						ch->inquest = NULL;
						return;
					}

					fMatch = true;
					break;
				}

				if( !str_cmp( word, "ChapToDo" ) )
				{
					CHAPTER_DATA * chapter;
					EVENT_DATA * event;

					if( (chapter = get_chapter( quest, fread_number( fp ) )) == NULL )
					{
						q_bug( "Fread_quest_data_header: FATAL!!! Couldn't find chapter!" );
						quest_cleanup( ch, true );
						ch->inquest = NULL;
						return;
					}

					for( event = chapter->first_event; event; event = event->next )
						event->done = true;

					for( ; ; )
					{
						int evnum;
						evnum = fread_number( fp );
						if( evnum == 0 )
							break;

						if( (event = get_event( chapter, evnum )) == NULL )
						{
							q_bug( "Fread_quest_data_header: FATAL!!! Couldn't find event!" );
							quest_cleanup( ch, true );
							ch->inquest = NULL;
							return;
						}
						event->done = false;
					}
					chapter->completed = false;
					fMatch = true;
					break;
				}
				break;

			case 'E':
				if( !str_cmp( word, "End" ) )
				{
					ch->inquest = quest;
					return;
				}
				break;
			case 'T':
				KEY( "TimeTook", 	quest->time_took, 	fread_number( fp ) );
				break;
		}

		if ( !fMatch )
		{
			q_bug( "Fread_qdata_header: no match: %s", word );
			fread_to_eol( fp );
		}
	}

	return;
}



/*
 * Funkcja przywraca graczowi questa
 * (je¶li istnieje plik ~/player/quests/<name>)
 */
void load_quest_data( CHAR_DATA *ch )
{
	FILE *		fp;
	char		filename	[256];

	sprintf( filename, "%s%s", QDATA_DIR, capitalize(ch->name) );

	if( (fp = fopen( filename, "r" )) == NULL )
		return;

	for ( ; ; )
	{
		char 	letter;
		char *	word;
		bool 	fMatch = false;

		letter = fread_letter( fp );
		if ( letter == '*' )
		{
			fread_to_eol( fp );
			continue;
		}

		if ( letter != '#' )
		{
			q_bug( "No '#' in QuestData file (file:%s)", filename );
			fclose( fp );
			return;
		}

		word = fread_word( fp );
		if( !str_cmp( word, "QUEST" ) )
		{
			fread_quest_data_header( ch, fp );	    fMatch = true;
		}
		else
			if( !str_cmp( word, "QUESTMOB" ) )
		{
			fread_quest_data_mob( ch, fp );	    fMatch = true;
		}
		else
			if( !str_cmp( word, "QUESTOBJ" ) )
		{
			fread_quest_data_obj( ch, fp );	    fMatch = true;
		}
		else
			if( !str_cmp( word, "END" ) )
				return;

		if ( !fMatch )
		{
			q_bug( "Fread_quest_data: no match: %s", word );
		}
	}

	return;
}


