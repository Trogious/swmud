/***************************************************************************
* Star Wars Reality Code Additions and changes from the Smaug Code         *
* copyright (c) 1997 by Sean Cooper                                        *
* Starwars and Starwars Names copyright(c) Lucasfilm Ltd.                  *
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
*		   New Star Wars Skills Unit    			   *
****************************************************************************/

#include <math.h>
#include <sys/types.h>
#include <ctype.h>
#include <time.h>
#include "mud.h"

void    add_reinforcements  args( ( CHAR_DATA *ch ) );
ch_ret  one_hit             args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
int     xp_compute                ( CHAR_DATA *ch , CHAR_DATA *victim );
ROOM_INDEX_DATA *generate_exit( ROOM_INDEX_DATA *in_room, EXIT_DATA **pexit );
int ris_save( CHAR_DATA *ch, int chance, int ris );
CHAR_DATA *get_char_room_mp( CHAR_DATA *ch, char *argument );
void 	hijack		    args( ( CHAR_DATA *ch, SHIP_DATA *ship ) );//Aldegard

extern int      top_affect;


/*
 * Do skilli make dodaæ takie rzeczy:
 *
 * - avail_locs rozwinac tak, zeby f-cje same sprawdzaly jaki skill tyczy
 *   sie jakiej czesci ciala. Np. jak wpisze makeconta uszy dupa, to f-cja
 *   ma mi powiedzieæ: sprobuj makejewelry
 * - po avail_loc'u ustalac przedrostek nazwy (naramienniki, naszyjnik, plecak)
 * - jakis system ustalania gendera (moze tez w tej strukturze)
 * - zrobic podreczne f-cje sprawdzajace, czy gracz ma klamot typu...
 * - makra do zabierania klamotu/zdejmowania value
 *
 * (najlepiej by³oby zrobiæ strukture globalna)
 *
 * Thanos
 */

struct	obj_def_name	/* defaultowe nazwy przedmiotu */
{
    int		loc;			// na co?
    int		gender;			// jaka p³eæ
    const char *	name[6];		// nazwa
};

struct skill_loc	/* pojedynczy skill */
{
    const char *		name;		// jaki
    struct obj_def_name	locs[32];	// na co mo¿na za³o¿yæ zrobiony skillem klamot
};

/*
 * Wiem, wiem.. Nie jest to najbardziej przejrzyste. Ale podnosi stabilno¶æ i
 * zmniejszy objêto¶æ pliku. Poza tym.. nie widzê innej mo¿liwo¶ci spolszczenia
 * tych skilli.
 * Analiza tej tablicy pomaga w rozwijaniu wyobra¼ni przestrzennej ;)
 * Thanos
 */
const	struct	skill_loc	skill_loc_list[ ]	=
{
    {	"makejewelry",    		{
		{ ITEM_WEAR_EYES,  	GENDER_PLURAL, 	{
		    "okulary",		"okularów",
		    "okularom",		"okulary",
		    "okularami",	"okularach"	}	},
		{ ITEM_WEAR_EARS,	GENDER_PLURAL, 	{
		    "kolczyki",		"kolczyków",
		    "kolczykom",	"kolczyki",
		    "kolczykami",	"kolczykach",	}	},
		{ ITEM_WEAR_FINGER,	GENDER_MALE, 	{
		    "pier¶cionek",	"pier¶cionka",
		    "pier¶cionkowi",	"pier¶cionek",
		    "pier¶cionkiem",	"pier¶cionku",	}	},
		{ ITEM_WEAR_NECK,	GENDER_MALE, 	{
		    "naszyjnik",	"naszyjnika",
		    "naszyjniku",	"naszyjnik",
		    "naszyjnikiem",	"naszyjniku",	}	},
		{ 0, 0, {"","","","","",""}    	},    	}	},
    {	"makearmor",		{
		{ ITEM_WEAR_FEET,  	GENDER_PLURAL, {
		    "buty",		"butów",
		    "butom",		"buty",
		    "butami",		"butach"	}	},
		{ ITEM_WEAR_LEGS,  	GENDER_PLURAL, 	{
		    "spodnie",		"spodni",
		    "spodniom",		"spodnie",
		    "spodniami",	"spodniach"	}	},
		{ ITEM_WEAR_HEAD,  	GENDER_MALE, 	{
		    "he³m",		"he³mu",
		    "he³mowi",		"he³m",
		    "he³mem",		"he³mie"	}	},
		{ ITEM_WEAR_HANDS, 	GENDER_PLURAL, 	{
		    "rêkawice",		"rêkawic",
		    "rêkawicom",	"rêkawice",
		    "rêkawicami",	"rêkawicach"	}	},
		{ ITEM_WEAR_HANDS, 	GENDER_PLURAL, 	{
		    "naramienniki",	"naramienników",
		    "naramiennikom",	"naramienniki",
		    "naramiennikami",	"naramiennikach"}	},
		{ 0, 0, {"","","","","",""}    	},    	}	},
    {	"makeshield",		{
		{ ITEM_WEAR_SHIELD, 	GENDER_FEMALE, 	{
		    "tarcza",		"tarczy",
		    "tarczy",		"tarczê",
		    "tarcz±",		"tarczy" 	}	},
		{ 0, 0, {"","","","","",""}    	},    	}	},
    {	"makeblaster",		{
		{ ITEM_WIELD, 		GENDER_MALE, 	{
		    "blaster",		"blastera",
		    "blasterowi",	"blaster",
		    "blasterem",	"blasterze" 	}	},
		{ 0, 0, {"","","","","",""}    	},    	}	},
    {	"makeblade",		{
		{ ITEM_WIELD, 		GENDER_NEUTRAL, {
		    "vibroostrze",	"vibroostrza",
		    "vibroostrzu",	"vibroostrze",
		    "vibroostrzem",	"vibroostrzu" 	}	},
		{ 0, 0, {"","","","","",""}    	},    	}	},
    {	"makelightsaber",	{
		{ ITEM_WIELD, 		GENDER_MALE, 	{
		    "miecz ¶wietlny",	"miecza ¶wietlnego",
		    "mieczowi ¶wietlnemu","miecz ¶wietlny",
		    "mieczem ¶wietlnym","mieczu ¶wietlnym"}	},
		{ 0, 0, {"","","","","",""}    	},    	}	},
    {	"makeflashlight",	{
		{ 1000, GENDER_FEMALE, 		{	// !!! Tak ma byc
		    "latarka",		"latarki",
		    "latarce",		"latarkê",
		    "latark±",		"latarce"}	},
		{ 0, 0, {"","","","","",""}    	},    	}	},
    {	"makecontainer",	{
		{ ITEM_WEAR_BODY, GENDER_MALE, 		{
		    "plecak",		"plecaka",
		    "plecakowi",	"plecak",
		    "plecakiem",	"plecaku" 	}	},
		{ ITEM_WEAR_WAIST, GENDER_MALE, 	{
		    "pas",		"pasa",
		    "pasowi",		"pas",
		    "pasem",		"pasie" 	} 	},
		{ ITEM_HOLD, GENDER_FEMALE, 		{
		    "torba",		"torby",
		    "torbie",		"torbê",
		    "torb±",		"torbie" 	} 	},
		{ ITEM_WEAR_WRIST, GENDER_MALE, 	{
		    "schowek",		"schowka",
		    "schowkowi",	"schowek",
		    "schowkiem",	"schowku" 	} 	},
		{ 0, 0, {"","","","","",""}    	},    	}	},

    {	"",			{
		{ 0, 0,	{"","","","","",""}	}	}    	}
};	// dopisac jeszcze makemissile i makegrenade (tak jak makeflashlight)

int get_skill_by_loc( int loc )
{
    int s, l;

    for( s=0; skill_loc_list[s].name; s++ )
    {
	for( l=0; skill_loc_list[s].locs[l].loc!=0; l++ )
	    if( skill_loc_list[s].locs[l].loc==loc )
		return s;
    }
    return -1;
}



DEF_DO_FUN( makeblade )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int level, chance;
    int charge = 0;
    bool checktool, checkdura, checkbatt, checkoven;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    int vnum;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf2;
    strcpy( arg , argument );

    switch( ch->substate )
    {
    	default:

	    if ( arg[0] == '\0' )
            {
    		send_to_char( "Sk³adnia: Makeblade <nazwa>" NL, ch);
		return;
            }

    	    checktool = false;
            checkdura = false;
            checkbatt = false;
            checkoven = false;

            if ( !IS_SET( ch->in_room->room_flags, ROOM_FACTORY ) )
            {
        	send_to_char( FB_RED "Musisz byæ w fabryce albo warsztacie aby to zrobiæ." EOL, ch);
        	return;
            }

	    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
            {
        	if (obj->item_type == ITEM_TOOLKIT)
            	    checktool = true;
		if (obj->item_type == ITEM_DURASTEEL)
          	    checkdura = true;
		if (obj->item_type == ITEM_BATTERY)
            	    checkbatt = true;
		if (obj->item_type == ITEM_OVEN)
            	    checkoven = true;
            }

            if ( !checktool )
            {
        	send_to_char( FB_RED "Potrzebujesz skrzynki z narzêdziami aby skonstruowaæ vibro-ostrze." EOL, ch);
		return;
            }

            if ( !checkdura )
            {
               send_to_char( FB_RED "Potrzebujesz durastali, z której mo¿na to zrobiæ." EOL, ch);
               return;
            }

            if ( !checkbatt )
            {
               send_to_char( FB_RED "Potrzebujesz ¼ród³a energii." EOL, ch);
               return;
            }

	    if ( !checkoven )
            {
        	send_to_char( FB_RED "Bêdzie Ci potrzebny ma³y piec do zrobienia tego." EOL, ch);
        	return;
            }

	    chance = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makeblade]);

            if ( number_percent( ) < chance )
	    {
    		send_to_char( FB_GREEN "Rozpoczynasz d³ugi proces tworzenia vibro-ostrza." EOL, ch);
		act( PLAIN, "$n bierze $s narzêdzia oraz ma³y piec i zaczyna nad czym¶ pracowaæ.",
			ch, NULL, argument , TO_ROOM );
		add_timer ( ch , TIMER_DO_FUN , 25 , do_makeblade , 1 );
		STRDUP( ch->dest_buf,arg);
    		return;
	    }

	    send_to_char(FB_RED "Nie jeste¶ w stanie z³o¿yæ tych wszystkich czê¶ci do kupy." EOL,ch);
	    learn_from_failure( ch, gsn_makeblade );
	    WAIT_STATE( ch, skill_table[gsn_makeblade]->beats/2 );
    	    return;

    	case 1:
	    if ( !*ch->dest_buf )
    		return;
    	    strcpy(arg, ch->dest_buf);
    	    STRDUP( ch->dest_buf, "" );
    	    break;

    	case SUB_TIMER_DO_ABORT:
    	    break_skill(FB_RED "Co¶ przeszkadza Ci w pracy i nie mo¿esz jej ukoñczyæ." EOL, ch);
    	    return;
    }

    ch->substate = SUB_NONE;

    level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makeblade]);
    vnum = 10422;

    if ( ( pObjIndex = get_obj_index( vnum ) ) == NULL )
    {
	send_to_char( FB_RED
	    "Przedmiot, który próbujesz stworzyæ nie znajduje siê w bazie danych muda." EOL
	    "Zg³o¶ to niezw³ocznie do administratorów albo poczt± na adres mud@swmud.pl." EOL, ch );
	bug( "Counldn't create obj (%d) for skill", vnum );
        return;
    }

    checktool = false;
    checkdura = false;
    checkbatt = false;
    checkoven = false;

    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
    {
	if (obj->item_type == ITEM_TOOLKIT)
	    checktool = true;
	if (obj->item_type == ITEM_OVEN)
	    checkoven = true;
	if (obj->item_type == ITEM_DURASTEEL && checkdura == false)
        {
    	    checkdura = true;
            separate_obj( obj );
            obj_from_char( obj );
            extract_obj( obj );
        }
	if (obj->item_type == ITEM_BATTERY && checkbatt == false )
        {
    	    charge = UMAX( 5, obj->value[0] );
    	    separate_obj( obj );
	    obj_from_char( obj );
	    extract_obj( obj );
	    checkbatt = true;
	}
    }

    chance = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makeblade]) ;

    if ( number_percent( ) > chance*2  || ( !checktool ) || ( !checkdura ) || ( !checkbatt ) || ( !checkoven ) )
    {
	send_to_char( FB_RED "W³±czasz nowo zrobione vibro-ostrze." EOL, ch);
	send_to_char( FB_RED "Buczenie z pocz±tku jest s³abe lecz pó¼niej wzmaga siê." EOL, ch);
	send_to_char( FB_RED "chwilê pó¼niej roztrzaskuje siê na tysi±ce kawa³ków." EOL, ch);
	learn_from_failure( ch, gsn_makeblade );
	return;
    }

    obj = create_object( pObjIndex, level );

    obj->item_type = ITEM_WEAPON;
    SET_BIT( obj->wear_flags, ITEM_WIELD );
    SET_BIT( obj->wear_flags, ITEM_TAKE );
    obj->level = level;
    obj->weight = 3;

    strcpy( buf, nocolor(arg) );
    strcat( buf, " vibroostrze ostrze" );
    STRDUP( obj->name, buf );

    sprintf( buf, "vibroostrze %s", arg );
    STRDUP( obj->przypadki[0], buf );

    sprintf( buf, "vibroostrza %s", arg );
    STRDUP( obj->przypadki[1], buf );

    sprintf( buf, "vibroostrzu %s", arg );
    STRDUP( obj->przypadki[2], buf );

    sprintf( buf, "vibroostrze %s", arg );
    STRDUP( obj->przypadki[3], buf );

    sprintf( buf, "vibroostrzem %s", arg );
    STRDUP( obj->przypadki[4], buf );

    sprintf( buf, "vibroostrzu %s", arg );
    STRDUP( obj->przypadki[5], buf );

    sprintf( buf, "Kto¶ zostawi³ tu %s", obj->przypadki[3] );
    STRDUP( obj->description, buf );

    CREATE( paf, AFFECT_DATA, 1 );
    paf->type               = -1;
    paf->duration           = -1;
    paf->location           = flag_value( apply_types_list, "backstab" );
    paf->modifier           = level/3;
    paf->bitvector          = 0;
    paf->next               = NULL;
    LINK( paf, obj->first_affect, obj->last_affect, next, prev );
    ++top_affect;
    CREATE( paf2, AFFECT_DATA, 1 );
    paf2->type               = -1;
    paf2->duration           = -1;
    paf2->location           = flag_value( apply_types_list, "hitroll" );
    paf2->modifier           = -2;
    paf2->bitvector          = 0;
    paf2->next               = NULL;
    LINK( paf2, obj->first_affect, obj->last_affect, next, prev );
    ++top_affect;
    obj->value[0] = INIT_WEAPON_CONDITION;
    obj->value[1] = (int) (level/20+10);      /* min dmg  */
    obj->value[2] = (int) (level/10+20);      /* max dmg */
    obj->value[3] = WEAPON_VIBRO_BLADE;
    obj->value[4] = charge;
    obj->value[5] = charge;
    obj->cost = obj->value[2]*10;
    obj->gender = GENDER_NEUTRAL;

    obj = obj_to_char( obj, ch );

    send_to_char( FB_GREEN "Koñczysz pracê i bierzesz do rêki nowo skonstruowane vibroostrze." EOL, ch);
    act( PLAIN, "$n Koñczy pracê nad vibroostrzem.", ch, NULL, argument , TO_ROOM );

    {
         long xpgain;

         xpgain = UMIN( obj->cost*200 ,( exp_level(ch->skill_level[ENGINEERING_ABILITY]+1) - exp_level(ch->skill_level[ENGINEERING_ABILITY]) ) );
         gain_exp(ch, xpgain, ENGINEERING_ABILITY);
         ch_printf( ch , "Otrzymujesz %d punktów do¶wiadczenia w zakresie in¿ynierii.", xpgain );
    }

    learn_from_success( ch, gsn_makeblade );
}

DEF_DO_FUN( makeblaster )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int level, chance;
    bool checktool, checkdura, checkbatt, checkoven, checkcond, checkcirc, checkammo;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    int vnum, power, scope, ammo;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf2;

    strcpy( arg , argument );

    switch( ch->substate )
    {
    	default:
            if ( arg[0] == '\0' )
            {
		send_to_char( FB_RED "Sk³adnia: makeblaster <nazwa>" EOL, ch);
                return;
            }

    	    checktool = false;
            checkdura = false;
            checkbatt = false;
            checkoven = false;
            checkcond = false;
            checkcirc = false;

            if ( !IS_SET( ch->in_room->room_flags, ROOM_FACTORY ) )
            {
        	send_to_char( FB_RED "Musisz byæ w fabryce albo warsztacie aby to zrobiæ." EOL, ch);
                return;
            }

            for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
            {
		if (obj->item_type == ITEM_TOOLKIT)
            	    checktool = true;
        	if (obj->item_type == ITEM_DURAPLAST)
          	    checkdura = true;
                if (obj->item_type == ITEM_BATTERY)
                    checkbatt = true;
                if (obj->item_type == ITEM_OVEN)
                    checkoven = true;
                if (obj->item_type == ITEM_CIRCUIT)
                    checkcirc = true;
                if (obj->item_type == ITEM_SUPERCONDUCTOR)
                    checkcond = true;
            }

            if ( !checktool )
            {
        	send_to_char( FB_RED "Potrzebujesz skrzynki z narzêdziami aby skonstruowaæ blaster." EOL, ch);
		return;
            }

            if ( !checkdura )
            {
        	send_to_char( FB_RED "Potrzebujesz durastali, z której mo¿na to zrobiæ." EOL, ch);
		return;
            }

            if ( !checkbatt )
            {
        	send_to_char( FB_RED "Potrzebujesz ¼ród³a energii." EOL, ch);
                return;
            }

            if ( !checkoven )
            {
               send_to_char( FB_RED "Bêdzie Ci potrzebny ma³y piec do zrobienia tego." EOL, ch);
               return;
            }

            if ( !checkcirc )
            {
		send_to_char( FB_RED "Potrzebujesz tablicy kontrolnej (z obwodami) aby kontrolowaæ mechanizm ogniowy." EOL, ch);
        	return;
            }

            if ( !checkcond )
            {
    		send_to_char( FB_RED "potrzebujesz nadprzewodnika." EOL, ch);
        	return;
            }

            chance = IS_NPC(ch) ? ch->top_level
	                 : (int) (ch->pcdata->learned[gsn_makeblaster]);

            if ( number_percent( ) < chance )
	    {
    		send_to_char( FB_GREEN "Rozpoczynasz d³ugi proces tworzenia blastera." EOL, ch);
    		act( PLAIN, "$n bierze $s narzêdzia oraz ma³y piec i zaczyna nad czym¶ pracowaæ.",
			ch, NULL, argument , TO_ROOM );
		add_timer ( ch , TIMER_DO_FUN , 25 , do_makeblaster , 1 );
    		STRDUP( ch->dest_buf,arg);
    		return;
	    }

	    send_to_char(FB_RED "Nie jeste¶ w stanie z³o¿yæ tych wszystkich czê¶ci do kupy." EOL,ch);
	    learn_from_failure( ch, gsn_makeblaster );
    	    WAIT_STATE( ch, skill_table[gsn_makeblaster]->beats/2 );
	    return;

    	case 1:
	    if ( !*ch->dest_buf )
    		return;
    	    strcpy(arg, ch->dest_buf);
    	    STRDUP( ch->dest_buf, "" );
    	    break;

    	case SUB_TIMER_DO_ABORT:
    	    break_skill(FB_RED "Co¶ przeszkadza Ci w pracy i nie mo¿esz jej ukoñczyæ." EOL, ch);
    	    return;
    }

    ch->substate = SUB_NONE;

    level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makeblaster]);
    vnum = 10420;

    if ( ( pObjIndex = get_obj_index( vnum ) ) == NULL )
    {
	send_to_char(
	    FB_RED "Przedmiot, który próbujesz stworzyæ nie znajduje siê w bazie danych muda." EOL
	    FB_RED "Zg³o¶ to niezw³ocznie do administratorów albo poczt± na adres mud@swmud.pl." EOL, ch );
	bug( "Counldn't create obj (%d) for skill", vnum );
	return;
    }

    checkammo= false;
    checktool = false;
    checkdura = false;
    checkbatt = false;
    checkoven = false;
    checkcond = false;
    checkcirc = false;
    power     = 0;
    scope     = 0;
    ammo = 0;

    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
    {
	if (obj->item_type == ITEM_TOOLKIT)
    	    checktool = true;
	if (obj->item_type == ITEM_OVEN)
    	    checkoven = true;
	if (obj->item_type == ITEM_DURAPLAST && checkdura == false)
        {
    	    checkdura = true;
            separate_obj( obj );
            obj_from_char( obj );
            extract_obj( obj );
        }
        if (obj->item_type == ITEM_AMMO && checkammo == false)
        {
    	    ammo = obj->value[0];
            checkammo = true;
            separate_obj( obj );
            obj_from_char( obj );
            extract_obj( obj );
        }
        if (obj->item_type == ITEM_BATTERY && checkbatt == false)
        {
    	    separate_obj( obj );
            obj_from_char( obj );
            extract_obj( obj );
            checkbatt = true;
        }
        if (obj->item_type == ITEM_LENS && scope == 0)
        {
    	    separate_obj( obj );
            obj_from_char( obj );
            extract_obj( obj );
            scope++;
        }
        if (obj->item_type == ITEM_SUPERCONDUCTOR && power<2)
        {
    	    power++;
            separate_obj( obj );
            obj_from_char( obj );
            extract_obj( obj );
            checkcond = true;
        }
        if (obj->item_type == ITEM_CIRCUIT && checkcirc == false)
        {
    	    separate_obj( obj );
            obj_from_char( obj );
            extract_obj( obj );
            checkcirc = true;
        }
    }

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makeblaster]) ;

    if ( number_percent( ) > chance*2  || ( !checktool ) || ( !checkdura ) || ( !checkbatt ) || ( !checkoven )  || ( !checkcond ) || ( !checkcirc) )
    {
       send_to_char( FB_RED "Przygl±dasz siê nowoskonstruowanemu blasterowi." EOL
    		     FB_RED "Pó¼niej Twój wzrok przenosi siê na niewykorzystan± czê¶æ." EOL, ch);
       send_to_char( FB_RED "przesuwasz prze³±cznik w dobrej nadziei." EOL, ch);
       send_to_char( FB_RED "Twój blaster staje w p³omieniach niszcz±c siê oraz parz±c Twoje rêce." EOL, ch);
       learn_from_failure( ch, gsn_makeblaster );
       return;
    }

    obj = create_object( pObjIndex, level );

    obj->item_type = ITEM_WEAPON;
    SET_BIT( obj->wear_flags, ITEM_WIELD );
    SET_BIT( obj->wear_flags, ITEM_TAKE );
    obj->level = level;
    obj->weight = 2+level/10;

    strcpy( buf, nocolor(arg) );
    strcat( buf, " blaster" );
    STRDUP( obj->name, buf );

    sprintf( buf, "blaster %s", arg );
    STRDUP( obj->przypadki[0], buf );

    sprintf( buf, "blastera %s", arg );
    STRDUP( obj->przypadki[1], buf );

    sprintf( buf, "blasterowi %s", arg );
    STRDUP( obj->przypadki[2], buf );

    sprintf( buf, "blaster %s", arg );
    STRDUP( obj->przypadki[3], buf );

    sprintf( buf, "blasterem %s", arg );
    STRDUP( obj->przypadki[4], buf );

    sprintf( buf, "blasterze %s", arg );
    STRDUP( obj->przypadki[5], buf );

    sprintf( buf, "Kto¶ zostawi³ tu %s", obj->przypadki[3] );
    STRDUP( obj->description, buf );


    STRDUP( obj->description, buf );
    CREATE( paf, AFFECT_DATA, 1 );
    paf->type               = -1;
    paf->duration           = -1;
    paf->location           = flag_value( apply_types_list, "hitroll" );
    paf->modifier           = URANGE( 0, 1+scope, level/30 );
    paf->bitvector          = 0;
    paf->next               = NULL;
    LINK( paf, obj->first_affect, obj->last_affect, next, prev );
    ++top_affect;
    CREATE( paf2, AFFECT_DATA, 1 );
    paf2->type               = -1;
    paf2->duration           = -1;
    paf2->location           = flag_value( apply_types_list, "damroll" );
    paf2->modifier           = URANGE( 0, power, level/30);
    paf2->bitvector          = 0;
    paf2->next               = NULL;
    LINK( paf2, obj->first_affect, obj->last_affect, next, prev );
    ++top_affect;
    obj->value[0] = INIT_WEAPON_CONDITION;       /* condition  */
    obj->value[1] = (int) (level/10+15);      /* min dmg  */
    obj->value[2] = (int) (level/5+25);      /* max dmg  */
    obj->value[3] = WEAPON_BLASTER;
    obj->value[4] = ammo;
    obj->value[5] = 2000;
    obj->cost = obj->value[2]*50;
    obj->gender = GENDER_MALE;

    obj = obj_to_char( obj, ch );

    send_to_char(
	FB_GREEN "Koñczysz pracê i bierzesz do rêki nowo skonstruowany blaster." EOL, ch);
    act( PLAIN, "$n koñczy konstruowaæ $s nowy blaster.", ch, NULL, argument , TO_ROOM );

    {
         long xpgain;

         xpgain = UMIN( obj->cost*50 ,( exp_level(ch->skill_level[ENGINEERING_ABILITY]+1) - exp_level(ch->skill_level[ENGINEERING_ABILITY]) ) );
         gain_exp(ch, xpgain, ENGINEERING_ABILITY);
         ch_printf( ch , "Otrzymujesz %d punktów do¶wiadczenia w zakresie in¿ynierii.", xpgain );
    }
    learn_from_success( ch, gsn_makeblaster );
}

