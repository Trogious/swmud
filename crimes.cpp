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
*  		           Crimes & Jails module    			   *
****************************************************************************/


#include <sys/types.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include "mud.h"


/* przez ile ticków w³adze planety nie zorientuj± siê o ucieczce */
#define JAIL_ACTIVITY		2

/* Pamietaj jeszcze o:
- depozyty - jak zrobiæ by da³o siê wykra¶æ?
- stra¿nik ma fizycznie ³aziæ po celach, a depozyt wyci±gaæ
- itd... :)
*/

const int crime_level_price = 400;

/* by Ganis */
int calculate_wanted_price( const CHAR_DATA *ch )
{
	CRIME_DATA		*crime;
	int				price = 0;

	for (crime = ch->first_crime; crime; crime = crime->next)
		price += crime->level * crime_level_price;

	return price;
}

/* by Ganis */
DEF_DO_FUN( corrupt )
{
	CHAR_DATA		* victim;
	CHAR_DATA		* xch;
	CRIME_DATA		* crime;
	CRIME_DATA		* n_crime;
	int				price;
	char			buf[MSL];
	static const SPEC_FUN* spec_name = spec_lookup("spec_police_fine");

	if (!spec_name)
	{
		bug("spec_police_fine function got lost... Corrupt won't work!");
		send_to_char("Sorry, co¶ przesta³o dzia³aæ w mudzie tak jak powinno :(" NL, ch);
		return;
	}

	if (!ch->first_crime)
	{
		send_to_char("Przecie¿ nie masz nic na sumieniu." NL, ch);
		return;
	}

	//Poszukajmy, czy w tym pokoju jest ktos, kogo mozna skorumpowac
	victim = NULL;
	for (xch = ch->in_room->first_person; xch; xch = xch->next_in_room)
	{

		if (xch->spec_fun == spec_name || xch->spec_2 == spec_name)
		{
			victim = xch;
			break;
		}
	}

	if (!victim)
	{
		send_to_char("Nie ma tu nikogo, kogo mo¿na by skorumpowaæ." NL, ch);
		return;
	}

	price = calculate_wanted_price(ch);

	if (price > ch->gold)
	{
		sprintf(buf, "Potrzebujesz %d kredytek, aby wymazaæ swoj± kryminaln± kartotekê." NL, price);
		send_to_char(buf, ch);
		return;
	}

	ch->gold -= price;
	victim->gold += price;
	sprintf(buf, "Przekazujesz %s %d kredytów w celu wymazania swojej kartoteki." NL,
				victim->przypadki[2], price);
	send_to_char(buf,ch);

	for (crime = ch->first_crime; crime; crime = n_crime)
	{
		n_crime = crime->next;
		free_crime(ch, crime);
	}
}

