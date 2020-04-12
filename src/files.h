/***************************************************************************
* Star Wars Reality Code Additions and changes from the Smaug Code         *
* copyright (c) 1997, 1998 by Sean Cooper                                  *
* Starwars and Starwars Names copyright (c) Lucasfilm Ltd.                 *
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
*			    Main mud header file			   *
****************************************************************************/

/*
 * Data files used by the server.
 *
 * AREA_LIST contains a list of areas to boot.
 * All files are read in completely at bootup.
 * Most output files (bug, idea, typo, shutdown) are append-only.
 *
 * The NULL_FILE is held open so that we have a stream handle in reserve,
 *   so players can go ahead and telnet to all the other descriptors.
 * Then we close it whenever we need to open a file (e.g. a save file).
 */

#if !defined( WIN32 )

#include		"../.mudrc"	/* Tam jest zdefiniowane MUD_DIR */    

#define NULL_FILE	"/dev/null"	/* To reserve stream*/

#define ETC_DIR		MUD_DIR 	"etc/"
#define WORLD_DIR	MUD_DIR 	"world/"
#define LOG_DIR		MUD_DIR 	"log/"		/* Log stuff Dir*/
#define SRC_DIR		MUD_DIR 	"src/"		/* Dla copyover */
#define BIN_DIR		MUD_DIR 	"bin/"
#define TMP_DIR		MUD_DIR 	"tmp/"

#define LISTS_DIR	ETC_DIR		"lists/"	/* listy 	*/
#define SCHEMAS_DIR	ETC_DIR		"schemas/xml/"	/* schematy	*/

#define PFILE_DIR	WORLD_DIR   	"player/"	/* Player Dir	*/
#define CLAN_DIR	WORLD_DIR	"clans/"	/* Clan data dir*/
#define NEWCLAN_DIR	WORLD_DIR	"newclans/"
#define SPACE_DIR       WORLD_DIR	"space/"
#define BOARD_DIR	WORLD_DIR	"notes/"	/* Board data dir*/
#define QINDEX_DIR	WORLD_DIR	"quests/"	/* QuestIndex files*/
#define AREA_DIR	WORLD_DIR	"area/"
#define DIALOG_DIR	WORLD_DIR	"dialogs/"
#define STOCK_MARKET_DIR	WORLD_DIR	"stocks/"
#define STORAGES_DIR	WORLD_DIR	"storages/"	/* Dane magazynów */

#define DEPOSIT_DIR	PFILE_DIR 	"deposit/"	/* Depozyty 	*/
#define BACKUP_DIR	PFILE_DIR 	"backup/"    	/* Backup Pfiles*/
#define GOD_DIR		PFILE_DIR 	"gods/"		/* God Info Dir	*/
#define CORPSE_DIR	PFILE_DIR 	"corpses/"	/* Corpses	*/
#define QDATA_DIR	PFILE_DIR 	"quests/"	/* Questy	*/
#define PDATA_DIR	PFILE_DIR	"pdata/"	/* Dane absolutne*/
#define PCHAR_DIR	PFILE_DIR	"pchar/"	/* Dane postaci */
#define CLONE_DIR	PFILE_DIR	"clones/"	/* Dane klonów  */
#define PHOME_DIR	PFILE_DIR	"homes/"	/* Dane domów   */
/* Backupy */
#define PDATA_BCK_DIR	BACKUP_DIR	"pdata/"	/* Dane absolutne*/
#define PCHAR_BCK_DIR	BACKUP_DIR	"pchar/"	/* Dane postaci */
#define CLONE_BCK_DIR	BACKUP_DIR	"clones/"	/* Dane klonów  */

#define GAMELOG_DIR	LOG_DIR		"game/"		/* Game Log Dir */

#define SHIP_TMP_DIR    TMP_DIR   	"ships/"	/* zapis stanu statków  Thanos */