DEF_DO_FUN( makelightsaber )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int chance;
    bool checktool, checkdura, checkbatt,
         checkoven, checkcond, checkcirc, checklens, checkgems, checkmirr;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    int vnum, level, gems, charge, gemtype;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf2;
    int i;
    strcpy( arg, argument );

    switch( ch->substate )
    {
    	default:
    	        if ( arg[0] == '\0' )
                {
                  send_to_char( "&RU¿ycie: Makelightsaber <nazwa>" EOL, ch);
                  return;
                }

    	        checktool = false;
                checkdura = false;
                checkbatt = false;
                checkoven = false;
                checkcond = false;
                checkcirc = false;
                checklens = false;
                checkgems = false;
                checkmirr = false;

                if ( !IS_SET( ch->in_room->room_flags, ROOM_SAFE ) || !IS_SET( ch->in_room->room_flags, ROOM_SILENCE ))
                {
                   send_to_char( "&RMusisz byæ w spokojnym i cichym miejscu aby skonstruowaæ miecz ¶wietlny." NL, ch);
                   return;
                }

                for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
                {
                  if (obj->item_type == ITEM_TOOLKIT)
                    checktool = true;
                  if (obj->item_type == ITEM_LENS)
                    checklens = true;
                  if (obj->item_type == ITEM_CRYSTAL)
                    checkgems = true;
                  if (obj->item_type == ITEM_MIRROR)
                    checkmirr = true;
                  if (obj->item_type == ITEM_DURAPLAST || obj->item_type == ITEM_DURASTEEL )
          	    	checkdura = true;
                  if (obj->item_type == ITEM_BATTERY)
                    checkbatt = true;
                  if (obj->item_type == ITEM_OVEN)
                    checkoven = true;
                  if (obj->item_type == ITEM_CIRCUIT)
                    checkcirc = true;
                  if (obj->item_type == ITEM_SUPERCONDUCTOR)
                    checkcond = true;
                }

                if ( !checktool )
                {
                   send_to_char( "&RPotrzebujesz skrzynki z narzêdziami aby skonstruowaæ miecz ¶wietlny." NL, ch);
                   return;
                }

                if ( !checkdura )
                {
                   send_to_char( "&RPotrzebujesz jakiego¶ materia³u, z którego mo¿na by zrobiæ rêkoje¶æ." NL, ch);
                   return;
                }

                if ( !checkbatt )
                {
                   send_to_char( "&RPotrzebujesz ¼ród³a energii." NL, ch);
                   return;
                }

                if ( !checkoven )
                {
                   send_to_char( "&RBêdzie Ci potrzebny ma³y piec." NL "&RJako¶ trzeba roztopiæ materia³ na rêkoje¶æ." NL, ch);
                   return;
                }

                if ( !checkcirc )
                {
                   send_to_char( "&RPotrzebujesz trochê kabli (obwody elektryczne)." NL, ch);
                   return;
                }

                if ( !checkcond )
                {
                   send_to_char( "&RPotrzebujesz nadprzewodnika." NL, ch);
                   return;
                }

                if ( !checklens )
                {
                   send_to_char( "&RPotrzebujesz soczewek aby skupiæ wi±zkê." NL, ch);
                   return;
                }

                if ( !checkgems )
                {
                   send_to_char( "&RMiecze ¶wietlne wymagaj± od 1 do 3 kryszta³ów aby dzia³aæ poprawnie." NL, ch);
                   return;
                }

                if ( !checkmirr )
                {
                   send_to_char( "&RPotrzebujesz ma³ego lustra o du¿ym nasileniu." NL, ch);
                   return;
                }

    		    send_to_char( "&GRozpoczynasz d³ugi proces tworzenia miecza ¶wietlnego." NL, ch);
    		    act( PLAIN, "$n bierze $s narzêdzia oraz ma³y piec i zaczyna nad czym¶ pracowaæ.", ch,
		        NULL, argument , TO_ROOM );
		   		add_timer ( ch , TIMER_DO_FUN , 25 , do_makelightsaber , 1 );
    		    STRDUP( ch->dest_buf,arg);
    		    return;
    	case 1:
    		if ( !*ch->dest_buf )
    		     return;
    		strcpy(arg, ch->dest_buf);
    		STRDUP( ch->dest_buf, "" );
    		break;

    	case SUB_TIMER_DO_ABORT:
    	    break_skill("&RCo¶ przeszkadza Ci w pracy i nie mo¿esz jej ukoñczyæ.\n\r", ch);
    	    return;
    }

    ch->substate = SUB_NONE;

    level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_lightsaber_crafting]);
    vnum = 10421;

    if ( ( pObjIndex = get_obj_index( vnum ) ) == NULL )
    {
         send_to_char( "&RPrzedmiot, który próbujesz stworzyæ nie znajduje siê w bazie danych.\n\rZg³o¶ to niezw³ocznie do administratorów albo poczt± na adres mud@sw.ar.lublin.pl.\n\r", ch );
         return;
    }

    checktool = false;
    checkdura = false;
    checkbatt = false;
    checkoven = false;
    checkcond = false;
    checkcirc = false;
    checklens = false;
    checkgems = false;
    checkmirr = false;
    gems = 0;
    charge = 0;
    gemtype =0;

    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
    {
       if (obj->item_type == ITEM_TOOLKIT)
          checktool = true;
       if (obj->item_type == ITEM_OVEN)
          checkoven = true;
       if ( (obj->item_type == ITEM_DURAPLAST || obj->item_type == ITEM_DURASTEEL) && checkdura == false)
       {
          checkdura = true;
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
       }
       if (obj->item_type == ITEM_DURASTEEL && checkdura == false)
       {
          checkdura = true;
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
       }
       if (obj->item_type == ITEM_BATTERY && checkbatt == false)
       {
          charge = UMIN(obj->value[1], 10);
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkbatt = true;
       }
       if (obj->item_type == ITEM_SUPERCONDUCTOR && checkcond == false)
       {
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkcond = true;
       }
       if (obj->item_type == ITEM_CIRCUIT && checkcirc == false)
       {
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkcirc = true;
       }
       if (obj->item_type == ITEM_LENS && checklens == false)
       {
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checklens = true;
       }
       if (obj->item_type == ITEM_MIRROR && checkmirr == false)
       {
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkmirr = true;
       }
       if (obj->item_type == ITEM_CRYSTAL && gems < 3)
       {
          gems++;
          if ( gemtype < obj->value[0] )
             gemtype = obj->value[0];
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkgems = true;
       }
    }

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_lightsaber_crafting]) ;

    if ( number_percent( ) > chance*2  || ( !checktool ) || ( !checkdura ) || ( !checkbatt ) || ( !checkoven )
                                       || ( !checkmirr ) || ( !checklens ) || ( !checkgems ) || ( !checkcond ) || ( !checkcirc) )

    {
       send_to_char( "&RBierzesz do rêki nowo stworzony miecz ¶wietlny i wciskasz prze³±cznik w dobrej nadziei.\n\r", ch);
       send_to_char( "&RZamiast ¶wiat³a z r±czki wydobywaj± siê tumany dymu.\n\r", ch);
       send_to_char( "&RUpuszczasz r±czkê i patrzysz jak topi siê na pod³odze.\n\r", ch);
       learn_from_failure( ch, gsn_lightsaber_crafting );
       return;
    }

    obj = create_object( pObjIndex, level );

    obj->item_type = ITEM_WEAPON;
    SET_BIT( obj->wear_flags, ITEM_WIELD );
    SET_BIT( obj->wear_flags, ITEM_TAKE );
    SET_BIT( obj->extra_flags, ITEM_ANTI_SOLDIER );
    SET_BIT( obj->extra_flags, ITEM_ANTI_THIEF );
    SET_BIT( obj->extra_flags, ITEM_ANTI_HUNTER );
    SET_BIT( obj->extra_flags, ITEM_ANTI_PILOT );
    SET_BIT( obj->extra_flags, ITEM_ANTI_CITIZEN );
    SET_BIT( obj->extra_flags, ITEM_PERSONAL );       /*Aldegard*/
    obj->level = level;
    obj->weight = 5;
    strcpy( buf, arg );
    strcat( buf, " miecz lightsaber saber" );
    STRDUP( obj->name, buf );
    strcpy( buf, arg );
    for( i=0; i<6; i++ )
    STRDUP( obj->przypadki[i], buf );
    strcat( buf, " le¿y tutaj przypadkowo." );
    STRDUP( obj->description, buf );
    strcpy( buf, arg );
    strcat( buf, " zapala siê z wyra¼nym buczeniem i lekkim blaskiem." );
    STRDUP( obj->action_desc, buf );
    CREATE( paf, AFFECT_DATA, 1 );
    paf->type               = -1;
    paf->duration           = -1;
    paf->location           = flag_value( apply_types_list, "hitroll" );
    paf->modifier           = URANGE( 0, gems, level/30 );
    paf->bitvector          = 0;
    paf->next               = NULL;
    LINK( paf, obj->first_affect, obj->last_affect, next, prev );
    ++top_affect;
    CREATE( paf2, AFFECT_DATA, 1 );
    paf2->type               = -1;
    paf2->duration           = -1;
    paf2->location           = flag_value( apply_types_list, "parry" );
    paf2->modifier           = ( level/3 );
    paf2->bitvector          = 0;
    paf2->next               = NULL;
    LINK( paf2, obj->first_affect, obj->last_affect, next, prev );
    ++top_affect;
    obj->value[0] = INIT_WEAPON_CONDITION;       /* condition  */
    obj->value[1] = (int) (level/10+(gemtype+gems)*2);      /* min dmg  */
    obj->value[2] = (int) (level/5+(gemtype+gems)*6);      /* max dmg */
    obj->value[3] = WEAPON_LIGHTSABER;
    obj->value[4] = charge;
    obj->value[5] = charge;
    obj->cost = obj->value[2]*75;
    obj->gender = GENDER_MALE;//added by Thanos (tymczasowo)

    obj = obj_to_char( obj, ch );

    send_to_char( "&GKoñczysz pracê i bierzesz do rêki nowo skonstruowany miecz ¶wietlny.&w\n\r", ch);
    act( PLAIN, "$n koñczy konstruowaæ $s nowy miecz ¶wietlny.", ch,
		NULL, argument , TO_ROOM );

    {
        long xpgain;

        xpgain = number_range( NEXT_LEVEL(ch,FORCE_ABILITY)/2, NEXT_LEVEL(ch,FORCE_ABILITY) );
        gain_exp(ch, xpgain, FORCE_ABILITY);
        ch_printf( ch , "Otrzymujesz %d punktów do¶wiadczenia w zakresie mocy.", xpgain );
    }
		learn_from_success( ch, gsn_lightsaber_crafting );
}


DEF_DO_FUN( makespice )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int chance;
    OBJ_DATA *obj;
    int i;
    switch( ch->substate )
    {
    	default:
    	        strcpy( arg, argument );

    	        if ( arg[0] == '\0' )
                {
                  send_to_char( "&RZ czego?\n\r&w", ch);
                  return;
                }

    	        if ( !IS_SET( ch->in_room->room_flags, ROOM_REFINERY ) )
                {
                   send_to_char( "&RMusisz byæ w rafinerii aby wytwarzaæ narkotyki z przypraw.\n\r", ch);
                   return;
                }

                if ( ms_find_obj(ch) )
                      return;

                if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
                {
                        send_to_char( "&RNie masz takiego przedmiotu.\n\r&w", ch );
                        return;
                }

                if ( obj->item_type != ITEM_RAWSPICE )
                {
                       send_to_char( "&RNie mo¿esz zrobiæ narkotyku z tego.\n\r&w",ch);
                       return;
                }

    	        chance = IS_NPC(ch) ? ch->top_level
	                 : (int) (ch->pcdata->learned[gsn_spice_refining]);
                if ( number_percent( ) < chance )
    		{
    		   send_to_char( "&GRozpoczynasz d³ugi proces rafinowania przypraw w narkotyki.\n\r", ch);
    		   act( PLAIN, "$n zaczyna nad czym¶ pracowaæ.", ch,
		        NULL, argument , TO_ROOM );
		   add_timer ( ch , TIMER_DO_FUN , 10 , do_makespice , 1 );
    		   STRDUP( ch->dest_buf,arg);
    		   return;
	        }
	        send_to_char("&RNie wiesz co zrobiæ z tymi sk³adnikami.\n\r",ch);
	        learn_from_failure( ch, gsn_spice_refining );
    	   	return;

    	case 1:
    		if ( !*ch->dest_buf )
    		   return;
    		strcpy(arg, ch->dest_buf);
    		STRDUP( ch->dest_buf, "" );
    		break;

    	case SUB_TIMER_DO_ABORT:
    	    break_skill("&RCo¶ przeszkadza Ci w pracy i nie mo¿esz jej ukoñczyæ.\n\r&w", ch);
    	    return;
    }

    ch->substate = SUB_NONE;

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
           send_to_char( "Wygl±da na to, ¿e przyprawy zosta³y stracone!\n\r", ch );
           return;
    }
    if ( obj->item_type != ITEM_RAWSPICE )
    {
          send_to_char( "&RSk³adniki zosta³y zmieszane lecz praca nie jest ukoñczona.\n\r&w",ch);
          return;
    }

    obj->value[1] = URANGE (10, obj->value[1], ( IS_NPC(ch) ? ch->top_level
	                 : (int) (ch->pcdata->learned[gsn_spice_refining]) ) +10);
    strcpy( buf, obj->name );
    strcat( buf, " narkotyk zio³o proch" );
    STRDUP( obj->name, buf );
    strcpy( buf, "narkotyk z " );
    strcat( buf, obj->przypadki[1] );
    for( i=0; i<6; i++ )
    STRDUP( obj->przypadki[i], buf );
    strcat( buf, " le¿y tutaj. Hmm.. Co to?" );
    buf[0]=UPPER(buf[0]);
    STRDUP( obj->description, buf );
    obj->item_type = ITEM_SPICE;

    send_to_char( "&GKoñczysz pracê.\n\r", ch);
    act( PLAIN, "$n koñczy $s pracê.", ch,
         NULL, argument , TO_ROOM );

    obj->gender = GENDER_MALE;//added by Thanos (tymczasowo)

    obj->cost += obj->value[1]*10;
    {
         long xpgain;

         xpgain = UMIN( obj->cost*50 ,( exp_level(ch->skill_level[ENGINEERING_ABILITY]+1) - exp_level(ch->skill_level[ENGINEERING_ABILITY]) ) );
         gain_exp(ch, xpgain, ENGINEERING_ABILITY);
         ch_printf( ch , "Otrzymujesz %d punktów do¶wiadczenia w zakresie in¿ynierii.", xpgain );
    }

    learn_from_success( ch, gsn_spice_refining );

}

DEF_DO_FUN( makegrenade )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int level, chance;
    int strength = 0;
    int weight = 0;
    bool checktool, checkdrink, checkbatt, checkchem, checkcirc;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    int vnum;
    int i;
    strcpy( arg , argument );

    switch( ch->substate )
    {
    	default:
    	        if ( arg[0] == '\0' )
                {
                  send_to_char( "&RU¿ycie: Makegrenade <nazwa>\n\r&w", ch);
                  return;
                }

    	        checktool  = false;
                checkdrink = false;
                checkbatt  = false;
                checkchem  = false;
                checkcirc  = false;

                if ( !IS_SET( ch->in_room->room_flags, ROOM_FACTORY ) )
                {
                   send_to_char( "&RMusisz byæ w fabryce albo warsztacie aby to zrobiæ.\n\r", ch);
                   return;
                }

                for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
                {
                  if (obj->item_type == ITEM_TOOLKIT)
                    checktool = true;
                  if (obj->item_type == ITEM_DRINK_CON && obj->value[1] == 0 )
          	    checkdrink = true;
                  if (obj->item_type == ITEM_BATTERY)
                    checkbatt = true;
                  if (obj->item_type == ITEM_CIRCUIT)
                    checkcirc = true;
                  if (obj->item_type == ITEM_CHEMICAL)
                    checkchem = true;
                }

                if ( !checktool )
                {
                   send_to_char( "&RPotrzebujesz skrzynki z narzêdziami aby skonstruowaæ granat.\n\r", ch);
                   return;
                }

                if ( !checkdrink )
                {
                   send_to_char( "&RPotrzebujesz pusty pojemnik na ciecz aby zmieszaæ i przechowaæ chemikalia.\n\r", ch);
                   return;
                }

                if ( !checkbatt )
                {
                   send_to_char( "&RPotrzebujesz baterii do zegara.\n\r", ch);
                   return;
                }

                if ( !checkcirc )
                {
                   send_to_char( "&RPotrzebujesz obwodu do zegara.\n\r", ch);
                   return;
                }

                if ( !checkchem )
                {
                   send_to_char( "&RPotrzebne bêd± chemikalia!\n\r", ch);
                   return;
                }

    	        chance = IS_NPC(ch) ? ch->top_level
	                 : (int) (ch->pcdata->learned[gsn_makegrenade]);
                if ( number_percent( ) < chance )
    		{
    		   send_to_char( "&GRozpoczynasz d³ugi proces tworzenia granatu.\n\r", ch);
    		   act( PLAIN, "$n bierze $s narzêdzia oraz pojemnik na ciecz i zaczyna nad czym¶ pracowaæ.", ch,
		        NULL, argument , TO_ROOM );
		   add_timer ( ch , TIMER_DO_FUN , 25 , do_makegrenade , 1 );
    		   STRDUP( ch->dest_buf,arg);
    		   return;
	        }
	        send_to_char("&RNie jeste¶ w stanie z³o¿yæ tych wszystkich czê¶ci do kupy.\n\r",ch);
	        learn_from_failure( ch, gsn_makegrenade );
    	   	return;

    	case 1:
    		if ( !*ch->dest_buf )
    		     return;
    		strcpy(arg, ch->dest_buf);
    		STRDUP( ch->dest_buf, "" );
    		break;

    	case SUB_TIMER_DO_ABORT:
    	    break_skill("&RCo¶ przeszkadza Ci w pracy i nie mo¿esz jej ukoñczyæ.\n\r", ch);
    	    return;
    }

    ch->substate = SUB_NONE;

    level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makegrenade]);
    vnum = 10425;

    if ( ( pObjIndex = get_obj_index( vnum ) ) == NULL )
    {
         send_to_char( "&RPrzedmiot, który próbujesz stworzyæ nie znajduje siê w bazie danych.\n\rZg³o¶ to niezw³ocznie do administratorów albo poczt± na adres mud@sw.ar.lublin.pl.\n\r", ch );
         return;
    }

    checktool = false;
    checkdrink = false;
    checkbatt = false;
    checkchem = false;
    checkcirc = false;

    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
    {
       if (obj->item_type == ITEM_TOOLKIT)
          checktool = true;
       if (obj->item_type == ITEM_DRINK_CON && checkdrink == false && obj->value[1] == 0 )
       {
          checkdrink = true;
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
       }
       if (obj->item_type == ITEM_BATTERY && checkbatt == false)
       {
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkbatt = true;
       }
       if (obj->item_type == ITEM_CHEMICAL)
       {
          strength = URANGE( 10, obj->value[0], level * 5 );
          weight = obj->weight;
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkchem = true;
       }
       if (obj->item_type == ITEM_CIRCUIT && checkcirc == false)
       {
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkcirc = true;
       }
    }

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makegrenade]) ;

    if ( number_percent( ) > chance*2  || ( !checktool ) || ( !checkdrink ) || ( !checkbatt ) || ( !checkchem ) || ( !checkcirc) )
    {
       send_to_char( "&RTu¿ przed koñcem pracy granat, który jest Twoim produktem wybucha!!!\n\r!!!BUUUM!!!\n\r", ch);
	damage( ch, ch, number_range( ch->max_hit/20, ch->max_hit/10 ), gsn_makegrenade );
       learn_from_failure( ch, gsn_makegrenade );
       return;
    }

    obj = create_object( pObjIndex, level );

    obj->item_type = ITEM_GRENADE;
    SET_BIT( obj->wear_flags, ITEM_HOLD );
    SET_BIT( obj->wear_flags, ITEM_TAKE );
    obj->level = level;
    obj->weight = weight;
    strcpy( buf , arg );
    strcat( buf , " granat");
    STRDUP( obj->name, buf );
    strcpy( buf, arg );
    for( i=0; i<6; i++ )
    STRDUP( obj->przypadki[i], buf );
    strcat( buf, " le¿y sobie tutaj. Wygl±da zupe³nie niewinnie." );
    STRDUP( obj->description, buf );
    obj->value[0] = strength/2;
    obj->value[1] = strength;
    obj->cost = obj->value[1]*5;
    obj->gender = GENDER_NEUTRAL;//added by Thanos (tymczasowo)

    obj = obj_to_char( obj, ch );

    send_to_char( "&GKoñczysz pracê i bierzesz do rêki nowo skonstruowany granat.&w\n\r", ch);
    act( PLAIN, "$n koñczy konstruowaæ $s nowy granat.", ch,
         NULL, argument , TO_ROOM );

    {
         long xpgain;

         xpgain = UMIN( obj->cost*50 ,( exp_level(ch->skill_level[ENGINEERING_ABILITY]+1) - exp_level(ch->skill_level[ENGINEERING_ABILITY]) ) );
         gain_exp(ch, xpgain, ENGINEERING_ABILITY);
         ch_printf( ch , "Otrzymujesz %d punktów do¶wiadczenia w zakresie in¿ynierii.", xpgain );
    }
        learn_from_success( ch, gsn_makegrenade );
}

