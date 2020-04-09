/***************************************************************************
 * Star Wars Reality Code Additions and changes from the Smaug Code	   *
 * copyright (c) 1997 by Sean Cooper					   *
 * Starwars and Starwars Names copyright(c) Lucas Film Ltd.		   *
 * SMAUG 1.0 (C) 1994, 1995, 1996 by Derek Snider			   *
 * SMAUG code team: Thoric, Altrag, Blodkai, Narn, Haus, 		   *
 * Scryn, Rennard, Swordbearer, Gorog, Grishnakh and Tricops		   *
 * Merc 2.1 Diku Mud improvments copyright (C) 1992, 1993 by Michael	   *
 * Chastain, Michael Quan, and Mitchell Tse.				   *
 * Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,	   *
 * Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.	   *
 * ------------------------------------------------------------------------ *
 *		      ____________   __     ______			   *
 *   Aldegard	Jot  (	 ___   ___| /  \   |  __  )   Thanos	  Trog	   *
 *	 ______________\  \|   |  /  ^	 \ |	 <_______________	   *
 *	 \________________/|___|/___/"\___\|__|\________________/	   *
 *		   \   \/ \/   //  \   |  __  )(  ___/` 		   *
 *		     \	     //  ^   \ |     <__)  \			   *
 *		       \_!_//___/"\___\|__|\______/TM			   *
 * (c) 2001, 2002	    M	    U	     D		      Ver 1.1	   *
 * ------------------------------------------------------------------------ *
 *			      Special clan module			   *
 ****************************************************************************/

#include <sys/types.h>
#include <ctype.h>
#include <time.h>
#include "mud.h"
#include <SWTimeStamp.h>

#define MAX_NEST	100

/* Trog: wlasciwie to wszystko w tym pliq jest moje, wiec nie bede sie
 juz podpisywal pod tym co robie bo mi sie nie chce.

 Ju¿ nie - Pixel :) */

//static	OBJ_DATA *	rgObjNest	[MAX_NEST];
extern int top_clan;

CLAN_DATA *first_clan = NULL;
CLAN_DATA *last_clan = NULL;
HQ_ROOM_DESC *first_hq_room_desc;
HQ_ROOM_DESC *last_hq_room_desc;

/* local routines */
void fread_clan args( ( CLAN_DATA *clan, FILE *fp ) );
bool load_clan_file args( ( char *clanfile ) );
void write_clan_list args( ( void ) );
void save_clan_list args( ( void ) );
void fread_politics args( ( CLAN_DATA *clan, char *filename ) );
void pclan_voting_end args( ( CLAN_DATA *clan ) );
void application args( ( CHAR_DATA *ch, char *argument ) );
int max_clan_id args( ( void ) );
static char* mk_filename args( ( int clan_id ) );
MEMBER_DATA* highest_ranked args( ( CLAN_DATA *clan, int max_rank ) );
void adjust_only args( ( CLAN_DATA *clan ) );
void clan_switch args( ( CHAR_DATA *ch, char *argument ) );
void clan_loan args( ( CHAR_DATA *ch, char *argument ) );

const char *clantypename[CLAN_MAXTYPE][6] =
{
{ "organizacja nieautoryzowana", "organizacji nieautoryzowanej",
		"organizacji nieautoryzowanej", "organizacjê nieautoryzowan±",
		"organizacji nieautoryzowanej", "organizacj± nieautoryzowan±" },
{ "organizacja", "organizacji", "organizacji", "organizacjê", "organizacji",
		"organizacj±" },
{ "organizacja podporz±dkowana", "organizacji podporz±dkowanej",
		"organizacji podporz±dkowanej", "organizacjê podporz±dkowan±",
		"organizacji podporz±dkowanej", "organizacj± podporz±dkowan±" },
{ "gildia", "gildii", "gildii", "gildiê", "gildii", "gildi±" },
{ "zakon", "zakonu", "zakonowi", "zakon", "zakonie", "zakonem" } };

const char *defaultrank[CLAN_LEADER + 1] =
{ "Rekrut", "Szeregowy", "Starszy szeregowy", "Kadet", "Starszy Kadet",
		"Pomocnik", "M³odszy oficer", "Oficer", "Starszy oficer", "Weteran",
		"Dyplomata", "Drugi mo¿ny", "Pierwszy mo¿ny", "Lider" };

void write_clan_all()
{
	CLAN_DATA *clan;

	FOREACH( clan, first_clan )
		save_clan2(clan);
	save_clan_list();
}

/*
 * Get pointer to clan structure from clan name or id.
 */
CLAN_DATA* get_clan(char *name)
{
	CLAN_DATA *clan;

	for (clan = first_clan; clan; clan = clan->next)
		if (!str_cmp(name, clan->name)
				|| (is_number(name) && (clan->clan_id == atoi(name))))
			return clan;

	return NULL;
}

void write_clan_list()
{
	CLAN_DATA *tclan;
	FILE *fpout;

	RESERVE_CLOSE;
	if (!(fpout = fopen( CLAN_LIST, "w")))
	{
		bug("FATAL: cannot open clan.lst for writing!");
		perror( CLAN_LIST);
		return;
	}
	FOREACH( tclan, first_clan )
		fprintf(fpout, "%s\n", tclan->filename);
	fprintf(fpout, "$\n");
	fclose(fpout);
	RESERVE_OPEN;

	save_clan_list();
}

void save_clan_list()
{
	ILD *first_ild = NULL;
	ILD *last_ild = NULL;

	ILD_CREATE(CLAN_DATA, clan, filename, first_ild, last_ild);
	save_list( CLAN_LISTXML, first_ild);
	ILD_FREE(first_ild);
}

/*
 * Save a clan's data to its data file
 */
void save_clan(CLAN_DATA *clan)
{
	FILE *fp;
	MEMBER_DATA *member;
	POLITICS_DATA *politics;
	char filename[256];
	int i;

	IF_BUG(clan == NULL, "")
		return;

	save_clan2(clan);
	return;

	if (!*clan->filename)
	{
		bug("save_clan: %s has no filename - fixing", clan->name);
		STRDUP(clan->filename, mk_filename(clan->clan_id));
	}

	sprintf(filename, "%s%s", CLAN_DIR, clan->filename);

	RESERVE_CLOSE;
	if (!(fp = fopen(filename, "w")))
	{
		bug("fopen");
		perror(filename);
	}
	else
	{
		fprintf(fp, "#CLAN\n");
		fprintf(fp, "Name	   %s~\n", clan->name);
		fprintf(fp, "Filename	   %s~\n", clan->filename);
		fprintf(fp, "Id	   %d\n", clan->clan_id);
		fprintf(fp, "Type	   %d\n", clan->type);
		fprintf(fp, "Flags	   %lld\n", clan->flags);
		fprintf(fp, "Description  %s~\n", clan->description);
		fprintf(fp, "Penalty	   %ld\n", clan->penalty);
		fprintf(fp, "PKills	   %d\n", clan->pkills);
		fprintf(fp, "PDeaths	   %d\n", clan->pdeaths);
		fprintf(fp, "MKills	   %d\n", clan->mkills);
		fprintf(fp, "MDeaths	   %d\n", clan->mdeaths);
		fprintf(fp, "Members	   %d\n", clan->members);
		fprintf(fp, "Funds	   %ld\n", clan->funds);
		fprintf(fp, "Loan	   %ld\n", clan->loan);
		fprintf(fp, "Repay_date   %d\n", (int) clan->repay_date);
		/* Stopnie klanowe */
		if (*clan->rank[0])
		{
			fprintf(fp, "Ranks	  ");
			for (i = 0; i <= CLAN_LEADER; i++)
				fprintf(fp, " %s~", clan->rank[i]);
			fprintf(fp, "\n");
		}
		/* Thanos */
		FOREACH( member, clan->first_member )
			fprintf(fp, "Member       %s~ %d %s~\n", member->name,
					member->status, member->bestowments);
		/* Polityka */
		FOREACH( politics, clan->first_politics )
		{
			fprintf(fp, "Politics     %d %d\n", politics->clan->clan_id,
					politics->relations);
		}
		/* voting polityczny */
		if (clan->vClan)
		{
			fprintf(fp, "Vclan	   %d\n", clan->vClan->clan_id);
			fprintf(fp, "Vrelations   %d\n", clan->vRelations);
			fprintf(fp, "Vyes	   %s~\n", clan->vYes);
		}
		fprintf(fp, "End\n\n#END\n");
		fclose(fp);
	}
	RESERVE_OPEN;
	write_clan_list();
}

CLAN_DATA* load_clan2(const char *filename)
{
	xmlDocPtr doc;
	xmlNodePtr root;
	xmlNodePtr node;
	xmlNodePtr child;
	xmlNodePtr gchild;
	CLAN_DATA *clan;
	CLAN_DATA *pClan;
	MEMBER_DATA *member;
	POLITICS_DATA *politics;
	int i;
	bool found;

	swXmlInitIO();

	if (!(doc = xsd_validate("clan", SCHEMA_CLAN, CLAN_DIR, filename)))
		return NULL;
	/*! tutaj jestesmy juz PEWNI, ze plik klanu jest poprawny */

	clan = new_clan();
	STRDUP(clan->filename, filename);

	root = xmlDocGetRootElement(doc);

	FOREACH( node, root->children )
	{
		EONLY(node);

		if (!swXmlStrcmp(node->name, "name"))
			swGetContent(&clan->name, node);
		else if (!swXmlStrcmp(node->name, "id"))
			swGetContentInt(&clan->clan_id, node);
		else if (!swXmlStrcmp(node->name, "type"))
		{
			int clanType;
			swGetContentInt(&clanType, node);
			clan->type = (clan_type) clanType;
		}
		else if (!swXmlStrcmp(node->name, "flags"))
			swGetContentInt64(&clan->flags, node);
		else if (!swXmlStrcmp(node->name, "description"))
			swGetContent(&clan->description, node);
		else if (!swXmlStrcmp(node->name, "penalty"))
			swGetContentLong(&clan->penalty, node);
		else if (!swXmlStrcmp(node->name, "pkills"))
			swGetContentInt(&clan->pkills, node);
		else if (!swXmlStrcmp(node->name, "pdeaths"))
			swGetContentInt(&clan->pdeaths, node);
		else if (!swXmlStrcmp(node->name, "mkills"))
			swGetContentInt(&clan->mkills, node);
		else if (!swXmlStrcmp(node->name, "mdeaths"))
			swGetContentInt(&clan->mdeaths, node);
		else if (!swXmlStrcmp(node->name, "membersno"))
			swGetContentInt(&clan->members, node);
		else if (!swXmlStrcmp(node->name, "funds"))
			swGetContentLong(&clan->funds, node);
		else if (!swXmlStrcmp(node->name, "loan"))
			swGetContentLong(&clan->loan, node);
		else if (!swXmlStrcmp(node->name, "repaydate"))
			swGetContentInt((int*) &clan->repay_date, node);
		else if (!swXmlStrcmp(node->name, "ranks"))
		{
			i = 0;
			FOREACH( child, node->children )
			{
				EONLY(child);

				if (!swXmlStrcmp(child->name, "rank"))
					swGetContent(&clan->rank[i++], child);
			}
		}
		else if (!swXmlStrcmp(node->name, "members"))
		{
			FOREACH( child, node->children )
			{
				EONLY(child);

				if (!swXmlStrcmp(child->name, "member"))
				{
					member = new_member( CLAN_MEMBER);
					FOREACH( gchild, child->children )
					{
						EONLY(gchild);

						if (!swXmlStrcmp(gchild->name, "name"))
							swGetContent(&member->name, gchild);
						else if (!swXmlStrcmp(gchild->name, "status"))
							swGetContentInt(&member->status, gchild);
						else if (!swXmlStrcmp(gchild->name, "bestowments"))
							swGetContent(&member->bestowments, gchild);
					}
					LINK(member, clan->first_member, clan->last_member, next,
							prev);
				}
			}
		}
		else if (!swXmlStrcmp(node->name, "relations"))
		{
			FOREACH( child, node->children )
			{
				EONLY(child);

				if (!swXmlStrcmp(child->name, "relation"))
				{
					politics = new_politics();
					found = true;
					FOREACH( gchild, child->children )
					{
						EONLY(gchild);

						if (!swXmlStrcmp(gchild->name, "clan"))
						{
							int clan_id;
							swGetContentInt(&clan_id, gchild);
							if (!(pClan = get_clan( itoa(clan_id))))
							{
								free_politics(politics);
								found = false;
								break;
							}
							politics->clan = pClan;

						}
						else if (!swXmlStrcmp(gchild->name, "relations"))
							swGetContentInt(&politics->relations, gchild);
					}
					if (found)
						LINK(politics, clan->first_politics,
								clan->last_politics, next, prev);
				}
			}
		}

	}

	swXmlCleanIO();
	xmlFreeDoc(doc);
	swCleanupParser();
	adjust_only(clan);
	return clan;
}