DEF_DO_FUN( bail )
{
	char 		buf 	[MAX_STRING_LENGTH];
	char 		arg1 	[MAX_INPUT_LENGTH];
	char 		arg2 	[MAX_INPUT_LENGTH];
	bool		found;
	int			cost;
	int			amount;
	CHAR_DATA *		victim;
	CRIME_DATA *	crime;
	CHAR_DATA *		mob;

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

	if ( arg1[0] == '\0' || arg2[0] == '\0' )
	{
		send_to_char( "Kogo chcesz wykupiæ i za ile?" NL, ch );
		return;
	}

	if( ( ( victim = get_char_world(ch, arg1) ) == NULL ))
	{
		send_to_char("Nie ma nikogo takiego w grze." NL, ch);
		return;
	}

	found = false;
	for ( crime = victim->first_crime; crime; crime = crime->next)
	{
		if( crime->jail_end )
		{
			found = true;
			break;
		}
	}

	if ( !found || IS_NPC( victim ) )
	{
		send_to_char("Ta osoba nie odsiaduje ¿adnego wyroku." NL, ch);
		return;
	}

	found = false;
	for( mob = ch->in_room->first_person; mob; mob = mob->next )
	{
		if( !IS_NPC( mob ) || !can_see( ch, mob ) )
			continue;
		if( ( mob->spec_fun && mob->spec_fun == spec_lookup( "spec_prosecutor" ) )
		||  ( mob->spec_2   && mob->spec_2   == spec_lookup( "spec_prosecutor" ) ) )
		{
			found = true;
			break;
		}
	}

	if( !found )
	{
		send_to_char("Nie ma tu nikogo kto móg³by przyj±æ twoj± kaucjê." NL, ch);
		return;
	}

	amount = atoi( arg2 );
	if( amount <= 0 )
	{
		ch_tell( mob, ch, "Za tyle to ja mogê zapuszkowaæ ciebie! Nie strój sobie ¿artów." );
		return;
	}

	if( amount > ch->gold )
	{
		ch_tell( mob, ch, "A masz tyle? Wróæ jak uzbierasz!" );
		return;
	}

	cost = 30 * victim->top_level + 100 * crime->level -
		3 * cha_app[ get_curr_cha( ch ) ].charm;

	if( amount < cost/10 )
	{
		cost+=cost/10;
		sprintf( buf,
			"Zwariowa³%s¶? Za takie drobniaki nie chce mi siê nawet s³uchaæ tego imienia!",
			SEX_SUFFIX_EAE( ch ) );
		ch_tell( mob, ch, buf );
		sprintf( buf,
			"Daj mi %d kredyt%s, a %s wyjdzie na wolno¶æ jeszcze dzi¶.",
			cost, NUMBER_SUFF( cost, "kê", "ki", "ek" ), victim->name );
		ch_tell( mob, ch, buf );
		return;
	}

	if( amount < cost )
	{
		cost+=cost/10;
		ch_tell( mob, ch, "To za ma³o..." );
		sprintf( buf,
			"Daj mi %d kredyt%s, a %s wyjdzie na wolno¶æ jeszcze dzi¶.",
			cost, NUMBER_SUFF( cost, "kê", "ki", "ek" ), victim->name );
		ch_tell( mob, ch, buf );
		return;
	}

	ch->gold -= amount;

	sprintf( buf, "Zgoda.. Uwolniê %s, ale pod warunkiem, ¿e wiêcej nie z³amie prawa.",
	victim->przypadki[3] );
	ch_tell( mob, ch, buf );
	do_emote( mob, (char *)"wyci±ga komunikator." );
	sprintf( buf, "Tu %s. Macie tam jeszcze %s %s?",
	mob->przypadki[0],
	SEX_STR( ch, "tego", "t±", "to" ),
	victim->przypadki[4] );
	do_say( mob, buf );
	do_say( mob, (char *)"Dobrze... Zwolniæ." );

	/* Tak jest! */
	crime->released = true;

	return;
}

/* zwraca wska¼nik na przestêpstwa pope³nione przez 'ch' na planecie 'planet' */
CRIME_DATA *find_crime( CHAR_DATA *ch, PLANET_DATA *planet )
{
	CRIME_DATA 	*crime;

	IF_BUG( ch==NULL, "" )
		return NULL;

	IF_BUG( planet==NULL, "(%s)", ch->name )
		return NULL;

	IF_BUG( IS_NPC(ch), "(%s)", ch->name )
		return NULL;

	if ( !ch->first_crime )
		return NULL;

	for ( crime = ch->first_crime; crime; crime = crime->next )
		if ( !str_cmp( planet->name, crime->planet ) )
			return crime;

	return NULL;
}


void crime_to_char( CHAR_DATA *ch, char *planet, int type )
{
	CRIME_DATA 	*crime;

	IF_BUG( ch==NULL, "" )
		return;

	IF_BUG( !*planet, "(%s)", ch->name )
		return;

	/*
	* je¶li gracz pope³ni³ ju¿ przestêpstwo na tej planecie --
	* zwiêkszmy wyrok i ewentualnie dodajmy typ do listy pope³nionych
	* wykroczeñ
	*/
	if ( ch->first_crime )
	{
		for ( crime = ch->first_crime; crime; crime = crime->next )
		{
			if ( !str_cmp( crime->planet, planet ) )
			{
				SET_BIT( crime->type, type );
			/*
			* to o ile wskoczy nam wska¼nik przegiêcia zale¿y
			* od typu przestêpstwa - im wiêcej tym gorzej
			*/
				crime->level += type;
				return;
			}
		}
	}

	/*
	* A je¶li nie jest recydywist± -- dopiszmy przestêpstwo do listy
	*/
	CREATE( crime, CRIME_DATA, 1 );
	LINK( crime, ch->first_crime, ch->last_crime, next, prev );
	STRDUP( crime->planet, capitalize( planet ) );
	crime->type		= type;
	crime->level	= type;
	crime->jail_vnum	= 0;
	crime->jail_end	= 0;
	crime->released	= false;
	return;
}