DEF_DO_FUN( makelandmine )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int level, chance;
    int strength = 0;
    int weight = 0;
    bool checktool, checkdrink, checkbatt, checkchem, checkcirc;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    int vnum;
    int i;
    strcpy( arg , argument );

    switch( ch->substate )
    {
    	default:
    	        if ( arg[0] == '\0' )
                {
                  send_to_char( "&RU¿ycie: Makelandmine <nazwa>\n\r&w", ch);
                  return;
                }

    	        checktool  = false;
                checkdrink = false;
                checkbatt  = false;
                checkchem  = false;
                checkcirc  = false;

                if ( !IS_SET( ch->in_room->room_flags, ROOM_FACTORY ) )
                {
                   send_to_char( "&RMusisz byæ w fabryce albo warsztacie aby to zrobiæ.\n\r", ch);
                   return;
                }

                for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
                {
                  if (obj->item_type == ITEM_TOOLKIT)
                    checktool = true;
                  if (obj->item_type == ITEM_DRINK_CON && obj->value[1] == 0 )
          	    checkdrink = true;
                  if (obj->item_type == ITEM_BATTERY)
                    checkbatt = true;
                  if (obj->item_type == ITEM_CIRCUIT)
                    checkcirc = true;
                  if (obj->item_type == ITEM_CHEMICAL)
                    checkchem = true;
                }

                if ( !checktool )
                {
                   send_to_char( "&RPotrzebujesz skrzynki z narzêdziami aby skonstruowaæ minê l±dow±.\n\r", ch);
                   return;
                }

                if ( !checkdrink )
                {
                   send_to_char( "&RPotrzebujesz pusty pojemnik na ciecz aby zmieszaæ i przechowaæ chemikalia.\n\r", ch);
                   return;
                }

                if ( !checkbatt )
                {
                   send_to_char( "&RPotrzebujesz baterii do detonatora.\n\r", ch);
                   return;
                }

                if ( !checkcirc )
                {
                   send_to_char( "&RPotrzebujesz obwodu do detonatora.\n\r", ch);
                   return;
                }

                if ( !checkchem )
                {
                   send_to_char( "&RPotrzebne bêd± chemikalia!\n\r", ch);
                   return;
                }

    	        chance = IS_NPC(ch) ? ch->top_level
	                 : (int) (ch->pcdata->learned[gsn_makelandmine]);
                if ( number_percent( ) < chance )
    		{
    		   send_to_char( "& GRozpoczynasz d³ugi proces tworzenia miny l±dowej.\n\r", ch);
    		   act( PLAIN, "$n bierze $s narzêdzia oraz pojemnik na ciecz i zaczyna nad czym¶ pracowaæ.", ch,
		        NULL, argument , TO_ROOM );
		   add_timer ( ch , TIMER_DO_FUN , 25 , do_makelandmine , 1 );
    		   STRDUP( ch->dest_buf,arg);
    		   return;
	        }
	        send_to_char("&RNie jeste¶ w stanie z³o¿yæ tych wszystkich czê¶ci do kupy.\n\r",ch);
	        learn_from_failure( ch, gsn_makelandmine );
    	   	return;

    	case 1:
    		if ( !*ch->dest_buf )
    		     return;
    		strcpy(arg, ch->dest_buf);
    		STRDUP( ch->dest_buf, "" );
    		break;

    	case SUB_TIMER_DO_ABORT:
    	    break_skill("&RCo¶ przeszkadza Ci w pracy i nie mo¿esz jej ukoñczyæ.\n\r", ch);
    	    return;
    }

    ch->substate = SUB_NONE;

    level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makelandmine]);
    vnum = 10427;

    if ( ( pObjIndex = get_obj_index( vnum ) ) == NULL )
    {
         send_to_char( "&RPrzedmiot, który próbujesz stworzyæ nie znajduje siê w bazie danych.\n\rZg³o¶ to niezw³ocznie do administratorów albo poczt± na adres mud@sw.ar.lublin.pl.\n\r", ch );
         return;
    }

    checktool = false;
    checkdrink = false;
    checkbatt = false;
    checkchem = false;
    checkcirc = false;

    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
    {
       if (obj->item_type == ITEM_TOOLKIT)
          checktool = true;
       if (obj->item_type == ITEM_DRINK_CON && checkdrink == false && obj->value[1] == 0 )
       {
          checkdrink = true;
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
       }
       if (obj->item_type == ITEM_BATTERY && checkbatt == false)
       {
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkbatt = true;
       }
       if (obj->item_type == ITEM_CHEMICAL)
       {
          strength = URANGE( 10, obj->value[0], level * 5 );
          weight = obj->weight;
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkchem = true;
       }
       if (obj->item_type == ITEM_CIRCUIT && checkcirc == false)
       {
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkcirc = true;
       }
    }

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makelandmine]) ;

    if ( number_percent( ) > chance*2  || ( !checktool ) || ( !checkdrink ) || ( !checkbatt ) || ( !checkchem ) || ( !checkcirc) )
    {
       send_to_char( "&RTu¿ przed koñcem pracy mina l±dowa, która jest Twoim produktem wybucha!!!\n\r!!!BUUUM!!!\n\r", ch);
/* to samo co przy makegrenade (Trog) */
       learn_from_failure( ch, gsn_makelandmine );
       return;
    }

    obj = create_object( pObjIndex, level );

    obj->item_type = ITEM_LANDMINE;
    SET_BIT( obj->wear_flags, ITEM_HOLD );
    SET_BIT( obj->wear_flags, ITEM_TAKE );
    obj->level = level;
    obj->weight = weight;
    strcpy( buf , arg );
    strcat( buf , " mina l±dowa");
    STRDUP( obj->name, buf );
    strcpy( buf, arg );
    for( i=0; i<6; i++ )
    STRDUP( obj->przypadki[i], buf );
    strcat( buf, " le¿y tutaj. Wyglada na kawalek p³askiego metalu." );
    STRDUP( obj->description, buf );
    obj->value[0] = strength/2;
    obj->value[1] = strength;
    obj->cost = obj->value[1]*5;
    obj->gender = GENDER_FEMALE;//added by Thanos (tymczasowo)

    obj = obj_to_char( obj, ch );

    send_to_char( "&GKoñczysz pracê i bierzesz do rêki nowo skonstruowan± minê l±dow±.&w\n\r", ch);
    act( PLAIN, "$n koñczy konstruowaæ $s now± minê l±dow±.", ch,
         NULL, argument , TO_ROOM );

    {
         long xpgain;

         xpgain = UMIN( obj->cost*50 ,( exp_level(ch->skill_level[ENGINEERING_ABILITY]+1) - exp_level(ch->skill_level[ENGINEERING_ABILITY]) ) );
         gain_exp(ch, xpgain, ENGINEERING_ABILITY);
         ch_printf( ch , "Otrzymujesz %d punktów do¶wiadczenia w zakresie in¿ynierii.", xpgain );
    }
        learn_from_success( ch, gsn_makelandmine );
}
DEF_DO_FUN( makelight )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int level, chance;
    int strength = 0;
    bool checktool, checkbatt, checkchem, checkcirc, checklens;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    int vnum;
    int i;
    strcpy( arg , argument );

    switch( ch->substate )
    {
    	default:
    	        if ( arg[0] == '\0' )
                {
                  send_to_char( "&RU¿ycie: Makeflashlight <nazwa>\n\r&w", ch);
                  return;
                }

    	        checktool  = false;
                checkbatt  = false;
                checkchem  = false;
                checkcirc  = false;
                checklens = false;

                if ( !IS_SET( ch->in_room->room_flags, ROOM_FACTORY ) )
                {
                   send_to_char( "&RMusisz byæ w fabryce albo warsztacie aby to zrobiæ.\n\r", ch);
                   return;
                }

                for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
                {
                  if (obj->item_type == ITEM_TOOLKIT)
                    checktool = true;
                  if (obj->item_type == ITEM_BATTERY)
                    checkbatt = true;
                  if (obj->item_type == ITEM_CIRCUIT)
                    checkcirc = true;
                  if (obj->item_type == ITEM_CHEMICAL)
                    checkchem = true;
                  if (obj->item_type == ITEM_LENS)
                    checklens = true;
                }

                if ( !checktool )
                {
                   send_to_char( "& RPotrzebujesz skrzynki z narzêdziami aby skonstruowaæ ¶wiat³o.\n\r", ch);
                   return;
                }

                if ( !checklens )
                {
                   send_to_char( "&RPotrzebujesz soczewek aby zrobiæ ¶wiat³o.\n\r", ch);
                   return;
                }

                if ( !checkbatt )
                {
                   send_to_char( "&RPotrzebujesz baterii aby ¶wiat³o dzia³a³o.\n\r", ch);
                   return;
                }

                if ( !checkcirc )
                {
                   send_to_char( "&RPotrzebujesz ma³ego obwodu.\n\r", ch);
                   return;
                }

                if ( !checkchem )
                {
                   send_to_char( "&RBêd± potrzebne chemikalia!\n\r", ch);
                   return;
                }

    	        chance = IS_NPC(ch) ? ch->top_level
	                 : (int) (ch->pcdata->learned[gsn_makelight]);
                if ( number_percent( ) < chance )
    		{
    		   send_to_char( "&GRozpoczynasz d³ugi proces tworzenia ¶wiat³a.\n\r", ch);
    		   act( PLAIN, "$ n bierze $s narzêdzia i zaczyna nad czym¶ pracowaæ.", ch,
		        NULL, argument , TO_ROOM );
		   add_timer ( ch , TIMER_DO_FUN , 10 , do_makelight , 1 );
    		   STRDUP( ch->dest_buf,arg);
    		   return;
	        }
	        send_to_char("&RNie jeste¶ w stanie z³o¿yæ tych wszystkich czê¶ci do kupy. \n\r",ch);
	        learn_from_failure( ch, gsn_makelight );
    	   	return;

    	case 1:
    		if ( !*ch->dest_buf )
    		     return;
    		strcpy(arg, ch->dest_buf);
    		STRDUP( ch->dest_buf, "" );
    		break;

    	case SUB_TIMER_DO_ABORT:
    	    break_skill("&RCo¶ przeszkadza Ci w pracy i nie mo¿esz jej ukoñczyæ.\n\r", ch);
    	    return;
    }

    ch->substate = SUB_NONE;

    level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makelight]);
    vnum = 10428;

    if ( ( pObjIndex = get_obj_index( vnum ) ) == NULL )
    {
         send_to_char( "&RPrzedmiot, który próbujesz stworzyæ nie znajduje siê w bazie danych.\n\rZg³o¶ to niezw³ocznie do administratorów albo poczt± na adres mud@sw.ar.lublin.pl.\n\r", ch );
         return;
    }

    checktool = false;
    checklens = false;
    checkbatt = false;
    checkchem = false;
    checkcirc = false;

    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
    {
       if (obj->item_type == ITEM_TOOLKIT)
          checktool = true;
       if (obj->item_type == ITEM_BATTERY && checkbatt == false)
       {
          strength = obj->value[0];
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkbatt = true;
       }
       if (obj->item_type == ITEM_CHEMICAL)
       {
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkchem = true;
       }
       if (obj->item_type == ITEM_CIRCUIT && checkcirc == false)
       {
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkcirc = true;
       }
       if (obj->item_type == ITEM_LENS && checklens == false)
       {
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checklens = true;
       }
    }

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makelight]) ;

    if ( number_percent( ) > chance*2  || ( !checktool ) || ( !checklens ) || ( !checkbatt ) || ( !checkchem ) || ( !checkcirc) )
    {
       send_to_char( "& RTu¿ przed koñcem pracy mina l±dowa, która jest Twoim produktem wybucha!!!\n\r!!!BUUUM!!!\n\r", ch);
/* --> makegrenade (Trog) */
       learn_from_failure( ch, gsn_makelight );
       return;
    }

    obj = create_object( pObjIndex, level );

    obj->item_type = ITEM_LIGHT;
    SET_BIT( obj->wear_flags, ITEM_TAKE );
    obj->level = level;
    obj->weight = 3;
    strcpy( buf , arg );
    strcat( buf , " light");
    STRDUP( obj->name, buf );
    strcpy( buf, arg );
    for( i=0; i<6; i++ )
    STRDUP( obj->przypadki[i], buf );
    strcat( buf, " le¿y tutaj przez przypadek." );
    STRDUP( obj->description, buf );
    obj->value[2] = strength;
    obj->cost = obj->value[2];
    obj->gender = GENDER_NEUTRAL;//added by Thanos (tymczasowo)

    obj = obj_to_char( obj, ch );

    send_to_char( "&GKoñczysz pracê i bierzesz do rêki nowo skonstruowane ¶wiat³o.&w\n\r", ch);
    act( PLAIN, "$n koñczy konstruowaæ $s nowe ¶wiat³o.", ch,
         NULL, argument , TO_ROOM );

    {
         long xpgain;

         xpgain = UMIN( obj->cost*100 ,( exp_level(ch->skill_level[ENGINEERING_ABILITY]+1) - exp_level(ch->skill_level[ENGINEERING_ABILITY]) ) );
         gain_exp(ch, xpgain, ENGINEERING_ABILITY);
         ch_printf( ch , "Otrzymujesz %d punktów do¶wiadczenia w zakresie in¿ynierii.", xpgain );
    }
        learn_from_success( ch, gsn_makelight );
}

DEF_DO_FUN( makejewelry )
{
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int level, chance;
    bool checktool, checkoven, checkmetal;
    OBJ_DATA *obj;
    OBJ_DATA *metal = 0;
    int value, cost;
    int i;
    int 	avail_locs  = 	ITEM_WEAR_EYES | ITEM_WEAR_EARS |
				ITEM_WEAR_FINGER | ITEM_WEAR_NECK;

    argument = one_argument( argument, arg );
    strcpy( arg2 , argument );

    value = flag_value( pl_wear_flags_list, arg );

    if( value == ITEM_WEAR_BODY
    ||  value == ITEM_WEAR_HEAD
    ||  value == ITEM_WEAR_LEGS
    ||  value == ITEM_WEAR_ARMS
    ||  value == ITEM_WEAR_ABOUT
    ||  value == ITEM_WEAR_EYES
    ||  value == ITEM_WEAR_WAIST
    ||  value == ITEM_HOLD
    ||  value == ITEM_WEAR_FEET
    ||  value == ITEM_WEAR_HANDS )
    {
        send_to_char( FB_RED "Nie mo¿esz zrobiæ bi¿uterii na t± czê¶æ cia³a." EOL, ch);
        send_to_char( FB_RED "Spróbuj MAKEARMOR." EOL, ch);
        return;
    }
    if( value == ITEM_WEAR_SHIELD )
    {
        send_to_char( FB_RED "Nie mo¿esz zrobiæ bi¿uterii, która bêdzie noszona jako tarcza." EOL, ch);
        send_to_char( FB_RED "Spróbuj MAKESHIELD." EOL, ch);
        return;
    }
    if( value == ITEM_WIELD )
    {
        send_to_char( FB_RED "Zamierzasz walczyæ swoj± bi¿uteri±?" EOL, ch);
        send_to_char( FB_RED "Spróbuj MAKEBLADE." EOL, ch);
        return;
    }

    switch( ch->substate )
    {
    	default:

    	    if( arg2[0] == '\0' || value==NO_FLAG ||
	    !IS_SET( avail_locs, value ) )
            {
		send_to_char( "Sk³adnia: Makejewelry <lokacja_na_ciele> <nazwa>" NL, ch);
		ch_printf( ch, "Dostêpne lokacje na ciele: %s" NL,
			    flag_string(pl_wear_flags_list, avail_locs ) );
              return;
            }

	    checktool = false;
            checkoven = false;
            checkmetal = false;

            if ( !IS_SET( ch->in_room->room_flags, ROOM_FACTORY ) )
            {
        	send_to_char( FB_RED "Musisz byæ w fabryce albo warsztacie aby to zrobiæ." EOL, ch);
		return;
            }

            for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
            {
        	if (obj->item_type == ITEM_TOOLKIT)
            	    checktool = true;
                if (obj->item_type == ITEM_OVEN)
          	    checkoven = true;
                if (obj->item_type == ITEM_RARE_METAL)
          	    checkmetal = true;
            }

            if ( !checktool )
            {
        	send_to_char( FB_RED "Potrzebujesz skrzynki z narzêdziami." EOL, ch);
		return;
            }

            if ( !checkoven )
            {
        	send_to_char( FB_RED "Bêdzie Ci potrzebny ma³y piec do zrobienia tego." EOL, ch);
		return;
            }

            if ( !checkmetal )
            {
        	send_to_char( "&RPotrzebujesz cennego (szlachetnego) metalu.\n\r", ch);
                return;
            }

    	    chance = IS_NPC(ch) ? ch->top_level
	                 : (int) (ch->pcdata->learned[gsn_makejewelry]);

	    if ( number_percent( ) < chance )
    	    {
    		send_to_char( FB_RED "Rozpoczynasz d³ugi proces tworzenia jakiej¶ bi¿uterii..." EOL, ch);
		act( PLAIN, "$n bierze $s narzêdzia oraz kawa³ek metalu i zaczyna nad czym¶ pracowaæ.",
		    ch, NULL, argument , TO_ROOM );
		add_timer ( ch , TIMER_DO_FUN , 15 , do_makejewelry , 1 );
    		STRDUP( ch->dest_buf,arg);
    		STRDUP( ch->dest_buf_2,arg2);
    		return;
	    }

	    send_to_char(FB_RED "Nie jeste¶ w stanie niczego z tym zrobiæ." EOL,ch);
	    learn_from_failure( ch, gsn_makejewelry );
	    WAIT_STATE( ch, skill_table[gsn_makejewelry]->beats/2 );
	    return;

    	case 1:
    	    if ( !*ch->dest_buf )
    		    return;
	    strcpy(arg, ch->dest_buf);
    	    STRDUP( ch->dest_buf, "" );

    	    if ( !*ch->dest_buf_2 )
    		return;
    	    strcpy(arg2, ch->dest_buf_2);
    	    STRDUP( ch->dest_buf_2, "" );
    	    break;

    	case SUB_TIMER_DO_ABORT:
    	    break_skill(FB_RED "Co¶ przeszkadza Ci w pracy i nie mo¿esz jej ukoñczyæ." EOL, ch);
    	    return;
    }

    ch->substate = SUB_NONE;

    level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makejewelry]);

    checkmetal = false;
    checkoven = false;
    checktool = false;
    value=0;
    cost=0;

    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
    {
       if (obj->item_type == ITEM_TOOLKIT)
          checktool = true;
       if (obj->item_type == ITEM_OVEN)
          checkoven = true;
       if (obj->item_type == ITEM_RARE_METAL && checkmetal == false)
       {
          checkmetal = true;
          separate_obj( obj );
          obj_from_char( obj );
          metal = obj;
       }
       if (obj->item_type == ITEM_CRYSTAL)
       {
          cost += obj->cost;
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
       }
    }

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makejewelry]) ;

    if ( number_percent( ) > chance*2  || ( !checkoven ) || ( !checktool ) || ( !checkmetal ) )
    {
       send_to_char( FB_GREEN "Koñczysz pracê i bierzesz do rêki nowo wytworzon± bi¿uteriê." EOL, ch);
       send_to_char( FB_RED "Nagle zdajesz sobie sprawê, ¿e zrobiona przez Ciebie bi¿uteria" EOL, ch);
       send_to_char( FB_RED "jest zwyk³ym, nieprzydatnym ¶mieciem. Chowasz to szybko ze wstydem." EOL, ch);
       learn_from_failure( ch, gsn_makejewelry );
       return;
    }

    obj = metal;

    obj->item_type = ITEM_TREASURE;

    value = flag_value( pl_wear_flags_list, arg );

    SET_BIT( obj->wear_flags, ITEM_TAKE );
    if( !IS_SET( avail_locs, value ) )
        SET_BIT( obj->wear_flags, ITEM_WEAR_NECK );
    else
        SET_BIT( obj->wear_flags, value );

    obj->level = level;
    strcpy( buf, arg2 );
    STRDUP( obj->name, buf );
    strcpy( buf, arg2 );
    for( i=0; i<6; i++ )
    STRDUP( obj->przypadki[i], buf );
    strcat( buf, " le¿y tutaj, pewnie kto¶ to upu¶ci³." );
    STRDUP( obj->description, buf );
    obj->value[0] = obj->value[1];
    obj->cost *= 10;
    obj->cost += cost;
    obj->gender = GENDER_FEMALE;

    obj = obj_to_char( obj, ch );

    send_to_char( FB_GREEN "Koñczysz pracê i bierzesz do rêki nowo wytworzon± bi¿uteriê." EOL, ch);
    act( PLAIN, "$n koñczy pracê nad now± bi¿uteri±.", ch, NULL, argument, TO_ROOM );

    learn_from_success( ch, gsn_makejewelry );
    {
         long xpgain;

         xpgain = UMIN( obj->cost*100 ,( exp_level(ch->skill_level[ENGINEERING_ABILITY]+1) - exp_level(ch->skill_level[ENGINEERING_ABILITY]) ) );
         gain_exp(ch, xpgain, ENGINEERING_ABILITY);
         ch_printf( ch , "Otrzymujesz %d punktów do¶wiadczenia w zakresie in¿ynierii.", xpgain );
    }

}