void save_clan2(CLAN_DATA *clan)
{
	xmlDocPtr doc;
	xmlNodePtr root;
	xmlNodePtr node;
	xmlNodePtr child;
	MEMBER_DATA *member;
	POLITICS_DATA *politics;
	char buf[MSL];
	int i;

	if (!*clan->filename)
	{
		bug("save_clan: %s has no filename - fixing", clan->name);
		STRDUP(clan->filename, mk_filename(clan->clan_id));
	}

	swXmlInitIO();
	doc = xmlNewDoc( BC"1.0" );
	root = xmlNewNode( NULL, BC"clan" );
	xmlNewNs( root, BC"http://swmud.pl/ns/swmud/1.0/clan", NULL );
	xmlNewNs( root, BC"http://www.w3.org/2001/XMLSchema-instance", BC"xsi" );
	swNewProp(root, "xsi:schemaLocation",
			"http://swmud.pl/ns/swmud/1.0/clan clan.xsd");
	xmlDocSetRootElement(doc, root);

	swNewChildText(root, NULL, "name", clan->name);
	swNewChildInt(root, NULL, "id", clan->clan_id);
	swNewChildInt(root, NULL, "type", clan->type);
	swNewChildInt64(root, NULL, "flags", clan->flags);
	swNewChildText(root, NULL, "description", clan->description);
	swNewChildInt(root, NULL, "penalty", clan->penalty);
	swNewChildInt(root, NULL, "pkills", clan->pkills);
	swNewChildInt(root, NULL, "pdeaths", clan->pdeaths);
	swNewChildInt(root, NULL, "mkills", clan->mkills);
	swNewChildInt(root, NULL, "mdeaths", clan->mdeaths);
	swNewChildInt(root, NULL, "membersno", clan->members);
	swNewChildInt(root, NULL, "funds", clan->funds);
	swNewChildInt(root, NULL, "loan", clan->loan);
	swNewChildInt(root, NULL, "repaydate", clan->repay_date);
	node = xmlNewChild( root, NULL, BC"ranks", NULL );
	if (*clan->rank[0])
		for (i = 0; i <= CLAN_LEADER; i++)
			swNewChildText(node, NULL, "rank", clan->rank[i]);
	node = xmlNewChild( root, NULL, BC"members", NULL );
	FOREACH( member, clan->first_member )
	{
		child = xmlNewChild( node, NULL, BC"member", NULL );
		swNewChildText(child, NULL, "name", member->name);
		swNewChildInt(child, NULL, "status", member->status);
		swNewChildText(child, NULL, "bestowments", member->bestowments);
	}
	node = xmlNewChild( root, NULL, BC"relations", NULL );
	FOREACH( politics, clan->first_politics )
	{
		child = xmlNewChild( node, NULL, BC"relation", NULL );
		swNewChildInt(child, NULL, "clan", politics->clan->clan_id);
		swNewChildInt(child, NULL, "type", politics->relations);
	}
	if (clan->vClan)
	{
		node = xmlNewChild( root, NULL, BC"pvoting", NULL );
		swNewChildInt(node, NULL, "clan", clan->vClan->clan_id);
		swNewChildInt(node, NULL, "relations", clan->vRelations);
		swNewChildText(node, NULL, "yes", clan->vYes);
	}

	/* saving stuff */
//	sprintf( buf, "Saving %s.xml...", clan->filename );
//	log_string_plus( buf, LOG_NORMAL, LEVEL_GREATER );
	sprintf(buf, "%s%s", CLAN_DIR, clan->filename);

	RESERVE_CLOSE;
	xmlSaveFormatFileEnc(buf, doc, "ISO-8859-2", 1);
	RESERVE_OPEN;

	swXmlCleanIO();
	xmlFreeDoc(doc);
	swCleanupParser();
}

/*
 * Read in actual clan data.
 */

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = true;			\
				    break;				\
				}

void fread_clan(CLAN_DATA *clan, FILE *fp)
{
	char buf[MSL];
	char *word;
	bool fMatch;
	int i;

	for (;;)
	{
		word = feof(fp) ? (char*) "End" : fread_word(fp);
		fMatch = false;

		switch (UPPER(word[0]))
		{
		case '*':
			fMatch = true;
			fread_to_eol(fp);
			break;

		case 'D':
			SKEY("Description", clan->description, fread_string(fp))
			;
			break;

		case 'E':
			if (!str_cmp(word, "End"))
			{
				adjust_only(clan);
				return;
			}
			break;

		case 'F':
			KEY("Funds", clan->funds, fread_number(fp))
			;
			SKEY("Filename", clan->filename, fread_string(fp))
			;
			KEY("Flags", clan->flags, fread_number(fp))
			;
			break;

		case 'I':
			KEY("Id", clan->clan_id, fread_number(fp))
			;
			break;

		case 'L':
			KEY("Loan", clan->loan, fread_number(fp))
			;
			break;

		case 'M':
			KEY("MDeaths", clan->mdeaths, fread_number(fp))
			;
			KEY("Members", clan->members, fread_number(fp))
			;
			KEY("MKills", clan->mkills, fread_number(fp))
			;
			SKEY("MainClan", clan->tmpstr, fread_string(fp))
			;
			/* Thanos */
			if (!str_cmp(word, "Member"))
			{
				MEMBER_DATA *member;

				member = new_member( CLAN_MEMBER);
				STRDUP(member->name, st_fread_string(fp));
				member->status = fread_number(fp);
				STRDUP(member->bestowments, st_fread_string(fp));
				LINK(member, clan->first_member, clan->last_member, next, prev);
				fread_to_eol(fp);
				fMatch = true;
			}
			break;

		case 'N':
			SKEY("Name", clan->name, fread_string(fp))
			;
			break;

		case 'P':
			KEY("PDeaths", clan->pdeaths, fread_number(fp))
			;
			KEY("PKills", clan->pkills, fread_number(fp))
			;
			KEY("Penalty", clan->penalty, fread_number(fp))
			;
			if (!str_cmp(word, "Politics"))
			{
				fMatch = true;
				fread_to_eol(fp);
			}
			break;

		case 'R':
			if (!str_cmp(word, "Ranks"))
			{
				for (i = 0; i <= CLAN_LEADER; i++)
					STRDUP(clan->rank[i], st_fread_string(fp));
				fMatch = true;
			}
			KEY("Repay_date", clan->repay_date, fread_number(fp))
			;
			break;

		case 'T':
			KEY("Type", clan->type, (clan_type )fread_number(fp))
			;
			break;

		case 'V':
			if (!str_cmp(word, "Vclan") || !str_cmp(word, "Vrelations")
					|| !str_cmp(word, "Vyes"))
			{
				fMatch = true;
				fread_to_eol(fp);
			}
			break;
		}

		if (!fMatch)
		{
			sprintf(buf, "fread_clan: no match: %s", word);
			log_string(buf);
		}
	}
}

/*
 * Load a clan file
 */

bool load_clan_file(char *clanfile)
{
	CLAN_DATA *clan;
	FILE *fp;
	char filename[MFL];
	bool found = false;

	clan = new_clan();

	sprintf(filename, "%s%s", CLAN_DIR, clanfile);

	if ((fp = fopen(filename, "r")) != NULL)
	{

		found = true;
		for (;;)
		{
			char letter;
			char *word;

			letter = fread_letter(fp);
			if (letter == '*')
			{
				fread_to_eol(fp);
				continue;
			}

			if (letter != '#')
			{
				bug("# not found.");
				break;
			}

			word = fread_word(fp);
			if (!str_cmp(word, "CLAN"))
			{
				fread_clan(clan, fp);
				break;
			}
			else if (!str_cmp(word, "END"))
				break;
			else
			{
				bug("bad section: %s.", word);
				break;
			}
		}
		fclose(fp);
	}
	else
	{
		perror(filename);
	}

	if (found)
	{
		LINK(clan, first_clan, last_clan, next, prev);

		/*
		 sprintf( filename, "%s%s.vault", CLAN_DIR, clan->filename );
		 if ( ( fp = fopen( filename, "r" ) ) != NULL )
		 {
		 int iNest;
		 bool found;
		 OBJ_DATA *tobj, *tobj_next;

		 if( !sysdata.silent )
		 log_string( "Loading clan storage room" );

		 rset_supermob(storeroom);
		 for ( iNest = 0; iNest < MAX_NEST; iNest++ )
		 rgObjNest[iNest] = NULL;

		 found = true;
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
		 bug( "# not found.(%s)", clan->name );
		 break;
		 }

		 word = fread_word( fp );
		 if ( !str_cmp( word, "OBJECT" ) )
		 fread_obj  ( supermob, fp, OS_CARRY );
		 else
		 if ( !str_cmp( word, "END"    ) )
		 break;
		 else
		 {
		 bug( "Load_clan_vault: bad section. (%s)", clan->name );
		 break;
		 }
		 }
		 fclose( fp );
		 for ( tobj = supermob->first_carrying; tobj; tobj = tobj_next )
		 {
		 tobj_next = tobj->next_content;
		 obj_from_char	( tobj );
		 obj_to_room	( tobj, storeroom );
		 }
		 release_supermob();
		 }
		 else
		 {
		 if( !sysdata.silent )
		 log_string( "Cannot open clan vault" );
		 }
		 */
	}
	else
		free_clan(clan);

	return found;
}

/*
 * Load in all the clan files.
 */
/*
 void load_clans( )
 {
 FILE *	fpList;
 char *	filename;
 CLAN_DATA *clan;

 first_clan	= NULL;
 last_clan	= NULL;

 RESERVE_CLOSE;
 if ( !(fpList = fopen( CLAN_LIST, "r" )) )
 {
 perror( CLAN_LIST );
 exit( 1 );
 }

 for ( ; ; )
 {
 filename = feof( fpList ) ? "$" : fread_word( fpList );
 if( !sysdata.silent )
 log_string( filename );
 if ( filename[0] == '$' )
 break;

 if ( !load_clan_file( filename ) )
 {
 perror( filename );
 bug( "Cannot load clan file: %s", filename );
 }
 }
 fclose( fpList );
 if( !sysdata.silent )
 log_string(" Done clans" );

 log_string( "Loading politics" );
 for ( clan=first_clan ; clan ; clan = clan->next )
 fread_politics( clan, clan->filename );
 if( !sysdata.silent )
 log_string(" Done politics" );

 RESERVE_OPEN;

 return;
 }

 void fread_politics( CLAN_DATA *clan, char *filename )
 {
 char		buf	[MAX_STRING_LENGTH];
 char *		word;
 POLITICS_DATA	*politics;
 FILE		*fp;
 bool		fMatch;

 sprintf( buf, "%s%s", CLAN_DIR, filename );
 if( !(fp = fopen( buf, "r" )) )
 return;

 for ( ; ; )
 {
 word   = feof( fp ) ? "End" : fread_word( fp );

 switch ( UPPER(word[0]) )
 {
 case '*':
 fread_to_eol( fp );
 break;

 case 'E':
 if ( !str_cmp( word, "End" ) )
 {
 fclose( fp );
 return;
 }
 break;
 case 'P':
 if( !str_cmp( word, "Politics" ) )
 {
 CREATE( politics, POLITICS_DATA, 1 );
 LINK( politics, clan->first_politics, clan->last_politics, next, prev );
 politics->clan		= get_clan( itoa(fread_number( fp )) );
 politics->relations	= fread_number( fp );
 }
 break;

 case 'V':
 if( !str_cmp( word, "Vclan" ) )
 clan->vClan = get_clan( itoa( fread_number( fp ) ) );
 KEY( "Vrelations",	clan->vRelations,	fread_number( fp ) );
 SKEY( "Vyes",	clan->vYes,		fread_string( fp ) );
 break;
 }
 }

 return;
 }
 */