void crime_remove( CHAR_DATA *ch, char *planet, int type )
{
	CRIME_DATA *crime;
	CRIME_DATA *crime_next;

	IF_BUG( ch==NULL, "" )
		return;

	IF_BUG( !*planet, "(%s)", ch->name )
		return;

	if ( !ch->first_crime )
		return;

	for ( crime = ch->first_crime; crime; crime = crime_next )
	{
		crime_next = crime->next;

		if ( !str_cmp( crime->planet, planet ) )
		{
		/*
			* UWAGA !!! tutaj specjalnie jest "==", a nie "IS_SET"
			* bo sprawdzamy, czy jest to OSTATNIA FLAGA
			*/
			if( crime->type == type
			||  crime->type <= 0 ) /* <-- np. CRIME_ANY albo.. bug ;) */
				free_crime( ch, crime );
			else
			{
				REMOVE_BIT( crime->type, type );
				crime->level -= type;
			}
		}
	}
	return;
}

char *crime_flag_string( CRIME_DATA *crime )
{
	static char		crime_buf	[MSL];
	bool 			crime_found   = false;

	crime_buf[0] ='\0';
	if( IS_SET( crime->type, CRIME_SWINDLE ) )
	{
		strcat( crime_buf, "szwindle" );
		crime_found = true;
	}
	if( IS_SET( crime->type, CRIME_HACKING ) )
	{
		if( crime_found )
			strcat( crime_buf, ", " );
		strcat( crime_buf, "w³amania komputerowe" );
		crime_found = true;
	}
	if( IS_SET( crime->type, CRIME_STEAL ) )
	{
		if( crime_found )
			strcat( crime_buf, ", " );
		strcat( crime_buf, "kradzie¿" );
		crime_found = true;
	}
	if( IS_SET( crime->type, CRIME_CONTRABAND ) )
	{
		if( crime_found )
			strcat( crime_buf, " i " );
		strcat( crime_buf, "kontrabandê" );
		crime_found = true;
	}
	if( IS_SET( crime->type, CRIME_MURDER ) )
	{
		if( crime_found )
			strcat( crime_buf, " oraz " );
		strcat( crime_buf, "zabójstwo" );
		crime_found = true;
	}

	if( !crime_found )
	{
		bug( "Did't find a valid crime->type" );
		strcat( crime_buf, "przekrêty wszelkiego rodzaju" );
	}
	return crime_buf;
}

/*funkcja pokazuje, na jakich planetach gracz jest poszukiwany*/
DEF_DO_FUN( wanted )
{
	CHAR_DATA 	*victim;
	CRIME_DATA  *crime;


	if( argument[0] == '\0' )
		victim = ch;
	else
	{
		if( IS_ADMIN( argument ) && !IS_ADMIN( ch->name ) )
		{
			ch_printf(ch,
			"%s jednym z W£ADCÓW TEGO ¦WIATA i nikt nie ma prawa go ¶cigaæ !!!" NL,
			capitalize(argument) );
			return;
		}
		else if( ( ( victim = get_char_world(ch, argument) ) == NULL ))
		{
			send_to_char("Nie ma nikogo takiego w grze." NL, ch);
			return;
		}
	}

	if( !IS_NPC( victim ) )
	{
		if( IS_ADMIN( victim->name ) && !IS_ADMIN( ch->name ) )
		{
			ch_printf(ch,
			"%s jednym z W£ADCÓW TEGO ¦WIATA i nikt nie ma prawa go ¶cigaæ !!!" NL,
			capitalize(argument) );
			return;
		}

		if ( !victim->first_crime )
		{
			victim == ch ?
				send_to_char( "Nie poszukuj± ciê na ¿adnej planecie." NL, ch ) :
				send_to_char( "Ta osoba nie jest nigdzie poszukiwana." NL, ch );
		}
		else
		{
			ch_printf( ch, FB_WHITE
			"Oto lista planet, na których %s jest%s poszukiwan%s:" EOL,
			victim == ch ? ""   : victim->przypadki[0],
			victim == ch ? "e¶" : "",
			SEX_SUFFIX_YAE( victim ) );

			for ( crime = victim->first_crime; crime; crime = crime->next)
			{
				PLANET_DATA	*planet = NULL;

				if( crime->jail_vnum )
					planet = get_room_index( crime->jail_vnum )->area->planet;

				ch_printf( ch, "- %s za %s - przewiduje siê%s." NL,
				crime->planet,
				crime_flag_string( crime ),
				crime->level <= 4  ? " ³agodn± karê wiêzienia"            :
				crime->level <= 8  ? " karê wiêzienia"                    :
				crime->level <= 16 ? " wysok± karê wiêzienia"             :
				crime->level <= 32 ? " konkretn± odsiadkê"                :
				crime->level <= 64 ? ", ¿e trochê posiedzisz"  	      :
				crime->level <=128 ? " przygotowanie ci oddzielnej celi"  :
							" DO¯YWOCIE"  );

				if( crime->jail_end )
					ch_printf( ch , NL FG_CYAN
				"Kara, któr± %s w³a¶nie odsiaduje%s na %s "
				"zakoñczy siê za %d godzi%s."			EOL,
					victim == ch ? "" 		: victim->przypadki[0],
					victim == ch ? "sz" 	: "",
					planet ? planet->name 	: "Generalnej Planecie Wiêziennej",
					crime->jail_end,
					NUMBER_SUFF(crime->jail_end, "ne", "ny", "n" ) );
			}
		}
		return;
	}
	send_to_char( "Ta osoba nie jest nigdzie poszukiwana." NL, ch );
	return;
}