DEF_DO_FUN( makearmor )
{
    char 	arg[MAX_INPUT_LENGTH];
    char 	arg2[MAX_INPUT_LENGTH];
    char 	buf[MAX_STRING_LENGTH];
    int 	level, chance;
    bool 	checksew, checkfab;
    OBJ_DATA *	obj;
    OBJ_DATA *	material = 0;
    int 	value, i;
    int 	avail_locs  = 	ITEM_WEAR_FEET | ITEM_WEAR_LEGS |
				ITEM_WEAR_HEAD | ITEM_WEAR_HANDS |
				ITEM_WEAR_ARMS;

    argument = one_argument( argument, arg );
    strcpy( arg2 , argument );

    value = flag_value( pl_wear_flags_list, arg );

    if( value == ITEM_WEAR_EYES
    ||	value == ITEM_WEAR_EARS
    ||  value == ITEM_WEAR_FINGER
    ||  value == ITEM_WEAR_NECK )
    {
        send_to_char( FB_RED "Nie mo¿esz robiæ ubrañ na t± czê¶æ cia³a." EOL, ch);
        send_to_char( FB_RED "Spróbuj MAKEJEWELRY." EOL, ch);
        return;
    }

    if( value == ITEM_WEAR_SHIELD )
    {
        send_to_char( FB_RED "Nie mo¿esz robiæ ubrañ noszonych jako tarcza." EOL, ch);
        send_to_char( FB_RED "Spróbuj MAKESHIELD." EOL, ch);
        return;
    }

    if( value == ITEM_WIELD )
    {
        send_to_char( FB_RED "Zamierzasz walczyæ swoim ubraniem?" EOL, ch);
        send_to_char( FB_RED "Spróbuj MAKEBLADE..." EOL, ch);
        return;
    }

    switch( ch->substate )
    {
    	default:
    	    if( arg2[0] == '\0' || value==NO_FLAG ||
	    !IS_SET( avail_locs, value ) )
            {
		send_to_char( "Sk³adnia: Makearmor <lokacja_na_ciele> <nazwa>" NL, ch);
		ch_printf( ch, "Dostêpne lokacje na ciele: %s" NL,
			    flag_string(pl_wear_flags_list, avail_locs ) );
              return;
            }

	    checksew = false;
            checkfab = false;

            if ( !IS_SET( ch->in_room->room_flags, ROOM_FACTORY ) )
            {
               send_to_char( "Musisz byæ w warsztacie lub fabryce by to zrobiæ." NL, ch);
               return;
            }

            for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
            {
              if (obj->item_type == ITEM_FABRIC)
                    checkfab = true;
                  if (obj->item_type == ITEM_THREAD)
          	    checksew = true;
            }

            if ( !checkfab )
            {
               send_to_char( "Potrzebujesz materia³u." NL, ch);
               return;
            }

            if ( !checksew )
            {
               send_to_char( "Potrzebujesz ig³y i kawa³ka nitki." NL, ch);
               return;
            }

	    chance = IS_NPC(ch) ? ch->top_level
	                 : (int) (ch->pcdata->learned[gsn_makearmor]);

	    if ( number_percent( ) < chance )
    	    {
    		send_to_char( FB_GREEN "Rozpoczynasz d³ugi proces tworzenia jakiego¶ pancerza." EOL, ch);
		act( PLAIN, "$n bierze $s pude³ko z przyborami do szycia oraz jaki¶ materia³ i zaczyna pracowaæ.",
		    ch, NULL, argument , TO_ROOM );
		add_timer ( ch , TIMER_DO_FUN , 15 , do_makearmor , 1 );
    		STRDUP( ch->dest_buf,arg);
    		STRDUP( ch->dest_buf_2,arg2);
    		return;
	    }

	    send_to_char(FB_RED "Zupe³nie nie wiesz jak to zrobiæ." EOL,ch);
	    learn_from_failure( ch, gsn_makearmor );
	    WAIT_STATE( ch, skill_table[gsn_makearmor]->beats/2 );
    	    return;

    	case 1:
    		if ( !*ch->dest_buf )
    		     return;
    		strcpy(arg, ch->dest_buf);
    		STRDUP( ch->dest_buf, "" );

    		if ( !*ch->dest_buf_2 )
    		     return;
    		strcpy(arg2, ch->dest_buf_2);
    		STRDUP( ch->dest_buf_2, "" );
    		break;

    	case SUB_TIMER_DO_ABORT:
    	    break_skill(FB_RED "Co¶ przeszkadza Ci w pracy i nie mo¿esz jej ukoñczyæ." EOL, ch);
    	    return;
    }

    ch->substate = SUB_NONE;

    level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makearmor]);

    checksew = false;
    checkfab = false;

    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
    {
       if (obj->item_type == ITEM_THREAD)
          checksew = true;
       if (obj->item_type == ITEM_FABRIC && checkfab == false)
       {
          checkfab = true;
          separate_obj( obj );
          obj_from_char( obj );
          material = obj;
       }
    }

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makearmor]) ;

    if ( number_percent( ) > chance*2  || ( !checkfab ) || ( !checksew ) )
    {
       send_to_char( FB_GREEN "Koñczysz pracê i bierzesz do rêki nowo skonstruowany pancerz." EOL, ch);
       send_to_char( FB_RED "Nagle zdajesz sobie sprawê, ¿e zrobiona przez Ciebie rzecz jest ca³kowicie" EOL, ch);
       send_to_char( FB_RED "bezu¿yteczna. Chowasz \"pancerz\" ze wstydem..." EOL, ch);
       learn_from_failure( ch, gsn_makearmor );
       return;
    }

    obj = material;

    obj->item_type = ITEM_ARMOR;

    value = flag_value( pl_wear_flags_list, arg );

    SET_BIT( obj->wear_flags, ITEM_TAKE );
    if( !IS_SET( avail_locs, value ) )
        SET_BIT( obj->wear_flags, ITEM_WEAR_BODY );
    else
        SET_BIT( obj->wear_flags, value );

    obj->level = level;
    strcpy( buf, arg2 );
    STRDUP( obj->name, buf );
    strcpy( buf, arg2 );
    for( i=0; i<6; i++ )
    STRDUP( obj->przypadki[i], buf );
    strcat( buf, " le¿y tutaj." );
    STRDUP( obj->description, buf );
    obj->value[0] = obj->value[1];
    obj->cost *= 10;
    obj->gender = GENDER_MALE;

    obj = obj_to_char( obj, ch );

    send_to_char( FB_RED "Koñczysz pracê i bierzesz do rêki nowo skonstruowany pancerz." EOL, ch);
    act( PLAIN, "$n koñczy szyæ nowy pancerz.", ch, NULL, argument , TO_ROOM );

    {
         long xpgain;

         xpgain = UMIN( obj->cost*100 ,( exp_level(ch->skill_level[ENGINEERING_ABILITY]+1) - exp_level(ch->skill_level[ENGINEERING_ABILITY]) ) );
         gain_exp(ch, xpgain, ENGINEERING_ABILITY);
         ch_printf( ch , "Otrzymujesz %d punktów do¶wiadczenia w zakresie in¿ynierii.", xpgain );
    }
        learn_from_success( ch, gsn_makearmor );
}


DEF_DO_FUN( makecomlink )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int chance;
    bool checktool, checkgem, checkbatt, checkcirc;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    int vnum;
    int i;
    strcpy( arg , argument );

    switch( ch->substate )
    {
    	default:

    	        if ( arg[0] == '\0' )
                {
                  send_to_char( "&RU¿ycie: Makecomlink <nazwa>\n\r&w", ch);
                  return;
                }

    	        checktool = false;
                checkgem  = false;
                checkbatt = false;
                checkcirc = false;

                if ( !IS_SET( ch->in_room->room_flags, ROOM_FACTORY ) )
                {
                   send_to_char( "&RMusisz byæ w fabryce albo warsztacie aby to zrobiæ.\n\r", ch);
                   return;
                }

                for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
                {
                  if (obj->item_type == ITEM_TOOLKIT)
                    checktool = true;
                  if (obj->item_type == ITEM_CRYSTAL)
          	    checkgem = true;
                  if (obj->item_type == ITEM_BATTERY)
                  checkbatt = true;
                  if (obj->item_type == ITEM_CIRCUIT)
                  checkcirc = true;
                }

                if ( !checktool )
                {
                   send_to_char( "&RPotrzebujesz skrzynki z narzêdziami aby skonstruowaæ komlink.\n\r", ch);
                   return;
                }

                if ( !checkgem )
                {
                   send_to_char( "&RBêdzie Ci potrzebny ma³y kryszta³.\n\r", ch);
                   return;
                }

                if ( !checkbatt )
                {
                   send_to_char( "&RPotrzebujesz ¼ród³a energii.\n\r", ch);
                   return;
                }

                if ( !checkcirc )
                {
                   send_to_char( "&RBêdzie Ci potrzebny ma³y obwód.\n\r", ch);
                   return;
                }

    	        chance = IS_NPC(ch) ? ch->top_level
	                 : (int) (ch->pcdata->learned[gsn_makecomlink]);
                if ( number_percent( ) < chance )
    		{
    		   send_to_char( "&GRozpoczynasz d³ugi proces tworzenia komlinku.\n\r", ch);
    		   act( PLAIN, "$n bierze $s skrzynkê z narzêdziami i zaczyna nad czym¶ pracowaæ.", ch,
		        NULL, argument , TO_ROOM );
		   add_timer ( ch , TIMER_DO_FUN , 10 , do_makecomlink , 1 );
    		   STRDUP( ch->dest_buf,arg);
    		   return;
	        }
	        send_to_char("&RNie jeste¶ w stanie z³o¿yæ tych wszystkich czê¶ci do kupy.\n\r",ch);
	        learn_from_failure( ch, gsn_makecomlink );
    	   	return;

    	case 1:
    		if ( !*ch->dest_buf )
    		     return;
    		strcpy(arg, ch->dest_buf);
    		STRDUP( ch->dest_buf, "" );
    		break;

    	case SUB_TIMER_DO_ABORT:
    	    break_skill("&RCo¶ przeszkadza Ci w pracy i nie mo¿esz jej ukoñczyæ.\n\r", ch);
    	    return;
    }

    ch->substate = SUB_NONE;

    vnum = 10430;

    if ( ( pObjIndex = get_obj_index( vnum ) ) == NULL )
    {
         send_to_char( "&RPrzedmiot, który próbujesz stworzyæ nie znajduje siê w bazie danych.\n\rZg³o¶ to niezw³ocznie do administratorów albo poczt± na adres mud@sw.ar.lublin.pl.\n\r", ch );
         return;
    }

    checktool = false;
    checkgem  = false;
    checkbatt = false;
    checkcirc = false;

    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
    {
       if (obj->item_type == ITEM_TOOLKIT)
          checktool = true;
       if (obj->item_type == ITEM_CRYSTAL && checkgem == false)
       {
          checkgem = true;
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
       }
       if (obj->item_type == ITEM_CIRCUIT && checkcirc == false)
       {
          checkcirc = true;
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
       }
       if (obj->item_type == ITEM_BATTERY && checkbatt == false )
       {
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkbatt = true;
       }
    }

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makecomlink]) ;

    if ( number_percent( ) > chance*2  || ( !checktool ) || ( !checkcirc ) || ( !checkbatt ) || ( !checkgem ) )
    {
       send_to_char( "&GKoñczysz pracê i bierzesz do rêki nowo skonstruowany komlink\n\r", ch);
       send_to_char( "&Rale chwilê pó¼niej rozpada siê on w Twoich rêkach.\n\r", ch);
       learn_from_failure( ch, gsn_makecomlink );
       return;
    }

    obj = create_object( pObjIndex, ch->top_level );

    obj->item_type = ITEM_COMLINK;
    SET_BIT( obj->wear_flags, ITEM_HOLD );
    SET_BIT( obj->wear_flags, ITEM_TAKE );
    obj->weight = 3;
    strcpy( buf, arg );
    strcat( buf, " comlink" );
    STRDUP( obj->name, buf );
    strcpy( buf, arg );
    for( i=0; i<6; i++ )
    STRDUP( obj->przypadki[i], buf );
    strcat( buf, " le¿y tutaj. Ciekawe kto go zostawi³?" );
    STRDUP( obj->description, buf );
    obj->cost = 50;
    obj->gender = GENDER_MALE;//added by Thanos (tymczasowo)

    obj = obj_to_char( obj, ch );

    send_to_char( "&GKoñczysz pracê i bierzesz do rêki nowo skonstruowany komlink.&w\n\r", ch);
    act( PLAIN, "$n koñczy konstruowaæ nowy komlink.", ch,
         NULL, argument , TO_ROOM );

    {
         long xpgain;

         xpgain = UMIN( obj->cost*100 ,( exp_level(ch->skill_level[ENGINEERING_ABILITY]+1) - exp_level(ch->skill_level[ENGINEERING_ABILITY]) ) );
         gain_exp(ch, xpgain, ENGINEERING_ABILITY);
         ch_printf( ch , "Otrzymujesz %d punktów do¶wiadczenia w zakresie in¿ynierii.", xpgain );
    }
        learn_from_success( ch, gsn_makecomlink );

}

DEF_DO_FUN( makeshield )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int chance;
    bool checktool, checkbatt, checkcond, checkcirc, checkgems;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    int vnum, level, charge;
    int gemtype = 0;
    int i;
    strcpy( arg, argument );

    switch( ch->substate )
    {
    	default:
    	        if ( arg[0] == '\0' )
                {
                  send_to_char( "&RU¿ycie: Makeshield <nazwa>\n\r&w", ch);
                  return;
                }

    	        checktool = false;
                checkbatt = false;
                checkcond = false;
                checkcirc = false;
                checkgems = false;

                if ( !IS_SET( ch->in_room->room_flags, ROOM_FACTORY ) )
                {
                   send_to_char( "&RMusisz byæ w fabryce albo warsztacie aby to zrobiæ.\n\r", ch);
                   return;
                }

                for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
                {
                  if (obj->item_type == ITEM_TOOLKIT)
                    checktool = true;
                  if (obj->item_type == ITEM_CRYSTAL)
                    checkgems = true;
                  if (obj->item_type == ITEM_BATTERY)
                    checkbatt = true;
                  if (obj->item_type == ITEM_CIRCUIT)
                    checkcirc = true;
                  if (obj->item_type == ITEM_SUPERCONDUCTOR)
                    checkcond = true;
                }

                if ( !checktool )
                {
                   send_to_char( "&RPotrzebujesz skrzynki z narzêdziami aby skonstruowaæ tarczê energetyczn±.\n\r", ch);
                   return;
                }

                if ( !checkbatt )
                {
                   send_to_char( "&RPotrzebujesz ¼ród³a energii.\n\r", ch);
                   return;
                }

                if ( !checkcirc )
                {
                   send_to_char( "&RPotrzebujesz tablicy kontrolnej (z obwodami).\n\r", ch);
                   return;
                }

                if ( !checkcond )
                {
                   send_to_char( "&RPotrzebujesz nadprzewodnika aby skonstruowaæ energetyczn± tarczê.\n\r", ch);
                   return;
                }

                if ( !checkgems )
                {
                   send_to_char( "&RPotrzebujesz ma³ego kryszta³u.\n\r", ch);
                   return;
                }

    	        chance = IS_NPC(ch) ? ch->top_level
	                 : (int) (ch->pcdata->learned[gsn_makeshield]);
                if ( number_percent( ) < chance )
    		{
    		   send_to_char( "&GRozpoczynasz d³ugi proces tworzenia energetycznej tarczy.\n\r", ch);
    		   act( PLAIN, "$n bierze $s narzêdzia i zaczyna nad czym¶ pracowaæ.", ch,
		        NULL, argument , TO_ROOM );
		   add_timer ( ch , TIMER_DO_FUN , 20 , do_makeshield , 1 );
    		   STRDUP( ch->dest_buf,arg);
    		   return;
	        }
	        send_to_char("&RNie jeste¶ w stanie z³o¿yæ tych wszystkich czê¶ci do kupy.\n\r",ch);
	        learn_from_failure( ch, gsn_makeshield );
    	   	return;

    	case 1:
    		if ( !*ch->dest_buf )
    		     return;
    		strcpy(arg, ch->dest_buf);
    		STRDUP( ch->dest_buf, "" );
    		break;

    	case SUB_TIMER_DO_ABORT:
    	    break_skill("&RCo¶ przeszkadza Ci w pracy i nie mo¿esz jej ukoñczyæ.\n\r", ch);
    	    return;
    }

    ch->substate = SUB_NONE;

    level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makeshield]);
    vnum = 10429;

    if ( ( pObjIndex = get_obj_index( vnum ) ) == NULL )
    {
         send_to_char( "&RPrzedmiot, który próbujesz stworzyæ nie znajduje siê w bazie danych.\n\rZg³o¶ to niezw³ocznie do administratorów albo poczt± na adres mud@sw.ar.lublin.pl.\n\r", ch );
         return;
    }

    checktool = false;
    checkbatt = false;
    checkcond = false;
    checkcirc = false;
    checkgems = false;
    charge = 0;

    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
    {
       if (obj->item_type == ITEM_TOOLKIT)
          checktool = true;

       if (obj->item_type == ITEM_BATTERY && checkbatt == false)
       {
          charge = UMIN(obj->value[1], 10);
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkbatt = true;
       }
       if (obj->item_type == ITEM_SUPERCONDUCTOR && checkcond == false)
       {
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkcond = true;
       }
       if (obj->item_type == ITEM_CIRCUIT && checkcirc == false)
       {
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkcirc = true;
       }
       if (obj->item_type == ITEM_CRYSTAL && checkgems == false)
       {
          gemtype = obj->value[0];
          separate_obj( obj );
          obj_from_char( obj );
          extract_obj( obj );
          checkgems = true;
       }
    }

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makeshield]) ;

    if ( number_percent( ) > chance*2  || ( !checktool ) || ( !checkbatt )
                                       || ( !checkgems ) || ( !checkcond ) || ( !checkcirc) )

    {
       send_to_char( "&GKoñczysz pracê i bierzesz do rêki nowo skonstruowan±, energetyczn± tarczê i wciskasz przycisk w dobrej nadzieji.\n\r", ch);
       send_to_char( "&RZamiast energii, z tarczy wydobywa siê dym.\n\r", ch);
       send_to_char( "&RUpuszczasz ten gor±cy sprzêt i patrzysz jak topi siê na ziemi.\n\r", ch);
       learn_from_failure( ch, gsn_makeshield );
       return;
    }

    obj = create_object( pObjIndex, level );

    obj->item_type = ITEM_ARMOR;
    SET_BIT( obj->wear_flags, ITEM_WIELD );
    SET_BIT( obj->wear_flags, ITEM_WEAR_SHIELD );
    obj->level = level;
    obj->weight = 2;
    STRDUP( obj->name, "energy shield" );
    strcpy( buf, arg );
    for( i=0; i<6; i++ )
    STRDUP( obj->przypadki[i], buf );
    strcat( buf, " le¿y tutaj." );
    STRDUP( obj->description, buf );
    obj->value[0] = (int) (level/10+gemtype*2);      /* condition */
    obj->value[1] = (int) (level/10+gemtype*2);      /* armor */
    obj->value[4] = charge;
    obj->value[5] = charge;
    obj->cost = obj->value[2]*100;
    obj->gender = GENDER_FEMALE;//added by Thanos (tymczasowo)

    obj = obj_to_char( obj, ch );

    send_to_char( "&GKoñczysz pracê i bierzesz do rêki nowo skonstruowan±, energetyczn± tarczê.&w\n\r", ch);
    act( PLAIN, "$n koñczy konstruowaæ $s now±, energetyczn± tarczê.", ch,
         NULL, argument , TO_ROOM );

    {
         long xpgain;

         xpgain = UMIN( obj->cost*50 ,( exp_level(ch->skill_level[ENGINEERING_ABILITY]+1) - exp_level(ch->skill_level[ENGINEERING_ABILITY]) ) );
         gain_exp(ch, xpgain, ENGINEERING_ABILITY);
         ch_printf( ch , "Otrzymujesz %d punktów do¶wiadczenia w zakresie in¿ynierii.", xpgain );
    }
        learn_from_success( ch, gsn_makeshield );

}

DEF_DO_FUN( makecontainer )
{
    char 	arg		[MAX_INPUT_LENGTH];
    char 	arg2		[MAX_INPUT_LENGTH];
    char 	buf		[MAX_STRING_LENGTH];
    int 	level, chance;
    bool 	checksew, checkfab;
    OBJ_DATA 	*obj;
    OBJ_DATA 	*material = 0;
    int 	value;
    int 	avail_locs  = 	ITEM_WEAR_BODY | ITEM_WEAR_WAIST |
				ITEM_HOLD | ITEM_WEAR_WRIST;


    argument = one_argument( argument, arg );
    strcpy( arg2 , argument );

    value = flag_value( pl_wear_flags_list, arg );


    if( value == ITEM_WEAR_EYES
    ||	value == ITEM_WEAR_EARS
    ||  value == ITEM_WEAR_FINGER
    ||  value == ITEM_WEAR_NECK )
    {
        send_to_char( "Nie mo¿esz zrobiæ pojemnika na tê czê¶æ cia³a." NL, ch);
        send_to_char( "Spróbuj MAKEJEWELRY.\n\r", ch);
        return;
    }

    if( value == ITEM_WEAR_FEET
    ||	value == ITEM_WEAR_LEGS
    ||	value == ITEM_WEAR_HEAD
    ||  value == ITEM_WEAR_HANDS
    ||  value == ITEM_WEAR_ARMS )
    {
        send_to_char( "Nie mo¿esz zrobiæ pojemnika na tê czê¶æ cia³a." NL, ch);
        send_to_char( "Spróbuj MAKEARMOR.\n\r", ch);
        return;
    }

    if( value == ITEM_WEAR_SHIELD )
    {
        send_to_char( "Hmmm... Tarcza nie mo¿e byæ pojemnikiem. No chyba, ¿e na pociski ;-)" NL, ch);
        send_to_char( "Spróbuj MAKESHIELD." NL, ch);
        return;
    }

    if( value == ITEM_WIELD )
    {
        send_to_char( "Masz zamiar walczyæ pojemnikiem? Tak jak stare babcie?" NL, ch);
        send_to_char( "Spróbuj MAKEBLADE..." NL, ch);
        return;
    }

    switch( ch->substate )
    {
    	default:
    	    if( arg2[0] == '\0' || value==NO_FLAG ||
	    !IS_SET( avail_locs, value ) )
            {
		send_to_char( "Sk³adnia: Makecontainer <lokacja_na_ciele> <nazwa>" NL, ch);
		ch_printf( ch, "Dostêpne lokacje na ciele: %s" NL,
			    flag_string(pl_wear_flags_list, avail_locs ) );
              return;
            }

    	    checksew = false;
            checkfab = false;

            if ( !IS_SET( ch->in_room->room_flags, ROOM_FACTORY ) )
            {
               send_to_char( "Musisz byæ w warsztacie lub fabryce by to zrobiæ." NL, ch);
               return;
            }

            for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
            {
              if (obj->item_type == ITEM_FABRIC)
                    checkfab = true;
                  if (obj->item_type == ITEM_THREAD)
          	    checksew = true;
            }

            if ( !checkfab )
            {
               send_to_char( "Potrzebujesz materia³u." NL, ch);
               return;
            }

            if ( !checksew )
            {
               send_to_char( "Potrzebujesz ig³y i kawa³ka nitki." NL, ch);
               return;
            }

    	    chance = IS_NPC(ch) ? ch->top_level
	                 : (int) (ch->pcdata->learned[gsn_makecontainer]);
            if ( number_percent( ) < chance )
    	    {
    	        send_to_char( FB_WHITE "Rozpoczynasz ¿mudny proces tworzenia pojemnika..." EOL, ch);
    	        act( PLAIN, "$n bierze swój zestaw m³odego krawca i materia³ i co¶ tam kombinuje.",
			ch, NULL, argument , TO_ROOM );
		add_timer ( ch, TIMER_DO_FUN, 5, do_makecontainer, 1 );
    		STRDUP( ch->dest_buf,arg);
    		STRDUP( ch->dest_buf_2,arg2);
    		return;
	    }

	    send_to_char("Nie bardzo wiesz co masz z tym wszystkim zrobiæ." NL,ch);
	    learn_from_failure( ch, gsn_makecontainer );
	    WAIT_STATE( ch, 3 );
    	    return;

    	case 1:
	    if ( !*ch->dest_buf )
    		return;
    	    strcpy(arg, ch->dest_buf);
    	    STRDUP( ch->dest_buf, "" );

    	    if ( !*ch->dest_buf_2 )
    		return;
    	    strcpy(arg2, ch->dest_buf_2);
    	    STRDUP( ch->dest_buf_2, "" );
    	    break;

	case SUB_TIMER_DO_ABORT:
	    break_skill("Co¶ ci przeszkodzi³o, przerywasz szycie pojemnika." NL, ch);
    	    return;
    }

    ch->substate = SUB_NONE;

    level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makecontainer]);

    checksew = false;
    checkfab = false;

    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
    {
       if (obj->item_type == ITEM_THREAD)
          checksew = true;

       if (obj->item_type == ITEM_FABRIC && checkfab == false)
       {
          checkfab = true;
          separate_obj( obj );
          obj_from_char( obj );
          material = obj;
       }

    }

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makecontainer]) ;

    if ( number_percent( ) > chance*2  || ( !checkfab ) || ( !checksew ) )
    {
        send_to_char(
        "Bierzesz do rêki nowiusieñki pojemnik."				   NL
        "Lecz dopiero teraz zdajesz sobie sprawê, ¿e uda³o ci siê stworzyæ" NL
        "najmniej przydatny pojemnik jaki mo¿e w ogóle istnieæ."		   NL
        "Szybko ukrywasz swoj± pora¿kê."					   NL
        , ch);
	learn_from_failure( ch, gsn_makecontainer );
	WAIT_STATE( ch, 3 );
	return;
    }

    obj = material;

    obj->item_type = ITEM_CONTAINER;

    value = flag_value( pl_wear_flags_list, arg );

    SET_BIT( obj->wear_flags, ITEM_TAKE );
    if( !IS_SET( avail_locs, value ) )
        SET_BIT( obj->wear_flags, ITEM_HOLD );
    else
        SET_BIT( obj->wear_flags, value );


    sprintf( buf, "torba pojemnik %s", nocolor(arg2) );
    STRDUP( obj->name, buf );

    sprintf( buf, "torba %s", arg2 );
    STRDUP( obj->przypadki[0], buf );

    sprintf( buf, "torby %s", arg2 );
    STRDUP( obj->przypadki[1], buf );

    sprintf( buf, "torbie %s", arg2 );
    STRDUP( obj->przypadki[2], buf );

    sprintf( buf, "torbê %s", arg2 );
    STRDUP( obj->przypadki[3], buf );

    sprintf( buf, "torb± %s", arg2 );
    STRDUP( obj->przypadki[4], buf );

    sprintf( buf, "torbie %s", arg2 );
    STRDUP( obj->przypadki[5], buf );

    sprintf( buf, "Kto¶ zostawi³ tu %s", obj->przypadki[3] );
    STRDUP( obj->description, buf );

    obj->level 		= level;
    obj->value[0] 	= level;
    obj->value[1] 	= 0;
    obj->value[2] 	= 0;
    obj->value[3] 	= 10;
    obj->cost 		*= 2;
    obj->gender 	= GENDER_FEMALE;

    obj = obj_to_char( obj, ch );

    send_to_char( "Koñczysz szycie i trzymasz w rêku nowiu¶ki pojemnik!" NL, ch);
    act( PLAIN, "$n koñczy szycie pojemnika.", ch, NULL, argument , TO_ROOM );

    {
         long xpgain;

         xpgain = UMIN( obj->cost*100 ,( exp_level(ch->skill_level[ENGINEERING_ABILITY]+1) - exp_level(ch->skill_level[ENGINEERING_ABILITY]) ) );
         gain_exp(ch, xpgain, ENGINEERING_ABILITY);
         ch_printf( ch , "Otrzymujesz %d punktów do¶wiadczenia w zakresie in¿ynierii." NL, xpgain );
    }
    learn_from_success( ch, gsn_makecontainer );
}