//byTrog - tutaj zaczyna sie crank czyli stopien w klanie ->...
/*
 void crank_set( CHAR_DATA *ch, CHAR_DATA *victim )
 {
 if( !IS_LEADER( ch ) )
 {
 send_to_char( "Nie jeste¶ osob± upowa¿nion±." NL, ch );
 return;
 }

 if( ch == victim )
 {
 send_to_char( "Sobie nie mo¿esz!" NL, ch );
 return;
 }

 if( victim->crank || IS_FIRST( victim ) || IS_SECOND( victim ) )
 {
 ch_printf( ch, "%s ma ju¿ nadany stopieñ." NL,
 victim->przypadki[0] );
 return;
 }

 victim->crank = ch->pcdata->clan->first_crank;
 ch_printf( ch, "Nadajesz " FB_WHITE "%s" PLAIN " pierwszy stopieñ w %s." NL,
 victim->przypadki[2], CLANTYPE( ch->pcdata->clan, 5 ) );
 ch_printf( victim, "%s nadaje Ci pierwszy stopieñ w %s." NL,
 ch->przypadki[0], CLANTYPE( ch->pcdata->clan, 5 ) );
 save_char_obj( victim );
 return;
 }

 void crank_rem( CHAR_DATA *ch, CHAR_DATA *victim )
 {
 if( !IS_LEADER( ch ) )
 {
 send_to_char( "Nie jeste¶ osob± upowa¿nion±." NL, ch );
 return;
 }

 if( ch == victim )
 {
 send_to_char( "Chcesz siê podaæ do dymisji?!" NL, ch );
 return;
 }

 if( victim->crank )
 {
 victim->crank = NULL;
 ch_printf( ch, "Usuwasz " FB_WHITE "%s" PLAIN " stopieñ w %s." NL,
 victim->przypadki[2], CLANTYPE( ch->pcdata->clan, 5 ) );
 ch_printf( victim, "%s usuwa Ci stopieñ w %s." NL,
 victim->przypadki[2], CLANTYPE( ch->pcdata->clan, 5 ) );
 save_char_obj( victim );
 }
 else
 ch_printf( ch, "%s nie posiada ¿adnego stopnia." NL,
 victim->przypadki[0] );
 return;
 }
 */

DEF_DO_FUN( crank )
{
	/*
	 CHAR_DATA	*victim;
	 CLAN_DATA	*clan;
	 CRANK_DATA	*crank;
	 char	arg [MIL];

	 argument = one_argument( argument, arg  );

	 if( IS_NPC( ch ) || !(clan = ch->pcdata->clan) )
	 {
	 send_to_char("Nie nale¿ysz do ¿adnej organizacji!" NL, ch );
	 return;
	 }
	 if( !IS_LEADER( ch ) )
	 {
	 send_to_char( "Nie jeste¶ osob± upowa¿nion±!" NL, ch );
	 return;
	 }

	 if( *arg=='\0' )
	 {
	 ch_printf( ch, "Zarz±dzanie stopniem %s:" NL
	 "Sk³adnia: crank <akcja> [imiê]" NL
	 NL "Akcje:\n\r set inc dec rem show." NL,
	 CLANTYPE( ch->pcdata->clan, 1 ) );
	 return;
	 }

	 if( !str_cmp( arg, "show" ) )
	 {
	 int i = 1;
	 send_to_char( "Lista stopni:" NL NL, ch );
	 for( crank = clan->first_crank; crank; crank = crank->next )
	 ch_printf( ch, " %2d: [%-15s]." NL, i++, crank->name );
	 return;
	 }

	 if( !str_cmp( arg, "set" ) )
	 {
	 if( !(victim = get_char_room( ch, argument )) || IS_NPC( victim ) )
	 {
	 send_to_char( "Nie ma nikogo takiego." NL, ch );
	 return;
	 }
	 if( !victim->pcdata->clan
	 || ch->pcdata->clan != victim->pcdata->clan )
	 {
	 ch_printf( ch, "Ta osoba nie nale¿y do Twoje%s %s." NL,
	 CLANSUFFIX( ch->pcdata->clan, "go", "j" ), CLANTYPE( ch->pcdata->clan, 1 ) );
	 return;
	 }

	 crank_set( ch, victim );
	 return;
	 }

	 if( !str_cmp( arg, "rem" ) )
	 {
	 if( !(victim = get_char_room( ch, argument )) || IS_NPC( victim ) )
	 {
	 send_to_char( "Nie ma nikogo takiego." NL, ch );
	 return;
	 }
	 if( !victim->pcdata->clan
	 || ch->pcdata->clan != victim->pcdata->clan )
	 {
	 ch_printf( ch, "Ta osoba nie nale¿y do Twoje%s %s." NL,
	 CLANSUFFIX( ch->pcdata->clan, "go", "j" ), CLANTYPE( ch->pcdata->clan, 1 ) );
	 return;
	 }

	 crank_rem( ch, victim );
	 return;
	 }

	 if( !str_prefix( arg, "increase" ) )
	 {
	 if( !(victim = get_char_room( ch, argument )) || IS_NPC( victim ) )
	 {
	 send_to_char( "Nie ma nikogo takiego." NL, ch );
	 return;
	 }
	 if( !victim->pcdata->clan
	 || clan != victim->pcdata->clan )
	 {
	 ch_printf( ch, "Ta osoba nie nale¿y do Twoje%s %s." NL,
	 CLANSUFFIX( clan, "go", "j" ), CLANTYPE( clan, 1 ) );
	 return;
	 }
	 if( victim->crank == clan->last_crank || !victim->crank->next )
	 {
	 ch_printf( ch, FB_WHITE "%s" PLAIN " posiada najwy¿szy mo¿liwy stopieñ." NL,
	 victim->przypadki[0] );
	 return;
	 }

	 victim->crank = victim->crank->next;
	 ch_printf( ch, "Zwiêkszasz " FB_WHITE "%s" PLAIN " stopieñ w swo%s %s." NL,
	 victim->przypadki[2],
	 CLANSUFFIX( clan, "im", "jej" ), CLANTYPE( clan, 4 ) );
	 ch_printf( victim, "%s zwiêksza Ci stopieñ w swo%s %s." NL,
	 ch->przypadki[0],
	 CLANSUFFIX( clan, "im", "jej" ), CLANTYPE( clan, 4 ) );

	 return;
	 }

	 if( !str_prefix( arg, "decrease" ) )
	 {
	 if( !(victim = get_char_world( ch, argument )) || IS_NPC( victim ) )
	 {
	 send_to_char( "Nie ma nikogo takiego." NL, ch );
	 return;
	 }
	 if( !victim->pcdata->clan
	 || clan != victim->pcdata->clan )
	 {
	 ch_printf( ch, "Ta osoba nie nale¿y do Twoje%s %s." NL,
	 IS_ORDER( clan ) ? "go" : "j", CLANTYPE( clan, 1 ) );
	 return;
	 }
	 if( victim->crank == clan->first_crank || !victim->crank->prev )
	 {
	 ch_printf( ch, FB_WHITE "%s" PLAIN " posiada najni¿szy mo¿liwy stopieñ." NL,
	 victim->przypadki[0] );
	 return;
	 }

	 victim->crank = victim->crank->prev;
	 ch_printf( ch, "Zmniejszasz " FB_WHITE "%s" PLAIN " stopieñ w swo%s %s." NL,
	 victim->przypadki[2],
	 CLANSUFFIX( clan, "im", "jej" ), CLANTYPE( clan, 4 ) );
	 ch_printf( victim, "%s zmniejsza Ci stopieñ w swo%s %s." NL,
	 ch->przypadki[0],
	 CLANSUFFIX( clan, "im", "jej" ), CLANTYPE( clan, 4 ) );

	 return;
	 }

	 do_crank( ch, "" );
	 return;
	 */
	huh(ch);
}

//...<- a tutaj mamy koniec cranka. byTrog

MEMBER_DATA* get_member(CLAN_DATA *clan, char *name)
{
	MEMBER_DATA *member;

	FOREACH( member, clan->first_member )
		if (!str_cmp(member->name, name))
			return member;
	return NULL;
}

MEMBER_DATA* highest_ranked(CLAN_DATA *clan, int max_rank)
{
	MEMBER_DATA *member;
	MEMBER_DATA *rmember = NULL;
	int rank = CLAN_WAITING;

	FOREACH( member, clan->first_member )
		if (member->status <= max_rank && member->status > rank)
		{
			rank = member->status;
			rmember = member;
		}

	return rmember;
}

void add_member(CLAN_DATA *clan, char *name, int status)
{
	MEMBER_DATA *member;
	MEMBER_DATA *smember;

	if ((smember = get_member(clan, name)) && (smember->status != CLAN_WAITING))
	{
		bug("%s is already a member of %s", name, clan->name);
		return;
	}

	if (status != CLAN_WAITING)
		clan->members++;

	if (smember && smember->status == CLAN_WAITING)
		smember->status = status;
	else
	{

		member = new_member(status);
		STRDUP(member->name, name);
		LINK(member, clan->first_member, clan->last_member, next, prev);
	}

	adjust_clan(clan);
}

void remove_member(CLAN_DATA *clan, char *name)
{
	MEMBER_DATA *member;

	if ((member = get_member(clan, name)))
	{
		switch (member->status)
		{
		case CLAN_LEADER:
			clan->leader = NULL;
			break;
		case CLAN_FIRST:
			clan->first = NULL;
			break;
		case CLAN_SECOND:
			clan->second = NULL;
			break;
		case CLAN_DIPLOMAT:
			clan->diplomat = NULL;
			break;
		case CLAN_WAITING:
			clan->members++;
			break;
		}

		if (clan->members > 0)
			clan->members--;
		UNLINK(member, clan->first_member, clan->last_member, next, prev);
		free_member(member);
		adjust_clan(clan);
	}
	/*
	 else
	 bug( "%s - no such member in %s", name, clan->name );
	 */
}

/* usunac ta funkcje i tresc przeniesc do miejsca wywolania
 jesli okaze sie, ze funkcja ta wywolywana jest tylko raz!!!
 */
void rename_member(const char *name, const char *newname)
{
	CLAN_DATA *clan;
	MEMBER_DATA *member;

	FOREACH( clan, first_clan )
		FOREACH( member, clan->first_member )
			if (!str_cmp(member->name, name))
				STRDUP(member->name, newname);
}

void induct_member(CLAN_DATA *clan, CHAR_DATA *ch)
{
	ch->pcdata->clan = clan; /* ? */
	save_char_obj(ch);
	add_member(clan, ch->name, CLAN_MEMBER);

	return;
}

void outcast_member(CLAN_DATA *clan, CHAR_DATA *ch)
{
	CLAN_DATA *pClan;
	MEMBER_DATA *member;
	bool found = false;

	if (ch->pcdata->clan == clan)
		ch->pcdata->clan = NULL;

	FOREACH( pClan, first_clan )
		if (pClan
				!= clan&& (member = get_member( pClan, ch->name )) && member->status > CLAN_WAITING)
		{
			if (!ch->pcdata->clan)
			{
				found = true;
				ch->pcdata->clan = pClan;
			}

			if (!found)
			{
//				if( ch->speaking & LANG_CLAN )
//					ch->speaking = LANG_COMMON;
//				REMOVE_BIT( ch->speaks, LANG_CLAN );
			}
			break;
		}

	remove_member(clan, ch->name);
	save_char_obj(ch);
	return;
}

void adjust_only(CLAN_DATA *clan)
{
	if (!clan->leader || clan->leader->status != CLAN_LEADER)
		if ((clan->leader = highest_ranked(clan, CLAN_LEADER)))
			clan->leader->status = CLAN_LEADER;

	if (!clan->first || clan->first->status != CLAN_FIRST)
		if ((clan->first = highest_ranked(clan, CLAN_FIRST)))
			clan->first->status = CLAN_FIRST;

	if (!clan->second || clan->second->status != CLAN_SECOND)
		if ((clan->second = highest_ranked(clan, CLAN_SECOND)))
			clan->second->status = CLAN_SECOND;

	if (clan->diplomat && clan->diplomat->status != CLAN_DIPLOMAT)
		clan->diplomat = NULL;
}

void adjust_clan(CLAN_DATA *clan)
{
	MEMBER_DATA *member;

	FOREACH( member, clan->first_member )
		switch (member->status)
		{
		case CLAN_LEADER:
			if (member != clan->leader)
				member->status = CLAN_MEMBER;
			break;
		case CLAN_FIRST:
			if (member != clan->first)
				member->status = CLAN_MEMBER;
			break;
		case CLAN_SECOND:
			if (member != clan->second)
				member->status = CLAN_MEMBER;
			break;
		case CLAN_DIPLOMAT:
			if (member != clan->diplomat)
				member->status = CLAN_MEMBER;
			break;
		}

	adjust_only(clan);
	save_clan(clan);
}

void application(CHAR_DATA *ch, char *argument)
{
	CLAN_DATA *clan;
	MEMBER_DATA *member;

	if (IS_NPC(ch))
	{
		huh(ch);
		return;
	}

	if (!*argument)
	{
		send_to_char("Gdzie chcesz z³o¿yæ podanie o przyjêcie?" NL, ch);
		return;
	}

	if (!(clan = get_clan(argument)))
	{
		send_to_char("Nie ma takiej organizacji, gildii ani zakonu." NL, ch);
		return;
	}
	else /* Jesli to jest tajny klan, to czlonek z induct musi stac obok nas. */
	if (IS_SET(clan->flags, CLAN_NOINFO))
	{
		CHAR_DATA *victim;
		bool found = false;

		for (victim = ch->in_room->first_person; victim && !found; victim =
				victim->next_in_room)
			if (!IS_NPC(victim) && (member = get_member( clan, victim->name ))
			&& MEMBER_BESTOW( member, "induct" ))
				found = true;

		if (!found)
		{
			send_to_char("Nie ma takiej organizacji, gildii ani zakonu." NL,
					ch);
			return;
		}
	}

	if ((member = get_member(clan, ch->name)) && member->status == CLAN_WAITING)
	{
		send_to_char("Podanie o przyjêcie zosta³o ju¿ z³o¿one wcze¶niej." NL,
				ch);
		return;
	}

	if (member)
	{
		ch_printf(ch, "Jeste¶ ju¿ cz³onki%s te%s %s." NL,
				MALE( ch ) ? "em" : "ni±", CLANSUFFIX(clan, "go", "j"),
				CLANTYPE(clan, 1));
		return;
	}

	act( COL_FORCE, "Sk³adasz podanie o przyjêcie do $t.", ch, clan->name, NULL,
			TO_CHAR);
	add_member(clan, ch->name, CLAN_WAITING);
	return;
}