/*
* WIÊZIENIA					(C) 2002 Thanos ;)
*/

/* zwraca losowo wybran± celê w wiêzieniu */
ROOM_INDEX_DATA *find_jail_cell( PLANET_DATA *planet )
{
	ROOM_INDEX_DATA	*	cell;
	int					cell_vnum;
	int			 		one_cell;
	int			 		found		= 0;

	/* liczymy ile cel ma wiêzienie */
	if ( planet )
		for( cell_vnum = planet->first_jail; cell_vnum <= planet->last_jail; cell_vnum++ )
		{
			if( ( cell = get_room_index( cell_vnum ) ) != NULL
			&&    cell->sector_type == SECT_JAIL_CELL )
				found++;
		}
	/* printf( "found %d cells" NL, found ); */
	if( !found )
	{
		bug( "Ooops: NO CELLS ON PLANET'S JAIL !!!" );
		return get_room_index( ROOM_VNUM_DEFAULT_CELL );
	}

	/* losujemy jedn± */
	one_cell 	= number_range( 1, found );
	/* printf( "choosed %d of %d" NL, one_cell, found ); */
	found 	= 0;
	for( cell_vnum = planet->first_jail; cell_vnum <= planet->last_jail; cell_vnum++ )
	{
		if( ( cell = get_room_index( cell_vnum ) ) != NULL
		&&    cell->sector_type == SECT_JAIL_CELL )
		{
			found++;
			if( found == one_cell )
				return cell;
		}
	}

	/* tu nas byæ nie powinno */
	bug( "Something's wrong" );
	return get_room_index( ROOM_VNUM_DEFAULT_CELL );
}

/* zwraca losowo wybrane biuro wiêzienia */
ROOM_INDEX_DATA *find_jail_office( PLANET_DATA *planet )
{
	ROOM_INDEX_DATA	*office;
	int			 office_vnum;
	int			 one_office;
	int			 found		= 0;

	/* liczymy ile cel ma wiêzienie */
	if ( planet )
		for( office_vnum = planet->first_jail; office_vnum <= planet->last_jail; office_vnum++ )
		{
			if( ( office = get_room_index( office_vnum ) ) != NULL
			&&    office->sector_type == SECT_JAIL_OFFICE )
				found++;
		}
	/* printf( "found %d offices" NL, found ); */
	if( !found )
	{
		bug( "Ooops: NO OFFICE ON PLANET'S JAIL !!!" );
		return get_room_index( ROOM_VNUM_DEFAULT_OFFICE );
	}

	/* losujemy jedno */
	one_office 	= number_range( 1, found );
	/* printf( "choosed %d of %d" NL, one_office, found ); */
	found 	= 0;
	for( office_vnum = planet->first_jail; office_vnum <= planet->last_jail; office_vnum++ )
	{
		if( ( office = get_room_index( office_vnum ) ) != NULL
		&&    office->sector_type == SECT_JAIL_OFFICE )
		{
			found++;
			if( found == one_office )
				return office;
		}
	}

	/* tu nas byæ nie powinno */
	bug( "Something's wrong" );
	return get_room_index( ROOM_VNUM_DEFAULT_OFFICE );
}