DEF_DO_FUN( makemissile )
{
    /* don't think we really need this */
    send_to_char( "&RSorry, this skill isn't finished yet :(\n\r", ch);
}

DEF_DO_FUN( gemcutting )
{
     send_to_char( "&RSorry, this skill isn't finished yet :(\n\r", ch);
}

DEF_DO_FUN( reinforcements )
{
    char arg[MAX_INPUT_LENGTH];
    int chance, credits;

    if ( IS_NPC( ch ) || !ch->pcdata )
    	return;

    strcpy( arg, argument );

    switch( ch->substate )
    {
    	default:
    	        if ( ch->backup_wait )
    	        {
    	            send_to_char( "&RTwoje posi³ki s± ju¿ w drodze.\n\r", ch );
    	            return;
    	        }

    	        if ( !ch->pcdata->clan )
    	        {
    	            send_to_char( "&RDopiero gdy bêdziesz nale¿eæ do jakiej¶ organizacji wezwanie posi³ków bêdzie mo¿liwe.\n\r", ch );
    	            return;
    	        }

    	        if ( ch->gold < ch->skill_level[LEADERSHIP_ABILITY] * 50 )
    	        {
    	            ch_printf( ch, "&RMasz zbyt ma³o kredytów aby wezwaæ posi³ki.\n\r" );
    	            return;
    	        }

    	        chance = (int) (ch->pcdata->learned[gsn_reinforcements]);
                if ( number_percent( ) < chance )
    		{
    		   send_to_char( "&GZaczynasz dzwoniæ po posi³ki.\n\r", ch);
    		   act( PLAIN, "$n wydaje jakie¶ rozkazy przez $s komlink.", ch,
		        NULL, argument , TO_ROOM );
		   add_timer ( ch , TIMER_DO_FUN , 1 , do_reinforcements , 1 );
    		   STRDUP( ch->dest_buf, arg);
    		   return;
	        }
	        send_to_char("&RDzwonisz po posi³ki ale nikt nie odpowiada.\n\r",ch);
	        learn_from_failure( ch, gsn_reinforcements );
    	   	return;

    	case 1:
    		if ( !*ch->dest_buf )
    		   return;
    		strcpy(arg, ch->dest_buf);
    		STRDUP( ch->dest_buf, "" );
    		break;

    	case SUB_TIMER_DO_ABORT:
    	    break_skill("&RPo³±czenie zosta³o przerwane podczas dzwonienia po posi³ki.\n\r", ch);
    	    return;
    }

    ch->substate = SUB_NONE;

    send_to_char( "&GTwoje posi³ki s± w drodze.\n\r", ch);
    credits = ch->skill_level[LEADERSHIP_ABILITY] * 50;
    ch_printf( ch, "Kosztuje ciê to %d kredytów.\n\r", credits);
    ch->gold -= UMIN( credits , ch->gold );

    learn_from_success( ch, gsn_reinforcements );

    if ( nifty_is_name( (char *)"Imperium" , ch->pcdata->clan->name ) )
       ch->backup_mob = MOB_VNUM_STORMTROOPER;
    else if ( nifty_is_name( (char *)"Nowa Republika" , ch->pcdata->clan->name ) )
       ch->backup_mob = MOB_VNUM_NR_TROOPER;
    else
       ch->backup_mob = MOB_VNUM_MERCINARY;

    ch->backup_wait = number_range(1,2);

}

DEF_DO_FUN( postguard )
{
    char arg[MAX_INPUT_LENGTH];
    int chance, credits;

    if ( IS_NPC( ch ) || !ch->pcdata )
    	return;

    strcpy( arg, argument );

    switch( ch->substate )
    {
    	default:
    	        if ( ch->backup_wait )
    	        {
    	            send_to_char( "&RTwoje posi³ki s± ju¿ w drodze.\n\r", ch );
    	            return;
    	        }

    	        if ( !ch->pcdata->clan )
    	        {
    	            send_to_char( "&RGdy bêdziesz nale¿eæ do jakiej¶ organizacji wezwanie stra¿nika bêdzie mo¿liwe.\n\r", ch );
    	            return;
    	        }

    	        if ( ch->gold < ch->skill_level[LEADERSHIP_ABILITY] * 30 )
    	        {
    	            ch_printf( ch, "&RMasz zbyt ma³o kredytów aby wezwaæ stra¿nika.\n\r", ch );
    	            return;
    	        }

    	        chance = (int) (ch->pcdata->learned[gsn_postguard]);
                if ( number_percent( ) < chance )
    		{
    		   send_to_char( "&GZaczynasz dzwoniæ po posi³ki.\n\r", ch);
    		   act( PLAIN, "$n wydaje jakie¶ rozkazy przez $s komlink.", ch,
		        NULL, argument , TO_ROOM );
		   add_timer ( ch , TIMER_DO_FUN , 1 , do_postguard , 1 );
    		   STRDUP( ch->dest_buf,arg);
    		   return;
	        }
	        send_to_char("&RDzwonisz po stra¿nika ale nikt nie odpowiada.\n\r",ch);
	        learn_from_failure( ch, gsn_postguard );
    	   	return;

    	case 1:
    		if ( !*ch->dest_buf )
    		   return;
    		strcpy(arg, ch->dest_buf);
    		STRDUP( ch->dest_buf, "" );
    		break;

    	case SUB_TIMER_DO_ABORT:
    	    break_skill("&RPo³±czenie zosta³o przerwane podczas dzwonienia po stra¿nika.\n\r", ch);
    	    return;
    }

    ch->substate = SUB_NONE;

    send_to_char( "&GTwój stra¿nik jest w drodze.\n\r", ch);

    credits = ch->skill_level[LEADERSHIP_ABILITY] * 30;
    ch_printf( ch, "Kosztuje ciê to %d kredytów.\n\r", credits);
    ch->gold -= UMIN( credits , ch->gold );

    learn_from_success( ch, gsn_postguard );

    if ( nifty_is_name( (char *)"Imperium" , ch->pcdata->clan->name ) )
       ch->backup_mob = MOB_VNUM_IMP_GUARD;
    else if ( nifty_is_name( (char *)"Nowa Republika" , ch->pcdata->clan->name ) )
       ch->backup_mob = MOB_VNUM_NR_GUARD;
    else
       ch->backup_mob = MOB_VNUM_BOUNCER;

    ch->backup_wait = 1;

}

void add_reinforcements( CHAR_DATA *ch )
{
     MOB_INDEX_DATA  * pMobIndex;
     OBJ_DATA        * blaster;
     OBJ_INDEX_DATA  * pObjIndex;

     if ( ( pMobIndex = get_mob_index( ch->backup_mob ) ) == NULL )
        return;

     if ( ch->backup_mob == MOB_VNUM_STORMTROOPER ||
          ch->backup_mob == MOB_VNUM_NR_TROOPER   ||
          ch->backup_mob == MOB_VNUM_MERCINARY       )
     {
        CHAR_DATA * mob[3];
        int         mob_cnt;

        send_to_char( "Posi³ki przyby³y na miejsce!.\n\r", ch );
        for ( mob_cnt = 0 ; mob_cnt < 3 ; mob_cnt++ )
        {
            int ability;
            mob[mob_cnt] = create_mobile( pMobIndex );
            char_to_room( mob[mob_cnt], ch->in_room );
            act( COL_IMMORT, "$N przybywa.", ch, NULL, mob[mob_cnt], TO_ROOM );
            mob[mob_cnt]->top_level = ch->skill_level[LEADERSHIP_ABILITY]/3;
            for ( ability = 0 ; ability < MAX_ABILITY ; ability++ )
                     mob[mob_cnt]->skill_level[ability] = mob[mob_cnt]->top_level;
            mob[mob_cnt]->hit = mob[mob_cnt]->top_level*15;
            mob[mob_cnt]->max_hit = mob[mob_cnt]->hit;
            mob[mob_cnt]->armor = (int)(100- mob[mob_cnt]->top_level*2.5);
            mob[mob_cnt]->damroll = mob[mob_cnt]->top_level/5;
            mob[mob_cnt]->hitroll = mob[mob_cnt]->top_level/5;
            if ( ( pObjIndex = get_obj_index( OBJ_VNUM_BLASTECH_E11 ) ) != NULL )
            {
                 blaster = create_object( pObjIndex, mob[mob_cnt]->top_level );
                 obj_to_char( blaster, mob[mob_cnt] );
                 equip_char( mob[mob_cnt], blaster, WEAR_WIELD );
            }
            if ( mob[mob_cnt]->master )
	       stop_follower( mob[mob_cnt] );
	    add_follower( mob[mob_cnt], ch );
            SET_BIT( mob[mob_cnt]->affected_by, AFF_CHARM );
            do_setblaster( mob[mob_cnt] , (char *)"full" );
        }
     }
     else
     {
        CHAR_DATA *mob;
        int ability;

        mob = create_mobile( pMobIndex );
        char_to_room( mob, ch->in_room );
        if ( ch->pcdata && ch->pcdata->clan )
        {
          char tmpbuf[MAX_STRING_LENGTH];

          sprintf( tmpbuf , "(%s) %s" , ch->pcdata->clan->name  , mob->long_descr );
          STRDUP( mob->long_descr, tmpbuf );
        }
        act( COL_IMMORT, "$N przybywa.", ch, NULL, mob, TO_ROOM );
        send_to_char( "Twój stra¿nik przyby³ na miejsce.\n\r", ch );
        mob->top_level = ch->skill_level[LEADERSHIP_ABILITY];
        for ( ability = 0 ; ability < MAX_ABILITY ; ability++ )
            mob->skill_level[ability] = mob->top_level;
        mob->hit = mob->top_level*15;
        mob->max_hit = mob->hit;
        mob->armor = (int)(100- mob->top_level*2.5);
        mob->damroll = mob->top_level/5;
        mob->hitroll = mob->top_level/5;
        if ( ( pObjIndex = get_obj_index( OBJ_VNUM_BLASTECH_E11 ) ) != NULL )
        {
            blaster = create_object( pObjIndex, mob->top_level );
            obj_to_char( blaster, mob );
            equip_char( mob, blaster, WEAR_WIELD );
        }

        /* for making this more accurate in the future */
        if ( ch->pcdata && ch->pcdata->clan )
           STRDUP( mob->mob_clan, ch->pcdata->clan->name );
     }
}

DEF_DO_FUN( torture )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance, dam;
    bool fail;

    if ( !IS_NPC(ch)
    &&  ch->pcdata->learned[gsn_torture] <= 0  )
    {
	send_to_char(
	    "Zupe³nie nie masz pojêcia jak siê do tego zabraæ.\n\r", ch );
	return;
    }

    if ( IS_NPC(ch) && IS_AFFECTED( ch, AFF_CHARM ) )
    {
	send_to_char( "W tej chwili nie mo¿esz tego zrobiæ.\n\r", ch );
	return;
    }

    one_argument( argument, arg );

    if ( ch->mount )
    {
	send_to_char( "Bêd±c na \"wierzchowcu\" nie jeste¶ w stanie podej¶æ bli¿ej.\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Kogo chcesz torturowaæ?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "Nie ma tutaj nikogo takiego.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Jeste¶ masochist± czy co?\n\r", ch );
	return;
    }

    if ( !IS_AWAKE(victim) )
    {
	send_to_char( "Najpierw musisz obudziæ sw± ofiarê.\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( victim->fighting )
    {
	send_to_char( "Nie mo¿esz torturowaæ kogo¶, kto walczy.\n\r", ch );
	return;
    }

    ch->alignment -= 100;
    ch->alignment = URANGE( -1000, ch->alignment, 1000 );

    WAIT_STATE( ch, skill_table[gsn_torture]->beats );

    fail = false;
    chance = ris_save( victim, ch->skill_level[HUNTING_ABILITY], RIS_PARALYSIS );
    if ( chance == 1000 )
      fail = true;
    else
      fail = saves_para_petri( chance, victim );

    if ( !IS_NPC(ch) && !IS_NPC(victim) )
      chance = sysdata.stun_plr_vs_plr;
    else
      chance = sysdata.stun_regular;
    if ( !fail
    && (  IS_NPC(ch)
    || (number_percent( ) + chance) < ch->pcdata->learned[gsn_torture] ) )
    {
	learn_from_success( ch, gsn_torture );
	WAIT_STATE( ch,     2 * PULSE_VIOLENCE );
	WAIT_STATE( victim, PULSE_VIOLENCE );
	act( COL_ACTION, "$N torturuje Cie powoli. Czujesz straszliwy ból.", victim, NULL, ch, TO_CHAR );
	act( COL_ACTION, "Torturujesz $N$3, zimnym wzrokiem patrz±c jak krzyczy z bólu.", ch, NULL, victim, TO_CHAR );
	act( COL_ACTION, "$n torturuje $N$3, zimnym wzrokiem patrz±c jak krzyczy z bólu!", ch, NULL, victim, TO_NOTVICT );

        dam = dice( ch->skill_level[LEADERSHIP_ABILITY]/10 , 4 );
        dam = URANGE( 0, victim->max_hit-10, dam );
        victim->hit -= dam;
        victim->max_hit -= dam;

        ch_printf( victim, "Tracisz %d punktów uderzeniowych. Utrata jest sta³a!" ,dam);
        ch_printf( ch, "%s traci %d punktów uderzeniowych na sta³e." EOL,
	    PERS(victim, ch,0), dam);

    }
    else
    {
	act( COL_ACTION, "$N próbuje odci±æ Ci palca!", victim, NULL, ch, TO_CHAR );
	act( COL_ACTION, "Robota zosta³a spaprana.", ch, NULL, victim, TO_CHAR );
	act( COL_ACTION, "$n próbuje bole¶nie torturowaæ $N$3.", ch, NULL, victim, TO_NOTVICT );
	WAIT_STATE( ch,     2 * PULSE_VIOLENCE );
        global_retcode = multi_hit( victim, ch, TYPE_UNDEFINED );
    }
    return;

}

DEF_DO_FUN( disguise )
{
    int			i;
    int 		chance;
    AFFECT_DATA 	af;
    CHAR_DATA *		victim;
    char 		arg	[MAX_INPUT_LENGTH];

    switch( ch->substate )
    {
	default:
	    if ( IS_NPC(ch) )
	    {
		send_to_char( "E tam, podszywanie.. po co to komu?" NL, ch );
    		return;
	    }

	    one_argument( argument, arg );

	    if ( arg[0] == '\0' )
	    {
		send_to_char( "Pod kogo chcesz siê podszyæ?" NL, ch );
		return;
	    }

	    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	    {
		send_to_char( "Nie ma tu nikogo takiego." NL, ch );
		return;
	    }

	    if( victim == ch )
	    {
		if( !IS_AFFECTED( ch, AFF_DISGUISE ) )
		{
		    ch_printf( ch, "Nie jeste¶ przebran%s.\n",
			SEX_SUFFIX_YAE( ch ) );
		    return;
		}
		ch_printf( ch, "Przebierasz siê z powrotem w swoje ³aszki..." NL );
    	    }
	    else
	    {
		act( COL_ACTION, "Uwa¿nie przygl±dasz siê $N$2.", ch, NULL, victim, TO_CHAR );
		act( COL_ACTION, "$n patrzy na ciebie.", ch, NULL, victim, TO_VICT );
		act( COL_ACTION, "$n patrzy na $N$3.",  ch, NULL, victim, TO_NOTVICT );
	    }

	    add_timer( ch, TIMER_DO_FUN, skill_table[gsn_disguise]->beats, do_disguise, 1 );
    	    STRDUP( ch->dest_buf,arg);
	    return;
    	case SUB_TIMER_DO_ABORT:
    	    break_skill( FB_RED "Przerywasz przebieranie siê." EOL, ch );
    	    return;
	case 1:
    	    if ( !*ch->dest_buf )
    		return;
    	    strcpy(arg, ch->dest_buf);
    	    STRDUP( ch->dest_buf, "" );
    	    break;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "Nie ma tu nikogo takiego." NL, ch );
	return;
    }
    // zdejmujemy disguise
    if( victim == ch )
    {
	if( !IS_AFFECTED( ch, AFF_DISGUISE ) )
	{
	    ch_printf( ch, "Nie jeste¶ przebran%s." NL,
		SEX_SUFFIX_YAE( ch ) );
	    return;
	}

	affect_strip( ch, gsn_disguise );
	ch_printf( ch, "Ok." NL "%s" NL,
	    skill_table[gsn_disguise]->msg_off );
	return;
    }

    chance = (int) (ch->pcdata->learned[gsn_disguise]);

    if( victim->top_level > ch->top_level + 10 )
    {
        ch_printf( ch,
	    "Przygl±dasz siê, ale nie umiesz dopasowaæ swojego wygl±du do %s." NL,
	    FEMALE(victim) ? "niej" : "niego" );
	if( number_percent( ) > number_range( 65, 89 ) )
	    learn_from_failure( ch, gsn_disguise );
	return;
    }

    if ( number_percent( ) > chance
    || IS_AFFECTED( victim, AFF_DISGUISE ) ) // <-- nie zapetlajmy sie
    {
        send_to_char( "Próbujesz siê przebraæ ale nie udaje Ci siê." NL, ch );
	if( number_percent( ) > number_range( 65, 89 ) )
	    learn_from_failure( ch, gsn_disguise );
	return;
    }

    STRDUP( ch->pcdata->fake_name, victim->name );
    STRDUP( ch->pcdata->fake_desc, victim->description );
    STRDUP( ch->pcdata->fake_long, victim->long_descr );
    for( i=0; i<6; i++ )
	STRDUP( ch->pcdata->fake_infl[i], victim->przypadki[i] );

    if( !IS_NPC( victim ) && victim->pcdata )
	STRDUP( ch->pcdata->fake_title, victim->pcdata->title );
    else
	STRDUP( ch->pcdata->fake_title, "" );

    // zeby affecty nie zachodzily na siebie
    affect_strip( ch, gsn_disguise );

    af.type      = gsn_disguise;
    af.location  = APPLY_NONE;
    af.modifier  = 0;
    af.duration  = 50 + ch->skill_level[SMUGGLING_ABILITY];
    af.bitvector = AFF_DISGUISE;
    affect_to_char( ch, &af );

    learn_from_success( ch, gsn_disguise );
    ch_printf( ch, "Uda³o siê... Mi³ego bycia %s!" NL,
	ch->pcdata->fake_infl[4] );
    return;
}

DEF_DO_FUN( mine )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    bool shovel;
    int move;

   if ( ch->pcdata->learned[gsn_mine] <= 0 )
    {
      ch_printf( ch, "Zupe³nie nie masz pojêcia jak to zrobiæ.\n\r" );
      return;
    }

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        send_to_char( "Czym chcesz zaminowaæ to pomieszczenie?\n\r", ch );
        return;
    }

    if ( ms_find_obj(ch) )
        return;

    shovel = false;
    for ( obj = ch->first_carrying; obj; obj = obj->next_content )
      if ( obj->item_type == ITEM_SHOVEL )
      {
	  shovel = true;
	  break;
      }

    obj = get_obj_list_rev( ch, arg, ch->in_room->last_content );
    if ( !obj )
    {
        send_to_char( "Nie widzisz tutaj niczego.\n\r", ch );
        return;
    }

    separate_obj(obj);
    if ( obj->item_type != ITEM_LANDMINE )
    {
	act( PLAIN, "To nie jest mina l±dowa!", ch, obj, 0, TO_CHAR );
        return;
    }

    if ( !CAN_WEAR(obj, ITEM_TAKE) )
    {
	act( PLAIN, "Nie mo¿esz zakopaæ $p.", ch, obj, 0, TO_CHAR );
        return;
    }

    switch( ch->in_room->sector_type )
    {
	case SECT_CITY:
	case SECT_INSIDE:
	    send_to_char( "Pod³o¿e jest zbyt twarde aby je rozkopaæ.\n\r", ch );
	    return;
	case SECT_WATER_SWIM:
	case SECT_WATER_NOSWIM:
	case SECT_UNDERWATER:
	    send_to_char( "Nie mo¿esz zakopaæ miny w wodzie.\n\r", ch );
	    return;
	case SECT_AIR:
	    send_to_char( "Co?! Chcesz zakopaæ minê w powietrzu?!\n\r", ch );
	    return;
    }

    if ( obj->weight > (UMAX(5, (can_carry_w(ch) / 10)))
    &&  !shovel )
    {
	send_to_char( "Potrzebujesz ³opaty aby zakopaæ co¶ tak du¿ego.\n\r", ch );
	return;
    }

    move = (obj->weight * 50 * (shovel ? 1 : 5)) / UMAX(1, can_carry_w(ch));
    move = URANGE( 2, move, 1000 );
    if ( move > ch->move )
    {
	send_to_char( "Nie masz tyle energii aby zakopaæ cos tak du¿ych rozmiarów.\n\r", ch );
	return;
    }
    ch->move -= move;

    SET_BIT( obj->extra_flags, ITEM_BURRIED );
    WAIT_STATE( ch, URANGE( 10, move / 2, 100 ) );

    STRDUP( obj->armed_by, ch->name );

    ch_printf( ch, "Uzbrajasz i zakopujesz %s.\n\r", obj->przypadki[3] );
    act( PLAIN, "$n uzbraja i zakopuje $p.", ch, obj, NULL, TO_ROOM );

    learn_from_success( ch, gsn_mine );

    return;
}