#define SHIP_DIR        SPACE_DIR	"ships/"
#define STARS_DIR       SPACE_DIR	"stars/"	/* Alde starsystems dir */
#define ASTRO_DIR	SPACE_DIR	"astro/" 	/* Alde Astro objects dir*/
#define PLANET_DIR      SPACE_DIR	"planets/"    
#define GUARD_DIR       SPACE_DIR	"planets/"    
#define SHIPDB_DIR     	SPACE_DIR	"shipdb/"	/* Thanos */
#define TURBOCAR_DIR  	SPACE_DIR	"turbocars/"	/* Trog */

#define PROG_DIR	AREA_DIR	"mudprogs/"	/* MUDProg files*/
#define AREA_BAK_DIR	AREA_DIR	"backups/"	/* backup area files*/
#define NAMESLIST_DIR	AREA_DIR	"nameslist/"	/* Trog: listy mobow */
#define HELPS_DIR	AREA_DIR	"helps/"	/* Trog: Helps files */
#define RACE_DIR	WORLD_DIR	"race/"	/* Trog: Racess files */

#define LANGUAGES_FILE	RACE_DIR	"languages.xml"	/* Trog: languages file */

#define BIN_FILE    BIN_DIR     "swmud"


#else


#include		"..\\.mudrc"	/* Tam jest zdefiniowane MUD_DIR */    

#define NULL_FILE	"nul"		/* To reserve stream*/

#define ETC_DIR		MUD_DIR 	"etc\\"
#define WORLD_DIR	MUD_DIR 	"world\\"
#define LOG_DIR		MUD_DIR 	"log\\"		/* Log stuff Dir*/
#define SRC_DIR		MUD_DIR 	"src\\"		/* Dla copyover */
#define BIN_DIR		MUD_DIR 	"bin\\"
#define TMP_DIR		MUD_DIR 	"tmp\\"

#define LISTS_DIR	ETC_DIR		"lists\\"	/* listy 	*/
#define SCHEMAS_DIR	ETC_DIR		"schemas\\xml\\"	/* schematy	*/

#define PFILE_DIR	WORLD_DIR   	"player\\"	/* Player Dir	*/
#define CLAN_DIR	WORLD_DIR	"clans\\"	/* Clan data dir*/
#define NEWCLAN_DIR	WORLD_DIR	"newclans\\"
#define SPACE_DIR       WORLD_DIR	"space\\"
#define BOARD_DIR	WORLD_DIR	"notes\\"	/* Board data dir*/
#define QINDEX_DIR	WORLD_DIR	"quests\\"	/* QuestIndex files*/
#define AREA_DIR	WORLD_DIR	"area\\"
#define DIALOG_DIR	WORLD_DIR	"dialogs\\"
#define	STOCK_MARKET_DIR	WORLD_DIR	"stocks\\"

#define DEPOSIT_DIR	PFILE_DIR 	"deposit\\"	/* Depozyty 	*/
#define BACKUP_DIR	PFILE_DIR 	"backup\\"    	/* Backup Pfiles*/
#define GOD_DIR		PFILE_DIR 	"gods\\"		/* God Info Dir	*/
#define CORPSE_DIR	PFILE_DIR 	"corpses\\"	/* Corpses	*/
#define QDATA_DIR	PFILE_DIR 	"quests\\"	/* Questy	*/
#define PDATA_DIR	PFILE_DIR	"pdata\\"	/* Dane absolutne*/
#define PCHAR_DIR	PFILE_DIR	"pchar\\"	/* Dane postaci */
#define CLONE_DIR	PFILE_DIR	"clones\\"	/* Dane klonów  */
#define PHOME_DIR	PFILE_DIR	"homes\\"	/* Dane domów   */
/* Backupy */
#define PDATA_BCK_DIR	BACKUP_DIR	"pdata\\"	/* Dane absolutne*/
#define PCHAR_BCK_DIR	BACKUP_DIR	"pchar\\"	/* Dane postaci */
#define CLONE_BCK_DIR	BACKUP_DIR	"clones\\"	/* Dane klonów  */

#define GAMELOG_DIR	LOG_DIR		"game\\"		/* Game Log Dir */