/* zwraca losowo wybrane biuro wiêzienia */
ROOM_INDEX_DATA *find_jail_entrance( PLANET_DATA *planet )
{
	ROOM_INDEX_DATA	*entrance;
	int			 entrance_vnum;
	int			 one_entrance;
	int			 found		= 0;

	/* liczymy ile wej¶æ ma wiêzienie */

/* Chodzi o to planet->first_jail. Jest taka mozliwosc, ze do krainki
nie ma przypisanej zadnej planety, wtedy planet jest NULL i mud pada.
Do jail.are przypisalem planete i juz nie pada ale mimo wszystko trzeba
to jakos ciekawie w ponizszym kodzie rozwiazac a ja nie mam pomyslu
*/
	/* Ok - sprobujmy tak: */
	if ( planet )             /* i starczy :P */
		for( entrance_vnum = planet->first_jail; entrance_vnum <= planet->last_jail; entrance_vnum++ )
		{
			if( ( entrance = get_room_index( entrance_vnum ) ) != NULL
			&&    entrance->sector_type == SECT_JAIL_ENTRANCE )
				found++;
		}
	/* printf( "found %d offices" NL, found ); */
	if( !found )
	{
		bug( "Ooops: NO ENTRANCE ON PLANET'S JAIL !!!" );
		return get_room_index( ROOM_VNUM_DEFAULT_ENTRANCE );
	}

	/* losujemy jedno */
	one_entrance= number_range( 1, found );
	/* printf( "choosed %d of %d" NL, one_office, found ); */
	found 	= 0;
	for( entrance_vnum = planet->first_jail; entrance_vnum <= planet->last_jail; entrance_vnum++ )
	{
		if( ( entrance = get_room_index( entrance_vnum ) ) != NULL
		&&    entrance->sector_type == SECT_JAIL_ENTRANCE )
		{
			found++;
			if( found == one_entrance )
				return entrance;
		}
	}

	/* tu nas byæ nie powinno */
	bug( "Something's wrong" );
	return get_room_index( ROOM_VNUM_DEFAULT_ENTRANCE );
}

/* zwraca szafê w biurze */
OBJ_DATA *find_jail_closet( CHAR_DATA *ch, PLANET_DATA *planet )
{
	ROOM_INDEX_DATA	*office;
	OBJ_DATA		*closet;

	office = find_jail_office( planet );

	if( office )
	{
		for ( closet = office->first_content;
			closet;
			closet = closet->next_content )
		{
			if( closet && closet->pIndexData->vnum == OBJ_VNUM_JAIL_CLOSET )
				return closet;
		}
	/* w razie gdyby kto¶ zniszczy³ szafê ;) */
		closet = create_object( get_obj_index( OBJ_VNUM_JAIL_CLOSET ), MAX_LEVEL );
		obj_to_room( closet, office );
		return closet;
	}

	/* je¶li co¶ poszlo nie tak, spróbujmy to naprawiæ */
	bug( "Fatal --> NO OFFICE, putting closet at ch->in_room" );
	closet = create_object( get_obj_index( OBJ_VNUM_JAIL_CLOSET ), MAX_LEVEL );
	obj_to_room( closet, ch->in_room );
	return closet;
}

/* tak - je¶li przedmiot jest depozytem */
bool is_deposit( OBJ_DATA *obj )
{
	DESCRIPTOR_DATA 	*d;
	CHAR_DATA		*ch;

	for ( d = first_descriptor; d; d = d->next )
	{
		if( ( ch = CH( d ) ) == NULL )
			continue;
		if( IS_NPC( ch ) ) /* ciekawe czy co¶ takiego mo¿e siê zdarzyæ ;) */
			continue;
		if( !ch->first_crime )
			continue;

		if( ch->deposit && ch->deposit == obj )
			return true;
	}
	return false;
}

/* zwraca w³a¶ciciela danego depozytu */
CHAR_DATA *deposit_owner( OBJ_DATA *obj )
{
	DESCRIPTOR_DATA 	*d;
	CHAR_DATA		*ch;

	for ( d = first_descriptor; d; d = d->next )
	{
		if( ( ch = CH( d ) ) == NULL )
			continue;
		if( IS_NPC( ch ) ) /* ciekawe czy co¶ takiego mo¿e siê zdarzyæ ;) */
			continue;
		if( !ch->first_crime )
			continue;

		if( ch->deposit && ch->deposit == obj )
			return ch;
	}
	return NULL;
}

