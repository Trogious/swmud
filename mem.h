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


void free_course( COURSE_DATA *course );
void free_courses( SHIP_DATA *ship );
EDD *new_ed();
void free_ed( EDD *ed );
ED *new_exit();
void free_exit( ED *pExit );
MPROG_DATA *new_mprog();
void free_mprog( MPROG_DATA *prg );
AFFECT_DATA *new_affect();
void free_affect( AFFECT_DATA *pAf );
RESET_DATA *new_reset();
void free_reset( RESET_DATA *pReset );
void free_area( AREA_DATA *are );
void free_note( NOTE_DATA *pnote );
void free_command( CMDTYPE *command );
void free_social( SOCIALTYPE *social );
void free_pcdata( PC_DATA *pcdata );
void free_char( CHAR_DATA *ch );
CLAN_DATA *new_clan();
void free_clan( CLAN_DATA *clan );
MEMBER_DATA *new_member( int status );
void free_member( MEMBER_DATA *member );
HELPS_FILE *new_helps_file();
void free_helps_file( HELPS_FILE *fHelp );
void clear_helps_file( char *filename );
HELP_DATA *new_help( HELPS_FILE *fHelp );
void free_help( HELP_DATA *pHelp );
SEASON_DATA *new_season( PLANET_DATA *planet, const char *name );
void free_who( WHO_DATA *who );
void free_ban( BAN_DATA *pban );
void free_mpact( MPROG_ACT_LIST * mpact );
void free_bounty( BOUNTY_DATA *bounty );
void free_last( LAST_DATA *last );
void free_alias( ALIAS_DATA *alias );
void free_desc( DESCRIPTOR_DATA *d );
void free_crime( CHAR_DATA *ch, CRIME_DATA *crime );
void free_wiz( WIZENT *wiz );
RID *new_room();
void free_room( RID *room );
void free_hhf( HHF_DATA *hhf );
OID *new_obj_index();
void free_obj_index( OID *obj );
void free_obj( OBJ_DATA *obj );
void free_nameslist( NAMESLIST_DATA *nameslist );
void free_season( SEASON_DATA *season );
void free_dock( DOCK_DATA *dock );
void free_chapter( CHAPTER_INDEX_DATA *chapt );
void free_qcmd( QUEST_CMND_DATA *pCmnd );
void free_qaction( QUEST_ACTION_DATA *action );
void free_smaug_aff( SMAUG_AFF *aff );
void free_star( STAR_DATA *star );
void free_moon( MOON_DATA *moon );
void free_missile( MISSILE_DATA *missile );
void free_starsystem( SPACE_DATA *starsystem );
void free_transponder( TRANSPONDER_DATA *trans );
void free_astro( ASTRO_DATA *astro );
void free_suspect( CHAR_DATA *ch, SUSPECT_DATA *sus );
void free_script( SCRIPT_DATA *script );
void free_mid( MOB_INDEX_DATA *mid );
void free_oid( OBJ_INDEX_DATA *oid );
void free_inform( INFORM_DATA *inform );
void free_ship( SHIP_DATA *ship );
SHIP_DATA *new_ship( );
SHIP_INDEX_DATA *new_ship_index( );
void free_sroom( SHIP_ROOM_DATA * sRoom );
void free_srooms( SHIP_INDEX_DATA *ship );
void free_part( PART_DATA *part );
void free_project( PROJECT_DATA *pro );
void free_complain( COMPLAIN_DATA *complain );
void free_skill( SKILLTYPE *skill );
RACE_DATA *new_race();
void free_race( RACE_DATA *race );
LANG_DATA *new_lang();
void free_lang( LANG_DATA *lang );
KNOWN_LANG *new_known_lang();
void free_known_lang( KNOWN_LANG *lang );
BETS_PROGRES * new_bets();
void free_bets(BETS_PROGRES *	pBets);
AUCTION_DATA * new_auction();
void free_auction(AUCTION_DATA *	pAuction);
MATERIAL_DATA * new_material();
void free_material( MATERIAL_DATA * pMaterial );
DIALOG_NODE *new_dialog_node();
void free_dialog_node( DIALOG_NODE *pNode );
DIALOG_DATA * new_dialog_data();
void free_dialog_data( DIALOG_DATA * pData );
DEPOSIT_DATA * new_deposit();
void free_deposit( DEPOSIT_DATA * pDeposit );
CARGO_DATA * new_cargo();
void free_cargo( CARGO_DATA * pCargo );
TURBOCAR *new_turbocar();
void free_turbocar( TURBOCAR *tc );
TC_STATION *new_station();
void free_station( TC_STATION *station );
FEVENT_DATA	*new_fevent();
void free_fevent( FEVENT_DATA *fevent );
MID *new_mob();
void free_mob( MID *mob );
REQ *new_requirement();
void free_requirement( REQ *req );
SHD *new_shop();
void free_shop( SHD *shop );
RHD *new_repair();
void free_repair( RHD *repair );
AREA_TMP_DATA *new_area_tmp();
void free_area_tmp( AREA_TMP_DATA *area_tmp );
ILD *new_ild();
void free_ild( ILD *ild );
POLITICS_DATA *new_politics();
void free_politics( POLITICS_DATA *politics );