DEF_DO_FUN( first_aid )
{
   OBJ_DATA   *medpac;
   CHAR_DATA  *victim;
   int         heal;
   char        buf[MAX_STRING_LENGTH];

   if ( ch->position == POS_FIGHTING )
   {
         send_to_char( "Nie mo¿esz tego zrobiæ podczas walki!" NL,ch );
         return;
   }

   medpac = get_eq_char( ch, WEAR_HOLD );
   if ( !medpac || medpac->item_type != ITEM_MEDPAC )
   {
         send_to_char( "Nie trzymasz w rêku apteczki." NL,ch );
         return;
   }

   if ( medpac->value[0] <= 0 )
   {
         send_to_char( "Twoja apteczka pierwszej pomocy jest pusta." NL,ch );
         return;
   }

   if ( argument[0] == '\0' )
      victim = ch;
   else
      victim = get_char_room( ch, argument );

   if ( !victim )
   {
       ch_printf( ch, "Nie widzisz tutaj nikogo takiego jak %s." NL , argument );
       return;
   }

   heal = number_range( 1, 150 );

   if ( heal > ch->pcdata->learned[gsn_first_aid]*2 )
   {
       ch_printf( ch, "Próbujesz swoich umiejêtno¶ci udzielania pierwszej pomocy ale nie udaje Ci siê." NL);
       learn_from_failure( ch , gsn_first_aid );
       return;
   }

   if ( victim == ch )
   {
       ch_printf( ch, "Dogl±dasz swoich ran." NL);
       sprintf( buf , "$n u¿ywa %s aby uleczyæ swoje rany." , medpac->przypadki[1] );
       act( COL_ACTION, buf, ch, NULL, victim, TO_ROOM );
   }
   else
   {
       sprintf( buf , "Dogl±dasz ran %s.", PERS(victim, ch,1) );
       act( COL_ACTION, buf, ch, NULL, victim, TO_CHAR );
       sprintf( buf , "$n u¿ywa %s aby uleczyæ rany %s." , medpac->przypadki[1], PERS(victim, ch,1) );
       act( COL_ACTION, buf, ch, NULL, victim, TO_NOTVICT );
       sprintf( buf , "$n u¿ywa %s aby uleczyæ Twoje rany." , medpac->przypadki[3] );
       act( COL_ACTION, buf, ch, NULL, victim, TO_VICT );
   }

   --medpac->value[0];
   victim->hit += URANGE ( 0, heal , victim->max_hit - victim->hit );

   WAIT_STATE( ch, skill_table[gsn_first_aid]->beats );

   learn_from_success( ch , gsn_first_aid );
}

DEF_DO_FUN( snipe )
{
   OBJ_DATA        * wield;
   char              arg[MAX_INPUT_LENGTH];
   char              arg2[MAX_INPUT_LENGTH];
   int            dir, dist;
   int            max_dist = 3;
   EXIT_DATA       * pexit;
   ROOM_INDEX_DATA * was_in_room;
   ROOM_INDEX_DATA * to_room;
   CHAR_DATA       * victim;
   int               chance;
   char              buf[MAX_STRING_LENGTH];
   bool              pfound = false;

   if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
    {
	send_to_char( "Nie mo¿esz zrobiæ tego st±d." NL, ch );
	return;
    }

   if ( get_eq_char( ch, WEAR_DUAL_WIELD ) != NULL )
   {
         send_to_char( "Nie mo¿esz tego zrobiæ trzymaj±c dwie bronie." NL,ch );
         return;
   }

   wield = get_eq_char( ch, WEAR_WIELD );
   if ( !wield || wield->item_type != ITEM_WEAPON || (wield->value[3] != WEAPON_BLASTER
   && wield->value[3] != WEAPON_BOWCASTER) )
   {
         send_to_char( "Chyba nie trzymasz w rekach blastera ani kuszy." NL,ch );
         return;
   }


   argument = one_argument( argument, arg );
   argument = one_argument( argument, arg2 );

   if ( ( dir = get_door( arg ) ) == -1 || arg2[0] == '\0' )
   {
     send_to_char( "U¿ycie: snipe <kierunek> <cel>" NL, ch );
     return;
   }

   if( !(pexit = get_exit( ch->in_room, dir ))
   || !CAN_ENTER( ch, pexit->to_room ) )
   {
     send_to_char( "Chcesz strzelaæ do kogo¶ przez ¶cianê!?" NL, ch );
     return;
   }

   if ( IS_SET( pexit->flags, EX_CLOSED ) )
   {
     send_to_char( "Chcesz strzelaæ do kogo¶ przez drzwi!?" NL, ch );
     return;
   }

   was_in_room = ch->in_room;

   for ( dist = 0; dist <= max_dist; dist++ )
   {
     if ( IS_SET( pexit->flags, EX_CLOSED ) )
        break;

     if ( !pexit->to_room )
        break;

    to_room = NULL;
    if ( pexit->distance > 1 )
       to_room = generate_exit( ch->in_room , &pexit );

    if ( to_room == NULL )
       to_room = pexit->to_room;

     char_from_room( ch );
     char_to_room( ch, to_room );


     if ( IS_NPC(ch) && ( victim = get_char_room_mp( ch, arg2 ) ) != NULL )
     {
        pfound = true;
        break;
     }
     else if ( !IS_NPC(ch) && ( victim = get_char_room( ch, arg2 ) ) != NULL )
     {
        pfound = true;
        break;
     }


     if ( ( pexit = get_exit( ch->in_room, dir ) ) == NULL )
        break;

   }

   char_from_room( ch );
   char_to_room( ch, was_in_room );

   if ( !pfound )
   {
       ch_printf( ch, "Nie widzisz tej osoby na %s!" NL, dir_name[dir] );
       char_from_room( ch );
       char_to_room( ch, was_in_room );
       return;
   }

    if ( victim == ch )
    {
	send_to_char( "Chcesz siê zastrzeliæ?" NL, ch );
	return;
    }

    if ( IS_SET( victim->in_room->room_flags, ROOM_SAFE ) )
    {
	send_to_char( "Nie mo¿esz nikogo tam zastrzeliæ." NL, ch );
	return;
    }

    if ( is_safe( ch, victim ) )
    {
        send_to_char( "Nie mo¿esz zraniæ tej osoby... Jeszcze." NL, ch );
        return;
	}

    if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
    {
        act( PLAIN, "$N kontroluje Ciê i nie pozwala do siebie strzelaæ.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "W³a¶nie walczysz i strzelasz!" NL, ch );
	return;
    }

    if ( !IS_NPC( victim ) && IS_SET( ch->act, PLR_NICE ) )
    {
      send_to_char( "Czujesz siê zbyt dobrze, aby to zrobiæ!" NL, ch );
      return;
    }

	if (wield->value[4] <= 0)  //Tanglor - zanim wystrzelimy
	{
		ch_printf(ch, "Naciskasz spust i stwierdzasz, ¿e skoñczy³a siê amunicja." NL);
		return;
	}

    chance = IS_NPC(ch) ? 100
           : (int)  (ch->pcdata->learned[gsn_snipe]) ;

    char_from_room( ch );
    char_to_room( ch, victim->in_room );

    if ( number_percent() < chance )
    {
		act( COL_ACTION, "Strzelasz do $N$1.", ch, NULL, victim, TO_CHAR );
		sprintf( buf, "Strza³ z %s trafia Ciê %s.", (wield->value[3] == WEAPON_BOWCASTER)
			? "kuszy" : "blastera", dir_rev_name[dir] );
		act( COL_ACTION, buf, ch, NULL, victim, TO_VICT );
		sprintf( buf, "Strza³ z %s trafia $N$3 %s.", (wield->value[3] == WEAPON_BOWCASTER)
			? "kuszy" : "blastera", dir_rev_name[dir] );
		act( COL_ACTION, buf, ch, NULL, victim, TO_NOTVICT );

		one_hit( ch, victim, TYPE_UNDEFINED );

		if( char_died(ch) )
			return;

       stop_fighting( ch , true );

       learn_from_success( ch, gsn_snipe );
    }
    else
    {
       act( COL_ACTION, "Strzelasz do $N$1, ale nie podchodzisz wystarczaj±co blisko.", ch, NULL, victim, TO_CHAR );
       sprintf( buf, "Nadchodz±cy %s strza³ z %s chybia $N$3.", dir_rev_name[dir],
		   (wield->value[3] == WEAPON_BOWCASTER) ? "kuszy" : "blastera" );
       act( COL_ACTION, buf, ch, NULL, victim, TO_NOTVICT );
       sprintf( buf, "Nadchodz±cy %s strza³ z %s chybia Ciê.", dir_rev_name[dir],
		   (wield->value[3] == WEAPON_BOWCASTER) ? "kuszy" : "blastera" );
       act( COL_ACTION, buf, ch, NULL, victim, TO_VICT );
       learn_from_failure( ch, gsn_snipe );
    }

    char_from_room( ch );
    char_to_room( ch, was_in_room );

   if ( IS_NPC(ch) )
      WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
   else
   {
   	if ( number_percent() < ch->pcdata->learned[gsn_third_attack] )
   	     WAIT_STATE( ch, 1 * PULSE_PER_SECOND );
   	else if ( number_percent() < ch->pcdata->learned[gsn_second_attack] )
   	     WAIT_STATE( ch, 2 * PULSE_PER_SECOND );
   	else
   	     WAIT_STATE( ch, 3 * PULSE_PER_SECOND );
   }
   if ( IS_NPC( victim ) && !char_died(victim) )
   {
      if ( IS_SET( victim->act , ACT_SENTINEL ) )
      {
         victim->was_sentinel = victim->in_room;
         REMOVE_BIT( victim->act, ACT_SENTINEL );
      }

      start_hating( victim , ch );
      start_hunting( victim, ch );

   }

}


DEF_DO_FUN( throw )
{
   OBJ_DATA        * obj;
   OBJ_DATA        * tmpobj;
   char              arg[MAX_INPUT_LENGTH];
   char              arg2[MAX_INPUT_LENGTH];
   char              arg3[MAX_INPUT_LENGTH];
   int            dir;
   EXIT_DATA       * pexit;
   ROOM_INDEX_DATA * was_in_room;
   ROOM_INDEX_DATA * to_room;
   CHAR_DATA       * victim;
   char              buf[MAX_STRING_LENGTH];
   //SHIP_DATA		* target;


   argument = one_argument( argument, arg );
   argument = one_argument( argument, arg2 );
   argument = one_argument( argument, arg3 );

   was_in_room = ch->in_room;

   if ( arg[0] == '\0' )
   {
     send_to_char( "U¿ycie: throw <przedmiot> [kierunek] [cel]" NL, ch );
     //send_to_char( "        throw <przedmiot> [nazwa statku]" NL, ch );
     //send_to_char( "        throw <przedmiot> outship" NL, ch );
     return;
   }

    /*
     * Bosh... to jaki¶ alpinista pisa³ poni¿sze ifchecki? :P
     * -- Thanos
     */
   obj = get_eq_char( ch, WEAR_MISSILE_WIELD );
   if ( !obj || !nifty_is_name( arg, obj->name ) )
      obj = get_eq_char( ch, WEAR_HOLD );
      if ( !obj || !nifty_is_name( arg, obj->name ) )
          obj = get_eq_char( ch, WEAR_WIELD );
          if ( !obj || !nifty_is_name( arg, obj->name ) )
              obj = get_eq_char( ch, WEAR_DUAL_WIELD );
              if ( !obj || !nifty_is_name( arg, obj->name ) )
   if ( !obj || !nifty_is_name_prefix( arg, obj->name ) )
      obj = get_eq_char( ch, WEAR_HOLD );
      if ( !obj || !nifty_is_name_prefix( arg, obj->name ) )
          obj = get_eq_char( ch, WEAR_WIELD );
          if ( !obj || !nifty_is_name_prefix( arg, obj->name ) )
              obj = get_eq_char( ch, WEAR_DUAL_WIELD );
              if ( !obj || !nifty_is_name_prefix( arg, obj->name ) )
   {
         ch_printf( ch, "Nie trzymasz niczego takiego jak: %s." NL, arg );
         return;
   }

    if ( IS_OBJ_STAT(obj, ITEM_NOREMOVE) )
    {
		act( PLAIN, "Nie mo¿esz odrzuciæ $p$1.", ch, obj, NULL, TO_CHAR );
		return;
    }

   if ( ch->position == POS_FIGHTING )
   {
       victim = who_fighting( ch );
       if ( char_died ( victim ) )
           return;
       act( COL_ACTION, "Rzucasz $p$4 w $N$3.", ch, obj, victim, TO_CHAR );
       act( COL_ACTION, "$n rzuca $p$4 w $N$3.", ch, obj, victim, TO_NOTVICT );
       act( COL_ACTION, "$n rzuca w ciebie $p$4.", ch, obj, victim, TO_VICT );
   }
   else if ( arg2[0] == '\0' )
   {
       sprintf( buf, "$n rzuca %s na pod³ogê." , obj->przypadki[3] );
       act( COL_ACTION, buf, ch, NULL, NULL, TO_ROOM );
       ch_printf( ch, "Rzucasz %s na pod³ogê." NL, obj->przypadki[3] );

       victim = NULL;
   }
   else if ( ( dir = get_door( arg2 ) ) != -1 )
   {
      if ( ( pexit = get_exit( ch->in_room, dir ) ) == NULL )
      {
         send_to_char( "Chcesz rzuciæ tym przez ¶cianê!?" NL, ch );
         return;
      }

      if ( IS_SET( pexit->flags, EX_CLOSED ) )
      {
          send_to_char( "Chcesz rzuciæ tym przez drzwi!?" NL, ch );
          return;
      }

      to_room = NULL;
      if ( pexit->distance > 1 )
		  to_room = generate_exit( ch->in_room , &pexit );

      if ( to_room == NULL )
		  to_room = pexit->to_room;

      char_from_room( ch );
      char_to_room( ch, to_room );

      victim = get_char_room( ch, arg3 );

      if ( victim )
      {
		  //if ( is_safe( ch, victim ) )
			//  return;

        if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
        {
        	act( PLAIN, "$N kontroluje Ciê i nie pozwala w siebie rzucaæ.", ch, NULL, victim, TO_CHAR );
			return;
        }

        if ( !IS_NPC( victim ) && IS_SET( ch->act, PLR_NICE ) )
        {
	        send_to_char( "Czujesz siê zbyt dobrze, aby to zrobiæ!" NL, ch );
    	    return;
        }

        char_from_room( ch );
        char_to_room( ch, was_in_room );


        if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
        {
	    	send_to_char( "Nie mo¿esz tego zrobiæ tutaj." NL, ch );
		    return;
        }

        to_room = NULL;
        if ( pexit->distance > 1 )
           to_room = generate_exit( ch->in_room , &pexit );

        if ( to_room == NULL )
           to_room = pexit->to_room;


        if ( IS_SET( to_room->room_flags, ROOM_SAFE ) )
        {
		    send_to_char( "Nie mo¿esz tam tego wrzuciæ." NL, ch );
	    	return;
        }

        char_from_room( ch );
        char_to_room( ch, to_room );

        sprintf( buf , "Kto¶ rzuca %s w Ciebie %s." , obj->przypadki[4] , dir_rev_name[dir] );
        act( COL_ACTION, buf , victim, NULL, ch, TO_CHAR );
        act( COL_ACTION, "Rzucasz $p$4 w $N$3.", ch, obj, victim, TO_CHAR );
        sprintf( buf, "Kto¶ rzuci³ %s w $N$3 %s." , obj->przypadki[4] , dir_rev_name[dir] );
        act( COL_ACTION, buf, ch, NULL, victim, TO_NOTVICT );


      }
      else
      {
         ch_printf( ch, "Rzucasz %s na %s." NL, obj->przypadki[3] , dir_where_name[get_dir( arg2 )] );
         sprintf( buf, "Kto¶ rzuci³ %s %s." , obj->przypadki[3] , dir_rev_name[dir] );
         act( COL_ACTION, buf, ch, NULL, NULL, TO_ROOM );

      }
   }
/*	else if ( ( target = ship_in_room( ch->in_room , arg2 ) ) != NULL )
	{
		to_room = NULL;
		if ( !target->hatchopen )
		{
			ch_printf( ch , "Statek ma zamkniêty w³az." EOL );
			return;
		}
		to_room = target->entrance;
		if ( IS_SET( to_room->room_flags, ROOM_SAFE ) )
		{
			send_to_char( "Nie mo¿esz tego tam wrzuciæ." NL, ch );
			return;
		}
		if ( IS_SET( ch->act, PLR_NICE ) )
		{
			send_to_char( "Czujesz siê zbyt dobrze, aby to zrobiæ!" NL, ch );
			return;
		}
		char_from_room( ch );
		char_to_room( ch, to_room );
		sprintf( buf, FB_YELLOW "Przez otwarty w³az wpada %s.", obj->przypadki[0] );
		act( COL_ACTION, buf, ch, NULL, NULL, TO_ROOM );
		ch_printf( ch, "Wrzucasz %s do %s" NL, obj->przypadki[3] , SHIPNAME( target ) );
	}
   else if ( !str_cmp( arg2 , "outship" ) && ( ( target = ship_from_room( ch->in_room ) ) != NULL ) )
   {
		to_room = NULL;
		if ( target->shipstate != SHIP_DOCKED )
		{
			ch_printf( ch , "Statek musi najpierw wyl±dowaæ." EOL );
			return;
		}
		if ( !target->hatchopen )
		{
			ch_printf( ch , "Otwórz najpierw w³az." EOL );
			return;
		}
		to_room = target->location;
		if ( IS_SET( to_room->room_flags, ROOM_SAFE ) )
		{
			send_to_char( "Nie mo¿esz tego tam wrzuciæ." NL, ch );
			return;
		}
		if ( IS_SET( ch->act, PLR_NICE ) )
		{
			send_to_char( "Czujesz siê zbyt dobrze, aby to zrobiæ!" NL, ch );
			return;
		}
		char_from_room( ch );
		char_to_room( ch, to_room );
		sprintf( buf, FB_YELLOW "Przez otwarty w³az %s wypada %s.", SHIPNAME( target ), obj->przypadki[0] );
		act( COL_ACTION, buf, ch, NULL, NULL, TO_ROOM );
		ch_printf( ch, "Wyrzucasz %s na l±dowisko." NL, obj->przypadki[3] );
   }
*/   else if ( ( victim = get_char_room( ch, arg2 ) ) != NULL )
   {
        if ( is_safe( ch, victim ) )
	return;

        if ( IS_AFFECTED(ch, AFF_CHARM) && ch->master == victim )
        {
        act( PLAIN, "$N kontroluje Ciê i nie pozwala w siebie rzuciæ.", ch, NULL, victim, TO_CHAR );
	return;
        }

        if ( !IS_NPC( victim ) && IS_SET( ch->act, PLR_NICE ) )
        {
        send_to_char( "Czujesz siê zbyt dobrze, aby to zrobiæ." NL, ch );
        return;
        }

   }
   else
   {
       ch_printf( ch, "Nie ma tutaj nikogo takiego." NL);
       return;
   }


   if ( obj == get_eq_char( ch, WEAR_WIELD )
   && ( tmpobj = get_eq_char( ch, WEAR_DUAL_WIELD)) != NULL )
       tmpobj->wear_loc = WEAR_WIELD;

//   target = NULL;
   unequip_char( ch, obj );
   separate_obj( obj );
   obj_from_char( obj );
   obj = obj_to_room( obj, ch->in_room );

   if ( obj->item_type != ITEM_GRENADE )
       damage_obj ( obj );

/* NOT NEEDED UNLESS REFERING TO OBJECT AGAIN

   if( obj_extracted(obj) )
      return;
*/
   if ( ch->in_room !=  was_in_room )
   {
     char_from_room( ch );
     char_to_room( ch, was_in_room );
   }

   if ( !victim || char_died( victim ) )
       learn_from_failure( ch, gsn_throw );
   else
   {

       WAIT_STATE( ch, skill_table[gsn_throw]->beats );
       if ( IS_NPC(ch) || number_percent( ) < ch->pcdata->learned[gsn_throw] )
       {
	 learn_from_success( ch, gsn_throw );
	 global_retcode = damage( ch, victim, number_range( obj->weight*2 , (obj->weight*2 + ch->perm_str) ), TYPE_HIT );
       }
       else
       {
	 learn_from_failure( ch, gsn_throw );
	 global_retcode = damage( ch, victim, 0, TYPE_HIT );
       }

       if ( IS_NPC( victim ) && !char_died ( victim) )
       {
          if ( IS_SET( victim->act , ACT_SENTINEL ) )
          {
             victim->was_sentinel = victim->in_room;
             REMOVE_BIT( victim->act, ACT_SENTINEL );
          }

          start_hating( victim , ch );
          start_hunting( victim, ch );

       }

   }

   return;

}

DEF_DO_FUN( beg )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int percent, xp;
    int amount;

    if ( IS_NPC (ch) ) return;

    argument = one_argument( argument, arg1 );

    if ( ch->mount )
    {
	send_to_char( "Nie mo¿esz zrobiæ tego dosiadaj±c \"wierzchowca\"." NL, ch );
	return;
    }

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Od kogo chcesz wy¿ebraæ pieni±dze?" NL, ch );
	return;
    }

    if ( ms_find_obj(ch) )
	return;

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nie ma tutaj nikogo takiego." NL, ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Siebie? To bezcelowe." NL, ch );
	return;
    }

    if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
    {
	send_to_char( "To nie jest najlepsze miejsce na ¿ebranie." NL, ch );
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
        send_to_char( "Hm, interesuj±ca taktyka walki." NL, ch );
        return;
    }

    if ( ch->position <= POS_SLEEPING )
    {
        send_to_char( "¦nisz o ¿ebraniu..." NL , ch );
        return;
    }


    if ( victim->position == POS_FIGHTING )
    {
        ch_printf( ch, "%s jest wyra¼nie zajêt%s." NL ,
	    capitalize(NAME(victim,0)), SEX_SUFFIX_YAE( victim ) );
        return;
    }
    if ( victim->position <= POS_SLEEPING )
    {
        ch_printf( ch, "A mo¿e by tak najpierw %s obudziæ?" NL ,
	    FEMALE(victim)?"j±":MALE(victim)?"go":"je" );
        return;
    }


	send_to_char( "¯ebrzesz o pieni±dze." NL, ch );
	act( COL_ACTION, "$n prosi Ciê na kolanach o pieni±dze.", ch, NULL, victim, TO_VICT    );
	act( COL_ACTION, "$n prosi na kolanach $N$3 o drobniaki.",  ch, NULL, victim, TO_NOTVICT );

    if ( !IS_NPC( victim ) )
    {
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_beg]->beats );
    percent  = number_percent( ) + ch->skill_level[SMUGGLING_ABILITY] + victim->top_level;

    if ( percent > ch->pcdata->learned[gsn_beg]  )
    {
	/*
	 * Failure.
	 */
	send_to_char( "Jako¶ nikt nie zwróci³ na ciebie uwagi..." NL, ch );
	act( COL_ACTION, "$n zaczyna graæ Ci na nerwach tym ¿ebraniem!", ch, NULL, victim, TO_VICT    );

        if ( victim->alignment < 0 && victim->top_level >= ch->top_level+5 )
	{
	  sprintf( buf, "%s jest dra¿ni±cym ¿ebrakiem i zaraz dostanie nauczkê!",
	    PERS(ch, victim, 0) );
	  do_yell( victim, buf );
          global_retcode = multi_hit( victim, ch, TYPE_UNDEFINED );
	}

	learn_from_failure( ch, gsn_beg );

	return;
    }


    amount = UMIN( victim->gold , number_range(1, 10) );
    if ( amount <= 0 )
    {
        do_look( victim , ch->name );
	do_say( victim , (char *)"Przykro mi, ale nie mam pieniêdzy." );
	learn_from_failure( ch, gsn_beg );
	return;
    }

	ch->gold     += amount;
	victim->gold -= amount;
	ch_printf( ch, "%s daje ci %d kredyt%s." NL,
	    capitalize(NAME(victim,0)),
		amount, NUMBER_SUFF( amount, "kê", "ki", "ek" ) );
	learn_from_success( ch, gsn_beg );
	xp = UMIN( amount*10 , ( exp_level( ch->skill_level[SMUGGLING_ABILITY]+1) - exp_level( ch->skill_level[SMUGGLING_ABILITY])  )  );
        xp = UMIN( xp , xp_compute( ch, victim ) );
        gain_exp( ch, xp, SMUGGLING_ABILITY );
        ch_printf( ch, "Otrzymujesz %ld punktów do¶wiadczenia w zakresie przemytu!" NL, xp );
        act( COL_ACTION, "$N daje $n$2 jakie¶ pieni±dze.",  ch, NULL, victim, TO_NOTVICT );
        act( COL_ACTION, "Dajesz $n$2 kilka kredytek.", ch, NULL, victim, TO_VICT    );

	return;

}