/*
* Jedna z bardziej lamerskich f-cji w tym kodzie :
*/
OBJ_DATA *prepare_deposit( CHAR_DATA *ch )
{
	OBJ_DATA	*deposit;
	char	buf		[MSL];

	deposit = create_object(
	get_obj_index( OBJ_VNUM_DEPOSIT ),
	ch->top_level );

	sprintf( buf, "depozyt %s", ch->przypadki[1] );
	STRDUP( deposit->name, buf );
	STRDUP( deposit->przypadki[0], buf );
	sprintf( buf, "depozytu %s", ch->przypadki[1] );
	STRDUP( deposit->przypadki[1], buf );
	sprintf( buf, "depozytowi %s", ch->przypadki[1] );
	STRDUP( deposit->przypadki[2], buf );
	sprintf( buf, "depozyt %s", ch->przypadki[1] );
	STRDUP( deposit->przypadki[3], buf );
	sprintf( buf, "depozytem %s", ch->przypadki[1] );
	STRDUP( deposit->przypadki[4], buf );
	sprintf( buf, "depozycie %s", ch->przypadki[1] );
	STRDUP( deposit->przypadki[5], buf );

	sprintf( buf,
	"Depozyt %s najwyra¼niej siê tutaj zawieruszy³.", ch->przypadki[1] );
	STRDUP( deposit->description, buf );

	ch->deposit 		= deposit; /*zapamiêtajmy wska¼nik*/

	STRDUP( deposit->owner_name, ch->name );

	/* ufffff */
	return deposit;
}

/*
* Zapisujemy depozyt gracza w pliku ../deposit/<imie_gracza>.deposit
*/
void save_deposit( CHAR_DATA *ch )
{
	if ( ch && ch->deposit )
	{
		FILE *	fp;
		char 	filename	[256];
		int 	templvl;

		sprintf( filename, "%s%s.deposit", DEPOSIT_DIR,
			capitalize( ch->name ) );

		if ( ( fp = fopen( filename, "w" ) ) == NULL )
		{
			bug( "fopen" );
			perror( filename );
		}
		else
		{
			templvl = ch->top_level;
			ch->top_level = LEVEL_HERO;
			if( ch->deposit )
				fwrite_obj(ch, ch->deposit, fp, 0, OS_CARRY );
			fprintf( fp, "#END\n" );
			ch->top_level = templvl;
			fclose( fp );
		}
	}
	return;
}

/* ³aduje depozyt z pliku */
void fread_deposit( CHAR_DATA *ch, OBJ_DATA *closet )
{
	FILE 	*fp;
	OBJ_DATA 	*obj;
	OBJ_DATA	*obj_next;
	char 	 filename	[256];
	int	 	 found		= 0;

	IF_BUG( closet==NULL, "" )
		return;

	IF_BUG( ch==NULL, "" )
		return;


	sprintf( filename, "%s%s.deposit", DEPOSIT_DIR, capitalize( ch->name ) );

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
				bug( "# not found. (%s)", ch->name );
				break;
			}

			word = fread_word( fp );
			if ( !str_cmp( word, "OBJECT" ) )
			{
				/*
				* niech nam pomo¿e ten, który wiecznie siê obija  ;)
				*/
				set_supermob( closet );
				fread_obj  ( supermob, fp, OS_CARRY );

				for ( obj = supermob->first_carrying; obj; obj = obj_next )
				{
					obj_next = obj->next_content;
					obj_from_char( obj );
					obj_to_obj( obj, closet );

					if( obj->pIndexData->vnum == OBJ_VNUM_DEPOSIT )
					{
						found++;
						ch->deposit = obj;
					}
				}

				if( found > 1 )
					bug( "Duplicate deposit!" );
				release_supermob();
			}
			else if ( !str_cmp( word, "END"    ) )
				break;
			else
			{
				bug( "bad section. %s", ch->name );
				break;
			}
		}
		fclose( fp );
	}
	return;
}

void load_deposit( CHAR_DATA *ch )
{
	CRIME_DATA		*crime;

	if( !ch || IS_NPC( ch ) || !ch->first_crime )
		return;

	for( crime = ch->first_crime; crime; crime = crime->next )
	{
		if( crime->jail_vnum && crime->jail_end )
		{
			fread_deposit( ch,
			find_jail_closet( ch, get_planet(crime->planet) ) );
			return;
		}
	}
	return;
}

/* niszczy wska¼nik na depozyt gracza oraz plik, w którym by³ zapisany
dobytek */
void destroy_deposit( CHAR_DATA *ch )
{
	char 	buf	[256];

	IF_BUG( ch==NULL, "" )
		return;

	if( !ch->deposit )
		return;

	sprintf( buf, "%s%s.deposit", DEPOSIT_DIR, capitalize( ch->name ) );
	unlink ( buf );
	extract_obj ( ch->deposit ); /* wiêc lepiej to najpierw opró¿niæ */
	ch->deposit = NULL;
	save_char_obj( ch );
	return;
}