DEF_DO_FUN( induct )
{
	char arg[MIL];
	CHAR_DATA *victim;
	CLAN_DATA *clan = CLAN(ch);
	MEMBER_DATA *member;
	int i = 0;

	if (!clan)
	{
		huh(ch);
		return;
	}

	if (!clan_bestow(ch, "induct"))
	{
		send_to_char("Nie masz odpowiednich uprawnieñ." NL, ch);
		return;
	}

	argument = one_argument(argument, arg);

	if (arg[0] == '\0')
	{
		ch_printf(ch,
				"Oto lista osób ubiegaj±cych siê o wst±pienie do twoje%s %s:" NL,
				CLANSUFFIX(clan, "go", "j"), CLANTYPE(clan, 1));
		FOREACH( member, clan->first_member )
			if (member->status == CLAN_WAITING)
			{
				if ((i % 3) && i > 1)
					send_to_char( NL, ch);
				ch_printf(ch, "%-15s", capitalize(member->name));
				i++;
			}
		if (i < 1)
			ch_printf(ch, "Nikt nie ubiega siê o wst±pienie do twoje%s %s." NL,
					CLANSUFFIX(clan, "go", "j"), CLANTYPE(clan, 1));
		else
			send_to_char( NL, ch);

		return;
	}

	if (!(victim = get_char_room(ch, arg)))
	{
		send_to_char("Nie ma tutaj nikogo takiego." NL, ch);
		return;
	}

	if (IS_NPC(victim))
	{
		ch_printf(ch, "Tej osoby nie mo¿esz przyj±æ do %s." NL,
				CLANTYPE(clan, 1));
		return;
	}

	if (!(member = get_member(clan, victim->name)))
	{
		ch_printf(ch,
				"Nikt taki nie zg³asza³ chêci wst±pienia do twoje%s %s." NL,
				CLANSUFFIX(clan, "go", "j"), CLANTYPE(clan, 1));
		return;
	}

	if (member->status != CLAN_WAITING)
	{
		ch_printf(ch, "Ta osoba jest ju¿ cz³onkiem twoje%s %s." NL,
				CLANSUFFIX(clan, "go", "j"), CLANTYPE(clan, 1));
		return;
	}

	if (!*argument)
	{
		send_to_char("Chcesz zaakceptowaæ podanie czy je odrzuciæ?" NL, ch);
		return;
	}

	if (!str_cmp(argument, "accept") || !str_cmp(argument, "tak"))
	{
		if (IS_IMMORTAL(victim) /* || ... */)
			send_to_char("Podanie tej osoby mo¿esz tylko odrzuciæ." NL, ch);
		else
		{
			induct_member(clan, victim);
			act( COL_FORCE, "Przyjmujesz $N$3 do $t.", ch, clan->name, victim,
					TO_CHAR);
			act( COL_FORCE, "$n przyjmuje $N$3 do $t.", ch, clan->name, victim,
					TO_NOTVICT);
			act( COL_FORCE, "$n przyjmuje Ciê do $t.", ch, clan->name, victim,
					TO_VICT);
		}
		return;
	}

	if (!str_cmp(argument, "deny") || !str_cmp(argument, "nie"))
	{
		remove_member(clan, victim->name);
		act( COL_FORCE, "Odrzucasz podanie $N$1 o przyjêcie do $t.", ch,
				clan->name, victim, TO_CHAR);
		act( COL_FORCE, "$n odrzuca podanie $N$1 o przyjêcie do $t.", ch,
				clan->name, victim, TO_NOTVICT);
		act( COL_FORCE, "$n odrzuca twoje podanie o przyjêcie do $t.", ch,
				clan->name, victim, TO_VICT);
		return;
	}

	do_induct(ch, (char*) "");
	return;
}

DEF_DO_FUN( outcast )
{
	char arg[MIL];
	CHAR_DATA *victim;
	CLAN_DATA *clan = CLAN(ch);
	MEMBER_DATA *member;
	MEMBER_DATA *vmember;
	RID *location;

	if (!clan)
	{
		huh(ch);
		return;
	}

	if (!(member = get_member(clan, ch->name)) || !clan_bestow(ch, "outcast"))
	{
		send_to_char("Nie masz odpowiednich uprawnieñ." NL, ch);
		return;
	}

	argument = one_argument(argument, arg);

	if (!*arg)
	{
		ch_printf(ch, "Kogo chcesz usun±æ z %s?" NL, CLANTYPE(clan, 1));
		return;
	}

	if (!str_cmp(ch->name, arg))
	{
		ch_printf(ch, "Chcesz usunac siebie z wlasne%s %s?" NL,
				CLANSUFFIX(clan, "go", "j"), CLANTYPE(clan, 1));
		return;
	}

	if (!(vmember = get_member(clan, arg)))
	{
		ch_printf(ch, "Tw%s %s nie posiada takiego cz³onka jak %s." NL,
				CLANSUFFIX(clan, "ój", "oja"), CLANTYPE(clan, 1),
				capitalize(arg));
		return;
	}

	if ((victim = get_player_world(ch, arg)))
	{
		//if( member->status > vmember->status )
		//{
		location = ch->in_room;
		char_from_room(ch);
		char_to_room(ch, victim->in_room);
		outcast_member(clan, victim);
		act( COL_FORCE, "Usuwasz $N$3 z $t." NL, ch, clan->name, victim,
				TO_CHAR);
		act( COL_FORCE, "$n usuwa $N$3 z $t." NL, ch, clan->name, victim,
				TO_NOTVICT);
		act( COL_FORCE, "$n usuwa Ciê z $t." NL, ch, clan->name, victim,
				TO_VICT);
		char_from_room(ch);
		char_to_room(ch, location);
		/*}
		 else
		 {
		 send_to_char( "Nie mo¿esz usun±æ kogo¶ wy¿szego stopniem." NL, ch );
		 ch_printf( victim, "W³a¶nie dosz³a do ciebie informacja, ¿e %s probowa³%s usun±æ cie z" NL
		 "%s %s." NL, PERS( ch, victim, 0 ), SEX_SUFFIX__AO( ch ), CLANTYPE( clan, 1 ), clan->name );
		 } Pixel tu byl */
	}
	else
	{
		ch_printf(ch, "%s nie nalezy juz do %s %s" NL, vmember->name,
				CLANTYPE(clan, 1), clan->name);
		remove_member(clan, arg);
	}
}

DEF_DO_FUN( setclan )
{
	char arg1[MIL];
	char arg2[MIL];
	CLAN_DATA *clan;
	MEMBER_DATA *member;

	if (IS_NPC(ch))
	{
		huh(ch);
		return;
	}

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);

	if (!*arg1 || !*arg2)
	{
		send_to_char("Usage: setclan <clan> <field> <value> [value] ..." NL,
				ch);
		send_to_char( NL "Field being one of:" NL, ch);
		send_to_char(" flags leader first second diplomat" NL, ch);
		send_to_char(" suborg penalty members rank" NL, ch);
		send_to_char(" funds addmember rmmember" NL, ch);
		send_to_char(" name filename desc fold" NL, ch);
		return;
	}

	if (!(clan = get_clan(arg1)))
	{
		send_to_char("No such clan." NL, ch);
		return;
	}

	if (!str_cmp(arg2, "leader"))
	{
		if ((member = get_member(clan, argument)))
		{
			clan->leader = member;
			member->status = CLAN_LEADER;
			adjust_clan(clan);
			donemsg(ch);
		}
		else
			send_to_char("This player doesn't belong to that clan." NL, ch);

		return;
	}

	if (!str_cmp(arg2, "flags"))
	{
		int v = flag_value(clan_flags_list, argument);

		if (v < 0)
		{
			send_to_char("No such flag." NL, ch);
			return;
		}
		else
		{
			TOGGLE_BIT(clan->flags, v);
			donemsg(ch);
		}

		return;
	}

	if (!str_cmp(arg2, "penalty"))
	{
		if (!IS_SET(clan->flags, CLAN_PENALTYQUIT))
			send_to_char("Clan doesn't have penalty_quit flag." NL, ch);
		else
		{
			clan->penalty = atoi(argument);
			save_clan(clan);
			donemsg(ch);
		}

		return;
	}

	if (!str_cmp(arg2, "suborg"))
	{
		/*
		 CLAN_DATA *suborg;
		 suborg = get_clan( argument );
		 */
		send_to_char(
				"Chwilowo nie dzialaja organizacje podporz±dkowane (suborg)." NL,
				ch);
		return;
	}

	if (!str_cmp(arg2, "first"))
	{
		if ((member = get_member(clan, argument)))
		{
			clan->first = member;
			member->status = CLAN_FIRST;
			adjust_clan(clan);
			donemsg(ch);
		}
		else
			send_to_char("This player doesn't belong to that clan." NL, ch);

		return;
	}

	if (!str_cmp(arg2, "second"))
	{
		if ((member = get_member(clan, argument)))
		{
			clan->second = member;
			member->status = CLAN_SECOND;
			adjust_clan(clan);
			donemsg(ch);
		}
		else
			send_to_char("This player doesn't belong to that clan." NL, ch);

		return;
	}

	if (!str_cmp(arg2, "diplomat"))
	{
		if ((member = get_member(clan, argument)))
		{
			clan->diplomat = member;
			member->status = CLAN_DIPLOMAT;
			adjust_clan(clan);
			donemsg(ch);
		}
		else
			send_to_char("This player doesn't belong to that clan." NL, ch);

		return;
	}

	if (!str_cmp(arg2, "members"))
	{
		clan->members = atoi(argument);
		save_clan(clan);
		donemsg(ch);

		return;
	}

	if (!str_cmp(arg2, "addmember"))
	{
		add_member(clan, capitalize(argument), CLAN_MEMBER);
		donemsg(ch);

		return;
	}

	if (!str_cmp(arg2, "rmmember"))
	{
		CHAR_DATA *victim;

		if ((victim = get_player_world(ch, argument)) && victim->pcdata->clan
				&& victim->pcdata->clan == clan)
		{
			victim->pcdata->clan = NULL;
			save_char_obj(victim);
		}
		remove_member(clan, argument);
		donemsg(ch);

		return;
	}

	if (!str_cmp(arg2, "funds"))
	{
		clan->funds = atoi(argument);
		save_clan(clan);
		donemsg(ch);

		return;
	}

	if (!str_cmp(arg2, "loan"))
	{
		struct tm *tms;

		clan->loan = atoi(argument);
		tms = localtime(&current_time);
		tms->tm_mday += CLAN_LOANTIME(clan);
		clan->repay_date = mktime(tms);
		save_clan(clan);
		donemsg(ch);

		return;
	}

	if (!str_cmp(arg2, "rank"))
	{
		char arg3[MIL];
		int val;

		argument = one_argument(argument, arg3);

		if (!*arg3 || (val = atoi(arg3)) < CLAN_MEMBER || val > CLAN_LEADER
				|| !*argument || strlen(argument) > 16)
		{
			ch_printf(ch,
					"Syntax: rank <nr> <rank>" NL NL "note: %d <= nr <= %d" NL
					"      0 < strlen(rank) <= 16" NL, CLAN_MEMBER,
					CLAN_LEADER);
			return;
		}

		STRDUP(clan->rank[val], argument);
		save_clan(clan);
		donemsg(ch);

		return;
	}

	/* zrobic to na flag_value */
	if (!str_cmp(arg2, "type"))
	{
		if (!str_cmp(argument, "organization")
				|| !str_cmp(argument, "organizacja"))
			clan->type = CLAN_ORGANIZATION;
		else if (!str_cmp(argument, "guild") || !str_cmp(argument, "gildia"))
			clan->type = CLAN_GUILD;
		else if (!str_cmp(argument, "order") || !str_cmp(argument, "zakon"))
			clan->type = CLAN_ORDER;
		else if (!str_cmp(argument, "suborg"))
			clan->type = CLAN_SUBORG;
		else
		{
			int t;
			send_to_char("Available clan types:", ch);
			for (t = 0; t < CLAN_MAXTYPE; t++)
				if (t != CLAN_SUBORG)
					ch_printf(ch, " %s", clantypename[t][0]);
			send_to_char(" suborg." NL, ch);
			return;
		}

		save_clan(clan);
		donemsg(ch);
		return;
	}

	if (!str_cmp(arg2, "name"))
	{
		STRDUP(clan->name, argument);
		save_clan(clan);
		donemsg(ch);
		return;
	}

	if (!str_cmp(arg2, "filename"))
	{
		STRDUP(clan->filename, argument);
		save_clan(clan);
		write_clan_list();
		donemsg(ch);
		return;
	}

	if (!str_cmp(arg2, "desc"))
	{
		string_append(ch, &clan->description);
		return;
	}

	if (!str_cmp(arg2, "fold"))
	{
		save_clan(clan);
		send_to_char("Ok, but needed only after setting desc." NL, ch);
		return;
	}

	do_setclan(ch, (char*) "");
	return;
}