#define SHIP_TMP_DIR    TMP_DIR   	"ships\\"	/* zapis stanu statków  Thanos */

#define SHIP_DIR        SPACE_DIR	"ships\\"
#define STARS_DIR       SPACE_DIR	"stars\\"	/* Alde starsystems dir */
#define ASTRO_DIR	SPACE_DIR	"astro\\" 	/* Alde Astro objects dir*/
#define PLANET_DIR      SPACE_DIR	"planets\\" 
#define GUARD_DIR       SPACE_DIR	"planets\\"    
#define SHIPDB_DIR     	SPACE_DIR	"shipdb\\"	/* Thanos */
#define TURBOCAR_DIR  	SPACE_DIR	"turbocars\\"	/* Trog */

#define PROG_DIR	AREA_DIR	"mudprogs\\"	/* MUDProg files*/
#define AREA_BAK_DIR	AREA_DIR	"backups\\"	/* backup area files*/
#define NAMESLIST_DIR	AREA_DIR	"nameslist\\"	/* Trog: listy mobow */
#define HELPS_DIR	AREA_DIR	"helps\\"	/* Trog: Helps files */
#define RACE_DIR	WORLD_DIR	"race\\"	/* Trog: Racess files */

#define LANGUAGES_FILE	RACE_DIR	"languages.xml"	/* Trog: languages file */

#define BIN_FILE    BIN_DIR     	"swmud.exe"


#endif


#define SMS_SCRIPT	BIN_DIR		"crashsms" 	/*sms*/

#define TMP_FILE      	TMP_DIR   	"TMP.FILE"      /* Do wielu rzeczy */
#define COPYOVER_FILE 	TMP_DIR 	"COPYOVER.TXT"  /* deskryptory 	*/
#define PID_FILE	TMP_DIR		"swmud.pid"	/* pid muda */
#define INFORM_LIST     TMP_DIR		"inform.lst"      
#define DISINTIGRATION_LIST TMP_DIR	"disintigration.lst"
#define SHIP_TMP_LIST	TMP_DIR		"ships.lst"	/* lista stanów statków Thanos */

#define WIZLIST_FILE	LISTS_DIR 	"wizlist.lst"	/* Wizlist	*/
#define AREA_LIST	LISTS_DIR	"area.lst"	/* List of areas*/
#define AREA_LISTXML	LISTS_DIR	"area.xml"	/* List of areas*/
#define CLAN_LIST	LISTS_DIR	"clan.lst"	/* List of clans*/
#define CLAN_LISTXML	LISTS_DIR	"clan.xml"	/* List of clans*/
#define NEWCLAN_LIST	LISTS_DIR	"newclan.lst"
#define SHIP_LIST       LISTS_DIR	"ship.lst"
#define TURBOCAR_LIST   LISTS_DIR	"turbocar.lst" /* Trog */
#define BUS_LIST	LISTS_DIR	"bus.lst"	/* Added by Thanos*/
#define PLANET_LIST     LISTS_DIR	"planet.lst"
#define SPACE_LIST      LISTS_DIR	"space.lst"
#define ASTRO_LIST      LISTS_DIR	"astro.lst"	/* Added by Aldegard*/
#define QUEST_LIST	LISTS_DIR	"quest.lst"	/* List of quests*/
#define HELPS_LIST	LISTS_DIR 	"helps.lst"	/* Lista plików z helpami */
#define SHIP_DB       	LISTS_DIR	"ship.db"	/* Thanos */
#define BOUNTY_LIST     		"bounty.lst"      
#define SENATE_LIST			"senate.lst"	/* List of senators*/
#define GUILD_LIST      		"guild.lst"     /* List of guilds  */
#define GOD_LIST			"gods.lst"	/* List of gods	   */
#define GUARD_LIST			"guard.lst"
#define RACE_LIST	LISTS_DIR	"race.lst"
#define RACE_LISTXML	LISTS_DIR	"race.xml"
#define DIALOG_LIST_FILE	LISTS_DIR		"dialogs.lst"
#define STOCKS_LIST	LISTS_DIR	"stocks.lst"
#define BOARD_FILE	BOARD_DIR	"boards.txt"	/* For bulletin boards*/
#define OLDNOTES_FILE   BOARD_DIR  	"OLDNOTES.TXT"  /* Stare notki     */