/*
* Funkcja 'aresztuje' gracza. Przenosi go do wiêzienia,
* ustala wyrok, przerzuca inventory gracza do depozytu. 	Thanos
*/
void jail_char( CHAR_DATA *victim, CHAR_DATA *policeman, CRIME_DATA *crime )
{
	int 	         time;
	OBJ_DATA 		*obj;
	OBJ_DATA 		*obj_next;
	OBJ_DATA 		*closet;
	OBJ_DATA 		*deposit;
	ROOM_INDEX_DATA 	*jail;
	PLANET_DATA		*planet;

	if( !victim || IS_NPC( victim ) || !policeman || !crime )
		return;

	if( ( planet = get_planet( crime->planet ) ) == NULL )
		return;

	act( COL_ACTION,
		"$n aresztuje $N$3 i zabiera $I do aresztu w imieniu prawa.",
		policeman, NULL, victim, TO_NOTVICT );
	act( COL_ACTION,
		"$n zabiera ciê do aresztu w imieniu prawa.",
		policeman, NULL, victim, TO_VICT    );

	/* najpierw przenie¶my gracza */
	jail 			= find_jail_cell( planet );
	char_from_room		( victim );
	char_to_room		( victim, jail );

	do_look( victim, (char *)"auto" );

	deposit = prepare_deposit( victim );

	/* rozbieramy gracza */
	for ( obj = victim->first_carrying; obj; obj = obj_next )
	{
		obj_next = obj->next_content;
		obj_from_char( obj );
		if ( IS_OBJ_STAT( obj, ITEM_INVENTORY )  )
			extract_obj( obj );  		/* niektóre rzeczy musz± znikn±æ */
		else
		{
			deposit->value[0] 	+= obj->weight;	/* zwiêkszamy pojemno¶æ */
			obj_to_obj( obj, deposit ); 	/* wsio do depozytu */
		}
	}
	/* i do szafy;) */
	closet 			= find_jail_closet( victim, planet );
	closet->value[0]   	       += deposit->weight;/* zwiêkszamy pojemno¶æ */
	obj_to_obj			( deposit, closet );

	/* jak wk³adaj± depozyt to niech zamkn± na wszelki szafe */
	SET_BIT			( closet->value[1], CONT_CLOSEABLE );
	SET_BIT			( closet->value[1], CONT_CLOSED );
	SET_BIT			( closet->value[1], CONT_LOCKED );
	SET_BIT			( closet->value[1], CONT_PICKPROOF );

	send_to_char( NL "Twoje rzeczy zosta³y zabrane do depozytu." NL, victim );

	/* ¿egnajcie pieni±¿ki */
	victim->gold 		= 0;
	send_to_char( "Twoje kredytki zosta³y skonfiskowane." NL, victim );

	/* kara zale¿y od poziomu przewinienia ( w tickach )*/
	time			= crime->level + 1 * number_range( 45, 75 );
	time 			= UMAX( 1, time/4 );
	time		       /= 2;
	crime->jail_end		= time;
	crime->jail_vnum 		= jail->vnum;


	ch_printf( victim, "Zostaniesz w areszcie przez %d godzi%s." NL,
	time, NUMBER_SUFF( time, "nê", "ny", "n"  ) );

	save_char_obj( victim ); /* z tej f-cji zapisze siê te¿ depozyt */
	/* po sprawie  :) */
	return;
}