DEF_DO_FUN( pickshiplock )
{
   do_pick( ch, argument );
}

DEF_DO_FUN( hijack )
{
    bool	ship_in_closebay(SHIP_DATA *ship);
	int chance;
    SHIP_DATA *ship;
    char buf[MAX_STRING_LENGTH];
    CREW_DATA *member;

    if ( (ship = ship_from_cockpit(ch->in_room)) == NULL )
    {
        send_to_char("&RMusisz byæ w kokpicie statku aby to zrobiæ!" NL,ch);
        return;
    }

    if ( !is_ship( ship ) )
    {
        send_to_char("&RTego statku, je¶li mo¿na tak to nazwaæ, nie da siê ukra¶æ!" NL,ch);
        return;
    }

    if ( (ship = ship_from_pilotseat(ch->in_room)) == NULL )
    {
        send_to_char("&RMusisz siedzieæ w fotelu pilota!" NL,ch);
        return;
    }

	if( !known_biotech( ship, ch ) )
	{
		ch_printf(ch, FB_RED "To jest wytwór obcej biotechnologi, niestety nie masz o niej zielonego pojêcia." EOL);
		return;
	}

	if ( check_pilot( ch , ship ) )
    {
        send_to_char("&RJaki jest cel tego?" NL,ch);
        return;
    }

    if ( check_crew ( ship, ch, "all" ) )
    {
   	 ch_printf(ch, FB_RED "Jaki jest cel tego?" EOL);
   	 return;
    }
    if ( ship->type == MOB_SHIP && get_trust(ch) < 102 )
    {
        send_to_char("&RNie jeste¶ w stanie ukra¶æ tego statku..." NL,ch);
        return;
    }

    if  ( is_platform(ship) )
    {
       send_to_char( "Nie mo¿esz zrobiæ tego tutaj." NL , ch );
       return;
    }

    if ( ship->lastdoc != ship->location )
    {
         if (ship->starsystem)
	 {
	    hijack( ch , ship );
	    return;
	 }
	 else
	 {
	    send_to_char("&rStatek nie jest zadokowany." NL,ch);
            return;
	 }
    }

    if ( ship->shipstate != SHIP_DOCKED && ship->shipstate != SHIP_DISABLED )
    {
        send_to_char("Ten statek nie dokuje w obecnej chwili." NL,ch);
        return;
    }

    if ( ship->shipstate == SHIP_DISABLED )
    {
        send_to_char("Napêd statku jest uszkodzony." NL,ch);
        return;
    }

	if ( ship_in_closebay(ship) == true )
    {
    	send_to_char( FB_RED "Ups!!!  hangar jest zamkniety." EOL, ch);
    	return;
    }

	chance = IS_NPC(ch) ? ch->top_level
             : (int)  (ch->pcdata->learned[gsn_hijack]) ;
    if ( number_percent( ) > chance )
    {
	send_to_char("Nie jeste¶ w stanie wprowadziæ odpowiedniego kodu." NL,ch);
	learn_from_failure( ch, gsn_hijack );
        return;
    }

    if ( is_fighter(ship) )
        chance = IS_NPC(ch) ? ch->top_level
		: (int)  (ch->pcdata->learned[gsn_starfighters]) ;

    if ( is_midship(ship) )
        chance = IS_NPC(ch) ? ch->top_level
                : (int)  (ch->pcdata->learned[gsn_midships]) ;

    if ( is_capital(ship) )
        chance = IS_NPC(ch) ? ch->top_level
	        : (int) (ch->pcdata->learned[gsn_capitalships]);

    if ( number_percent( ) < chance )
    {
       if (ship->hatchopen)
       {
	    ship->hatchopen = false;
    	    sprintf( buf , FB_YELLOW "Klapa statku %s zamyka siê." , ship->name);
       	    echo_to_room( ship->location, buf );
       	    echo_to_room( ship->entrance, FB_YELLOW "Klapa statku zamyka siê." );
       	}

    	send_to_char( "Sekwencja startowa rozpoczêta." NL, ch);
    	act( PLAIN, "$n uruchamia statek i rozpoczyna sekwencjê startow±.", ch,
			NULL, argument , TO_ROOM );

	echo_to_ship( ship , FB_YELLOW "Statek buczy i unosi siê ponad ziemiê.");
    	sprintf( buf, FB_YELLOW "%s zaczyna startowaæ.", ship->name );
    	echo_to_room( ship->location, buf );
    	ship->shipstate = SHIP_LAUNCH;
    	ship->currspeed = ship->realspeed;

	if ( is_fighter(ship) )
    	    learn_from_success( ch, gsn_starfighters );

	if ( is_midship(ship) )
    	    learn_from_success( ch, gsn_midships );

	if ( is_capital(ship) )
    	    learn_from_success( ch, gsn_capitalships );

   CREATE( member, CREW_DATA, 1 );
   member->name = ch;
   member->ship = ship;
   member->rank = 1;
   LINK( member, first_cmember, last_cmember, next, prev );
        learn_from_success( ch, gsn_hijack );
        sprintf( buf, "%s porwa³ %s!", ch->name, ship->name );
    	log_string( buf );

        return;
    }

    send_to_char("Nie jeste¶ w stanie ustawiæ wszystkiego odpowiednio!" NL,ch);

    if ( is_fighter(ship) )
        learn_from_failure( ch, gsn_starfighters );

    if ( is_midship(ship) )
	learn_from_failure( ch, gsn_midships );

    if ( is_capital(ship) )
	learn_from_failure( ch, gsn_capitalships );

    return;
}

DEF_DO_FUN( add_patrol )
{}

DEF_DO_FUN( special_forces )
{}

DEF_DO_FUN( elite_guard )
{

}
/*
DEF_DO_FUN( jail )
{
    CHAR_DATA *victim =NULL;
    CLAN_DATA   *clan =NULL;
    ROOM_INDEX_DATA *jail =NULL;

    if ( IS_NPC (ch) ) return;

    if ( !ch->pcdata || ( clan = ch->pcdata->clan ) == NULL )
    {
	send_to_char( "Tylko cz³onkowie organizacji mog± wtr±caæ do wiêzieñ." NL, ch );
	return;
    }

    jail = get_room_index( clan->jail );
    if ( !jail && clan->mainorg )
       jail = get_room_index( clan->mainorg->jail );

    if ( !jail )
    {
	send_to_char( "Twoja organizacja nie ma odpowiedniego wiêzienia." NL, ch );
	return;
    }

    if ( jail->area && ch->in_room->area
    && jail->area != ch->in_room->area &&
    ( !jail->area->planet || jail->area->planet != ch->in_room->area->planet ) )
    {
         send_to_char( "Wiêzienie Twojej organizacji jest zbyt daleko." NL, ch );
	 return;
    }

    if ( ch->mount )
    {
	send_to_char( "Nie mo¿esz tego zrobiæ dosiadaj±c \"wierzchowca\"." NL, ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Kogo wtr±ciæ do wiêzienia?" NL, ch );
	return;
    }

    if ( ( victim = get_char_room( ch, argument ) ) == NULL )
    {
	send_to_char( "Nie ma tutaj nikogo takiego." NL, ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Siebie? To bezcelowe." NL, ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "To strata czasu." NL, ch );
	return;
    }

    if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
    {
	send_to_char( "To nie najlepsze miejsce na to." NL, ch );
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
        send_to_char( "Hm, interesuj±ca technika walki." NL , ch );
        return;
    }

    if ( ch->position <= POS_SLEEPING )
    {
        send_to_char( "Sni Ci siê wiêzienie!" NL , ch );
        return;
    }

    if ( victim->position >= POS_SLEEPING )
    {
        send_to_char( "Najpierw trzeba og³uszyæ ofiarê." NL , ch );
        return;
    }

	send_to_char( "Eskortujesz sw± ofiarê do wiêzienia." NL, ch );
	act( COL_ACTION, "Masz dziwne uczucie, ¿e siê przemieszczasz." NL, ch, NULL, victim, TO_VICT    );
	act( COL_ACTION, "$n eskortuje $N do wiêzienia." NL,  ch, NULL, victim, TO_NOTVICT );

    char_from_room ( victim );
    char_to_room ( victim , jail );

    act( COL_ACTION, "Drzwi otwieraj± siê na chwilê i $n wpada do pomieszczenia." NL,  victim, NULL, NULL, TO_ROOM );

    learn_from_success( ch , gsn_jail );

    return;
}
*/

/* completely rewritten by Thanos */
DEF_DO_FUN( smalltalk )
{
    char 		buf  	[MAX_STRING_LENGTH];
    char 		arg1 	[MAX_INPUT_LENGTH];
    CHAR_DATA *		victim 	= NULL;
    PLANET_DATA *	planet 	= NULL;
    CLAN_DATA   *	clan 	= NULL;
    int 		chance;

    if( !check_knowledge( ch, gsn_smalltalk ) )
	return;

    if ( IS_NPC(ch) || !ch->pcdata )
    {
	send_to_char( "Tak tak, pogadaj sobie..." NL, ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    switch( ch->substate )
    {
    	default:
	    if ( ch->mount )
	    {
		mount_msg( ch );
		return;
	    }

	    if ( arg1[0] == '\0' )
	    {
		send_to_char( "Z kim chcesz urz±dziæ sobie pogawêdkê?" NL, ch );
		return;
	    }

	    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
	    {
		hes_not_here_msg( ch );
		return;
	    }

	    if ( victim == ch )
	    {
		send_to_char( "No tak. Zaczynasz gadaæ do siebie... Jest ¼le." NL, ch );
		return;
	    }

	    if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
	    {
		send_to_char( "To nie jest najlepsze miejsce na to." NL, ch );
		return;
	    }

	    if ( ch->position == POS_FIGHTING )
	    {
    		send_to_char( "Hmm... interesuj±ca technika walki." NL , ch );
    		return;
	    }

	    if ( victim->position == POS_FIGHTING )
	    {
    		send_to_char( "Twój rozmówca jest teraz troszeczkê zajêty." NL, ch );
    		return;
	    }

	    if ( ch->position <= POS_SLEEPING )
	    {
    		send_to_char( "Chcesz gadaæ przez sen? To niezbyt dyplomatyczne." NL, ch );
    		return;
	    }

	    if ( IS_NPC(ch)
	    || !ch->pcdata
	    || !ch->pcdata->clan
	    || !ch->in_room->area
	    || !ch->in_room->area->planet )
	    {
		send_to_char( "Rozmowa tutaj nic nie da." NL, ch );
		return;
	    }

	    send_to_char( "Zaczynasz rozmowê..." NL, ch );
	    act( COL_ACTION, "$n u¶miecha siê do Ciebie i mówi 'witaj'.", ch, NULL, victim, TO_VICT    );
	    act( COL_ACTION, "$n zaczyna pogawêdkê z $N$4...", ch, NULL, victim, TO_NOTVICT );

	    STRDUP( ch->dest_buf, arg1 );
	    add_timer( ch, TIMER_DO_FUN,
			    skill_table[gsn_smalltalk]->beats/4,
			    do_smalltalk, 1 );
	    return;

    	case 1:
    	    if ( !*ch->dest_buf )
	    {
		bug( "NULL dest_buf: %s", ch->name );
		send_to_char( "Ups. Co¶ jest nie tak!" NL, ch );
		return;
	    }
	    strcpy( arg1, ch->dest_buf );
    	    STRDUP( ch->dest_buf, "" );
	    ch->substate = SUB_NONE;
	    break;

	case SUB_TIMER_DO_ABORT:
	    break_skill( "Przerywasz rozmowê w ma³o kulturalny sposób." NL, ch );
	    return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	hes_not_here_msg( ch );
	return;
    }

    if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
    {
	send_to_char( "To nie jest najlepsze miejsce na to." NL, ch );
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "Hmm... interesuj±ca technika walki." NL , ch );
	return;
    }

    if ( victim->position == POS_FIGHTING )
    {
	send_to_char( "Twój rozmówca jest teraz troszeczkê zajêty." NL, ch );
	return;
    }

    if ( ch->position <= POS_SLEEPING )
    {
	send_to_char( "Chcesz gadaæ przez sen? To niezbyt dyplomatyczne." NL, ch );
	return;
    }

    if ( ( clan = ch->pcdata->clan->mainorg ) == NULL )
       clan = ch->pcdata->clan;

    planet = ch->in_room->area->planet;

    if ( clan != planet->governed_by )
    {
	send_to_char( "Mo¿e najpierw niech twoja organizacja przejmie tê planetê." NL, ch );
        return;
    }

    if ( !IS_NPC(victim)
    || (victim->s_vip_flags && victim->s_vip_flags[0] != '\0' ) )
    {
	send_to_char( "W tej sytuacji twoje dyplomacja nic nie da..." NL, ch );
	check_social( victim, "marszcz", ch->name );
	return;
    }

    if ( victim->position <= POS_SLEEPING )
    {
	ch_printf( ch,"Ale %s ¶pi w najlepsze. Jak chcesz zacz±æ rozmowê?" NL,
	    PERS(victim, ch,0) );
	return;
    }

    /* test charyzmy */
    if( get_curr_cha( ch ) < get_curr_cha( victim )
    &&  number_percent( )  <(get_curr_cha( victim )-get_curr_cha( ch ))*10 )
    {
	act( COL_ACTION, "$N nie chce zwracaæ uwagi na takiego pionka jak ty.", ch, NULL, victim, TO_CHAR    );
	act( COL_ACTION, "$n co¶ tam od ciebie chcia³$o. Pionek.", ch, NULL, victim, TO_VICT    );
	act( COL_ACTION, "$N nie chce zwracaæ uwagi na takiego pionka jak $n.",  ch, NULL, victim, TO_NOTVICT );
	return;
    }

    /* srednia levelu i znajomo¶ci skilla przeciw levelowi i losowi */
    chance =  (ch->skill_level[DIPLOMACY_ABILITY] +
	       ch->pcdata->learned[gsn_smalltalk] ) / 2;

    if (  chance < victim->top_level + number_percent() + 30 )
    {
	/*
	 * Failure.
	 */
	act( COL_ACTION, "Czujesz, ¿e zosta³$a¶ kompletnie zignorowan$y!", ch, NULL, victim, TO_CHAR    );
	act( COL_ACTION, "$n Zaczyna Ciê denerwowaæ ci±g³± chêci± rozmowy!", ch, NULL, victim, TO_VICT    );
	act( COL_ACTION, "$n zagaduje $N$3, ale $E wyra¼nie $i ignoruje.",  ch, NULL, victim, TO_NOTVICT );

        if ( victim->alignment < -500 && victim->top_level >= ch->top_level+5 )
	{
	  sprintf( buf, "ZAMKNIJ SIÊ %s!", PERS(ch, victim,0) );
	  do_yell( victim, buf );
          global_retcode = multi_hit( victim, ch, TYPE_UNDEFINED );
	}

	learn_from_failure( ch, gsn_smalltalk );
	return;
    }

    planet->pop_support += 0.2;
    act( FG_CYAN, "$N mówi 'O tak $n, to ¶wietnie, ¿e poruszy³$a¶ ten temat...'",  ch, NULL, victim, TO_NOTVICT );
    act( FG_CYAN, "$N mówi 'O tak $n, to ¶wietnie, ¿e poruszy³$a¶ ten temat...'",  ch, NULL, victim, TO_CHAR );
    send_to_char( "Ofiara po³knê³a haczyk!" NL, ch );
    send_to_char( "Poparcie dla Twojej organizacji wzrasta nieznacznie." NL, ch );

    gain_exp(ch, victim->top_level*10, DIPLOMACY_ABILITY);
    ch_printf( ch , "Otrzymujesz %d punktów do¶wiadczenia w zakresie dyplomacji." NL,
	victim->top_level*10 );

    learn_from_success( ch, gsn_smalltalk );

    if ( planet->pop_support > 100 )
        planet->pop_support = 100;

    return;
}

DEF_DO_FUN( propeganda )
{
    char 		buf  [MAX_STRING_LENGTH];
    char 		arg1 [MAX_INPUT_LENGTH];
    CHAR_DATA *		victim;
    PLANET_DATA *	planet;
    CLAN_DATA   *	clan;

   if ( IS_NPC(ch) || !ch->pcdata || !ch->pcdata->clan || !ch->in_room->area || !ch->in_room->area->planet )
   {
       send_to_char( "Jaki by³by tego cel?" NL, ch );
       return;
   }

    argument = one_argument( argument, arg1 );

    if ( ch->mount )
    {
	send_to_char( "Dosiadaj±c nie mo¿esz." NL, ch );
	return;
    }

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Do kogo?" NL, ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nie ma tutaj nikogo takiego." NL, ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Siebie? To bezcelowe." NL, ch );
	return;
    }

    /* by Ratm. Ta komenda ma dzialac ponoc tylko na moby */
    if( !IS_NPC( victim ) )
    {
	ch_printf(ch, "%s nie jest mieszkañcem tej planety." NL, victim->name );
	return;
    }

    /* by Ratm. Je¶li mob jest roslina lub owadem to nie bywa rozmowny */
    if( !IS_INTELLIGENT( victim ) )
    {
	send_to_char( "¦wietny wybór, oto rozmówca godny Ciebie!" NL, ch );
	return;
    }

    if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
    {
	send_to_char( "To nie jest najlepsze miejsce na to." NL, ch );
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
        send_to_char( "Hm, interesuj±ca technika walki." NL , ch );
        return;
    }

    if ( victim->position == POS_FIGHTING )
    {
        send_to_char( "Osoba ta jest teraz zajêta." NL , ch );
        return;
    }


    if ( victim->s_vip_flags && victim->s_vip_flags[0] != '\0' )
    {
        send_to_char( "Dyplomacja w niczym nie pomo¿e." NL , ch );
        return;
    }

    if ( ch->position <= POS_SLEEPING )
    {
        send_to_char( "W snach?" NL , ch );
        return;
    }

    if ( victim->position <= POS_SLEEPING )
    {
        send_to_char( "Chcesz rozmawiaæ ze ¶pi±cym." NL , ch );
        return;
    }

    if ( ( clan = ch->pcdata->clan->mainorg ) == NULL )
       clan = ch->pcdata->clan;

    planet = ch->in_room->area->planet;

    sprintf( buf, ", oraz z³u jakie panuje na %s" , planet->governed_by ? planet->governed_by->name : "" );
    ch_printf( ch, "Rozmawiasz o korzy¶ciach %s%s." NL, ch->pcdata->clan->name,
        planet->governed_by == clan ? "" : buf );
    act( COL_ACTION, "$n rozmawia o swojej organizacji." NL, ch, NULL, victim, TO_VICT    );
    act( COL_ACTION, "$n mówi $N o swojej organizacji." NL,  ch, NULL, victim, TO_NOTVICT );

    WAIT_STATE( ch, skill_table[gsn_propeganda]->beats );

    if (victim->top_level - get_curr_cha(ch) > ch->pcdata->learned[gsn_propeganda])
	{

		if (planet->governed_by != clan)
		{
			sprintf(buf, "%s jest zdrajc±!", ch->name);
			do_yell(victim, buf);
			global_retcode = multi_hit(victim, ch, TYPE_UNDEFINED);
		}

		return;
	}

    if ( planet->governed_by == clan )
    {
       planet->pop_support += .5 + ch->top_level/50;
       send_to_char( "Powszechne poparcie dla Twej organizacji wzrasta." NL, ch );
    }
    else
    {
       planet->pop_support -= ch->top_level/50;
       send_to_char( "Powszechne poparcie dla obecnego rz±du maleje." NL, ch );
    }

    gain_exp(ch, victim->top_level*100, DIPLOMACY_ABILITY);
    ch_printf( ch , " Otrzymujesz %d punktów do¶wiadczenia w zakresie dyplomacji." NL, victim->top_level*100 );

    learn_from_success( ch, gsn_propeganda );

    planet->pop_support = URANGE( -100, planet->pop_support, 100 );

    return;
}

DEF_DO_FUN( bribe )
{
    char arg1 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    PLANET_DATA *planet;
    CLAN_DATA   *clan;
    int amount = 0;

    if ( IS_NPC(ch) || !ch->pcdata || !ch->pcdata->clan || !ch->in_room->area || !ch->in_room->area->planet )
    {
      send_to_char( "Jaki jest cel tego?" NL, ch );
      return;
    }

    argument = one_argument( argument, arg1 );

    if ( ch->mount )
    {
	send_to_char( "Zsi±dz najpierw z \"wierzchowca\"." NL, ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Przekupiæ kogo i jak± ilo¶ci±?" NL, ch );
	return;
    }

    amount = atoi( argument );

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nie ma tutaj nikogo takiego." NL, ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "To bezcelowe." NL, ch );
	return;
    }

    if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
    {
	send_to_char( "To nie najlepsze miejsce na przekupstwo." NL, ch );
	return;
    }

    if ( amount <= 0 )
    {
       	send_to_char( "Nieco wiêksza ilo¶æ pieniêdzy by³aby lepszym pomys³em." NL, ch );
	return;
    }

    if( ch->gold < amount )
    {
	send_to_char( "Chcesz zaoferowac wiêcej kredytek ni¿ masz!" NL, ch );
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
        send_to_char( "Hm, interesuj±ca technika walki." NL , ch );
        return;
    }

    if ( victim->position == POS_FIGHTING )
    {
        send_to_char( "Ta osoba jest teraz zajêta." NL , ch );
        return;
    }

    if ( ch->position <= POS_SLEEPING )
    {
        send_to_char( "W snach?" NL , ch );
        return;
    }

    if ( victim->position <= POS_SLEEPING )
    {
        send_to_char( "Najpierw trzeba kogo¶ obudziæ, ¿eby go potem przekupiæ." NL , ch );
        return;
    }

    if ( victim->s_vip_flags && victim->s_vip_flags[0] != '\0' )
    {
        send_to_char( "Dyplomacja nie przyda siê w tym przypadku." NL , ch );
        return;
    }

    ch->gold -= amount;
    victim->gold += amount;

	ch_printf( ch, "dajesz ma³y prezent na rzecz %s." NL, ch->pcdata->clan->name );
	act( COL_ACTION, "$n chce Cie przekupiæ." NL, ch, NULL, victim, TO_VICT    );
	act( COL_ACTION, "$n daje $N jakie¶ pieni±dze." NL,  ch, NULL, victim, TO_NOTVICT );

    if ( !IS_NPC( victim ) )
	return;

    WAIT_STATE( ch, skill_table[gsn_bribe]->beats );

    if ( victim->top_level - amount > ch->pcdata->learned[gsn_bribe]  )
        return;

    if ( ( clan = ch->pcdata->clan->mainorg ) == NULL )
       clan = ch->pcdata->clan;

    planet = ch->in_room->area->planet;


    if ( clan == planet->governed_by )
    {
      planet->pop_support += URANGE( 0.1 , amount/1000 , 2 );
      send_to_char( "Poparcie dla Twojej organizacji nieznacznie wzrasta." NL, ch );

      amount = UMIN( (amount/(30-get_curr_int(ch))) ,( exp_level(ch->skill_level[DIPLOMACY_ABILITY]+1) - exp_level(ch->skill_level[DIPLOMACY_ABILITY]) ) );

      gain_exp(ch, amount , DIPLOMACY_ABILITY);
      ch_printf( ch , " Otrzymujesz %d punktów do¶wiadczenia w zakresie dyplomacji." NL, amount );

      learn_from_success( ch, gsn_bribe );
    }

    if ( planet->pop_support > 100 )
        planet->pop_support = 100;
}