#define SKILL_FILE	ETC_DIR 	"skills.dat"   	/* Skill table	*/
#define HERB_FILE	ETC_DIR 	"herbs.dat"	/* Herb table	*/
#define SOCIAL_FILE	ETC_DIR 	"socials.dat"  	/* Socials	*/
#define COMMAND_FILE	ETC_DIR 	"commands.dat" 	/* Commands	*/
#define TONGUE_FILE   	ETC_DIR 	"langs.dat"     /* jêzyki        */
#define PROJECTS_FILE	ETC_DIR		"projects.dat"	/* Dla techników*/
#define AUCTION_FILE    ETC_DIR		"auction.data"	//spis aukcji
#define MATERIAL_FILE	ETC_DIR		"material.dat"	//
#define SYSDATA_FILE	ETC_DIR		"sysdata.dat"	/* sysdata 	*/
#define CLONINGS_FILE	ETC_DIR		"clonings.dat"	/*klonownie: Than*/
#define BAN_LIST	ETC_DIR		"ban.lst"       /* List of bans */

#define SHUTDOWN_FILE	LOG_DIR		"shutdown.txt"	/* For 'shutdown'*/
#define BOOTLOG_FILE	GAMELOG_DIR 	"boot.txt"	/* Bootup err file */
#define BUG_FILE	GAMELOG_DIR 	"bugs.txt"	/* For bug( )*/
#define PROGBUG_FILE	GAMELOG_DIR 	"ProgBugs.txt"	/* For prog_bug( )*/

#define PBUG_FILE	GAMELOG_DIR 	"pbugs.txt"	/* For 'bug'		 */
#define PIDEA_FILE	GAMELOG_DIR 	"pideas.txt"  	/* For 'idea'		 */
#define PTYPO_FILE	GAMELOG_DIR 	"ptypos.txt"  	/* For 'typo'		 */
#define PBUG_FIX_FILE	GAMELOG_DIR 	"pbugs.fixed"	/* For fixed bugs	 */
#define PIDEA_FIX_FILE	GAMELOG_DIR 	"pideas.fixed"  /* For fixed ideas	 */
#define PTYPO_FIX_FILE	GAMELOG_DIR 	"ptypos.fixed"  /* For fixed typos	 */

#define LOG_FILE	GAMELOG_DIR 	"log.txt"	/* For talking in logged rooms */
#define WHO_FILE	GAMELOG_DIR 	"WHO"	  	/* Who output file   */
#define WEBWHO_FILE	GAMELOG_DIR 	"WEBWHO"	/* WWW Who outpt file*/
#define REQUEST_PIPE	GAMELOG_DIR 	"REQUESTS"	/* Request FIFO	*/
#define LOGALL_FILE	GAMELOG_DIR 	"LOGALL.LOG"	/* security */
#define LASTCMD_FILE	GAMELOG_DIR 	"LASTCMD.LOG"	/* w razie padów */
#define USAGE_FILE	GAMELOG_DIR 	"usage.txt"     /* How many people on*/
#define QBUG_FILE	GAMELOG_DIR 	"QBUGS.txt"     /*bugi silnika questów*/

#define SHIP_TMP_AREA			"fighters.xml"			/* kraina dla shedita */

#define	SCHEMA_LIST			SCHEMAS_DIR	"list.xsd"		/* schemat dla krainek */
#define	SCHEMA_AREA			SCHEMAS_DIR	"area.xsd"		/* schemat dla krainek */
#define	SCHEMA_RACE			SCHEMAS_DIR	"race.xsd"		/* schemat dla krainek */
#define	SCHEMA_CLAN			SCHEMAS_DIR	"clan.xsd"		/* schemat dla krainek */
#define	SCHEMA_LANGUAGES	SCHEMAS_DIR	"languages.xsd"	/* schemat dla krainek */