/* uwalniamy gracza -- je¶li ju¿ odsiedzia³ swoje */
void unjail_char( CHAR_DATA *ch, CRIME_DATA *crime )
{
	PLANET_DATA	*planet = NULL;

	if( !ch || !crime )
	return;

	if( crime->jail_vnum )
		planet = get_room_index( crime->jail_vnum )->area->planet;

	ch->position = POS_STANDING;

	echo_to_room( ch->in_room, PLAIN
	"Stra¿nik wiêzienia podchodzi do drzwi twojej celi..." );

	echo_to_room( ch->in_room, EOL
	"Drzwi otwieraj± siê." EOL );

	act( COL_SAY, "Stra¿nik mówi '$n - pójdziesz ze mn±.'", ch, NULL, NULL, TO_ROOM );
	act( COL_SAY, "Stra¿nik mówi '$n - pójdziesz ze mn±.'", ch, NULL, NULL, TO_CHAR );

	act( PLAIN,"Stra¿nik wyprowadza $n$3 przed gmach wiêzienia.", ch, NULL, NULL, TO_ROOM );
	act( PLAIN,"Stra¿nik wyprowadza ciê przed gmach wiêzienia.", ch, NULL, NULL, TO_CHAR );

	char_from_room	( ch );

	echo_to_room( get_room_index( crime->jail_vnum ), EOL
	"Drzwi zamykaj± siê siê." EOL );

	char_to_room	( ch, find_jail_entrance( planet ) );
	do_look		( ch, (char *)"auto" );

	act( COL_SAY, "Stra¿nik mówi 'Jeste¶ woln$y $n.'", ch, NULL, NULL, TO_ROOM );
	act( COL_SAY, "Stra¿nik mówi 'Jeste¶ woln$y $n.'", ch, NULL, NULL, TO_CHAR );

	if( crime->released )
	{
		act( COL_SAY, "Stra¿nik mówi 'Kto¶ wp³aci³ za ciebie kaucjê.'", ch, NULL, NULL, TO_ROOM );
		act( COL_SAY, "Stra¿nik mówi 'Kto¶ wp³aci³ za ciebie kaucjê.'", ch, NULL, NULL, TO_CHAR );
	}
	else
	{
		act( COL_SAY, "Stra¿nik mówi 'Twoja odsiadka u nas siê skoñczy³a.'", ch, NULL, NULL, TO_ROOM );
		act( COL_SAY, "Stra¿nik mówi 'Twoja odsiadka u nas siê skoñczy³a.'", ch, NULL, NULL, TO_CHAR );
	}

	act( COL_SAY, "Stra¿nik mówi 'Oby¶ nie wraca³$o do nas wiêcej.'", ch, NULL, NULL, TO_ROOM );
	act( COL_SAY, "Stra¿nik mówi 'Oby¶ nie wraca³$o do nas wiêcej.'", ch, NULL, NULL, TO_CHAR );

	ch_printf( ch, NL FB_WHITE "Wychodzisz na WOLNO¦Æ!" NL EOL );

	/* chwila napiêcia */
	if( ch->deposit )
	{
		act( COL_SAY,
		"Stra¿nik mówi 'By³bym zapomnia³. Twój depozyt zostawiam ci do dyspozycji.'",
		ch, NULL, NULL, TO_CHAR );
		empty_obj( ch->deposit, NULL, ch->in_room );
	}
	else
	{
		act( COL_SAY,
		"Stra¿nik mówi 'A je¶li chodzi o twój depozyt... Có¿. Gdzie¶ siê zawieruszy³.'",
		ch, NULL, NULL, TO_CHAR );
	}

	act( PLAIN, "Stra¿nik odchodzi by dalej wykonywaæ swoje obowi±zki.", ch, NULL, NULL, TO_ROOM );
	act( PLAIN, "Stra¿nik odchodzi by dalej wykonywaæ swoje obowi±zki.", ch, NULL, NULL, TO_CHAR );

	free_crime		( ch, crime );
	destroy_deposit	( ch );
	save_char_obj	( ch );
	/* i po sprawie ;) */
	return;
}



/* sprawdza czy gracz 'dobrze' odsiaduje ;) */
void crime_update( void )
{
	DESCRIPTOR_DATA 	*d;
	CHAR_DATA		*ch;
	CRIME_DATA		*crime;
	CRIME_DATA		*crime_next;

	for ( d = first_descriptor; d; d = d->next )
	{
		if( ( ch = CH( d ) ) == NULL )
			continue;

		if( IS_NPC( ch ) )
			continue;

		if( !ch->first_crime )
			continue;

		for ( crime = ch->first_crime; crime; crime = crime_next )
		{
			crime_next	= crime->next;

			if( crime->jail_end > 0 )
				crime->jail_end--;

			if( crime->jail_end < 0 )
				crime->jail_end = 0;

			/* koniec kary - freedom */
			if( ( crime->jail_vnum && ch->in_room
			&&  crime->jail_vnum == ch->in_room->vnum
			&& !crime->jail_end ) || crime->released )
			{
				unjail_char( ch, crime );
				break;
			}

			/* te¿ freedom, ale inny ;) */
			if( crime->jail_end  %  JAIL_ACTIVITY == 0
			&&  crime->jail_vnum && ch->in_room
			&&  crime->jail_vnum != ch->in_room->vnum )
			{
				ch_printf( ch, FB_WHITE
					"W³adze %s w³a¶nie zorientowa³y siê o twojej ucieczce!" EOL,
					crime->planet );

				crime->jail_vnum = 0;
				crime->jail_end  = 0;
				crime->level	*= 2;	/* recydywa */

				/* a co to? ucieka i jeszcze chce rzeczy? ;) */
				destroy_deposit( ch );
				break;
			}

		}
	}
}