//polityka i glosowanie.
DEF_DO_FUN( relations )
{
	POLITICS_DATA *politics;
	CLAN_DATA *clan = CLAN(ch);
	CLAN_DATA *sclan;
	CLAN_DATA *tclan;
	CLAN_DATA *vclan;
	MEMBER_DATA *member;
	char arg1[MIL];
	char arg2[MIL];
	char *buf;
	char *buf2;
	char vyesbuf[MSL];
	char namebuf[MSL];
	bool pnone;
	bool match;
	int vrelations;
	int value;
	int numvotes;

	if (!clan)
	{
		huh(ch);
		return;
	}

	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
	value = atoi(arg2);

	if (!IS_ORG(clan))
	{
		send_to_char(
				"Tylko organizacje niezale¿ne mog± zajmowaæ siê polityk±." NL,
				ch);
		return;
	}

	if (*arg1 == '\0')
	{
		ch_printf(ch,
				FB_CYAN "Oto jak przedstawia siê polityka prowadzona przez twoj± organizacjê:" EOL NL);
		if (clan->diplomat)
			ch_printf(ch, FB_CYAN "Dyplomata: " PLAIN "%s" FB_CYAN "." EOL NL,
					clan->diplomat->name);

		FOREACH( politics, clan->first_politics )
			ch_printf(ch,
					"Twoja organizacja prowadzi %s politykê w stosunku do: " FG_CYAN "%s" PLAIN "." NL,
					bit_name(relation_types_list, politics->relations),
					CLANNAME(politics->clan));

		if (!(member = get_member(clan, ch->name))
				|| member->status < CLAN_DIPLOMAT)
			return;

		send_to_char( NL, ch);

		FOREACH( sclan, first_clan )
		{
			if (sclan == clan)
				continue;

			pnone = true;
			FOREACH( politics, sclan->first_politics )
				if (politics->clan == clan)
				{
					ch_printf(ch,
							"Organizacja " FG_CYAN "%s" PLAIN " prowadzi %s politykê z twoj± organizacj±." NL,
							CLANNAME(sclan),
							bit_name(relation_types_list, politics->relations));
					pnone = false;
				}

			if (pnone)
				ch_printf(ch,
						"Organizacja " FG_CYAN "%s" PLAIN " nie prowadzi ¿adnej polityki z twoj± organizacj±." NL,
						CLANNAME(sclan));
		}

		return;
	}

	if (!str_cmp(arg1, "change") || !str_cmp(arg1, "zmien"))
	{
		if (!IS_DIPLOMAT(ch))
		{
			send_to_char("Nie jeste¶ dyplomat± tej organizacji." NL, ch);
			return;
		}

		if (!*arg2)
		{
			send_to_char("Jak± politykê chcesz prowadziæ?" NL, ch);
			return;
		}

		if (*argument == '\0')
		{
			send_to_char("Z jak± organizacj± chcesz prowadziæ t± politykê?" NL,
					ch);
			return;
		}

		if (value < -1 || value > 1)
		{
			send_to_char("Nieznany rodzaj polityki." NL, ch);
			return;
		}

		if (!(tclan = get_clan(argument)))
		{
			send_to_char("Taka organizacja nie istnieje." NL, ch);
			return;
		}

		if (clan->vClan)
		{
			send_to_char(
					"Musisz poczekaæ na zakoñczenie obecnego g³osowania." NL,
					ch);
			return;
		}

		FOREACH( politics, clan->first_politics )
			if (politics->clan == tclan && politics->relations == value)
			{
				send_to_char(
						"Obecnie prowadzisz w³a¶nie tak± politykê. Po co g³osowaæ drugi raz?" NL,
						ch);
				return;
			}

		clan->vRelations = value;
		clan->vClan = tclan;
		ch_printf(ch,
				"Zmiana polityki w stosunku od organizacji: " FG_CYAN "%s" EOL
				"poddana pod g³osowanie" NL, CLANNAME(tclan));

		save_clan(clan);
		return;
	}

	if (!str_cmp(arg1, "vote") || !str_cmp(arg1, "glosuj"))
	{
		if (!(member = get_member(clan, ch->name))
				|| member->status < CLAN_DIPLOMAT)
		{
			send_to_char("Nie masz uprawnieñ." NL, ch);
			return;
		}

		if (!(vclan = clan->vClan))
		{
			send_to_char(
					"Obecnie nie ma ¿adnego g³osowania dotycz±cego polityki." NL,
					ch);
			return;
		}

		vrelations = clan->vRelations;

		if (*arg2 == '\0')
		{
			send_to_char(
					FB_CYAN "Temat g³osowania dotycz±cego zmiany polityki w Twojej organizacji:" EOL NL,
					ch);
			ch_printf(ch,
					"G³osowanie dotyczy zmiany polityki na %s w stosunku do organizacji: "
					FG_CYAN "%s" PLAIN "." NL NL,
					bit_name(relation_types_list, vrelations), CLANNAME(vclan));
			ch_printf(ch, "G³osy oddane na TAK: %s." NL,
					(*clan->vYes != '\0') ? clan->vYes : "");

			return;
		}

		if (!str_cmp(arg2, "tak") || !str_cmp(arg2, "yes"))
		{
			if (member->status == CLAN_DIPLOMAT)
			{
				send_to_char(
						"Nie masz uprawnieñ do g³osowania na temat polityki." NL,
						ch);
				return;
			}

			if (is_name(ch->name, clan->vYes))
			{
				send_to_char("Nie mo¿esz g³osowac dwukrotnie." NL, ch);
				return;
			}

			ch_printf(ch,
					"G³osowanie dotyczy zmiany polityki na %s w stosunku do organizacji: "
					FG_CYAN "%s" PLAIN "." NL NL,
					bit_name(relation_types_list, vrelations), CLANNAME(vclan));
			send_to_char("Oddajesz swój g³os na TAK." NL, ch);
			sprintf(namebuf, " %s", ch->name);
			if (*clan->vYes == '\0')
				STRDUP(clan->vYes, namebuf);
			else
			{
				buf2 = clan->vYes;
				strcat(buf2, namebuf);
				STRDUP(clan->vYes, buf2);
			}

			buf = clan->vYes;
			numvotes = 1;

			while ((buf = one_argument(buf, vyesbuf)) && (buf[0] != '\0'))
				numvotes++;

			if (numvotes < 3)
			{
				save_clan(clan);
				return;
			}

			match = false;
			FOREACH( politics, clan->first_politics )
				if (politics->clan == vclan)
				{
					match = true;
					break;
				}

			if (match)
			{
				politics->clan = vclan;
				politics->relations = vrelations;
			}
			else
			{
				CREATE(politics, POLITICS_DATA, 1);
				LINK(politics, clan->first_politics, clan->last_politics, next,
						prev);
				politics->clan = vclan;
				politics->relations = vrelations;
			}
			ch_printf(ch,
					"Polityka w stosunku do organizacji: " FG_CYAN "%s" EOL
					"zosta³a zmieniona pomy¶lnie i wprowadzona w ¿ycie." NL,
					CLANNAME(vclan));

			pclan_voting_end(clan);
			save_clan(clan);
			return;
		}

		if (!str_cmp(arg2, "nie") || !str_cmp(arg2, "no"))
		{
			if (member->status == CLAN_DIPLOMAT)
			{
				send_to_char(
						"Nie masz uprawnieñ do g³osowania na temat polityki." NL,
						ch);
				return;
			}

			if (is_name(ch->name, clan->vYes))
			{
				send_to_char("Nie mo¿esz g³osowac dwukrotnie." NL, ch);
				return;
			}

			ch_printf(ch,
					"G³osowanie dotyczy zmiany polityki na %s w stosunku do organizacji: "
					FG_CYAN "%s" PLAIN "." NL NL,
					bit_name(relation_types_list, vrelations), CLANNAME(vclan));
			send_to_char("Oddajesz swój g³os na NIE." NL, ch);
			ch_printf(ch,
					"Polityka w stosunku od organizacji: " FG_CYAN "%s" EOL
					"nie zosta³a zmieniona. G³osowanie nie przebieg³o pomy¶lnie." NL,
					CLANNAME(vclan));
			pclan_voting_end(clan);
			save_clan(clan);

			return;
		}
	}

	if (!str_cmp(arg1, "help") || !str_cmp(arg1, "pomoc"))
	{
		send_to_char("Sk³adnia: relations [komenda] [warto¶æ] [organizacja]" NL
		" komenda: change vote" NL
		" wartosc: tak nie typ_polityki" NL
		" organizacja: nazwa_organizacji id_organizacji" NL NL, ch);
		return;
	}

	send_to_char("Nie znana komenda. Spróbój: relations pomoc." NL, ch);
	return;
}

void pclan_voting_end(CLAN_DATA *clan)
{
	clan->vClan = NULL;
	clan->vRelations = 0;
	STRDUP(clan->vYes, "");

	return;
}
//koniec polityki i glosowania.

DEF_DO_FUN( showclan )
{
	CLAN_DATA *clan;
	POLITICS_DATA *politics;
	MEMBER_DATA *member;
	int i = 0;
	int applicants = 0;

	if (IS_NPC(ch))
	{
		huh(ch);
		return;
	}

	if (!*argument)
	{
		send_to_char("Usage: showclan <clan>" NL, ch);
		return;
	}

	if (!(clan = get_clan(argument)))
	{
		send_to_char("No such clan." NL, ch);
		return;
	}

	ch_printf(ch,
			FG_CYAN "Name: " PLAIN "%s" NL FG_CYAN "Filename: " PLAIN "%s" FG_CYAN "	Id: " PLAIN "%4d"
			FG_CYAN "  Type: " PLAIN "%d" FG_CYAN " (" PLAIN "%s" FG_CYAN ")" NL,
			clan->name, clan->filename, clan->clan_id, clan->type,
			CLANTYPE(clan, 0));

	ch_printf(ch,
			FG_CYAN "Flags: " PLAIN "%s" FG_CYAN "  Penalty: " PLAIN "%ld" NL,
			flag_string(clan_flags_list, clan->flags), clan->penalty);

	ch_printf(ch,
			FG_CYAN "Leader: " PLAIN "%s" FG_CYAN "  Das erste:): " PLAIN "%s" FG_CYAN "  Second: "
			PLAIN "%s" FG_CYAN "  Diplomat: " PLAIN "%s" NL, LEADER_NAME(clan),
			FIRST_NAME(clan), SECOND_NAME(clan), DIPLOMAT_NAME(clan));

	ch_printf(ch, FG_CYAN "Members(" PLAIN "%d" FG_CYAN "):" PLAIN,
			clan->members);
	FOREACH( member, clan->first_member )
		if (member->status != CLAN_WAITING)
			ch_printf(ch, "%s%s", (i++) % 4 ? "    " : NL, member->name);
		else
			applicants++;

	i = 0;
	ch_printf(ch, NL FG_CYAN "Applicants(" PLAIN "%d" FG_CYAN "):" PLAIN,
			applicants);
	FOREACH( member, clan->first_member )
		if (member->status == CLAN_WAITING)
			ch_printf(ch, "%s%s", (i++) % 4 ? "    " : NL, member->name);

	send_to_char( NL FG_CYAN "Politics:" EOL, ch);
	FOREACH( politics, clan->first_politics )
		ch_printf(ch, "%s %d" NL, politics->clan->name, politics->relations);

	send_to_char( FG_CYAN "Ranks:" NL, ch);
	for (i = 0; i <= CLAN_LEADER; i++)
		ch_printf(ch, FG_CYAN "%2d. " PLAIN "%s" NL, i, clan->rank[i]);

	ch_printf(ch,
			FG_CYAN "Funds: " PLAIN "%ld" FG_CYAN "  Loan: " PLAIN "%ld" FG_CYAN "  repay_date: " PLAIN "%s" NL,
			clan->funds, clan->loan,
			(int) clan->repay_date ?
					static_cast<SWString>(SWTimeStamp(clan->repay_date)).c_str() :
					"");
	ch_printf(ch,
			FG_CYAN "PKills: " PLAIN "%6d" FG_CYAN " PDeaths: " PLAIN "%6d" NL
			FG_CYAN "MKills: " PLAIN "%6d" FG_CYAN " MDeaths: " PLAIN "%6d" NL,
			clan->pkills, clan->pdeaths, clan->mkills, clan->mdeaths);
	ch_printf(ch, FG_CYAN "Description:" EOL "%s" EOL, clan->description);

	return;
}