void do_dominate( CHAR_DATA *ch, char *argument ) //byTrog
{
    CHAR_DATA	*victim;
    AFFECT_DATA	af;
    int		chance,modifier;
    char	buf[MAX_STRING_LENGTH];


    if( *argument=='\0' )
    {
	send_to_char( "Nad kim chcesz przej±æ kontrolê?" NL, ch );
	return;
    }

    if( !( victim = get_char_room( ch, argument ) ) )
    {
	send_to_char( "Nie ma tutaj nikogo takiego." NL, ch );
	return;
    }

    if( victim == ch )
    {
	send_to_char( "Chcesz przej±æ kontrolê nad sob±?!" NL, ch );
	return;
    }

    if( ch->mount )
    {
	send_to_char( "Najpierw zsi±d¼ z \"wierzchowca\"." NL, ch );
	return;
    }

    if( ch->position <= POS_SLEEPING )
    {
	send_to_char( "W snach?" NL, ch );
	return;
    }

    if( victim->position <= POS_SLEEPING )
    {
	send_to_char( "Najpierw trzeba t± osobê obudziæ!" NL, ch );
	return;
    }

    if( !IS_NPC( victim ) && !IS_NPC( ch ) )
    {
	send_to_char( "Próbujesz zdominowaæ sw± ofiarê ale czujesz, ¿e ta jest zbyt silna na to." NL, ch );
	send_to_char( "Czujesz jakby kto¶ próbowa³ Cie zdominowaæ ale jest na to za s³aby." NL, ch );
	return;
    }

    if( IS_AFFECTED( ch, AFF_CHARM )
    ||  IS_AFFECTED( ch, AFF_DOMINATED )
    ||  IS_AFFECTED( victim, AFF_DOMINATED )
    ||  IS_AFFECTED( victim, AFF_CHARM )
    ||  ch->top_level < victim->top_level )
//	|| ch->skill_level[LEADERSHIP_ABILITY] < victim->skill_level[LEADERSHIP_ABILITY] )
    {
	send_to_char( "Nie uda³o Ci siê." NL, ch );
	return;
    }

    chance = (int) (ch->pcdata->learned[gsn_dominate]);
    if( number_percent() > chance ) //tutaj celowo jest > a nie <
    {
	send_to_char( "Nie uda³o Ci siê." NL, ch );
	learn_from_failure( ch, gsn_dominate );
	return;
    }

    if( get_curr_int(victim)+5 > get_curr_int(ch)
    || get_curr_cha(victim)+6 > get_curr_cha(ch)
    || IS_SET( victim->immune, RIS_CHARM ) )
    {
	if( is_safe( ch, victim ) )
	    send_to_char( "Ofiara patrzy na Ciebie ze z³o¶ci±." NL, ch );
	else
	{
	    send_to_char( "Ofiara zauwa¿y³a Twoj± próbê dominacji i... ups." NL, ch );
	    set_fighting( ch, victim );
	}
	return;
    }

    modifier = 0;
    if( (get_curr_int(ch) - get_curr_int(victim)) >= 8 )
	modifier += 10;
    if( (get_curr_cha(ch) - get_curr_cha(victim)) >= 8 )
	modifier += 12;


    if ( victim->master )
        stop_follower( victim );
    add_follower( victim, ch );
    victim->master = ch;

    af.type      = gsn_dominate;
    af.duration  = dice( 1, number_range( 9, 17 ) ) + modifier * (ch->top_level / 2);
    af.location  = 0;
    af.modifier  = 0;
    af.bitvector = AFF_DOMINATED;
    affect_to_char( victim, &af );
    act( COL_FORCE, "Patrzysz na $n$3. Czy¿ ta osoba nie jest mi³a?" NL, ch, NULL, victim, TO_VICT );
    act( COL_FORCE, "$N zaczyna zachowywaæ siê inaczej." NL, ch, NULL, victim, TO_ROOM );
    send_to_char( "Ok. Uda³o siê." NL, ch );

    if( get_curr_int(ch) >= 20 && get_curr_cha(ch) >= 20 )
	learn_from_success( ch, gsn_dominate );

    sprintf( buf, "%s has dominated %s.", ch->name, victim->name);
    log_string_plus( buf, LOG_NORMAL, ch->top_level );

    return;
}


DEF_DO_FUN( seduce )
{}

DEF_DO_FUN( mass_propeganda )
{}

DEF_DO_FUN( gather_intelligence )
{}

void hijack ( CHAR_DATA *ch, SHIP_DATA *ship )
{
    int chance;
    CREW_DATA *member;
    SHIP_DATA *target;

    if ( ship->shipstate == SHIP_DISABLED )
    {
        send_to_char("Napêd statku jest uszkodzony." NL,ch);
        return;
    }

    if (ship->shipstate1 != 0)
    {
	send_to_char(FB_RED "BzZzzbBbbzZzz..." NL,ch);
    	return;
    }

    if ( ship->shipstate == SHIP_DOCKED2 || ship->shipstate == SHIP_DOCKED2_2 )
    {
        send_to_char(FB_RED "Statek w³a¶nie cumuje." EOL,ch);
        return;
    }
    chance = IS_NPC(ch) ? ch->top_level
             : (int)  (ch->pcdata->learned[gsn_hijack]) ;
    if ( number_percent( ) > chance )
    {
	send_to_char("Nie jeste¶ w stanie wprowadziæ odpowiedniego kodu." NL,ch);
	learn_from_failure( ch, gsn_hijack );
        return;
    }

    if ( is_fighter(ship) )
        chance = IS_NPC(ch) ? ch->top_level
		: (int)  (ch->pcdata->learned[gsn_starfighters]) ;

    if ( is_midship(ship) )
        chance = IS_NPC(ch) ? ch->top_level
                : (int)  (ch->pcdata->learned[gsn_midships]) ;

    if ( is_capital(ship) )
        chance = IS_NPC(ch) ? ch->top_level
	        : (int) (ch->pcdata->learned[gsn_capitalships]);

    if ( number_percent( ) < chance )
    {
    	send_to_char( FB_YELLOW "Przejmujesz kontrolê nad statkiem." NL, ch);
    	act( PLAIN, "$n przejmuje kontrolê nad statkiem.", ch,
			NULL, NULL , TO_ROOM );

	echo_to_ship( ship , FB_YELLOW "Statek gwa³townie przyspiesza.");
        if ( ship->shipstate == SHIP_DOCKED2_FIN )
	{
	    SHIPDOCK_DATA *dock, *targetdock;

	    if ( ( ( dock = shipdock_from_slave(ship) ) != NULL ) && ( ( target = dock->target ) != NULL ) &&
	    ( ( targetdock = shipdock_from_room( target, dock->targetvnum ) ) != NULL ) )
	    {
	        dock->targetvnum = 0;
	        dock->target = NULL;
	        dock->master_slave = -1;
    	        targetdock->targetvnum = 0;
	        targetdock->target = NULL;
	        targetdock->master_slave = -1;
	        ship->shipstate = SHIP_READY;
    	        ship->currspeed = ship->realspeed;
	        ship->hx = ship->vx - target->vx - ship->vx;
	        ship->hy = ship->vy - target->vy - ship->vy;
	        ship->hz = ship->vz - target->vz - ship->vz;
                echo_to_room( get_room_index(dock->vnum),FB_YELLOW "¦luza rêkawa cumowniczego zamyka siê.");
                echo_to_room( get_room_index(targetdock->vnum),FB_YELLOW "¦luza rêkawa cumowniczego zamyka siê.");
                sound_to_room( get_room_index(dock->vnum) , "!!SOUND(door)" );
                sound_to_room( get_room_index(targetdock->vnum) , "!!SOUND(door)" );
	    }
	    else
	    {
    	        send_to_char( FB_RED "No i nic, chyba zabezpieczenia s± lepsze ni¿ wygl±da³o to na pierwszy rzut oka." EOL, ch);
	        bug( "/dock/ Sh: %s", SHIPNAME( ship ) );
		return;
	    }
	}
	else
	    ship->currspeed = ship->realspeed;
        CREATE( member, CREW_DATA, 1 );
        member->name = ch;
        member->ship = ship;
        member->rank = 0;
        LINK( member, first_cmember, last_cmember, next, prev );

	return;
    }

    send_to_char("Nie jeste¶ w stanie ustawiæ wszystkiego odpowiednio!" NL,ch);

    if ( is_fighter(ship) )
        learn_from_failure( ch, gsn_starfighters );

    if ( is_midship(ship) )
	learn_from_failure( ch, gsn_midships );

    if ( is_capital(ship) )
	learn_from_failure( ch, gsn_capitalships );

    return;
}

DEF_DO_FUN( makehackdev )
{
    SKILLTYPE *skill;
    char arg[MIL];
    char buf[MSL];
    int level, chance;
    bool checkbatt, checkcond, checkcirc;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *pObjIndex;
    int vnum;
    int quality = 0;

    if( IS_NPC(ch) )
    	return;

    strcpy( arg , argument );

    switch( ch->substate )
    {
    	default:
            if ( !*arg )
            {
            	send_to_char( FB_RED "Sk³adnia: makehackdev <nazwa>" EOL, ch);
                return;
            }

            checkbatt = false;
            checkcond = false;
            checkcirc = false;

            if ( !IS_SET( ch->in_room->room_flags, ROOM_FACTORY ) )
            {
        	send_to_char( FB_RED "Musisz byæ w fabryce albo warsztacie aby to zrobiæ." EOL, ch);
                return;
            }

            for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
            {
                if (obj->item_type == ITEM_BATTERY)
                    checkbatt = true;
                if (obj->item_type == ITEM_CIRCUIT)
                    checkcirc = true;
                if (obj->item_type == ITEM_SUPERCONDUCTOR)
                    checkcond = true;
            }

            if ( !checkbatt )
            {
            	send_to_char( FB_RED "Potrzebujesz ¼ród³a energii." EOL, ch);
                return;
            }

            if ( !checkcirc )
            {
            	send_to_char( FB_RED "Potrzebujesz tablicy kontrolnej (z obwodami) aby kontrolowaæ urz±dzenie." EOL, ch);
            	return;
            }

            if ( !checkcond )
            {
            	send_to_char( FB_RED "potrzebujesz nadprzewodnika." EOL, ch);
            	return;
            }

            chance = IS_NPC(ch) ? ch->top_level
	                 : (int) (ch->pcdata->learned[gsn_makehackdev]);

        if ( number_percent( ) < chance )
	    {
    		send_to_char( FB_GREEN "Rozpoczynasz proces wytwarzania urz±dzenia hakujacego." EOL, ch);
    		act( PLAIN, "$n miesza jakimi¶ obwodami i zaczyna nad czym¶ pracowaæ.",
    				ch, NULL, argument , TO_ROOM );
    		add_timer ( ch , TIMER_DO_FUN , 15 , do_makehackdev , 1 );
    		ch->tempnum = ch->pcdata->learned[gsn_makehackdev];
    		STRDUP( ch->dest_buf, arg );
    		return;
	    }

	    send_to_char(FB_RED "Nie jeste¶ w stanie po³±czyæ tych wszystkich obwodów w jedn± ca³o¶æ." EOL,ch);
	    learn_from_failure( ch, gsn_makehackdev );
    	    WAIT_STATE( ch, skill_table[gsn_makehackdev]->beats/2 );
	    return;

    	case 1:
	    if ( !*ch->dest_buf )
    		return;
    	    strcpy(arg, ch->dest_buf);
    	    STRDUP( ch->dest_buf, "" );

    	    skill = get_skilltype(gsn_makehackdev);
    	    if( skill->participants > 1 )
    		{
    			int cnt = 1;
    			CD *tmp;
    			TIMER *t;

    			for ( tmp = ch->in_room->first_person; tmp; tmp = tmp->next_in_room )
    			if(  tmp != ch
    			&& (t = get_timerptr( tmp, TIMER_DO_FUN ))
    			&& t->count >= 1 && t->do_fun == do_makehackdev
    			&& tmp->dest_buf
    			&& !str_cmp( tmp->dest_buf, arg ) )
    				++cnt;

    			if( cnt >= skill->participants )
    			{
    				for ( tmp = ch->in_room->first_person; tmp; tmp = tmp->next_in_room )
    					if(  tmp != ch
    					&& (t = get_timerptr( tmp, TIMER_DO_FUN ))
    					&& t->count >= 1 && t->do_fun == do_makehackdev
    					&& tmp->dest_buf
    					&& !str_cmp( tmp->dest_buf, arg ) )
    					{
    						extract_timer( tmp, t );
    						act( COL_FORCE, "Pomagasz $n$2 w pracy nad urz±dzeniem.", ch, NULL, tmp, TO_VICT );
    						act( COL_FORCE, "$N pomaga Ci skonstruowaæ urz±dzenie!", ch, NULL, tmp, TO_CHAR );
    						act( COL_FORCE, "$N pomaga $n$2 skonstruowaæ urz±dzenie!", ch, NULL, tmp, TO_NOTVICT );
    						learn_from_success( tmp, gsn_makehackdev );

    						ch->tempnum += tmp->tempnum;
    						tmp->substate = SUB_NONE;
    						tmp->tempnum = -1;
    						STRDUP( ch->dest_buf, "" );
    					}

    				send_to_char( "Koncentrujesz ca³e swoje skupienie na tworzeniu urz±dzenia!" NL, ch );
    				quality /= 10;
    			}
    			else
    			{
    				send_to_char( "Niestety, nie uda³o siê. Przyda³a by siê dodatkowa para r±k." NL, ch );

    				learn_from_failure( ch, gsn_makehackdev );
    				return;
    			}
    		}
    	    break;

    	case SUB_TIMER_DO_ABORT:
    	    break_skill(FB_RED "Co¶ strasznie Ciê rozproszy³o i nie mo¿esz ukoñczyæ sk³adania obwodów." EOL, ch);
    	    return;
    }

    ch->substate = SUB_NONE;
    quality = ch->tempnum;
    ch->tempnum = -1;

	level = IS_NPC(ch) ? ch->top_level : (int) (ch->pcdata->learned[gsn_makehackdev]);
	vnum = 86;

	if ( ( pObjIndex = get_obj_index( vnum ) ) == NULL )
	{
		send_to_char(
				FB_RED "Przedmiot, który próbujesz stworzyæ nie znajduje siê w bazie danych muda." EOL
				FB_RED "Zg³o¶ to niezw³ocznie do administratorów albo poczt± na adres mud@swmud.pl." EOL, ch );
		bug( "Counldn't create obj (%d) for do_makehackdev", vnum );
		return;
	}

    checkbatt = false;
    checkcond = false;
    checkcirc = false;

    for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
    {
        if (obj->item_type == ITEM_BATTERY && checkbatt == false)
        {
    	    separate_obj( obj );
            obj_from_char( obj );
            extract_obj( obj );
            checkbatt = true;
        }
        if (obj->item_type == ITEM_SUPERCONDUCTOR)
        {
            separate_obj( obj );
            obj_from_char( obj );
            extract_obj( obj );
            checkcond = true;
        }
        if (obj->item_type == ITEM_CIRCUIT && checkcirc == false)
        {
    	    separate_obj( obj );
            obj_from_char( obj );
            extract_obj( obj );
            checkcirc = true;
        }
    }

    chance = IS_NPC(ch) ? ch->top_level
                : (int) (ch->pcdata->learned[gsn_makehackdev]-7) ;

    if ( number_percent( ) > chance  || ( !checkbatt ) || ( !checkcond ) || ( !checkcirc) )
    {
       send_to_char( FB_RED "Ogl±dasz swóje nowe urz±dzenie do ³amania zabezpieczeñ w statkach." EOL
    		     FB_RED "Wciskasz kilka przycisków, ale niestety, ¿adnej odpowiedzi od urz±dzenia." EOL
				 FB_RED "Ze z³o¶ci niszczysz urz±dzenie rzucaj±c o pod³ogê." EOL, ch);
       learn_from_failure( ch, gsn_makehackdev );
       return;
    }

    obj = create_object( pObjIndex, level );

    obj->item_type = ITEM_SHIPHCKDEV;
    SET_BIT( obj->wear_flags, ITEM_HOLD );
    SET_BIT( obj->wear_flags, ITEM_TAKE );
    obj->level = level;
    obj->weight = 2+level/15;

    strcpy( buf, nocolor(arg) );
    strcat( buf, " urz±dzenie hakuj±ce ³ami±ce" );
    STRDUP( obj->name, buf );

    sprintf( buf, "urz±dzenie hakuj±ce %s", arg );
    STRDUP( obj->przypadki[0], buf );

    sprintf( buf, "urz±dzena hakuj±cego %s", arg );
    STRDUP( obj->przypadki[1], buf );

    sprintf( buf, "urz±dzeniu hakuj±cemu %s", arg );
    STRDUP( obj->przypadki[2], buf );

    sprintf( buf, "urz±dzenie hakuj±ce %s", arg );
    STRDUP( obj->przypadki[3], buf );

    sprintf( buf, "urz±dzeniem hakuj±cym %s", arg );
    STRDUP( obj->przypadki[4], buf );

    sprintf( buf, "urz±dzeniu hakuj±cym %s", arg );
    STRDUP( obj->przypadki[5], buf );

    sprintf( buf, "Kto¶ zostawi³ tu %s", obj->przypadki[3] );
    STRDUP( obj->description, buf );


    STRDUP( obj->description, buf );
    obj->value[0] = UMIN( quality, 50 );
    obj->cost = obj->value[0]*ch->skill_level[ENGINEERING_ABILITY]*5;
    obj->gender = GENDER_NEUTRAL;

    obj = obj_to_char( obj, ch );

    send_to_char(
	FB_GREEN "Koñczysz pracê i testujesz swoje nowe urz±dzenie. Wszystko dzia³a poprawnie." EOL, ch);
    act( PLAIN, "$n koñczy konstruowaæ jakie¶ dziwne urz±dzenie.", ch, NULL, argument , TO_ROOM );

    {
         long xpgain = obj->cost/5*(chance/12);

         xpgain = UMIN( xpgain ,( exp_level(ch->skill_level[ENGINEERING_ABILITY]+1) - exp_level(ch->skill_level[ENGINEERING_ABILITY]) ) );
         gain_exp(ch, xpgain, ENGINEERING_ABILITY);
         ch_printf( ch , "Otrzymujesz %d punktów do¶wiadczenia w zakresie in¿ynierii." NL, xpgain );
    }
    learn_from_success( ch, gsn_makehackdev );
    WAIT_STATE( ch, get_skilltype(gsn_makehackdev)->beats );
}

DEF_DO_FUN( hackshiplock )
{
	SHIP_DATA	*ship;
	OBJ_DATA	*obj;
	bool		found = false;


	if( !*argument )
	{
		send_to_char( "Klapê którego statku chcesz... hmmm... \"otworzyæ\"? :-)", ch );
		return;
	}

	if(!(ship=ship_in_room(ch->in_room, argument)))
	{
		act(PLAIN, "Nie ma tu ¿adnego $T.", ch, NULL, argument, TO_CHAR);
		return;
	}

	if(ship->shipstate != SHIP_DOCKED && ship->shipstate != SHIP_DISABLED)
	{
		send_to_char(FB_RED "Ten statek zacz±³ ju¿ startowaæ." EOL, ch);
		return;
	}

	if(!known_biotech(ship, ch))
	{
		ch_printf(ch,
				  FB_RED
				  "To jest wytwór obcej biotechnologi, niestety nie masz o niej zielonego pojêcia."
				  EOL);
		return;
	}

    if(!ship->hatchopen)
	{
    	if(!str_cmp(ship->lock_key, "0000"))
    	{
    		send_to_char(PLAIN "Ten statek nie posiada zabezpieczeñ wej¶cia." NL, ch);
			return;
		}

        for ( obj = ch->last_carrying; obj; obj = obj->prev_content )
            if (obj->item_type == ITEM_SHIPHCKDEV && obj->wear_loc == WEAR_HOLD)
            {
            	found = true;
            	break;
            }

        if( !found )
        {
    		send_to_char(PLAIN "Nie trzymasz odpowiedniego urz±dzenia." NL, ch);
    		return;
        }

        int chance = IS_NPC(ch) ? ch->top_level
                    : (int) (ch->pcdata->learned[gsn_hackshiplock]/2+UMIN(50,obj->value[0])-1) ;
        if(number_percent() > chance)
        {
    		act(PLAIN, "Ech, nie uda³o Ci siê otworzyæ klapy $T.", ch, NULL, SHIPNAME(ship),
    			TO_CHAR);
    		act(PLAIN, "$n poróbuje z³amaæ zabezpieczenia klapy $T.", ch, NULL, SHIPNAME(ship),
    			TO_ROOM);
            learn_from_failure( ch, gsn_hackshiplock );

            if(chance > 95)
            {
        		act(PLAIN, "Z³a obs³uga $p w³a¶nie spowodowa³a jego zniszczenie.", ch, obj, NULL,
        			TO_CHAR);
        		separate_obj( obj );
        		obj_from_char( obj );
        		extract_obj( obj );
			}
    		return;
		}

		ship->hatchopen = true;
		act(PLAIN, "Hyhyhy, otwierasz klapê $T.", ch, NULL, SHIPNAME(ship),
			TO_CHAR);
		act(PLAIN, "$n otwiera klapê $T.", ch, NULL, SHIPNAME(ship),
			TO_ROOM);
		echo_to_room(ship->entrance,
					 FB_YELLOW "Klapa niespodziewanie otwiera siê od zewn±trz.");
		sound_to_room(ship->entrance, "!!SOUND(door)");
		sound_to_room(ship->location, "!!SOUND(door)");


        long xpgain = UMIN( chance*25 ,( exp_level(ch->skill_level[SMUGGLING_ABILITY]+1) - exp_level(ch->skill_level[SMUGGLING_ABILITY]) ) );
        gain_exp(ch, xpgain, ENGINEERING_ABILITY);
        ch_printf( ch , "Otrzymujesz %d punktów do¶wiadczenia w zakresie przemytu." NL, xpgain );
        learn_from_success( ch, gsn_hackshiplock );

        WAIT_STATE( ch, get_skilltype(gsn_hackshiplock)->beats );
		return;
	}
	send_to_char(FB_GREEN "Klapa jest ju¿ otwarta." EOL, ch);
	return;
}