AREA_DATA		* new_area		args( ( ) );
HELPS_FILE		* new_helps_file	args( ( ) );
HELP_DATA		* new_help		args( ( HELPS_FILE *fHelp ) );
SHIP_DATA		* new_ship		args( ( ) );
SHIP_INDEX_DATA * new_ship_index	args( ( ) );
SEASON_DATA		* new_season		args( ( PLANET_DATA *planet, char *name ) );
CLAN_DATA		* new_clan		args( ( ) );
MEMBER_DATA		* new_member		args( ( int ststus ) );
RACE_DATA		* new_race		args( ( ) );
MATERIAL_DATA	* new_material	args ( ( void ) );
DIALOG_DATA		* new_dialog_data	args ( ( void ) );
DIALOG_NODE		* new_dialog_node	args ( ( void ) );
BETS_PROGRES	* new_bets				(void);
AUCTION_DATA	* new_auction			(void);
STOCK_EXCHANGE_DATA * new_stock_exchange(void);
DEPOSIT_DATA	* new_deposit			(void);
HQ_DATA *		new_hq_data( void );

void	free_part		args( ( PART_DATA		*part	) );
void	free_project	args( ( PROJECT_DATA		*pro	) );
void	free_helps_file args( ( HELPS_FILE		*fHelp	) );
void	free_help		args( ( HELP_DATA		*pHelp	) );
void	free_ed			args( ( EXTRA_DESCR_DATA	*ed	) );
void	free_exit		args( ( EXIT_DATA		*pExit	) );
void	free_mprog		args( ( MPROG_DATA		*prg	) );
void	free_affect		args( ( AFFECT_DATA		*pAf	) );
void	free_reset		args( ( RESET_DATA		*pReset ) );
void	free_area		args( ( AREA_DATA		*are	) );
void	free_note		args( ( NOTE_DATA		*pnote	) );
void	free_command	args( ( CMDTYPE			*command) );
void	free_pcdata		args( ( PC_DATA			*pcdata ) );
void	free_char		args( ( CHAR_DATA		*ch	) );
void	free_social		args( ( SOCIALTYPE		*social ) );
void	free_course		args( ( COURSE_DATA		*course ) );
void	free_courses	args( ( SHIP_DATA		*ship	) );
void	free_who		args( ( WHO_DATA		*who	) );
void	free_room		args( ( ROOM_INDEX_DATA		*room	) );
void	free_ban		args( ( BAN_DATA		*pban	) );
void	free_mpact		args( ( MPROG_ACT_LIST		*mpact	) );
void	free_mid		args( ( MOB_INDEX_DATA		*mid	) );
void	free_oid		args( ( OBJ_INDEX_DATA		*oid	) );
void	free_obj		args( ( OBJ_DATA		*obj	) );
void	free_bounty		args( ( BOUNTY_DATA		*bounty ) );
void	free_alias		args( ( ALIAS_DATA		*alias	) );
void	free_clan		args( ( CLAN_DATA		*clan	) );
void	free_member		args( ( MEMBER_DATA		*member ) );
void	free_wiz		args( ( WIZENT			*wiz	) );
void	free_hhf		args( ( HHF_DATA		*hhf	) );
void	free_nameslist	args( ( NAMESLIST_DATA	     *nameslist ) );
void	free_script		args( ( SCRIPT_DATA		*script ) );
void	free_season		args( ( SEASON_DATA		*season ) );
void	free_dock		args( ( DOCK_DATA		*dock	) );
void	free_qcmd		args( ( QUEST_CMND_DATA		*pCmnd	) );
void	free_chapter	args( ( CHAPTER_INDEX_DATA	*chapt	) );
void	free_smaug_aff	args( ( SMAUG_AFF		*aff	) );
void	free_star		args( ( STAR_DATA		*star	) );
void	free_moon		args( ( MOON_DATA		*moon	) );
void	free_missile	args( ( MISSILE_DATA		*missile) );
void	free_starsystem args( ( SPACE_DATA	    *starsystem ) );
void	free_transponder	args( ( TRANSPONDER_DATA	*trans	) );
void	free_astro		args( ( ASTRO_DATA		*astro	) );
void	free_suspect	args( ( CHAR_DATA *ch, SUSPECT_DATA *sus) );
void	free_last		args( ( LAST_DATA		*last	) );
void	free_inform		args( ( INFORM_DATA		*inform ) );
void	free_ship		args( ( SHIP_DATA		*ship	) );
void	free_complain	args( ( COMPLAIN_DATA		*comp	) );
void	free_skill		args( ( SKILLTYPE *skill ) );
void	free_race		args( ( RACE_DATA *race ) );
void	free_material	args( ( MATERIAL_DATA * pMat ));
void	free_dialog_data	args ( ( DIALOG_DATA * pData ) );
void	free_dialog_node	args ( ( DIALOG_NODE *pBase ) );
void	free_auction		(AUCTION_DATA * pAuction);
void	free_stock_exchange	(STOCK_EXCHANGE_DATA * pStock);
void	free_bets			(BETS_PROGRES * pBets);
void	free_deposit		(DEPOSIT_DATA * pDeposit);