DEF_DO_FUN( makeclan )
{
	CLAN_DATA *clan;

	if (!*argument)
	{
		send_to_char("Syntax: makeclan <clan name>" NL, ch);
		return;
	}

	clan = new_clan();
	STRDUP(clan->name, argument);
	clan->clan_id = ++sysdata.max_clan_id;
	save_sysdata(sysdata);
	STRDUP(clan->filename, mk_filename(clan->clan_id));
	LINK(clan, first_clan, last_clan, next, prev);
}

DEF_DO_FUN( clan )
{
	char arg[MSL];

	argument = one_argument(argument, arg);

	if (*arg == '\0')
	{
		send_to_char("Skladnia: clan <argument>" NL
		"Argumentem moze byc:" NL NL
		" application appoint buyship capture" NL
		" demote donate empower induct info list" NL
		" loan members outcast relations resign" NL
		" switch withdraw" NL NL, ch);
		return;
	}
	if (!str_cmp(arg, "application"))
	{
		application(ch, argument);
		return;
	}
	if (!str_cmp(arg, "appoint"))
	{
		do_appoint(ch, argument);
		return;
	}
	if (!str_cmp(arg, "buyship"))
	{
		do_clanbuyship(ch, argument);
		return;
	}
	if (!str_cmp(arg, "capture"))
	{
		do_capture(ch, argument);
		return;
	}
	if (!str_cmp(arg, "demote"))
	{
		do_demote(ch, argument);
		return;
	}
	if (!str_cmp(arg, "donate"))
	{
		do_clan_donate(ch, argument);
		return;
	}
	if (!str_cmp(arg, "empower"))
	{
		do_empower(ch, argument);
		return;
	}
	if (!str_cmp(arg, "induct"))
	{
		do_induct(ch, argument);
		return;
	}
	if (!str_cmp(arg, "info"))
	{
		do_claninfo(ch, argument);
		return;
	}
	if (!str_cmp(arg, "list"))
	{
		do_clans(ch, argument);
		return;
	}
	if (!str_cmp(arg, "loan"))
	{
		clan_loan(ch, argument);
		return;
	}
	if (!str_cmp(arg, "members"))
	{
		clan_members(ch);
		return;
	}
	if (!str_cmp(arg, "outcast"))
	{
		do_outcast(ch, argument);
		return;
	}
	if (!str_cmp(arg, "relations"))
	{
		do_relations(ch, argument);
		return;
	}
	if (!str_cmp(arg, "resign"))
	{
		do_resign(ch, argument);
		return;
	}
	if (!str_cmp(arg, "switch"))
	{
		clan_switch(ch, argument);
		return;
	}
	if (!str_cmp(arg, "withdraw"))
	{
		do_clan_withdraw(ch, argument);
		return;
	}

	do_clan(ch, (char*) "");
}

DEF_DO_FUN( clans )
{
	CLAN_DATA *clan;
	int count = 0;

	ch_printf(ch, FB_WHITE "Lista autoryzowanych organizacji:" EOL);
	FOREACH( clan, first_clan )
		if (IS_ORG(clan))
		{
			if ( IS_SET(clan->flags,
					CLAN_NOINFO) && !get_member( clan, ch->name ) && !IS_IMMORTAL( ch ))
				continue;

			ch_printf(ch, "%-35s", clan->name);
			ch_printf(ch, "  [Id:%2d]" NL, clan->clan_id);
			if (clan->first_suborg)
			{
				CLAN_DATA *suborg;

				for (suborg = clan->first_suborg; suborg;
						suborg = suborg->next_suborg)
					ch_printf(ch, "   %-22s  [Id:%2d]" NL, suborg->name,
							suborg->clan_id);
			}
			count++;
		}

	ch_printf(ch, FB_WHITE "Gildie:" EOL);
	FOREACH( clan, first_clan )
		if (IS_GUILD(clan))
		{
			if ( IS_SET(clan->flags,
					CLAN_NOINFO) && !get_member( clan, ch->name ) && !IS_IMMORTAL( ch ))
				continue;

			ch_printf(ch, "%-35s  [Id:%2d]" NL, clan->name, clan->clan_id);
			count++;
		}

	ch_printf(ch, FB_WHITE "Zakony:" EOL);
	FOREACH( clan, first_clan )
		if (IS_ORDER(clan))
		{
			if ( IS_SET(clan->flags,
					CLAN_NOINFO) && !get_member( clan, ch->name ) && !IS_IMMORTAL( ch ))
				continue;

			ch_printf(ch, "%-35s  [Id:%2d]" NL, clan->name, clan->clan_id);
			count++;
		}

	if (!count)
		send_to_char(
				"Nie ma ¿adnych organizacji, gildii ani zakonów... jeszcze." NL,
				ch);

	send_to_char( NL "Patrz te¿: Planets" NL, ch);
}

DEF_DO_FUN( claninfo )
{
	CLAN_DATA *clan;
	CLAN_DATA *suborg;
	PLANET_DATA *planet;
	SHIP_DATA *ship;
	MEMBER_DATA *member;
	long revenue;
	int support;
	int pCount;
	int i, j;
	bool is_member = false;

	if (IS_NPC(ch))
		return;

	pCount = support = revenue = i = j = 0;

	if (!str_cmp(argument, "") && ch->pcdata->clan)
		clan = ch->pcdata->clan;
	else if (!str_cmp(argument, "") && !ch->pcdata->clan)
	{
		send_to_char("Skladnia: info <nazwa | id>" NL, ch);
		return;
	}
	else
		clan = get_clan(argument);

	if (!clan)
	{
		ch_printf(ch, "Brak informacji w bazie danych na temat %s." NL,
				argument);
		return;
	}
	else if (!(is_member = (((member = get_member(clan, ch->name)) != NULL
			&& member->status > CLAN_WAITING) || IS_IMMORTAL(ch)))
			&& IS_SET(clan->flags, CLAN_NOINFO))
	{
		ch_printf(ch, "Brak informacji w bazie danych na temat %s." NL,
				argument);
		return;
	}

	ch_printf(ch,
			FB_YELLOW
			"%s" FG_CYAN " (Id:" PLAIN "%d" FG_CYAN ") to %s. %s: " PLAIN "%s" FG_CYAN "." NL,
			clan->name, clan->clan_id, CLANTYPE(clan, 0),
			CRANK(clan, CLAN_LEADER), LEADER_NAME(clan));

	if (clan->first)
	{
		ch_printf(ch, FG_CYAN "W dowodzeniu pomag%s mu " PLAIN "%s",
				clan->second ? "aj±" : "a", FIRST_NAME(clan));

		if (clan->second)
			ch_printf(ch, FG_CYAN " i " PLAIN "%s" FG_CYAN "." EOL,
					SECOND_NAME(clan));
		else
			ch_printf(ch, FG_CYAN "." EOL);
	}
	else if (clan->second)
		ch_printf(ch, FG_CYAN "W dowodzeniu pomaga mu " PLAIN "%s"
		FG_CYAN "." NL, SECOND_NAME(clan));

	if ( IS_ORG( clan ) && clan->diplomat)
		ch_printf(ch,
				FG_CYAN "Polityk± organizacji zajmuje siê " PLAIN "%s" FG_CYAN "." EOL,
				DIPLOMAT_NAME(clan));

	ch_printf(ch, FG_CYAN "%s posiada " PLAIN "%d" FG_CYAN " cz³onk%s",
			capitalize(CLANTYPE(clan, 0)), clan->members,
			NUMBER_SUFF(clan->members, "a", "ów", "ów"));
	if (is_member)
		ch_printf(ch,
				FG_CYAN " a na koncie ma " PLAIN "%d" FG_CYAN " kredytek." EOL,
				clan->funds);
	else
		send_to_char("." NL, ch);

	// Narazie wszystkie rodzaje klanow moga ogladac m.in swoje statki  -- Pixel
	if ( IS_ORG( clan ) || IS_SUBORG(clan) || IS_ORDER(clan) || IS_GUILD(clan))
	{
		{
			int x = 0;
			char buf[MSL];

			buf[0] = '\0';
			FOREACH( planet, first_planet )
				if (clan == planet->governed_by)
				{
					x++;
					strcat(buf, " ");
					strcat(buf, capitalize(planet->name));
				}
			if (x)
				ch_printf(ch,
						FG_CYAN "Planety podlegaj±ce organizacji:" PLAIN "%s" FG_CYAN "." EOL,
						buf);
		}

		if (!is_member)
			return;

		if (clan->loan)
			ch_printf(ch,
					FG_CYAN "Organizacja musi sp³aciæ po¿yczkê wysoko¶ci: " PLAIN "%ld"
					FG_CYAN " do dnia: " PLAIN "%s" FG_CYAN "." NL, clan->loan,
					static_cast<SWString>(SWTimeStamp(clan->repay_date)).c_str());

		FOREACH( planet, first_planet )
			if (clan == planet->governed_by)
			{
				support += planet->pop_support;
				pCount++;
				revenue += get_taxes(planet);
			}

		if (pCount > 1)
			support /= pCount;

		ch_printf(ch,
				FG_CYAN
				"Dziêki poparciu " PLAIN "%d" FG_CYAN ", przychody organizacji wynosz± " PLAIN "%ld" FG_CYAN "." EOL,
				support, revenue);

		ch_printf(ch, FG_CYAN "Organizacje podlegajace:" EOL);
		for (suborg = clan->first_suborg; suborg; suborg = suborg->next_suborg)
		{
			j++;
			ch_printf(ch,
					FG_CYAN "%d." FG_YELLOW " %s" FG_CYAN ", Lider:" PLAIN " %s" FG_CYAN ", Mo¿ni:" PLAIN
					" %s" FG_CYAN " i" PLAIN " %s" FG_CYAN "." NL, j,
					CLANNAME(suborg), suborg->leader, suborg->first,
					suborg->second);
		}
		ch_printf(ch,
				FG_CYAN "Ilo¶æ organizacji podlegaj±cych:" PLAIN " %d" FG_CYAN "." NL,
				j);

		ch_printf(ch,
				FG_CYAN "Bilans zgonów:" PLAIN " %d " FG_CYAN "zabójstw," PLAIN " %d " FG_CYAN "zgonów." NL,
				clan->pkills, clan->pdeaths);

		ch_printf(ch, FG_CYAN "Statki kosmiczne:" NL);
		FOREACH( ship, first_ship )
			if (!str_cmp(clan->name, ship->owner))
			{
				i++;
				ch_printf(ch,
						FG_CYAN "%d." PLAIN " %s" FG_CYAN ", Pilot:" PLAIN " %s" FG_CYAN ", Drugi pilot:" PLAIN
						" %s" FG_CYAN "." NL, i, ship->name, ship->pilot,
						ship->copilot);
			}
		ch_printf(ch,
				FG_CYAN "Ilo¶æ statków kosmicznych:" PLAIN " %d" FG_CYAN "." EOL,
				i);
	}
}

void clan_members(CHAR_DATA *ch)
{
	CLAN_DATA *clan = CLAN(ch);
	MEMBER_DATA *member;
	int i;

	if (!clan)
	{
		send_to_char(
				"Nie nale¿ysz do ¿adnej organizacji, gildii ani ¿adnego zakonu." NL,
				ch);
		return;
	}

	ch_printf(ch,
			FG_CYAN "%s: " PLAIN "%s" FG_CYAN ". Pomagaj± mu: %s: " PLAIN "%s" FG_CYAN
			" i %s: " PLAIN "%s" FG_CYAN "." EOL, CRANK(clan, CLAN_LEADER),
			LEADER_NAME(clan), CRANK(clan, CLAN_FIRST), FIRST_NAME(clan),
			CRANK(clan, CLAN_SECOND), SECOND_NAME(clan));

	if ( IS_ORG( clan ) && clan->diplomat)
		ch_printf(ch,
				FG_CYAN "Polityk± organizacji zajmuje siê: " PLAIN "%s" FG_CYAN "." EOL,
				DIPLOMAT_NAME(clan));

	ch_printf(ch, FG_CYAN "Oto lista cz³onków twoje%s %s:" EOL,
			CLANSUFFIX(clan, "go", "j"), CLANTYPE(clan, 1));
	i = 0;
	FOREACH( member, clan->first_member )
		if (member->status > CLAN_WAITING)
		{
			if ((i % 3 == 0) && i > 1)
				send_to_char( NL, ch);
			ch_printf(ch, "%-15s", capitalize(member->name));
			i++;
		}

	ch_printf(ch,
			NL NL FG_CYAN "%s posiada " PLAIN "%d" FG_CYAN " cz³onk%s." EOL,
			capitalize(CLANTYPE(clan, 0)), clan->members,
			(clan->members == 1) ? "a" : "ów");

	return;
}

void clan_switch(CHAR_DATA *ch, char *argument)
{
	CLAN_DATA *clan;
	MEMBER_DATA *member;

	if (!(clan = get_clan(argument))
			|| (IS_SET(clan->flags, CLAN_NOINFO)
					&& (!(member = get_member(clan, ch->name))
							|| member->status < CLAN_MEMBER) && !IS_IMMORTAL(ch)))
		send_to_char("Nie ma takiej organizacji, gildii ani zakonu." NL, ch);
	else if ((member = get_member(clan, ch->name))
			&& member->status >= CLAN_MEMBER)
	{
		ch->pcdata->clan = clan;
		save_char_obj(ch);
		ch_printf(ch, "Zaczynasz zajmowaæ siê %s %s." NL, CLANTYPE(clan, 5),
				clan->name);
	}
	else
		ch_printf(ch, "Nie nale¿ysz do te%s %s." NL,
				CLANSUFFIX(clan, "go", "j"), CLANTYPE(clan, 1));
}

DEF_DO_FUN( resign )
{
	CLAN_DATA *clan = CLAN(ch);
	long loose_exp;

	if (!clan)
	{
		huh(ch);
		return;
	}

	if (IS_SET(clan->flags, CLAN_NOQUIT))
	{
		ch_printf(ch, "Nie mo¿esz zrezygnowaæ z cz³onkostwa w %s." NL,
				CLANTYPE(clan, 4));
		return;
	}

	if (IS_SET(clan->flags, CLAN_PENALTYQUIT))
	{
		if ((ch->gold < clan->penalty) && (ch->pcdata->bank < clan->penalty))
		{
			ch_printf(ch,
					"Nie masz tyle kredytek aby zap³aciæ karê za rezygnacjê z cz³onkostwa w %s." NL,
					CLANTYPE(clan, 4));
			return;
		}
		else
		{
			long penalty;
			penalty = clan->penalty;
			ch_printf(ch,
					"P³acisz %ld kredytek kary za rezygnacjê z cz³onkostwa w %s." NL,
					clan->penalty, CLANTYPE(clan, 4));
			(ch->gold >= penalty) ? (ch->gold -= penalty) : (ch->pcdata->bank -=
											penalty);
		}
	}

	outcast_member(ch->pcdata->clan, ch);
	act( COL_FORCE, "Rezygnujesz z cz³onkostwa w $t", ch, clan->name, NULL,
			TO_CHAR);
	loose_exp =
			UMAX(
					ch->experience[DIPLOMACY_ABILITY] - exp_level( ch->skill_level[DIPLOMACY_ABILITY] ),
					0);
	ch->experience[DIPLOMACY_ABILITY] -= loose_exp;
	ch->experience[DIPLOMACY_ABILITY] = URANGE(0,
			ch->experience[DIPLOMACY_ABILITY],
			ch->experience[DIPLOMACY_ABILITY]);
	ch_printf(ch, "Tracisz %ld punktów do¶wiadczenia w dyplomacji." NL,
			loose_exp);
}

DEF_DO_FUN( clan_withdraw )
{
	CLAN_DATA *clan = CLAN(ch);
	long ammount;

	if (!clan)
	{
		send_to_char(
				"Musisz najpierw nale¿eæ do jakiej¶ organizacji, gildii, lub zakonu." NL,
				ch);
		return;
	}

	if ( /*!ch->in_room ||*/!IS_SET(ch->in_room->room_flags, ROOM_BANK))
	{
		send_to_char("Mo¿esz to zrobiæ jedynie w banku!" NL, ch);
		return;
	}

	if (!clan_bestow(ch, "withdraw"))
	{
		send_to_char( FB_RED "Chyba nikt ciê do tego nie upowa¿ni³!" NL, ch);
		return;
	}

	if ( /*(IS_ORG( clan ) || IS_SUBORG( clan )) &&*/clan->loan)
	{
		send_to_char(
				"Aby wyp³acaæ gotówkê z konta organizacji trzeba najpierw sp³aciæ po¿yczkê." NL,
				ch);
		return;
	}

	if (!(ammount = atoi(argument)) || ammount < 0)
	{
		send_to_char("Ile chcesz wyp³aciæ?" NL, ch);
		return;
	}

	if (ammount > clan->funds)
	{
		ch_printf(ch, "%s nie ma a¿ tyle w kasie!" NL, clan->name);
		return;
	}

	ch_printf(ch, "Wyp³acasz %ld kredyt%s z funduszy %s." NL, ammount,
			NUMBER_SUFF(ammount, "kê", "ki", "ek"), clan->name);
	clan->funds -= ammount;
	ch->gold += ammount;
	save_clan(clan);
	save_char_obj(ch);
}

DEF_DO_FUN( clan_donate )
{
	CLAN_DATA *clan = CLAN(ch);
	long ammount;

	if (!clan)
	{
		send_to_char(
				"Musisz najpierw nale¿eæ do jakiej¶ organizacji, gildii, lub zakonu." NL,
				ch);
		return;
	}

	if (/*!ch->in_room ||*/!IS_SET(ch->in_room->room_flags, ROOM_BANK))
	{
		send_to_char("By to zrobiæ musisz byæ w banku!" NL, ch);
		return;
	}

	if (!(ammount = atoi(argument)) || ammount < 0)
	{
		send_to_char("Ile chcesz wyp³aciæ?" NL, ch);
		return;
	}

	if (ammount > ch->gold)
	{
		send_to_char("Nie masz tyle!" NL, ch);
		return;
	}

	ch_printf(ch, "Wp³acasz %ld kredyt%s na konto %s." NL, ammount,
			NUMBER_SUFF(ammount, "kê", "ki", "ek"), clan->name);
	clan->funds += ammount;
	ch->gold -= ammount;
	save_clan(clan);
	save_char_obj(ch);
}

static char* mk_filename(int clan_id)
{
	static char filename[MFL];

	sprintf(filename, "%d.xml", clan_id);
	return filename;
}

DEF_DO_FUN( appoint )
{
	CHAR_DATA *victim;
	CLAN_DATA *clan = CLAN(ch);
	MEMBER_DATA *member;
	char arg[MIL];

	if (!clan)
	{
		huh(ch);
		return;
	}

	argument = one_argument(argument, arg);

	if (!IS_LEADER(ch))
	{
		send_to_char("Tylko lider mo¿e to zrobiæ!" NL, ch);
		return;
	}

	if (!*argument)
	{
		send_to_char(
				"Sk³adnia: appoint <imiê> stopieñ | pierwszy | drugi | dyplomata" NL,
				ch);
		return;
	}

	if (!(member = get_member(clan, arg)))
	{
		ch_printf(ch, "Nikt taki nie nale¿y do twoje%s %s." NL,
				CLANSUFFIX(clan, "go", "j"), CLANTYPE(clan, 1));
		return;
	}

	if (!(victim = get_char_room(ch, arg)) || IS_NPC(victim))
	{
		ch_printf(ch, "Aby awansowac cz³onka %s, koniecznie jest spotkanie." NL,
				CLANTYPE(clan, 1));
		return;
	}

	if (victim == ch)
	{
		ch_printf(ch, "Jeste¶ liderem - to najwy¿sze stanowisko w %s." NL,
				CLANTYPE(clan, 4));
		return;
	}

	if (member->status == CLAN_WAITING)
	{
		ch_printf(ch, "%s dopiero oczekuje na zostanie cz³onki%s." NL,
				SEX_STR(victim, "ni±", "em", "em"));
		return;
	}

	if (is_name(argument, "rank stopien"))
	{
		if (member->status > 9)
			ch_printf(ch, "%s posiada ju¿ najwy¿szy stopieñ w %s." NL,
					victim->przypadki[0], CLANTYPE(clan, 4));
		else
		{
			member->status++;
			act( COL_FORCE, "Zwiêkszasz $N$3 stopieñ w $t.", ch, clan->name,
					victim, TO_CHAR);
			act( COL_FORCE, "$n zwiêksza $N$3 stopieñ w $t.", ch, clan->name,
					victim, TO_NOTVICT);
			act( COL_FORCE, "$n zwiêksza ci stopieñ w $t.", ch, clan->name,
					victim, TO_VICT);
		}
	}
	else if (is_name(argument, "first pierwszy"))
	{
		if (member->status == CLAN_FIRST)
			ch_printf(ch, "%s jest ju¿ pierwsz%s twoj%s %s." NL,
					victim->przypadki[0], SEX_STR(victim, "±", "ym", "ym"),
					CLANSUFFIX(clan, "go", "j"), CLANTYPE(clan, 1));
		else
		{
			act( COL_FORCE, "Czynisz $N$3 pierwszym w $t.", ch, clan->name,
					victim, TO_CHAR);
			act( COL_FORCE, "$n czyni $N$3 pierwszym w $t.", ch, clan->name,
					victim, TO_NOTVICT);
			act( COL_FORCE, "$n czyni ciê pierwszym w $t.", ch, clan->name,
					victim, TO_VICT);
			clan->first = member;
			member->status = CLAN_FIRST;
		}
	}
	else if (is_name(argument, "second drugi"))
	{
		if (member->status == CLAN_SECOND)
			ch_printf(ch, "%s jest ju¿ drug%s twoj%s %s." NL,
					victim->przypadki[0], SEX_STR(victim, "±", "im", "im"),
					CLANSUFFIX(clan, "go", "j"), CLANTYPE(clan, 1));
		else
		{
			act( COL_FORCE, "Czynisz $N$3 drugim w $t.", ch, clan->name, victim,
					TO_CHAR);
			act( COL_FORCE, "$n czyni $N$3 drugim w $t.", ch, clan->name,
					victim, TO_NOTVICT);
			act( COL_FORCE, "$n czyni ciê drugim w $t.", ch, clan->name, victim,
					TO_VICT);
			clan->second = member;
			member->status = CLAN_SECOND;
		}
	}
	else if (is_name(argument, "diplomat dyplomata")
			&& clan->type <= CLAN_SUBORG)
	{
		if (member->status == CLAN_DIPLOMAT)
			ch_printf(ch, "%s jest ju¿ dyplomat± twoj%s %s." NL,
					victim->przypadki[0], CLANSUFFIX(clan, "go", "j"),
					CLANTYPE(clan, 1));
		else
		{
			if ((victim->main_ability != DIPLOMACY_ABILITY)
					|| victim->skill_level[DIPLOMACY_ABILITY] < 90)
			{
				send_to_char(
						"Ta osoba nie jest odpowiednim kandydatem na dyplomatê Twojej organizacji." NL,
						ch);
				return;
			}

			act( COL_FORCE, "Czynisz $N$3 dyplomat± w $t.", ch, clan->name,
					victim, TO_CHAR);
			act( COL_FORCE, "$n czyni $N$3 dyplomat± w $t.", ch, clan->name,
					victim, TO_NOTVICT);
			act( COL_FORCE, "$n czyni ciê dyplomat± w $t.", ch, clan->name,
					victim, TO_VICT);
			clan->diplomat = member;
			member->status = CLAN_DIPLOMAT;
		}
	}
	else
		do_appoint(ch, (char*) "");

	adjust_clan(clan);
}

DEF_DO_FUN( demote )
{
	CLAN_DATA *clan = CLAN(ch);
	MEMBER_DATA *member;
	char arg[MIL];

	if (!clan)
	{
		huh(ch);
		return;
	}

	argument = one_argument(argument, arg);

	if (!IS_LEADER(ch))
	{
		send_to_char("Tylko lider mo¿e to zrobiæ!" NL, ch);
		return;
	}

	if (!*argument)
	{
		send_to_char(
				"Sk³adnia: demote <imiê> stopieñ | pierwszy | drugi | dyplomata" NL,
				ch);
		return;
	}

	if (!(member = get_member(clan, arg)))
	{
		ch_printf(ch, "Nikt taki nie nale¿y do twoje%s %s." NL,
				CLANSUFFIX(clan, "go", "j"), CLANTYPE(clan, 1));
		return;
	}

	if (member->status == CLAN_WAITING)
	{
		ch_printf(ch, "%s dopiero oczekuje na przyjêcie." NL, member->name);
		return;
	}

	if (is_name(argument, "rank stopien"))
	{
		if (member->status <= CLAN_MEMBER)
			ch_printf(ch, "%s posiada ju¿ najni¿szy stopieñ w %s." NL,
					member->name, CLANTYPE(clan, 4));
		else
		{
			member->status--;
			act( COL_FORCE, "$T uzyskuje ni¿szy stopieñ w $t.", ch, clan->name,
					member->name, TO_CHAR);
		}
	}
	else if (is_name(argument, "first pierwszy"))
	{
		if (member->status != CLAN_FIRST)
			ch_printf(ch, "%s nie jest pierwszym twoj%s %s." NL, member->name,
					CLANSUFFIX(clan, "go", "j"), CLANTYPE(clan, 1));
		else
		{
			act( COL_FORCE, "$T traci status pierwszego w $t.", ch, clan->name,
					member->name, TO_CHAR);
			clan->first = NULL;
			member->status = CLAN_MEMBER;
		}
	}
	else if (is_name(argument, "second drugi"))
	{
		if (member->status != CLAN_SECOND)
			ch_printf(ch, "%s nie jest drugim twoj%s %s." NL, member->name,
					CLANSUFFIX(clan, "go", "j"), CLANTYPE(clan, 1));
		else
		{
			act( COL_FORCE, "$T traci status drugiego w $t.", ch, clan->name,
					member->name, TO_CHAR);
			clan->second = NULL;
			member->status = CLAN_MEMBER;
		}
	}
	else if (is_name(argument, "diplomat dyplomata")
			&& clan->type <= CLAN_SUBORG)
	{
		if (member->status != CLAN_DIPLOMAT)
			ch_printf(ch, "%s nie jest dyplomat± twoj%s %s." NL, member->name,
					CLANSUFFIX(clan, "go", "j"), CLANTYPE(clan, 1));
		else
		{
			act( COL_FORCE, "$T traci status dyplomaty $t.", ch, clan->name,
					member->name, TO_CHAR);
			clan->diplomat = NULL;
			member->status = CLAN_MEMBER;
		}
	}
	else
		do_demote(ch, (char*) "");

	adjust_clan(clan);
}

DEF_DO_FUN( capture )
{
	CLAN_DATA *clan = CLAN(ch);
	PLANET_DATA *planet;
	PLANET_DATA *cPlanet;
	float support = 0.0;
	int pCount = 0;
	char buf[MSL];

	if (!clan || !ch->in_room || !ch->in_room->area)
	{
		huh(ch);
		return;
	}

	if (!IS_ORG(clan) && !IS_SUBORG(clan))
	{
		send_to_char(
				"Tylko organizacje mog± przejmowaæ w³adze na planetach." NL,
				ch);
		return;
	}

	if (!(planet = ch->in_room->area->planet))
	{
		send_to_char("Musisz byæ na planecie by móc j± przej±æ." NL, ch);
		return;
	}

	if (clan == planet->governed_by)
	{
		send_to_char("Ta planeta ju¿ nale¿y do twojej organizacji." NL, ch);
		return;
	}

	if (planet->starsystem)
	{
		SHIP_DATA *ship;
		CLAN_DATA *sClan;

		for (ship = planet->starsystem->first_ship; ship;
				ship = ship->next_in_starsystem)
			if ((sClan = get_clan(ship->owner)) && sClan == planet->governed_by)
			{
				send_to_char(
						"Planeta nie mo¿e byæ przejêta, gdy¿ jest chroniona przez orbituj±cy statek." NL,
						ch);
				return;
			}
	}

	if (IS_SET(planet->flags, PLANET_NOCAPTURE))
	{
		send_to_char("Ta planeta nie mo¿e byæ przejêta." NL, ch);
		return;
	}

	if (planet->pop_support > 0)
	{
		send_to_char(
				"Mieszkañcy planety nie zgadzaj± siê na t± zmianê w tej chwili." NL,
				ch);
		return;
	}

	FOREACH( cPlanet, first_planet )
		if (clan == cPlanet->governed_by)
		{
			pCount++;
			support += cPlanet->pop_support;
		}

	if (support < 0)
	{
		send_to_char("Poparcie dla twojej organizacji jest niewystarczaj±ce!" NL
		"Spróbuj polepszyæ lojalno¶æ na planetach, które ju¿ kontrolujesz." NL,
				ch);
		return;
	}

	planet->governed_by = clan;
	planet->pop_support = 50;

	sprintf(buf, "%s przejmuje kontrolê nad planet± %s!", clan->name,
			planet->name);
	echo_to_all(buf, 0);
	save_planet(planet);
}

DEF_DO_FUN( empower )
{
	CHAR_DATA *victim;
	CLAN_DATA *clan = CLAN(ch);
	MEMBER_DATA *member;
	char arg[MIL];
	char arg2[MIL];
	char buf[MSL];

	if (!clan)
	{
		huh(ch);
		return;
	}

	if (!IS_LEADER(ch))
	{
		send_to_char("Tylko lider mo¿e tego dokonaæ." NL, ch);
		return;
	}

	argument = one_argument(argument, arg);
	argument = one_argument(argument, arg2);

	if (!*arg)
	{
		send_to_char("Kogo (i do czego) chcesz upowa¿niæ?" NL, ch);
		return;
	}

	if (!str_cmp(arg, "list"))
	{
		send_to_char("Obecnie mo¿esz upowa¿niaæ do:" NL NL
		" withdraw induct outcast" NL
		" clear", ch);
		if ( IS_ORG( clan ) || IS_SUBORG(clan))
			send_to_char(" clanbuyship clansellship" NL " loan" NL, ch);
		return;
	}

	if (!(member = get_member(clan, arg)))
	{
		ch_printf(ch, "Ta osoba nie nale¿y do twoje%s %s." NL,
				CLANSUFFIX(clan, "go", "j"), CLANTYPE(clan, 1));
		return;
	}

	if (!str_cmp(ch->name, member->name))
	{
		ch_printf(ch,
				"Po co? Przecie¿ i tak bêdziesz mia³%s wszystkie uprawnienia." NL,
				SEX_SUFFIX__AO(ch));
		return;
	}

	if (!str_cmp(arg2, "clear"))
	{
		STRDUP(member->bestowments, "");
		ch_printf(ch, "%s traci wszelkie uprawnienia." NL, member->name);
		return;
	}

	if (!(victim = get_char_room(ch, arg)) || IS_NPC(victim))
	{
		send_to_char("Nie ma tutaj nikogo takiego." NL, ch);
		return;
	}

	if (victim == ch)
	{
		send_to_char("Po co? Przecie¿ masz wszystkie uprawnienia." NL, ch);
		return;
	}

	if (!*arg2 || !str_cmp(arg2, "list"))
	{
		ch_printf(ch, "%s jest upowa¿nion%s do: %s." NL, victim->przypadki[0],
				SEX_SUFFIX_YAE(victim), member->bestowments);
		return;
	}
	else if (is_name(arg2, member->bestowments))
	{
		ch_printf(ch, "%s jest ju¿ upowa¿nion%s do: %s." NL,
				victim->przypadki[0], SEX_SUFFIX_YAE(victim), arg2);
		return;
	}
	else if (!str_cmp(arg2, "pilot") && (IS_ORG( clan ) || IS_SUBORG(clan)))
	{
		sprintf(buf, "%s %s", member->bestowments, "pilot");
		STRDUP(member->bestowments, buf);
		ch_printf(victim, "%s upowa¿nia ciê do latania statkami organiacji." NL,
				PERS(ch, victim, 0));
		send_to_char("Ok. Upowa¿aniasz do latania statkami organizacji." NL,
				ch);
	}
	else if (!str_cmp(arg2, "withdraw"))
	{
		sprintf(buf, "%s %s", member->bestowments, "withdraw");
		STRDUP(member->bestowments, buf);
		ch_printf(victim, "%s upowa¿nia ciê do wyp³acania funduszy %s." NL,
				PERS(ch, victim, 0), CLANTYPE(clan, 1));
		ch_printf(ch, "Ok. Upowa¿aniasz %s do wyp³acania funduszy %s." NL,
				victim->przypadki[1], CLANTYPE(clan, 1));
	}
	else if (!str_cmp(arg2, "clanbuyship")
			&& (IS_ORG( clan ) || IS_SUBORG(clan)))
	{
		sprintf(buf, "%s %s", member->bestowments, "clanbuyship");
		STRDUP(member->bestowments, buf);
		ch_printf(victim,
				"%s przydziela ci prawo do kupowania statków dla organizacji." NL,
				PERS(ch, victim, 0));
		send_to_char(
				"Ok. Upowa¿aniasz do kupowania statków na rzecz organizacji." NL,
				ch);
	}
	else if (!str_cmp(arg2, "clansellship")
			&& (IS_ORG( clan ) || IS_SUBORG(clan)))
	{
		sprintf(buf, "%s %s", member->bestowments, "clansellship");
		STRDUP(member->bestowments, buf);
		ch_printf(victim,
				"%s przydziela ci prawo do sprzedawania statków nale¿±cych do organizacji." NL,
				PERS(ch, victim, 0));
		send_to_char(
				"Ok. Upowa¿aniasz do sprzedawania statków nale¿±cych do organizacji." NL,
				ch);
	}
	else if (!str_cmp(arg2, "induct"))
	{
		sprintf(buf, "%s %s", member->bestowments, "induct");
		STRDUP(member->bestowments, buf);
		ch_printf(victim,
				"%s upowa¿nia ciê do przyjmowania nowych cz³onków w szeregi %s." NL,
				PERS(ch, victim, 0), CLANTYPE(clan, 2));
		send_to_char("Ok. Upowa¿aniasz do przyjmowania nowych cz³onków." NL,
				ch);
	}
	else if (!str_cmp(arg2, "outcast"))
	{
		sprintf(buf, "%s %s", member->bestowments, "outcast");
		STRDUP(member->bestowments, buf);
		ch_printf(victim, "%s upowa¿nia ciê do usuwani± cz³onków z %s." NL,
				PERS(ch, victim, 0), CLANTYPE(clan, 2));
		send_to_char("Ok. Upowa¿aniasz do usuwania cz³onków." NL, ch);
	}
	else if (!str_cmp(arg2, "loan") && (IS_ORG( clan ) || IS_SUBORG(clan)))
	{
		sprintf(buf, "%s %s", member->bestowments, "loan");
		STRDUP(member->bestowments, buf);
		ch_printf(victim,
				"%s upowa¿nia ciê do pobierania po¿yczek na rzecz organizacji." NL,
				PERS(ch, victim, 0));
		send_to_char(
				"Ok. Upowa¿aniasz do brania po¿yczek na rzecz organizacji." NL,
				ch);
	}
	else
		do_empower(ch, (char*) "list");

	save_char_obj(victim); /* clan gets saved when pfile is saved - Trog: bullshit :) */
	save_clan(clan);
}

bool clan_bestow(CHAR_DATA *ch, const char *cmd)
{
	MEMBER_DATA *member;

	if (ch->pcdata && (member = get_member(ch->pcdata->clan, ch->name))
			&& (is_name(cmd, member->bestowments)
					|| member->status == CLAN_LEADER))
		return true;
	return false;
}

void clan_loan(CHAR_DATA *ch, char *argument)
{
	CLAN_DATA *clan = CLAN(ch);
	char arg[MIL];
	long ammount;

	if (!clan || (!IS_ORG(clan) && !IS_SUBORG(clan)))
	{
		send_to_char("Nie zajmujesz siê ¿adn± organizacj± w tej chwili." NL,
				ch);
		return;
	}

	if (!clan_bestow(ch, "loan"))
	{
		send_to_char("Nie masz odpowiednich uprawnieñ." NL, ch);
		return;
	}

	argument = one_argument(argument, arg);

	if (!*arg)
	{
		send_to_char(
				"Chcesz wzi±æ po¿yczkê (borrow), czy j± sp³aciæ (repay)?" NL,
				ch);
		return;
	}

	if (!str_cmp(arg, "borrow"))
	{
		struct tm *tms;

		if ((ammount = atoi(argument)) < CLAN_MINLOAN)
		{
			ch_printf(ch, "Nie op³aca siê braæ mniejszej po¿yczki ni¿ %ld." NL,
					CLAN_MINLOAN);
			return;
		}

		if ((ammount + clan->loan) > CLAN_MAXLOAN(clan))
		{
			ch_printf(ch, "Nie mo¿esz wzi±æ wiêkszej po¿yczki ni¿ %ld." NL,
					CLAN_MAXLOAN(clan));
			return;
		}

		clan->loan += ammount;
		clan->funds += ammount;
		tms = localtime(&current_time);
		tms->tm_mday += CLAN_LOANTIME(clan);
		clan->repay_date = mktime(tms);
		ch_printf(ch, "Bierzesz po¿yczkê wysoko¶ci: %ld." NL, ammount);

		save_clan(clan);
	}
	else if (!str_cmp(arg, "repay"))
	{
		if (clan->loan > clan->funds)
		{
			ch_printf(ch,
					"Organizacja nie posiada wystarczaj±cych funduszy aby sp³aciæ po¿yczkê." NL);
			return;
		}

		clan->funds -= clan->loan;
		clan->loan = 0;
		clan->repay_date = 0;
		send_to_char("Sp³acasz po¿yczkê." NL, ch);

		save_clan(clan);
	}
	else
		clan_loan(ch, (char*) "");
}

