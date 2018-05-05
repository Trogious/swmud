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
*       	   Miscellanous OLC Module Added by Thanos		   *
****************************************************************************/

#include "olc.h"
#include "classes/SWPazaakCard.h"

extern	int	top_helps_file;

RACE_DATA 	  *first_race;
RACE_DATA 	  *last_race;
RACE_DATA 	  *base_race;
LANG_DATA 	  *first_lang;
LANG_DATA 	  *last_lang;
LANG_DATA 	  *lang_base;


void	save_helps			args( () );
void	save_helps_list		args( () );
DIALOG_DATA * get_dialog( char * name );

void show_obj_values( CHAR_DATA * ch, OBJ_INDEX_DATA * obj )
{
	char buf [MSL] = { '\0' };

	switch ( obj->item_type )
	{
		default:
			break;

		case ITEM_WEAPON:
			sprintf( buf, "[v0] " FG_YELLOW "condition:     " PLAIN "%d" NL
					"[v1] " FG_YELLOW "min damage:    " PLAIN "%d" NL
							"[v2] " FG_YELLOW "max damage:    " PLAIN "%d" NL
							"[v3] " FG_YELLOW "weapontype:    " PLAIN "%s" NL
							"[v4] " FG_YELLOW "charges:       " PLAIN "%d" NL
							"[v5] " FG_YELLOW "max. charges:  " PLAIN "%d" NL,
					obj->value[0],	obj->value[1],	obj->value[2],
					bit_name( weapon_types_list, obj->value[3] ),
					obj->value[4],	obj->value[5] );
			break;

		case ITEM_BOLT: case ITEM_BATTERY: case ITEM_AMMO:
			sprintf( buf, "[v0] " FG_YELLOW "charges:       " PLAIN "%d" NL,
					 obj->value[0] );
			break;

		case ITEM_LIGHT:
			sprintf( buf, "[v2] " FG_YELLOW "light:         " PLAIN "%s" NL,
					 obj->value[2] == -1 ? "infinite" : itoa(obj->value[2]) );
			break;

		case ITEM_RAWSPICE: case ITEM_SPICE:
			sprintf( buf, "[v0] " FG_YELLOW "spicetype:     " PLAIN "%s" NL
					"[v1] " FG_YELLOW "grade:         " PLAIN "%d" NL,
					bit_name( spice_types_list, obj->value[0] ),
					obj->value[1] );
			break;

		case ITEM_CRYSTAL:
			sprintf( buf, "[v0] " FG_YELLOW "gemtype:       " PLAIN "%s" NL,
					 bit_name( crystal_types_list, obj->value[0] ) );
			break;

		case ITEM_ARMOR:
			sprintf( buf, "[v0] " FG_YELLOW "condition:     " PLAIN "%d" NL
					"[v1] " FG_YELLOW "ac:            " PLAIN "%d" NL,
					obj->value[0],    obj->value[1] );
			break;

		case ITEM_SALVE:
			sprintf( buf, "[v0] " FG_YELLOW "slevel:        " PLAIN "%d" NL
					"[v1] " FG_YELLOW "maxdoses:      " PLAIN "%d" NL
							"[v2] " FG_YELLOW "doses:         " PLAIN "%d" NL
							"[v3] " FG_YELLOW "delay:         " PLAIN "%d" NL
							"[v4] " FG_YELLOW "spell1:        " PLAIN "%s" NL
							"[v5] " FG_YELLOW "spell2:        " PLAIN "%s" NL,
					obj->value[0],  obj->value[1],
					obj->value[2],  obj->value[3],
					obj->value[4] != -1
							? skill_table[obj->value[4]]->name : "none",
					obj->value[5] != -1
							? skill_table[obj->value[5]]->name : "none" );
			break;

		case ITEM_POTION: case ITEM_PILL:
			sprintf( buf, "[v0] " FG_YELLOW "slevel:        " PLAIN "%d" NL
					"[v1] " FG_YELLOW "spell1:        " PLAIN "%s" NL
							"[v2] " FG_YELLOW "spell2:        " PLAIN "%s" NL
							"[v3] " FG_YELLOW "spell3:        " PLAIN "%s" NL,
					obj->value[0],
					obj->value[1] != -1
							? skill_table[obj->value[1]]->name : "none",
					obj->value[2] != -1
							? skill_table[obj->value[2]]->name : "none",
					obj->value[3] != -1
							? skill_table[obj->value[3]]->name : "none" );
			break;

		case ITEM_DEVICE:
			sprintf( buf, "[v0] " FG_YELLOW "slevel:        " PLAIN "%d" NL
					"[v1] " FG_YELLOW "maxcharges:    " PLAIN "%d" NL
							"[v2] " FG_YELLOW "charges:       " PLAIN "%d" NL
							"[v3] " FG_YELLOW "spell:         " PLAIN "%s" NL,
					obj->value[0],  obj->value[1],  obj->value[2],
					obj->value[3] != -1
							? skill_table[obj->value[3]]->name : "none" );
			break;

		case ITEM_CONTAINER:
			sprintf( buf, "[v0] " FG_YELLOW "capacity:      " PLAIN "%d" NL
					"[v1] " FG_YELLOW "cont. flags:   " PLAIN "%s" NL
							"[v2] " FG_YELLOW "key:           " PLAIN "%d " FG_YELLOW "(%s)" EOL,
					obj->value[0],
					flag_string( container_flags_list, obj->value[1] ),
					obj->value[2],
					get_obj_index( obj->value[2] ) ?
							get_obj_index( obj->value[2] )->name : "NO SUCH OBJECT!" );
			break;

			/* Aldegard */
			/* Trog - OLC ma byc po angielsku! - poprawiam */
		case ITEM_SHIPDEVICE:
			sprintf( buf, "[v0] " FG_YELLOW "Type:          " PLAIN "%-10s Val: %d" NL
					"[v1] " FG_YELLOW "Value:     " PLAIN "%d" NL,
					obj->value[0]==1 ? "Map" : "Other",
					obj->value[0],
					obj->value[1] );
			break;

		case ITEM_SHIPMODULE:
			sprintf( buf, "[v0] " FG_YELLOW "Typ:           " PLAIN "%d" NL
					"[v1] " FG_YELLOW "Value:         " PLAIN "%d" NL
							"[v2] " FG_YELLOW "v2:            " PLAIN "%d" NL
							"[v3] " FG_YELLOW "size:          " PLAIN "%d" NL
							"[v4] " FG_YELLOW "status:        " PLAIN "%d" NL
							"[v5] " FG_YELLOW "crs:           " PLAIN "%d" NL
							"[v6] " FG_YELLOW "price:         " PLAIN "%d" NL,
					obj->value[0],
					obj->value[1],
					obj->value[2],
					obj->value[3],
					obj->value[4],
					obj->value[5],
					obj->cost );
			break;

		case ITEM_MONEY:
			sprintf( buf, "[v0] " FG_YELLOW "credits         " PLAIN "%d" NL,
					 obj->value[0] );
			break;

		case ITEM_PIECE:
			sprintf( buf, "[v0] " FG_YELLOW "other piece    " PLAIN "%d " FG_YELLOW "(%s)" EOL
					"[v1] " FG_YELLOW "dest. object   " PLAIN "%d " FG_YELLOW "(%s)" EOL,
					obj->value[0],
					get_obj_index( obj->value[0] ) ?
							get_obj_index( obj->value[0] )->name : "NO SUCH OBJECT!",
					obj->value[1],
					get_obj_index( obj->value[1] ) ?
							get_obj_index( obj->value[1] )->name : "NO SUCH OBJECT!" );
			break;

		case ITEM_FOOD:
			sprintf( buf, "[v0] " FG_YELLOW "food hours " PLAIN "%d" NL
					"[v3] " FG_YELLOW "poisoned   " PLAIN "%d" NL,
					obj->value[0],
					obj->value[3]);
			break;

		case ITEM_DRINK_CON: case ITEM_FOUNTAIN:
			sprintf( buf, "[v0] " FG_YELLOW "liquid total    " PLAIN "%d" NL
					"[v1] " FG_YELLOW "liquid left     " PLAIN "%d" NL
							"[v2] " FG_YELLOW "liquid          " PLAIN "%s" NL
							"[v3] " FG_YELLOW "poisoned        " PLAIN "%d" NL,
					obj->value[0],
					obj->value[1],
					bit_name( liquid_types_list, obj->value[2] ),
					obj->value[3]);
			break;

		case ITEM_SWITCH: case ITEM_LEVER: case ITEM_BUTTON:
			sprintf( buf, "[v0] " FG_YELLOW "tflags:        " PLAIN "%s" NL,
					 flag_string( container_flags_list, obj->value[0] ) );
			break;

		case ITEM_GRENADE: case ITEM_LANDMINE:
			sprintf( buf, "[v0] " FG_YELLOW "explosion strength:  " PLAIN "%d" NL
					"[v1] " FG_YELLOW "explosion range:     " PLAIN "%d" NL
							"[v2] " FG_YELLOW "explosion time:      " PLAIN "%d" NL
							"[v3] " FG_YELLOW "grenade type:        " PLAIN "%s" NL,
					obj->value[0],
					obj->value[1],
					obj->value[2],
					bit_name( granadetypes_list, obj->value[3] ) );
			break;

		case ITEM_SHIPHCKDEV:
			sprintf( buf, "[v0] " FG_YELLOW "quality:        " PLAIN "%d" NL,
					 obj->value[0] );
			break;

		case ITEM_PAZAAK_CARD:
			SWPazaakCard card((SWPazaakCard::Type) obj->value[0], (SWPazaakCard::Sign) obj->value[1],
				(SWPazaakCard::Variant) obj->value[2], obj->value[3]);
			sprintf( buf, "[v0] " FG_YELLOW "type:           " PLAIN "%d " FG_YELLOW "(%s)" EOL
					"[v1] " FG_YELLOW "sign:           " PLAIN "%d" NL
					"[v2] " FG_YELLOW "variant:        " PLAIN "%d" NL
					"[v3] " FG_YELLOW "double_signed:  " PLAIN "%d" NL,
					 obj->value[0], card.description().c_str(),
				obj->value[1], obj->value[2], obj->value[3]);
			break;
	}

	if( *buf )
	{
		send_to_char( FG_CYAN "ObjIndex values:" EOL, ch );
		send_to_char( buf, ch );
	}
	return;
}

bool set_obj_values( CHAR_DATA * ch, OBJ_INDEX_DATA * pObj, int value_num,
					 char *argument )
{
	char buf [ MAX_STRING_LENGTH ];
	int value;

	switch ( pObj->item_type )
	{

		default:
			switch ( value_num )
			{
				default:
					send_to_char( FB_WHITE "Unknown value." EOL NL, ch );
					break;
				case 0: case 1: case 2: case 3: case 4: case 5:
					pObj->value[value_num] = atoi( argument );
					ch_printf( ch, "Value %d set." NL, value_num );
					break;
			}
			break;

		case ITEM_SWITCH: case ITEM_LEVER: case ITEM_BUTTON:
			switch ( value_num )
			{
				default:
					do_help( ch, (char *)"ITEM_SWITCH_LEVER_BUTTON" );
					return false;
				case 0:
					if ((value = flag_value( container_flags_list, argument )) != NO_FLAG)
						TOGGLE_BIT( pObj->value[0], value );
					send_to_char( "Flags toggled." NL, ch );
					break;
			}
			break;

		case ITEM_BOLT:
			switch ( value_num )
			{
				default:
					do_help( ch, (char *)"ITEM_BOLT" );
					return false;
				case 0:
					send_to_char( "Charges set." NL, ch );
					pObj->value[0] = atoi( argument );
					break;
			}
			break;

		case ITEM_RAWSPICE:
			switch ( value_num )
			{
				default:
					do_help( ch, (char *)"ITEM_RAWSPICE" );
					return false;
				case 0:
					send_to_char( "Spice type set." NL, ch );
					pObj->value[0] = flag_value( spice_types_list, argument );
					break;
				case 1:
					send_to_char( "Grade set." NL, ch );
					pObj->value[1] = atoi( argument );
					break;
			}
			break;

		case ITEM_CRYSTAL:
			switch ( value_num )
			{
				default:
					do_help( ch, (char *)"ITEM_CRYSTAL" );
					return false;
				case 0:
					send_to_char( "Gem type set." NL, ch );
					pObj->value[0] = flag_value( crystal_types_list, argument );
					break;
			}
			break;

		case ITEM_ARMOR:
			switch ( value_num )
			{
				default:
					do_help( ch, (char *)"ITEM_ARMOR" );
					return false;
				case 0:
					send_to_char( "Condition set." NL, ch );
					pObj->value[0] = atoi( argument );
					break;
				case 1:
					send_to_char( "AC set." NL, ch );
					pObj->value[1] = atoi( argument );
					break;
			}
			break;

		case ITEM_SALVE:
			switch ( value_num )
			{
				default:
					do_help( ch, (char *)"ITEM_SALVE" );
					return false;
				case 0:
					send_to_char( "Spell level set." NL, ch );
					pObj->value[0] = atoi( argument );
					break;
				case 1:
					send_to_char( "Max doses set." NL, ch );
					pObj->value[1] = atoi( argument );
					break;
				case 2:
					send_to_char( "Curr. doses set." NL, ch );
					pObj->value[2] = atoi( argument );
					break;
				case 3:
					send_to_char( "Delay set." NL, ch );
					pObj->value[3] = atoi( argument );
					break;
				case 4:
					send_to_char( "Spell 1 set." NL, ch );
					pObj->value[4] = skill_lookup( argument );
					break;
				case 5:
					send_to_char( "Spell 2 set." NL, ch );
					pObj->value[5] = skill_lookup( argument );
					break;
			}
			break;

		case ITEM_DEVICE:
			switch ( value_num )
			{
				default:
					do_help( ch, (char *)"ITEM_DEVICE" );
					return false;
				case 0:
					send_to_char( "Spell level set." NL, ch );
					pObj->value[0] = atoi( argument );
					break;
				case 1:
					send_to_char( "Max. charges set." NL, ch );
					pObj->value[1] = atoi( argument );
					break;
				case 2:
					send_to_char( "Curr charges set." NL, ch );
					pObj->value[2] = atoi( argument );
					break;
				case 3:
					send_to_char( "Spell set." NL, ch );
					pObj->value[3] = skill_lookup( argument );
					break;
			}
			break;


		case ITEM_LIGHT:
			switch ( value_num )
			{
				default:
					do_help( ch, (char *)"ITEM_LIGHT" );
					return false;
				case 2:
					send_to_char( "Light duration set." NL, ch );
					pObj->value[2] = atoi( argument );
					break;
			}
			break;

		case ITEM_POTION:
		case ITEM_PILL:
			switch ( value_num )
			{
				default:
					do_help( ch, (char *)"ITEM_SCROLL_POTION_PILL" );
					return false;
				case 0:
					send_to_char( "Spell level set." NL, ch );
					pObj->value[0] = atoi( argument );
					break;
				case 1:
				case 2:
				case 3:
				case 4:
					sprintf( buf, "Spell %d set." NL, value_num );
					send_to_char( buf, ch );
					pObj->value[value_num] = skill_lookup( argument );
					break;
			}
			break;

		case ITEM_WEAPON:
			switch ( value_num )
			{
				default:
					do_help( ch, (char *)"ITEM_WEAPON" );
					return false;
				case 0:
					send_to_char( "Condition set." NL, ch );
					pObj->value[0] = atoi( argument );
					break;
				case 1:
					send_to_char( "Min. damage set." NL, ch );
					pObj->value[1] = atoi( argument );
					break;
				case 2:
					send_to_char( "Max. damage set." NL, ch );
					pObj->value[2] = atoi( argument );
					break;
				case 3:
					send_to_char( "Weapon type set." NL, ch );
					pObj->value[3] = flag_value( weapon_types_list, argument );
					break;
				case 4:
					send_to_char( "Charges set." NL, ch );
					pObj->value[4] = atoi( argument );
					break;
				case 5:
					send_to_char( "Max. charges set." NL, ch );
					pObj->value[5] = atoi( argument );
					break;
			}
			break;

		case ITEM_CONTAINER:
			switch ( value_num )
			{
				default:
					do_help( ch, (char *)"ITEM_CONTAINER" );
					return false;
				case 0:
					send_to_char( "Capacity set." NL, ch );
					pObj->value[0] = atoi( argument );
					break;
				case 1:
					if ((value = flag_value( container_flags_list, argument )) != NO_FLAG)
						TOGGLE_BIT( pObj->value[1], value );
					else
					{
						do_help( ch, (char *)"ITEM_CONTAINER" );
						return false;
					}
					send_to_char( "Container flags set." NL, ch );
					break;
				case 2:
					if ( atoi( argument ) != 0 )
					{
						if ( !get_obj_index( atoi( argument ) ) )
						{
							send_to_char( "No such object. No key set." NL, ch );
							return false;
						}
						if ( get_obj_index( atoi( argument ) )->item_type != ITEM_KEY )
						{
							send_to_char( "That item is not a key." NL, ch );
							return false;
						}
					}
					send_to_char( "Key to container set." NL, ch );
					pObj->value[2] = atoi( argument );
					break;
			}
			break;

		case ITEM_DRINK_CON:
		case ITEM_FOUNTAIN:
			switch ( value_num )
			{
				default:
					do_help( ch, (char *)"ITEM_DRINK" );
					return false;
				case 0:
					send_to_char( "Capacity set." NL, ch );
					pObj->value[0] = atoi( argument );
					break;
				case 1:
					send_to_char( "Curr. capacity set." NL, ch );
					pObj->value[1] = atoi( argument );
					break;
				case 2:
					send_to_char( "Liquid type set." NL, ch );
					pObj->value[2] = flag_value( liquid_types_list, argument );
					break;
				case 3:
					send_to_char( "'Poison' value set." NL, ch );
					pObj->value[3] = atoi( argument );
					break;
			}
			break;

		case ITEM_FOOD:
			switch ( value_num )
			{
				default:
					do_help( ch, (char *)"ITEM_FOOD" );
					return false;
				case 0:
					send_to_char( "Hours of food set." NL, ch );
					pObj->value[0] = atoi( argument );
					break;
				case 3:
					send_to_char( "'Poison' value set." NL, ch );
					pObj->value[3] = atoi( argument );
					break;
			}
			break;

		case ITEM_GRENADE: case ITEM_LANDMINE:
			switch ( value_num )
			{
				int val;
				default:
					do_help( ch, (char *)"ITEM_GRENADE" );
					return false;
				case 0:
					send_to_char( "Explosion strength set." NL, ch );
					pObj->value[0] = atoi( argument );
					break;
				case 1:
					send_to_char( "Explosion range set." NL, ch );
					pObj->value[1] = atoi( argument );
					break;
				case 2:
					send_to_char( "Explosion time set." NL, ch );
					pObj->value[2] = atoi( argument );
					break;
				case 3:
				//	if (argument[0]='\0')
				//		;
					val = flag_value( granadetypes_list, argument );
					if( val != NO_FLAG )
					{
						pObj->value[3] = val;
						send_to_char( "Grenade type set." NL, ch );
					}
					else
						send_to_char( "No such type." NL, ch );
					break;
				case 4:
					send_to_char( "Cannot edit this value." NL, ch );
					break;
			}
			break;

		case ITEM_MONEY:
			switch ( value_num )
			{
				default:
					do_help( ch, (char *)"ITEM_MONEY" );
					return false;
				case 0:
					send_to_char( "Ammount set." NL, ch );
					pObj->value[0] = atoi( argument );
					break;
			}
			break;
	}

	show_obj_values( ch, pObj );

	return true;
}

bool set_value( CHAR_DATA * ch, OBJ_INDEX_DATA * pObj, char *argument,
				int value )
{
	if ( argument[0] == '\0' )
	{
		set_obj_values( ch, pObj, -1, "\0" );
		return false;
	}

	if ( set_obj_values( ch, pObj, value, argument ) )
		return true;

	return false;
}


void show_help( CHAR_DATA *ch, char *argument )
{
	int 		x,i		= 0;
	bool 		found		= false;
	const 	struct  flag_type *	flag_table;
	int			col;


	if( !argument || argument[0] == '\0' )
	{
		send_to_pager( "Available helps:" NL, ch );

		for( x=0; *flag_stat_table[x].name;x++ )
		{

			if( !flag_stat_table[x].olc )
				continue;

			i++;
			pager_printf( ch, FG_CYAN "%-17s" PLAIN, flag_stat_table[x].name );

			if( i % 4 == 0 )
				send_to_pager( EOL, ch );
			else
				send_to_pager( " ", ch );
		}

		send_to_pager( NL
				"Try also '?' with: qedit, medit, oedit, redit, rpedit, opedit, mpedit.", ch );
		send_to_char( EOL, ch );
		return;
	}

	for( x=0; *flag_stat_table[x].name; x++ )
	{

		if( !flag_stat_table[x].olc )
			continue;

		if( !str_prefix( argument, flag_stat_table[x].name ) )
		{
			found = true;
			break;
		}
	}

	if( !found )
	{
		do_help( ch, argument );
		return;
	}

	pager_printf( ch, FB_WHITE "Available %s:" EOL , flag_stat_table[x].name );

    // bleh
	if( !str_cmp( flag_stat_table[x].name, "ship_classes" )
			|| !str_cmp( flag_stat_table[x].name, "ship_classes_names" ) )
		col = 2;
	else
		col = 4;

	flag_table=flag_stat_table[x].structure;
	for( i=0; *flag_table[i].name; )
	{
		if( !str_cmp( flag_stat_table[x].name, "ship_classes" ) )
			pager_printf( ch, FG_CYAN "%-35s" PLAIN,
						  strlower( flag_table[i].name ) );
		else
			pager_printf( ch, FG_CYAN "%-15s" PLAIN, flag_table[i].name );

		i++;
		if( i % col == 0 )
			send_to_pager( EOL, ch );
		else
			send_to_pager( " ", ch );
	}
	if( i % col )
		send_to_pager( EOL, ch );
	return;
}

bool is_builder( CHAR_DATA *ch, AREA_DATA *area )
{
	CHAR_DATA *rch;

	if( !area )
		return false; // NIE WYWALAC !!!

	rch = CH( ch->desc );
	if ( ( rch->pcdata->security >= area->security
			  || is_name( rch->name, area->builder ) ) )
		return true;

	return false;
}

bool can_edit( CHAR_DATA *ch, int vnum )
{
	AREA_DATA	*area;

	if( IS_NPC( ch ) )
		return false;

	if( vnum == 0 && ch->pcdata->security == 10) // Thanos: tymczasowo
		return true;

	if( !(area = get_vnum_area( vnum )) )
		return false;

	if( IS_ADMIN( ch->name ) || (ch->pcdata->security >= area->security)
		   || is_builder( ch, area ) )
		return true;

	return false;
}
//done byTrog
/*                   ^   Dla mnie gites -- Thanos :P */
/* a tak w ogole to get_vnum_area nie powinno bazowac na low_r_vnum i
   hi_r_vnum tylko poprostu na low_vnum i hi_vnum  -- Than */
/* Trog: mowisz i masz :-) */

AREA_DATA *get_vnum_area( int vnum )
{
	AREA_DATA *pArea;

	for( pArea = first_area; pArea; pArea = pArea->next )
		if( vnum >= pArea->lvnum && vnum <= pArea->uvnum )
			return pArea;
	return NULL;
}

AREA_DATA *get_area_by_id( int area_id )
{
	AREA_DATA *pArea;

	for ( pArea = first_area; pArea; pArea = pArea->next )
		if ( area_id == pArea->area_id )
			return pArea;

	return NULL;
}

void edit_done( CHAR_DATA *ch, char *argument )
{
	ch->desc->olc_editing	= NULL;
	ch->desc->connected		= CON_PLAYING;
	send_to_char( "Done." NL, ch );
	return;
}


DEF_DO_FUN( mindex )
{
	char			arg  [ MAX_INPUT_LENGTH ];
	char			arg2 [ MAX_INPUT_LENGTH ];
	MOB_INDEX_DATA	* pMob;
	AREA_DATA		* are;
	char			areastr [MSL];
	DIALOG_DATA		* pData;

	argument = one_argument( argument, arg );
	argument = one_argument( argument, arg2 );

	if ( arg[0] == '\0' )
	{
		send_to_char( "Mindex whom?" NL, ch );
		return;
	}

	if( !can_edit( ch, atoi(arg) ) )
	{
		send_to_char( "No permission. Sorry." NL, ch );
		return;
	}

	if ( !( pMob = get_mob_index( atoi( arg ) ) ) )
	{
		send_to_char( "Invalid mob index VNUM." NL, ch );
		return;
	}


	if( (are=get_vnum_area(pMob->vnum) ) )
		sprintf( areastr, "%s (%s) %s [%d]", are->name, are->author, are->filename, are->area_id );
	else
		sprintf( areastr, "none" );
	pager_printf( ch,
				  FG_CYAN "Vnum: "   FB_WHITE " %d  " FG_CYAN "Race:"    PLAIN " [%s] " EOL
						  FG_CYAN "Area: "   FB_WHITE " %s" EOL
						  FG_CYAN "Level:"   PLAIN " %d  " FG_CYAN "Sex:"     PLAIN " [%s]  "
						  FG_CYAN "Credits:" PLAIN " %d  " FG_CYAN "Align:"   PLAIN " %d" EOL,
				  pMob->vnum,	    pMob->race->name,
				  areastr,
				  pMob->level,    flag_string( sex_types_list, pMob->sex ),
				  pMob->gold,	    pMob->alignment  );

	pager_printf( ch, FG_CYAN "Name: " PLAIN " %s" NL, pMob->player_name );

	pager_printf( ch,
				  FG_CYAN "Short: " PLAIN
						  "%s" FG_CYAN "@" PLAIN "%s" FG_CYAN "@" PLAIN
						  "%s" FG_CYAN "@" PLAIN "%s" FG_CYAN "@" PLAIN
						  "%s" FG_CYAN "@" PLAIN "%s" FG_CYAN "." EOL
						  FG_CYAN "Long description: " PLAIN "%s" PLAIN,
				  pMob->przypadki[0],	    pMob->przypadki[1],
				  pMob->przypadki[2],	    pMob->przypadki[3],
				  pMob->przypadki[4],	    pMob->przypadki[5],	pMob->long_descr );

	pager_printf( ch, FG_CYAN "Description:" EOL "%s" PLAIN,
				  pMob->description );

	pager_printf( ch, FG_CYAN "Affected by: " PLAIN "[%s]" FG_CYAN "." EOL,
				  flag_string( aff_flags_list, pMob->affected_by )  );

	pager_printf( ch, FG_CYAN "Act: "        PLAIN "[%s]" FG_CYAN "." EOL,
				  flag_string( act_flags_list, pMob->act ) );

	pager_printf( ch, FG_CYAN "Body Parts:"  PLAIN " [%s]" FG_CYAN "." EOL,
				  flag_string( part_flags_list,pMob->xflags) );

	pager_printf(ch,  FG_CYAN "Vip-flags:"   PLAIN " %s" FG_CYAN "." EOL,
				 ( pMob->s_vip_flags && pMob->s_vip_flags[0] != '\0' )
						 ? pMob->s_vip_flags : "none"    );

	pager_printf( ch,
				  FG_CYAN "Str:" PLAIN " %d  " FG_CYAN "Int:" PLAIN " %d  "
						  FG_CYAN "Wis:" PLAIN " %d  " FG_CYAN "Dex:" PLAIN " %d  "
						  FG_CYAN "Con:" PLAIN " %d  " FG_CYAN "Cha:" PLAIN " %d  "
						  FG_CYAN "Lck:" PLAIN " %d  " FG_CYAN "Frc:" PLAIN " %d" NL,
				  pMob->perm_str,pMob->perm_int,pMob->perm_wis,pMob->perm_dex,
				  pMob->perm_con,pMob->perm_cha,pMob->perm_lck,pMob->perm_frc );

	pager_printf( ch, FG_CYAN "Resistant:"   PLAIN " [%s]  ",
				  flag_string( ris_flags_list,pMob->resistant) );

	pager_printf( ch, FG_CYAN "Immune:"      PLAIN " [%s]  ",
				  flag_string( ris_flags_list,pMob->immune) );

	pager_printf( ch, FG_CYAN "Susceptible:"      PLAIN " [%s]  " EOL,
				  flag_string( ris_flags_list,pMob->susceptible) );

	pager_printf( ch, FG_CYAN "Hitroll:"     PLAIN " %d  "
			FG_CYAN "Damroll:"     PLAIN " %d  "
					FG_CYAN "AC:"	    PLAIN " %d  "
					FG_CYAN "Attacks:"     PLAIN " [%s]  ",
			pMob->hitroll,     	pMob->damroll,
			pMob->ac,		flag_string(attack_flags_list,pMob->attacks) );

	pager_printf( ch, FG_CYAN "Defenses:"    PLAIN " [%s]  " EOL,
				  flag_string(def_flags_list,pMob->defenses) );

	pager_printf( ch, FG_CYAN "Hit dice: " PLAIN "%d" FG_CYAN "d" PLAIN "%d" FG_CYAN "+" PLAIN "%d" FG_CYAN "  Damage dice: " PLAIN "%d" FG_CYAN "d" PLAIN "%d" FG_CYAN "+" PLAIN "%d" NL,
		pMob->hitnodice,
		pMob->hitsizedice,
		pMob->hitplus,
		pMob->damnodice,
		pMob->damsizedice,
		pMob->damplus );

	send_to_pager( FG_CYAN "Speaks/Speaking: (currently not working => common)" EOL, ch );
	send_to_pager( FG_CYAN "Dialog: " PLAIN, ch );
	if ( pMob->dialog_name[0] != '\0' && ( pData = get_dialog( pMob->dialog_name ) ) != NULL )
		pager_printf( ch, "%s" EOL, pData->name );
	else
		send_to_pager( "NONE" EOL, ch );

	if ( pMob->spec_fun )
		pager_printf( ch, FG_CYAN "Spec fun: " PLAIN "%s" FG_CYAN ".   ",
					  lookup_spec( pMob->spec_fun ) );

	if ( pMob->spec_2 )
		pager_printf( ch, FG_CYAN "Spec2 fun: " PLAIN "%s" FG_CYAN ".",
					  lookup_spec( pMob->spec_2 ) );

	if ( pMob->spec_fun || pMob->spec_2 )
		send_to_pager( EOL, ch );

	if ( pMob->pShop )
	{
		SHOP_DATA *pShop;
		int iTrade;

		pShop = pMob->pShop;

		pager_printf( ch,
					  FG_CYAN "Shop data for [" PLAIN "%5d" FG_CYAN "]:" EOL
							  FG_CYAN "  Markup for purchaser: " PLAIN "%d" FG_CYAN "%%" EOL
							  FG_CYAN "  Markdown for seller:  " PLAIN "%d" FG_CYAN "%%" EOL
							  FG_CYAN "  Hours: " PLAIN "%d" FG_CYAN " to " PLAIN "%d" FG_CYAN "." EOL
							  FG_CYAN "  Flags: " PLAIN "%s" FG_CYAN "." EOL,
					  pShop->keeper, 		pShop->profit_buy,
					  pShop->profit_sell, 	pShop->open_hour,
					  pShop->close_hour,	flag_string( shop_flags_list, pShop->flags ) );

		for ( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
		{
			if ( pShop->buy_type[iTrade] != 0 )
			{
				if ( iTrade == 0 )
				{
					send_to_pager( FG_CYAN "  Number Trades Type" EOL, ch );
					send_to_pager( FG_CYAN "  ------ -----------" EOL, ch );
				}
				pager_printf( ch,
							  FG_CYAN "  [" PLAIN "%4d" FG_CYAN "] " PLAIN "%s" NL,
							  iTrade,
							  bit_name( obj_types_list, pShop->buy_type[iTrade] ) );
			}
		}
	}

	if ( pMob->rShop )
	{
		REPAIR_DATA *rShop;

		rShop = pMob->rShop;

		pager_printf( ch, FG_CYAN "Repair data:" EOL );
		pager_printf( ch, "  " FG_CYAN "Profit: " PLAIN "%3d" FG_CYAN "%%  Type: " PLAIN "%d" FG_CYAN " (" PLAIN "%s" FG_CYAN ")" EOL,
					  rShop->profit_fix,
					  rShop->shop_type,
					  rShop->shop_type==1?"standard":"recharge");
		pager_printf( ch, "  " FG_CYAN "Hours: " PLAIN "%2d" FG_CYAN " to " PLAIN "%2d" FG_CYAN "." NL,
					  rShop->open_hour,
					  rShop->close_hour );
		pager_printf( ch, "  " FG_CYAN "fix 0 [" PLAIN "%s" FG_CYAN "]    fix 1 [" PLAIN "%s" FG_CYAN "]    fix 2 [" PLAIN "%s" FG_CYAN "]" EOL,
					  bit_name( obj_types_list, rShop->fix_type[0] ),
					  bit_name( obj_types_list, rShop->fix_type[1] ),
					  bit_name( obj_types_list, rShop->fix_type[2] ) );

	}
	return;
}

AREA_DATA *find_area( char *name )
{
	AREA_DATA 	*tarea;
	bool 	 found		= false;

	for ( tarea = first_area; tarea; tarea = tarea->next )
		if ( !str_cmp( tarea->filename, name ) )
	{
		found = true;
		break;
	}

	if ( !found )
		for ( tarea = first_build; tarea; tarea = tarea->next )
			if ( !str_cmp( tarea->filename, name ) )
	{
		found = true;
		break;
	}

	if( !found )
		return NULL;

	return tarea;
}



bool medit_create( CHAR_DATA *ch, char *argument )
{
	AREA_DATA      *pArea;
	MOB_INDEX_DATA *pMob;
	char 	    arg1	[MAX_INPUT_LENGTH];
	char 	    arg2	[MAX_INPUT_LENGTH];
	int             value;

	argument 	= one_argument( argument, arg1 );
	argument 	= one_argument( argument, arg2 );

	if ( arg1[0] == '\0' || (value = atoi( arg1 ))==0 )
	{
		send_to_char( "Sk�adnia: create <vnum>" NL, ch );
		return false;
	}

	if( !can_edit( ch, value ) )
	{
		send_to_char( "No permission. Sorry." NL, ch );
		return false;
	}

	pArea = get_vnum_area( value );
	if ( !pArea )
	{
		send_to_char( "No such area." NL, ch );
		return false;
	}


	if ( value > pArea->uvnum )
		pArea->uvnum = value;

	if ( get_mob_index( value ) )
	{
		send_to_char( "Mobile vnum already exists." NL, ch );
		return false;
	}

	pMob = make_mobile( value, 0, (char *)"mob" );
	if ( !pMob )
	{
		send_to_char( "Error." NL, ch );
		log_string( "Medit_create: make_mobile failed." );
		return false;
	}

	pMob->vnum              = value;

	ch->desc->olc_editing   = (void *) pMob;
	send_to_char( "Mobile created." NL, ch );
	return true;
}


DEF_DO_FUN( medit )
{
	MOB_INDEX_DATA *pMob;
	char            arg1 [MAX_STRING_LENGTH];

	argument = one_argument( argument, arg1 );

	if ( is_number( arg1 ) )
	{

		if( !can_edit( ch, atoi(arg1) ) )
		{
			send_to_char( "No permission. Sorry." NL, ch );
			return;
		}

		if ( !( pMob = get_mob_index( atoi( arg1 ) ) ) )
		{
			send_to_char( "This vnum doesn't exist." NL, ch );
			return;
		}

		ch->desc->olc_editing	= (void *) pMob;
		ch->desc->connected		= CON_MEDITOR;
		medit( ch->desc, (char *)"show" );
		return;
	}
	else
	{
		if ( !str_cmp( arg1, "create" ) )
		{
			if ( medit_create( ch, argument ) )
				ch->desc->connected = CON_MEDITOR;
			return;
		}
	}

	send_to_char( "Syntax: medit <vnum>." NL, ch );
	return;
}

void medit( DESCRIPTOR_DATA *d, char *argument )
{
	CHAR_DATA			* ch = d->character;
	MOB_INDEX_DATA		* pMob;
	char				arg	[ MAX_STRING_LENGTH ];
	char				arg1	[ MAX_STRING_LENGTH ];
	char				arg2	[ MAX_STRING_LENGTH ];
	char				buf	[ MAX_STRING_LENGTH ];
	int					value;
	int					minattr 		= 1;
	int					maxattr 		= 25;
	DIALOG_DATA			* pData;
	RACE_DATA			* race;

	strcpy( arg, argument );
	smash_tilde( argument );
	argument = one_argument( argument, arg1 );
	strcpy( arg2, argument );
	value = atoi( arg2 );

	if( !(pMob = ( MOB_INDEX_DATA * ) ch->desc->olc_editing) )
	{
		bug( "medit: null pMob." );
		edit_done( ch, (char *)"" );
		return;
	}

	if ( !str_prefix( arg1, "show" ) )
	{
		sprintf( buf, "%d %s", pMob->vnum, arg2 );
		do_mindex( ch, buf );
		return;
	}

	GET_DONE( );
	GET_HELP( arg2 );
	GET_CREATE( medit_create( ch, arg2 ) );

	if ( !str_prefix( arg1, "next" ) || !str_cmp( arg1, ">" ) )
	{
		MOB_INDEX_DATA *pMobNext;

		if( (pMobNext = get_mob_index( pMob->vnum +1 )) == NULL )
			send_to_char( "I cannot see next vnum Mob" NL, ch );
		else
		{
			ch->desc->olc_editing = ( void * ) pMobNext;
			sprintf( buf, "%d %s", pMobNext->vnum, arg2 );
			do_mindex( ch, buf );
			send_to_char( "Ok. Next Mob editing" NL, ch );
		}
		return;
	}

	if ( !str_prefix( arg1, "prev" ) || !str_cmp( arg1, "<" ) )
	{
		MOB_INDEX_DATA *pMobPrev;

		if( (pMobPrev = get_mob_index( pMob->vnum -1 )) == NULL )
			send_to_char( "I cannot see previous vnum Mob" NL, ch );
		else
		{
			ch->desc->olc_editing = ( void * ) pMobPrev;
			sprintf( buf, "%d %s", pMobPrev->vnum, arg2 );
			do_mindex( ch, buf );
			send_to_char( "Ok. Prev Mob editing" NL, ch );
		}
		return;
	}

	if ( !str_prefix( arg1, "shop" ) )
	{
		argument = one_argument( argument, arg1 );
		strcpy( arg2, argument );

		if ( arg1[0] == '\0' )
		{
			send_to_char( "Syntax:  shop hours  <opening> <closing>" NL,	ch );
			send_to_char( "         shop profit <buying>  <selling>" NL,	ch );
			send_to_char( "         shop type   <0-4> <item type>" NL,	ch );
			send_to_char( "         shop <flag>" NL,						ch );
			send_to_char( "         shop create" NL,						ch );
			send_to_char( "         shop delete" NL,						ch );
			return;
		}

		if ( !str_cmp( arg1, "create" ) )
		{
			SHOP_DATA *	shop;

			if ( pMob->pShop )
			{
				send_to_char( "This mobile already has a shop." NL, ch );

				return;
			}

			CREATE( shop, SHOP_DATA, 1 );
			shop->keeper	= pMob->vnum;
			shop->profit_buy	= 120;
			shop->profit_sell	= 90;
			shop->open_hour	= 0;
			shop->close_hour	= 23;
			pMob->pShop		= shop;
			LINK( shop, first_shop, last_shop, next, prev );
			send_to_char( "Done." NL, ch );

			return;
		}

		if ( !pMob->pShop )
		{
			send_to_char(
					"This mob has no shop. Set it using 'shop create'." NL, ch );
			return;
		}

		/* Godziny musza byc dodatnie - Ganis */
		if ( !str_cmp( arg1, "hours" ) )
		{
			int hour1;
			int hour2;
			argument = one_argument( argument, arg1 );
			strcpy( arg2, argument );

			if ( arg1[0] == '\0' || !is_number( arg1 )
						  || arg2[0] == '\0' || !is_number( arg2 ) )
			{
				send_to_char( "Syntax:  shop hours <opening> <closing>" NL,
							  ch );
				return;
			}

			hour1 = atoi( arg1 );
			hour2 = atoi( arg2 );

			if ( hour1 < 0 || hour2 < 0 )
			{
				send_to_char( "Hours must be positive numbers." NL, ch );
				return;
			}

			pMob->pShop->open_hour = hour1;
			pMob->pShop->close_hour = hour2;

			send_to_char( "Shop hours set." NL, ch);
			return;
		}

		if ( !str_cmp( arg1, "profit" ) )
		{
			argument = one_argument( argument, arg1 );
			strcpy( arg2, argument );

			if ( arg1[0] == '\0' || !is_number( arg1 )
						  || arg2[0] == '\0' || !is_number( arg2 ) )
			{
				send_to_char( "Syntax:  shop profit <buying> <selling>" NL,
							  ch );
				return;
			}

			pMob->pShop->profit_buy     = atoi( arg1 );
			pMob->pShop->profit_sell    = atoi( arg2 );

			send_to_char( "Shop profit set." NL, ch);
			return;
		}

		/* Nie mozna ustawic ujemnego typu - Ganis */
		if ( !str_cmp( arg1, "type" ) )
		{
			int arg1_int;
			argument = one_argument( argument, arg1 );
			strcpy( arg2, argument );

			if ( !arg1[0] || !is_number( arg1 )
						  || !arg2[0] 
						  || ( arg1_int = atoi( arg1 ) ) < 0 )
			{
				send_to_char( "Syntax:  shop type <0-4> <item type>" NL, ch );
				return;
			}

			if ( arg1_int >= MAX_TRADE )
			{
				sprintf( buf, "May sell %d items max." NL, MAX_TRADE );
				send_to_char( buf, ch );
				return;
			}

			if ( ( value = flag_value( obj_types_list, argument ) ) < 0 )
			{
				send_to_char( "That type of item is not known." NL, ch );
				return;
			}

			pMob->pShop->buy_type[arg1_int] = value;

			send_to_char( "Shop type set." NL, ch);
			return;
		}

		if( (value = flag_value( shop_flags_list, arg1 )) > NO_FLAG )
		{
			TOGGLE_BIT( pMob->pShop->flags, value );
			send_to_char( "Shop flag toggled." NL, ch );
			return;
		}

		if ( !str_cmp( arg1, "delete" ) )
		{
			if ( !pMob->pShop )
			{
				send_to_char( "Cannot delete a shop that is non-existant." NL,
							  ch );
				return;
			}

			UNLINK( pMob->pShop, first_shop, last_shop, next, prev );
			DISPOSE( pMob->pShop );
			pMob->pShop = NULL;
			send_to_char( "Shop deleted." NL, ch);
			return;
		}

		send_to_char( "Syntax:  shop hours  <opening> <closing>" NL, ch );
		send_to_char( "         shop profit <buying>  <selling>" NL, ch );
		send_to_char( "         shop type   <0-4> <item type>"  NL, ch );
		send_to_char( "         shop delete" NL,                     ch );
		return;
	}

	if ( !str_prefix( arg1, "dialog" ) )
	{
		pData = get_dialog( argument );
		if ( !pData )
		{
			send_to_char( "Nie ma takiego dialogu." NL, ch );
			return;
		}
		STRDUP( pMob->dialog_name, pData->name );
		send_to_char( "Dialog zosta� zmieniony." NL,  ch );
		return;
	}

	if ( !str_prefix( arg1, "repair" ) )
	{
		argument = one_argument( argument, arg1 );
		strcpy( arg2, argument );

		if ( arg1[0] == '\0' )
		{
			send_to_char( "Syntax:  repair hours  <opening> <closing>" NL, ch );
			send_to_char( "         repair profit <value>" NL, ch );
			send_to_char( "         repair type   (1=standard, 2=recharge)" NL, ch );
			send_to_char( "         repair fix <0-2> <item_type>" NL,   ch );
			send_to_char( "         repair create" NL,                     ch );
			send_to_char( "         repair delete" NL,                     ch );
			return;
		}

		if( !str_cmp( arg1, "create" ) )
		{
			REPAIR_DATA *	rShop;

			if( pMob->rShop )
			{
				send_to_char( "That mobile already has a repairshop." NL, ch );
				return;
			}

			CREATE( rShop, REPAIR_DATA, 1 );
			rShop->keeper	= pMob->vnum;
			rShop->profit_fix	= 100;
			rShop->shop_type	= SHOP_FIX;
			rShop->open_hour	= 0;
			rShop->close_hour	= 23;
			pMob->rShop		= rShop;
			LINK( rShop, first_repair, last_repair, next, prev );

			send_to_char( "Repairshop created." NL, ch );
			return;
		}

		if( !pMob->rShop )
		{
			send_to_char("This mobile doesn't have a repairshop. Create it using 'repair create'." NL, ch );
			return;
		}

		if ( !str_cmp( arg1, "profit" ) )
		{
			value = atoi( arg2 );

			if ( value < 1 || value > 1000 )
			{
				send_to_char( "Out of range." NL, ch );
				return;
			}
			pMob->rShop->profit_fix = value;
			send_to_char( "Profit set." NL, ch );
			return;
		}

		if ( !str_cmp( arg1, "type" ) )
		{
			value = atoi( arg2 );

			if ( value < 1 || value > 2 )
			{
				send_to_char( "Out of range." NL, ch );
				return;
			}
			pMob->rShop->shop_type = value;
			send_to_char( "Repair type set." NL, ch );
			return;
		}

		/* Godziny musza byc dodatnie - Ganis */
		if ( !str_cmp( arg1, "hours" ) )
		{
			int hour1;
			int hour2;
			argument = one_argument( argument, arg1 );
			strcpy( arg2, argument );

			if ( arg1[0] == '\0' || !is_number( arg1 )
						  || arg2[0] == '\0' || !is_number( arg2 ) )
			{
				send_to_char( "Syntax:  repair hours <opening> <closing>" NL,
							  ch );
				return;
			}

			hour1 = atoi( arg1 );
			hour2 = atoi( arg2 );

			if ( hour1 < 0 || hour2 < 0 )
			{
				send_to_char( "Hours must be positive numbers." NL, ch );
				return;
			}

			pMob->rShop->open_hour = hour1;
			pMob->rShop->close_hour = hour2;

			send_to_char( "Repair hours set." NL, ch);
			return;
		}

		/* Nie mozna ustawic ujemnego fix'a - Ganis */
		if ( !str_cmp( arg1, "fix" ) )
		{
			int arg1_int;
			argument = one_argument( argument, arg1 );
			strcpy( arg2, argument );

			if ( !arg1[0] || !is_number( arg1 )
						  || !arg2[0] || ( arg1_int = atoi( arg1 ) ) < 0 )
			{
				send_to_char( "Syntax:  repair fix <0-2> <item_type>" NL, ch );
				return;
			}

			if ( arg1_int >= MAX_FIX )
			{
				buf[0] = '\0';
				sprintf(buf, "May repair %d items max." NL, MAX_FIX);
				send_to_char( buf, ch );
				return;
			}

			if ( ( value = flag_value( obj_types_list, argument ) ) < 0 )
			{
				send_to_char( "That type of item is not known." NL, ch );
				return;
			}

			pMob->rShop->fix_type[arg1_int] = value;
			send_to_char( "Repair fix type set." NL, ch);
			return;
		}
		if ( !str_cmp( arg1, "delete" ) )
		{
			UNLINK( pMob->rShop, first_repair, last_repair, next, prev );
			DISPOSE( pMob->rShop );
			pMob->rShop = NULL;
			send_to_char( "Repairshop deleted." NL, ch);
			return;
		}
	}

	SET_STR( "name", pMob->player_name, arg2 );

	SET_INT_LIMIT( "strength", 	pMob->perm_str, atoi( arg2 ), minattr, maxattr );
	SET_INT_LIMIT( "intelligence",pMob->perm_int,atoi( arg2 ),minattr,maxattr );
	SET_INT_LIMIT( "wisdom",   	pMob->perm_wis, atoi( arg2 ), minattr, maxattr );
	SET_INT_LIMIT( "dexterity",	pMob->perm_dex, atoi( arg2 ), minattr, maxattr );
	SET_INT_LIMIT( "constitution",pMob->perm_con,atoi( arg2 ),minattr,maxattr );
	SET_INT_LIMIT( "charisma", 	pMob->perm_cha, atoi( arg2 ), minattr, maxattr );
	SET_INT_LIMIT( "luck",     	pMob->perm_lck, atoi( arg2 ), minattr, maxattr );
	SET_INT_LIMIT( "lck",      	pMob->perm_lck, atoi( arg2 ), minattr, maxattr );
	SET_INT_LIMIT( "frc",      	pMob->perm_frc, atoi( arg2 ), minattr, maxattr );

	SET_INT( "ac",				pMob->ac, 	atoi( arg2 ) );
	SET_INT( "hitroll",			pMob->hitroll, 	atoi( arg2 ) );
	SET_INT( "damroll",			pMob->damroll, 	atoi( arg2 ) );

	SET_INT( "hitnodice",		pMob->hitnodice, 	atoi( arg2 ) );
	SET_INT( "hitsizedice",		pMob->hitsizedice, 	atoi( arg2 ) );
	SET_INT( "hitplus",			pMob->hitplus, 	atoi( arg2 ) );
	SET_INT( "damnodice",		pMob->damnodice, 	atoi( arg2 ) );
	SET_INT( "damsizedice",		pMob->damsizedice, 	atoi( arg2 ) );
	SET_INT( "damplus",			pMob->damplus, 	atoi( arg2 ) );


	if ( !str_prefix( arg1, "short" ) )
	{
		int   stat;	//Added by Ratm
		int   indexd=0;
		int   indexp;
		char *short_descr;
		buf[0] = '\0'; // done

		if ( arg2[0] == '\0' )
		{
			send_to_char( "Syntax:  short <string>" NL, ch );
			return;
		}

		short_descr = arg2;

		for( stat=0 ;stat<6; stat++ )
		{
			for( indexp=0; short_descr[indexd] != '@' && short_descr[indexd] != '\0'; )
				buf[indexp++]=short_descr[indexd++];

			if( short_descr[indexd++] == '@' )
			{
				buf[indexp]='\0';
				STRDUP( pMob->przypadki[stat],buf);
			}
			else
			{
				buf[indexp]='\0';
				for( ;stat<6;stat++ )
					STRDUP( pMob->przypadki[stat],buf);
			}
		}
		send_to_char( "Short description set." NL, ch);
		return;
	}


	if ( !str_cmp( arg1, "long" ) )
	{
		if ( arg2[0] == '\0' )
		{
			send_to_char( "Syntax:  long <string>" NL, ch );
			return;
		}

		strcat( arg2, NL );
		STRDUP( pMob->long_descr, arg2 );
		pMob->long_descr[0] = UPPER( pMob->long_descr[0] );

		send_to_char( "Long description set." NL, ch);
		return;
	}


	SET_STR_EDIT( "description", pMob->description, arg2 )

			if ( !str_prefix( arg1, "copy" ) )
	{
		MOB_INDEX_DATA *pCmob;
		int i;
		if ( arg2[0] == '\0' || !is_number( arg2 ) )
		{
			send_to_char( "Sk�adnia copy <vnum>." NL, ch);
			return;
		}
		if( !(pCmob = get_mob_index( atoi( arg2 ) ) ) )
		{
			send_to_char( "Nie ma moba o takim vnumie." NL, ch);
			return;
		}
		STRDUP( pMob->player_name, pCmob->player_name );
		STRDUP( pMob->long_descr , pCmob->long_descr  );
		STRDUP( pMob->description, pCmob->description );

		for( i=0;i<6;i++ )
			STRDUP( pMob->przypadki[i], pCmob->przypadki[i] );

		pMob->spec_fun	= pCmob->spec_fun;
		pMob->spec_2	= pCmob->spec_2;

		pMob->level		= pCmob->level;
		pMob->act		= pCmob->act;
		pMob->affected_by	= pCmob->affected_by;
		pMob->alignment		= pCmob->alignment;
		pMob->sex		= pCmob->sex;
		pMob->race           	= pCmob->race;
		pMob->resistant		= pCmob->resistant;
		pMob->immune		= pCmob->immune;
		pMob->susceptible	= pCmob->susceptible;
		pMob->gold		= pCmob->gold;
		pMob->ac		= pCmob->ac;
		pMob->hitroll		= pCmob->hitroll;
		pMob->damroll		= pCmob->damroll;
		pMob->perm_str		= pCmob->perm_str;
		pMob->perm_int		= pCmob->perm_int;
		pMob->perm_wis		= pCmob->perm_wis;
		pMob->perm_dex		= pCmob->perm_dex;
		pMob->perm_con		= pCmob->perm_con;
		pMob->perm_cha		= pCmob->perm_cha;
		pMob->perm_lck		= pCmob->perm_lck;
		pMob->perm_frc		= pCmob->perm_frc;
		pMob->speaking		= pCmob->speaking;
		pMob->attacks		= pCmob->attacks;
		pMob->defenses		= pCmob->defenses;
		pMob->xflags		= pCmob->xflags;

		send_to_char( "Mob skopiowany." NL, ch);
		return;
	}
	if ( ( value = flag_value(sex_types_list, arg ) ) != -1 )
	{
		pMob->sex = value;
		ch_printf( ch,"Sex set to %s." NL, flag_string(sex_types_list,value ) );
		return;
	}

	if ( (race = find_race( arg )) )
	{
		pMob->race = race;
		send_to_char( "Race set." NL, ch );
		return;
	}

	if ( ( value = flag_value( act_flags_list, arg ) ) != -1 )
	{
		/* Trog: Dungal buga znalazl, jak mob nie ma tej flagi mud pada,
		w sumie oczywiste.
		*/
		if( value != ACT_IS_NPC )
		{
			TOGGLE_BIT( pMob->act, value );
			send_to_char( "Act flag toggled." NL, ch );
		}
		else
			send_to_char( "You cannot toggle ACT_IS_NPC flag." NL, ch );
		return;
	}


	if ( !str_prefix( arg1, "level" ) )
	{
		if ( arg2[0] == '\0' || !is_number( arg2 ) )
		{
			send_to_char( "Syntax:  level <number>" NL, ch );
			return;
		}

		value 		= atoi( arg2 );
		pMob->level 	= UMAX( 1, value );
		pMob->ac 	= 0-value*2.5;
		pMob->hitroll 	= value/5;
		pMob->damroll 	= value/5;
		send_to_char( "Level set." NL, ch);
		return;
	}

	PSET_INT( "credits",	pMob->gold, 	atoi( arg2 ) );
	PSET_INT( "alignment",	pMob->alignment, URANGE( -1000, atoi(arg2), 1000 ) );

	if ( !str_cmp( arg1, "spec" ) )
	{
		if ( arg2[0] == '\0' )
		{
			send_to_char( "Syntax:  spec <special function>" NL, ch );
			return;
		}

		if ( spec_lookup( arg2 ) )
		{
			pMob->spec_fun = spec_lookup( arg2 );
			send_to_char( "Spec set." NL, ch);
			return;
		}

		if ( !str_cmp( arg2, "none" ) )
		{
			pMob->spec_fun = NULL;
			send_to_char( "Spec removed." NL, ch);
			return;
		}
	}
	if ( !str_prefix( arg1, "spec2" ) )
	{
		if ( arg2[0] == '\0' )
		{
			send_to_char( "Syntax:  spec2 <special function>" NL, ch );
			return;
		}

		if ( spec_lookup( arg2 ) )
		{
			pMob->spec_2 = spec_lookup( argument );
			send_to_char( "Spec2 set." NL, ch);
			return;
		}

		if ( !str_cmp( arg2, "none" ) )
		{
			pMob->spec_2 = NULL;
			send_to_char( "Spec2 removed." NL, ch);
			return;
		}
	}
/*
	if ( !str_cmp( arg1, "speaks" ) )
	{
		int 	value;

		if ( !argument || argument[0] == '\0' )
		{
			send_to_char( "Usage: speaks <language> [language] ..." NL, ch );
			return;
		}
		while ( argument[0] != '\0' )
		{
			argument = one_argument( argument, arg2 );
			value = get_langflag( arg2 );
			if ( value == LANG_UNKNOWN )
				ch_printf( ch, "Unknown language: %s" NL, arg2 );
			else
			{
				int valid_langs = LANG_COMMON | LANG_WOOKIEE | LANG_TWI_LEK | LANG_RODIAN
						| LANG_HUTT | LANG_MON_CALAMARI | LANG_NOGHRI | LANG_GAMORREAN
						| LANG_JAWA | LANG_ADARIAN | LANG_EWOK | LANG_VERPINE | LANG_DEFEL
						| LANG_TRANDOSHAN | LANG_CHADRA_FAN | LANG_QUARREN | LANG_DUINUOGWUIN
						| LANG_YUUZHAN_VONG;

				if ( !(value &= valid_langs) )
				{
					ch_printf( ch, "Players may not know %s." NL, arg2 );
					continue;
				}
				TOGGLE_BIT( pMob->speaks, value );
			}
		}
		send_to_char( "Done." NL, ch );
		return;
	}
*/
	if ( !str_cmp( arg1, "speaking" ) )
	{
		LANG_DATA	*lang;

		if( !*argument )
		{
			send_to_char( "Usage: speaking <language> [language]..." NL, ch );
			return;
		}
		if( !(lang = find_lang( argument )) )
		{
			ch_printf( ch, "Unknown language: %s" NL, argument );
			return;
		}
		pMob->speaking = lang;
		send_to_char( "Done." NL, ch );
		return;
	}



	if ( !str_cmp( arg1, "vip" ) )
	{
		if ( arg2[0] == '\0' )
		{
			send_to_char( "Usage: vip <planet>" NL "Or:  vip clear" NL, ch );
			return;
		}

		if( !str_cmp( arg2, "clear" ) )
		{
			STRDUP( pMob->s_vip_flags, (char *)"" );
			ch_printf( ch,"All Vip-flags removed." NL );
			return;
		}

		if( !get_planet( arg2 ) )
			ch_printf( ch,  "!!! Warning !!! There is no such planet!" NL
					"Leave it if you're sure about that." NL);

		toggle_string( &pMob->s_vip_flags, arg2 );

		if( is_name( arg2, pMob->s_vip_flags ) )
			send_to_char( "Vip-Flag added." NL, ch );
		else
			send_to_char( "Vip-Flag removed." NL, ch );

		return;
	}

	if ( !str_cmp( arg1, "resistant" ) )
	{
		char	arg3	[MIL];

		if ( !argument || argument[0] == '\0' )
		{
			send_to_char( "Usage: resistant <flag> [flag]..." NL, ch );
			return;
		}
		while ( argument[0] != '\0' )
		{
			argument = one_argument( argument, arg3 );
			value = flag_value( ris_flags_list, arg3 );
			if ( value == -1 )
				ch_printf( ch, "Unknown flag: %s" NL, arg3 );
			else
			{
				TOGGLE_BIT( pMob->resistant, value );
				ch_printf( ch, "Resistant flag '%s' toggled." NL, arg3 );
			}
		}
		return;
	}
	if ( !str_cmp( arg1, "immune" ) )
	{
		char	arg3	[MIL];

		if ( !argument || argument[0] == '\0' )
		{
			send_to_char( "Usage: immune <flag> [flag]..." NL, ch );
			return;
		}
		while ( argument[0] != '\0' )
		{
			argument = one_argument( argument, arg3 );
			value = flag_value( ris_flags_list, arg3 );
			if ( value == -1 )
				ch_printf( ch, "Unknown flag: %s" NL, arg3 );
			else
			{
				TOGGLE_BIT( pMob->immune, value );
				ch_printf( ch, "Immune flag '%s' toggled." NL, arg3 );
			}
		}
		return;
	}
	if ( !str_prefix( arg1, "susceptible" ) )
	{
		char	arg3	[MIL];

		if ( !argument || argument[0] == '\0' )
		{
			send_to_char( "Usage: susceptible <flag> [flag]..." NL, ch );
			return;
		}
		while ( argument[0] != '\0' )
		{
			argument = one_argument( argument, arg3 );
			value = flag_value( ris_flags_list, arg3 );
			if ( value == -1 )
				ch_printf( ch, "Unknown flag: %s" NL, arg3 );
			else
			{
				TOGGLE_BIT( pMob->susceptible, value );
				ch_printf( ch, "Susceptible flag '%s' toggled." NL, arg3 );
			}
		}
		return;
	}
	if ( !str_prefix( arg1, "attacks" ) )
	{
		char	arg3	[MIL];

		if ( !argument || argument[0] == '\0' )
		{
			send_to_char( "Usage: attack <flag> [flag]..." NL, ch );
			return;
		}
		while ( argument[0] != '\0' )
		{
			argument = one_argument( argument, arg3 );
			value = flag_value( attack_flags_list, arg3 );
			if ( value < 0 )
				ch_printf( ch, "Unknown flag: %s" NL, arg3 );
			else
			{
				TOGGLE_BIT( pMob->attacks, value );
				ch_printf( ch, "Attack flag '%s' toggled." NL, arg3 );
			}
		}
		return;
	}
	if ( !str_prefix( arg1, "defenses" ) )
	{
		char	arg3	[MIL];

		if ( !argument || argument[0] == '\0' )
		{
			send_to_char( "Usage: defenses <flag> [flag]..." NL, ch );
			return;
		}
		while ( argument[0] != '\0' )
		{
			argument = one_argument( argument, arg3 );
			value = flag_value( def_flags_list, arg3 );
			if ( value < 0 )
				ch_printf( ch, "Unknown flag: %s" NL, arg3 );
			else
			{
				TOGGLE_BIT( pMob->defenses, value );
				ch_printf( ch, "Defense flag '%s' toggled." NL, arg3 );
			}
		}
		return;
	}
	if ( !str_prefix( arg1, "parts" ) )
	{
		char	arg3	[MIL];

		if ( !argument || argument[0] == '\0' )
		{
			send_to_char( "Usage: parts <flag> [flag]..." NL, ch );
			return;
		}
		while ( argument[0] != '\0' )
		{
			argument = one_argument( argument, arg3 );
			value = flag_value( part_flags_list, arg3 );
			if ( value < 0 )
				ch_printf( ch, "Unknown flag: %s" NL, arg3 );
			else
			{
				ch_printf( ch, "Part flag '%s' toggled." NL, arg3 );
				TOGGLE_BIT( pMob->xflags, value );
			}
		}
		return;
	}
	if ( ( value = flag_value( aff_flags_list, arg ) ) != -1 )
	{
		TOGGLE_BIT( pMob->affected_by, value );
		send_to_char( "Affect flag toggled." NL, ch );
		return;
	}

	interpret( ch, arg );
}

bool oedit_create( CHAR_DATA *ch, char *argument )
{
	AREA_DATA        *pArea;
	OBJ_INDEX_DATA   *pObj;
	int               value;
	char 	    arg1[MAX_INPUT_LENGTH];
	char 	    arg2[MAX_INPUT_LENGTH];

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

	if ( arg1[0] == '\0' || (value = atoi( arg1 )) == 0 )
	{
		send_to_char( "Sk�adnia: create <vnum>" NL, ch );
		return false;
	}
	if( !can_edit( ch, value ) )
	{
		send_to_char( "No permission. Sorry." NL, ch );
		return false;
	}
	pArea = get_vnum_area( value );
	if ( !pArea )
	{
		send_to_char( "No such area." NL, ch );
		return false;
	}
	if ( get_obj_index( value ) )
	{
		send_to_char( "Ten vnum jest ju� zaj�ty." NL, ch );
		return false;
	}

	if ( value > pArea->uvnum )
		pArea->uvnum = value;

	pObj = make_object( value, 0, (char *)"przedmiot" );

	ch->desc->olc_editing = ( void * ) pObj;

	send_to_char( "Przedmiot stworzony." NL, ch );
	return true;
}

DEF_DO_FUN( oindex )
{
	char            buf  [ MAX_STRING_LENGTH ];
	char            arg  [ MAX_INPUT_LENGTH ];
	char            arg2 [ MAX_INPUT_LENGTH ];
	OBJ_INDEX_DATA *pObj;
	AFFECT_DATA    *paf;
	REQUIREMENT_DATA*req;
	int             cnt;
	AREA_DATA *		are;
	char		areastr	[MSL];

	argument = one_argument( argument, arg );
	argument = one_argument( argument, arg2 );

	if ( arg[0] == '\0' )
	{
		send_to_char( "Oindex what?" NL, ch );
		return;
	}

	if( !can_edit( ch, atoi(arg) ) )
	{
		send_to_char( "No permission. Sorry." NL, ch );
		return;
	}

	if ( !( pObj = get_obj_index( atoi( arg ) ) ) )
	{
		send_to_char( "Invalid obj index VNUM." NL, ch );
		return;
	}

	if( (are=get_vnum_area(pObj->vnum) ) )
		sprintf( areastr, "%s (%s) %s [%d]", are->name, are->author, are->filename, are->area_id );
	else
		sprintf( areastr, "none" );
	sprintf( buf,
			 FG_CYAN "Vnum: " FB_WHITE "%d   "
					 FG_CYAN "Level:" PLAIN " %d   "
					 FG_CYAN "Name: " PLAIN "%s" FG_CYAN "." EOL
					 FG_CYAN "Area: " PLAIN "%s" EOL,
			 pObj->vnum,
			 pObj->level,
			 pObj->name,
			 areastr );
	send_to_char( buf, ch );


	sprintf( buf,
			 FG_CYAN "Type:" PLAIN " [%s]  "
					 FG_CYAN "Weight:" PLAIN " %d  "
					 FG_CYAN "Cost:" PLAIN " %d  "
					 FG_CYAN "Layers:" PLAIN " %d  "
					 FG_CYAN "Gender:" PLAIN " [%s]" NL,
			 bit_name( obj_types_list, pObj->item_type),
			 pObj->weight,
			 pObj->cost,
			 pObj->layers,
			 flag_string(gender_types_list,pObj->gender) );
	send_to_char( buf, ch );

	sprintf( buf, FG_CYAN "Wear flags: " PLAIN "[%s]" FG_CYAN "." EOL,
			 flag_string(wear_flags_list,pObj->wear_flags) );
	send_to_char( buf, ch );

	sprintf( buf, FG_CYAN "Extra flags: " PLAIN "[%s]" FG_CYAN "." EOL,
			 flag_string(obj_flags_list,pObj->extra_flags));
	send_to_char( buf, ch );

	if ( pObj->first_extradesc )
	{
		EXTRA_DESCR_DATA *ed;

		send_to_char( FG_CYAN "Extra description keywords: " PLAIN, ch );

		for ( ed = pObj->first_extradesc; ed; ed = ed->next )
		{
			send_to_char( ed->keyword, ch );
			send_to_char( " ", ch );
		}

		send_to_char( FG_CYAN "." EOL, ch );
	}

	sprintf( buf,
			 FG_CYAN "Short: " PLAIN
					 "%s" FG_CYAN "@" PLAIN "%s" FG_CYAN "@" PLAIN
					 "%s" FG_CYAN "@" PLAIN "%s" FG_CYAN "@" PLAIN
					 "%s" FG_CYAN "@" PLAIN "%s" FG_CYAN "." EOL,
			 pObj->przypadki[0],
			 pObj->przypadki[1],
			 pObj->przypadki[2],
			 pObj->przypadki[3],
			 pObj->przypadki[4],
			 pObj->przypadki[5]	    );
	send_to_char( buf, ch );

	sprintf( buf, FG_CYAN "Long description: " PLAIN "%s" EOL,
			 pObj->description );
	send_to_char( buf, ch );

	sprintf( buf, FG_CYAN "Actiondesc: " PLAIN "%s" EOL,
			 pObj->action_desc );
	send_to_char( buf, ch );

	for ( cnt = 0, paf = pObj->first_affect; paf; paf = paf->next )
	{
		if ( cnt == 0 )
		{
			send_to_char(
					FG_YELLOW "Number Modifier           Affects" EOL
					FG_YELLOW "------ ------------------ -------" EOL, ch );
		}
		sprintf( buf, FG_YELLOW "[%4d] %-18s %s" EOL, cnt,
				 STRING_AFFECT(paf->location, paf->modifier ),
				 bit_name(  apply_types_list, paf->location ) );
		send_to_char( buf, ch );
		cnt++;
	}


	show_obj_values( ch, pObj );

	send_to_char( EOL, ch );
	for( cnt=1, req = pObj->first_requirement; req; req=req->next, cnt++ )
	{
		if( cnt == 1 )
			send_to_char( FB_CYAN "Requirements:" EOL, ch );
		ch_printf( ch, FG_YELLOW "  [%3d]" PLAIN "  ", cnt );
		show_req( ch, req );
		send_to_char( EOL, ch );
	}

	return;
}



void oedit( DESCRIPTOR_DATA *d, char *argument )
{
	CHAR_DATA *		ch = d->character;
	OBJ_INDEX_DATA   *	pObj;
	EXTRA_DESCR_DATA *	ed;
	AFFECT_DATA      *	pAf;
	REQUIREMENT_DATA *	rEq;
	char              	arg	[ MAX_STRING_LENGTH ];
	char              	arg1	[ MAX_STRING_LENGTH ];
	char              	arg2	[ MAX_STRING_LENGTH ];
	char              	buf	[ MAX_STRING_LENGTH ];
	int               	value;

	strcpy( arg, argument );
	smash_tilde( argument );
	argument = one_argument( argument, arg1 );
	strcpy( arg2, argument );

	if( !(pObj = ( OBJ_INDEX_DATA * ) ch->desc->olc_editing) )
	{
		bug( "oedit: null pObj." );
		edit_done( ch, (char *)"" );
		return;
	}

	if ( !str_prefix( arg1, "show" ) )
	{
		sprintf( buf, "%d", pObj->vnum );
		do_oindex( ch, buf );
		return;
	}

	GET_DONE( );
	GET_HELP( arg2 );
	GET_CREATE( oedit_create( ch, arg2 ) );

	if ( !str_prefix( arg1, "next" ) || !str_cmp( arg1, ">" ) )
	{
		OBJ_INDEX_DATA *pObjNext;

		if( (pObjNext = get_obj_index( pObj->vnum +1 )) == NULL )
			send_to_char( "I cannot see next vnum Obj" NL, ch );
		else
		{
			ch->desc->olc_editing = ( void * ) pObjNext;
			sprintf( buf, "%d %s", pObjNext->vnum, arg2 );
			do_oindex( ch, buf );
			send_to_char( "Ok. Next Obj editing" NL, ch );
		}
		return;
	}

	if ( !str_prefix( arg1, "prev" ) || !str_cmp( arg1, "<" ))
	{
		OBJ_INDEX_DATA *pObjPrev;

		if( (pObjPrev = get_obj_index( pObj->vnum -1 )) == NULL )
			send_to_char( "I cannot see previous vnum Obj" NL, ch );
		else
		{
			ch->desc->olc_editing = ( void * ) pObjPrev;
			sprintf( buf, "%d %s", pObjPrev->vnum, arg2 );
			do_oindex( ch, buf );
			send_to_char( "Ok. Prev Obj editing" NL, ch );
		}
		return;
	}




	if ( !str_prefix( arg1, "addaffect" ) )
	{
		int	flag_val;

		argument = one_argument( argument, arg1 );
		strcpy( arg2, argument );
		if ( arg1[0] == '\0' || arg2[0] == '\0' )
		{
			send_to_char( "Sk�adnia:  addaffect <apply_type> <modyfikator>" NL,
						  ch );
			return;
		}

		if ( (flag_val = flag_value( apply_types_list, arg1 )) == NO_FLAG )
		{
			send_to_char( "Taka cecha nie istnieje." NL, ch );
			return;
		}

		int modifier;
		if (flag_val == APPLY_AFFECT)
		{
			if ((modifier = flag_value(aff_flags_list, arg2)) == NO_FLAG)
			{
				send_to_char( "Taki wp�yw nie istnieje." NL, ch );
				return;
			}
		}
		else
		if (flag_val == APPLY_RESISTANT || flag_val == APPLY_IMMUNE || flag_val == APPLY_SUSCEPTIBLE)
		{
			if ((modifier = flag_value(ris_flags_list, arg2)) == NO_FLAG)
			{
				send_to_char( "Taki wp�yw nie istnieje." NL, ch );
				return;
			}
		}
		else
		{
			modifier = ::atoi(arg2);
		}

		CREATE( pAf, AFFECT_DATA, 1 );
		pAf->location   =   flag_val;
		pAf->modifier   =   modifier;
		pAf->type       =   -1;
		pAf->duration   =   -1;
		pAf->bitvector  =   0;
		pAf->next       =   NULL;

		LINK( pAf, pObj->first_affect,
			  pObj->last_affect, next, prev );

		send_to_char( "Wp�yw dodany." NL, ch);
		return;
	}


	if ( !str_prefix( arg1, "delaffect" ) )
	{
		int i = 0,nr;

		strcpy( arg1, argument );

		if ( arg1[0] == '\0' || !is_number( arg1 ) )
		{
			send_to_char( "Sk�adnia:  delaffect <nr>" NL, ch );
			return;
		}
		nr = atoi( arg1 );

		for ( pAf = pObj->first_affect, i=0; pAf; pAf = pAf->next, i++ )
		{
			if ( nr == i )
			{
				UNLINK( pAf, pObj->first_affect, pObj->last_affect, next, prev );
				DISPOSE( pAf );
				send_to_char( "Removed." NL, ch );
				--top_affect;
				return;
			}
		}
		send_to_char( "Not found." NL, ch );
		return;
	}

	if ( !str_prefix( arg1, "addrequirement" ) )
	{
		int	flag_val;

		argument = one_argument( argument, arg1 );
		strcpy( arg2, argument );
		if ( arg1[0] == '\0' || arg2[0] == '\0' )
		{
			send_to_char( "Syntax:  addrequirement <location> <modifier>" NL, ch );
			send_to_char( "Syntax:  addrequirement skill <percentage> <skill_name>" NL, ch );
			return;
		}

		if ( (flag_val = flag_value( req_types_list, arg1 )) == NO_FLAG )
		{
			send_to_char( "No such requirement." NL, ch );
			return;
		}


		CREATE( rEq, REQUIREMENT_DATA, 1 );
		rEq->location   =   flag_val;
    //    rEq->modifier   =   GET_AFF_MOD( pAf->location, arg2 );
		if( rEq->location == REQ_SKILL )
		{
			argument = one_argument( argument, arg1 );
			rEq->modifier = atoi( arg1 );
			rEq->type = skill_lookup( argument );
			if( rEq->type == -1 )
			{
				send_to_char( "Invalid Skill name." NL, ch );
				DISPOSE( rEq );
				return;
			}
		}
		else
			if( rEq->location == REQ_CLASS )
		{
			argument = one_argument(argument, arg2 );
			rEq->modifier = flag_value( class_types_list, arg2 );
		}
		else
			if( rEq->location == REQ_SEX )
		{
			argument = one_argument(argument, arg2 );
			rEq->modifier = flag_value( sex_types_list, arg2 );
		}
		else
			if( rEq->location == REQ_RACE )
		{
			argument = one_argument(argument, arg2 );
			rEq->modifier = 0;//flag_value( race_types_list, arg2 );
			if( rEq->modifier == -1 )
			{
				send_to_char( "Invalid Race name." NL, ch );
				DISPOSE( rEq );
				return;
			}
		}
		else
			if( rEq->location == REQ_AFFECT )
		{
			argument = one_argument(argument, arg2 );
			rEq->modifier = flag_value( aff_flags_list, arg2 );
		}
		else
		{
			rEq->modifier = atoi( arg2 );
			rEq->type       =   0;
		}
		LINK( rEq, pObj->first_requirement,
			  pObj->last_requirement, next, prev );

		send_to_char( "Requirement added." NL, ch);
		return;

	}

	if ( !str_prefix( arg1, "delrequirement" ) )
	{
		int	cnt, nr;

		strcpy( arg1, argument );

		if ( arg1[0] == '\0' || !is_number( arg1 ) )
		{
			send_to_char( "Syntax:  delrequirement <nr>" NL, ch );
			return;
		}

		nr = atoi( arg1 );
		for( cnt=1, rEq = pObj->first_requirement; rEq; rEq=rEq->next, cnt++ )
		{
			if( nr == cnt )
			{
				UNLINK( rEq, pObj->first_requirement,
						pObj->last_requirement, next, prev );
				DISPOSE( rEq );

				send_to_char( "Requirement deleted." NL, ch );
				return;
			}
		}
		send_to_char( "No such requirement." NL, ch );
		return;
	}

	if ( ( value = flag_value( obj_types_list, arg ) ) != NO_FLAG )
	{
		pObj->item_type = value;
		send_to_char( "Typ ustalony." NL, ch );

		pObj->value[0] = 0;	pObj->value[1] = 0;
		pObj->value[2] = 0;	pObj->value[3] = 0;
		pObj->value[4] = 0;	pObj->value[5] = 0;


		return;
	}


	if ( ( value = flag_value( obj_flags_list, arg ) ) != NO_FLAG )
	{
		TOGGLE_BIT( pObj->extra_flags, value );
		send_to_char( "Extra flaga prze��czona." NL, ch );
		return;
	}

	if ( ( value = flag_value( wear_flags_list, arg ) ) != NO_FLAG )
	{
		TOGGLE_BIT( pObj->wear_flags, value );
		send_to_char( "Wear flaga prze��czona." NL, ch );
		return;
	}

	if ( ( value = flag_value(gender_types_list, arg ) ) != NO_FLAG )
	{
		pObj->gender = value;
		send_to_char( "P�e� przedmiotu ustalona." NL, ch );
		return;
	}

	SET_STR( "name", 		pObj->name, 		arg2 );
	SET_STR_NONE( "actiondesc", pObj->action_desc, 	arg2 );

	if ( !str_prefix( arg1, "short" ) )
	{
		int   stat;
		int   indexd=0;
		int   indexp;
		char *short_descr;
		buf[0] = '\0';

		if ( arg2[0] == '\0' )
		{
			send_to_char( "Sk�adnia:  short <mianownik@dope�niacz@celownik@...@miejscownik>" NL, ch );
			return;
		}

		short_descr = arg2;

		for( stat=0 ;stat<6; stat++ )
		{
			for( indexp=0; short_descr[indexd] != '@' && short_descr[indexd] != '\0'; )
				buf[indexp++]=short_descr[indexd++];

			if( short_descr[indexd++] == '@' )
			{
				buf[indexp]='\0';
				STRDUP( pObj->przypadki[stat],buf);
			}
			else
			{
				buf[indexp]='\0';
				for( ;stat<6;stat++ )
					STRDUP( pObj->przypadki[stat],buf);
			}
		}
		send_to_char( "Przypadki ustalone." NL, ch );
		return;
	}


	if ( !str_prefix( arg1, "long" ) )
	{
		if ( arg2[0] == '\0' )
		{
			send_to_char( "Sk�adnia:  long <opis>" NL, ch );
			return;
		}

		STRDUP( pObj->description, arg2 );
		pObj->description[0]	= UPPER( pObj->description[0] );
		send_to_char( "D�ugi opis ustalony." NL, ch );
		return;
	}

	SET_INT( "level", 	pObj->level,	UMAX( 1, atoi(arg2) ) );

	SET_VALUE( "value0", ch, pObj, 0, arg2 );
	SET_VALUE( "v0", 	 ch, pObj, 0, arg2 );
	SET_VALUE( "value1", ch, pObj, 1, arg2 );
	SET_VALUE( "v1", 	 ch, pObj, 1, arg2 );
	SET_VALUE( "value2", ch, pObj, 2, arg2 );
	SET_VALUE( "v2", 	 ch, pObj, 2, arg2 );
	SET_VALUE( "value3", ch, pObj, 3, arg2 );
	SET_VALUE( "v3", 	 ch, pObj, 3, arg2 );
	SET_VALUE( "value4", ch, pObj, 4, arg2 );
	SET_VALUE( "v4", 	 ch, pObj, 4, arg2 );
	SET_VALUE( "value5", ch, pObj, 5, arg2 );
	SET_VALUE( "v5", 	 ch, pObj, 5, arg2 );

	SET_INT( "weight", 	pObj->weight,	UMAX(0, atoi(arg2)) );
	SET_INT( "cost", 	pObj->cost,		UMAX(0, atoi(arg2)) );
	SET_INT( "layers", 	pObj->layers,	atoi(arg2) );

	if ( !str_cmp( arg1, "ed" ) )
	{
		if ( arg2[0] == '\0' )
		{
			send_to_char( "Sk�adnia:  ed add    <s�owo_kluczowe>" NL, ch );
			send_to_char( "           ed delete <s�owo_kluczowe>" NL, ch );
			send_to_char( "           ed edit   <s�owo_kluczowe>" NL, ch );
			return;
		}

		argument = one_argument( argument, arg1 );
		strcpy( arg2, argument );

		if ( !str_cmp( arg1, "add" ) )
		{
			if ( arg2[0] == '\0' )
			{
				send_to_char( "Sk�adnia:  ed add <s�owo_kluczowe>" NL, ch );
				return;
			}

			CHECK_SUBRESTRICTED( ch );

			ed = SetOExtraProto( pObj, arg2 );

			string_append( ch, &ed->description );
			return;
		}


		if ( !str_cmp( arg1, "edit" ) )
		{
			if ( arg2[0] == '\0' )
			{
				send_to_char( "Sk�adnia:  ed edit <s�owo_kluczowe>" NL, ch );
				return;
			}

			for ( ed = pObj->first_extradesc; ed; ed = ed->next )
			{
				if ( is_name( arg2, ed->keyword ) )
					break;
			}

			if ( !ed )
			{
				send_to_char( "S�owo kluczowe nie odnalezione." NL, ch );
				return;
			}

			string_append( ch, &ed->description );

			return;
		}


		if ( !str_cmp( arg1, "delete" ) )
		{
			if ( arg2[0] == '\0' )
			{
				send_to_char( "Syntax:  ed delete <s�owo_kluczowe>" NL, ch );
				return;
			}

			if ( DelOExtraProto( pObj, arg2 ) )
				send_to_char( "Opis extra dodatku usuni�ty." NL, ch );
			return;
		}
	}


	interpret( ch, arg );
	return;
}



DEF_DO_FUN( oedit )
{
	OBJ_INDEX_DATA *pObj;
	char            arg1 [MAX_STRING_LENGTH];

	argument = one_argument( argument, arg1 );

	if ( is_number( arg1 ) )
	{
		if( !can_edit( ch, atoi(arg1) ) )
		{
			send_to_char( "No permission. Sorry." NL, ch );
			return;
		}
		if ( !( pObj = get_obj_index( atoi( arg1 ) ) ) )
		{
			send_to_char( "That vnum does not exist." NL, ch );
			return;
		}

		ch->desc->olc_editing = ( void * ) pObj;
		ch->desc->connected = CON_OEDITOR;
		oedit( ch->desc, (char *)"show" );
		return;
	}
	else
	{
		if ( !str_cmp( arg1, "create" ) )
		{
			if ( oedit_create( ch, argument ) )
				ch->desc->connected = CON_OEDITOR;
			return;
		}
	}

	send_to_char( "Syntax: oedit <vnum>." NL, ch );
	return;
}

bool redit_create( CHAR_DATA *ch, char *argument )
{
	AREA_DATA        *pArea;
	ROOM_INDEX_DATA  *pRoom;
	char 	    arg1[MAX_INPUT_LENGTH];
	char 	    arg2[MAX_INPUT_LENGTH];
	int               value;

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

	if ( arg1[0] == '\0' || (value = atoi( arg1 ))==0 )
	{
		send_to_char( "Sk�adnia: create <vnum>" NL, ch );
		return false;
	}
	if( !can_edit( ch, value ) )
	{
		send_to_char( "No permission. Sorry." NL, ch );
		return false;
	}
	pArea = get_vnum_area( value );
	if ( !pArea )
	{
		send_to_char( "No such area." NL, ch );
		return false;
	}
	if ( get_room_index( value ) )
	{
		send_to_char( "Taki pok�j ju� istnieje." NL, ch );
		return false;
	}

	pRoom                   = make_room( value, 0 );
	pRoom->area		    = pArea;
	if ( value > pArea->uvnum )
		pArea->uvnum = value;
	ch->desc->olc_editing   = (void *)pRoom;
	send_to_char( "Lokacja stworzona." NL, ch );
	return true;
}

DEF_DO_FUN( redit )
{
	ROOM_INDEX_DATA *pRoom;
	int              value;
	char             arg1 [MAX_STRING_LENGTH];

	argument = one_argument( argument, arg1 );

	pRoom = ch->in_room;

	if ( is_number( arg1 ) )
	{
		value = atoi( arg1 );
		if ( !( pRoom = get_room_index( value ) ) )
		{
			send_to_char( "That vnum does not exist." NL, ch );
			return;
		}
	}
	else
	{
		if ( !str_cmp( arg1, "create" ) )
			if ( redit_create( ch, argument ) )
		{
			char_from_room( ch );
			char_to_room( ch, (ROOM_INDEX_DATA *)ch->desc->olc_editing );
			ch->desc->connected = CON_REDITOR;
			return;
		}
	}

	if( !can_edit( ch, pRoom->vnum ) )
	{
		send_to_char( "No permission. Sorry." NL, ch );
		return;
	}

	char_from_room( ch );
	char_to_room( ch, pRoom );

	ch->desc->olc_editing = ( void * ) pRoom;
	ch->desc->connected = CON_REDITOR;
	redit( ch->desc, (char *)"show" );
	return;
}



void redit( DESCRIPTOR_DATA *d, char *argument )
{
	CHAR_DATA *		ch = d->character;
	ROOM_INDEX_DATA *	pRoom;
	EXIT_DATA *		pExit;
	EXIT_DATA *		rExit;
	EXIT_DATA *		rExit_next;
	EXTRA_DESCR_DATA *	ed;
	char              	arg	[ MAX_STRING_LENGTH ];
	char              	arg1	[ MAX_STRING_LENGTH ];
	char              	arg2	[ MAX_STRING_LENGTH ];
	char              	buf	[ MAX_STRING_LENGTH ];
	int               	value;
	int               	door;

	strcpy( arg, argument );
	smash_tilde( argument );
	argument = one_argument( argument, arg1 );
	strcpy( arg2, argument );

	if( !can_edit( ch, ch->in_room->vnum ) ) //Trog: latka.
	{
		edit_done( ch, (char *)"" );
		return;
	}
	pRoom = ch->in_room;

	if ( !str_prefix( arg1, "show" ) )
	{
		sprintf( buf, "%d", pRoom->vnum );
		do_rstat( ch, buf );
		return;
	}

	GET_DONE( );
	GET_HELP( arg2 );

	if ( ( value = flag_value( room_flags_list, arg1 ) ) != NO_FLAG )
	{
		TOGGLE_BIT( pRoom->room_flags, value );

		send_to_char( "Flaga lokacji prze��czona." NL, ch );
		return;
	}
	if ( ( value = flag_value( sector_types_list, arg1 ) ) != NO_FLAG )
	{
		pRoom->sector_type = value;
		send_to_char( "Typ sektora ustalony." NL, ch );
		return;
	}

	if ( !str_cmp( arg1, "ed" ) )
	{
		if ( arg2[0] == '\0' )
		{
			send_to_char( "Sk�adnia:  ed add    <s�owo_kluczowe>" NL, ch );
			send_to_char( "           ed delete <s�owo_kluczowe>" NL, ch );
			send_to_char( "           ed edit   <s�owo_kluczowe>" NL, ch );
			return;
		}

		argument = one_argument( argument, arg1 );
		strcpy( arg2, argument );

		if ( !str_cmp( arg1, "add" ) )
		{
			if ( arg2[0] == '\0' )
			{
				send_to_char( "Sk�adnia:  ed add <s�owo_kluczowe>" NL, ch );
				return;
			}

			CHECK_SUBRESTRICTED( ch );
			ed = SetRExtra( pRoom, arg2 );
			if ( ch->substate == SUB_REPEATCMD )
				ch->tempnum = SUB_REPEATCMD;
			else
				ch->tempnum = SUB_NONE;
			string_append( ch, &ed->description );
			return;
		}


		if ( !str_cmp( arg1, "edit" ) )
		{
			if ( arg2[0] == '\0' )
			{
				send_to_char( "Sk�adnia:  ed edit <s�owo_kluczowe>" NL, ch );
				return;
			}

			for ( ed = pRoom->first_extradesc; ed; ed = ed->next )
			{
				if ( is_name( arg2, ed->keyword ) )
					break;
			}

			if ( !ed )
			{
				send_to_char( "S�owo kluczowe nie odnalezione." NL, ch );
				return;
			}

			string_append( ch, &ed->description );
			return;
		}


		if ( !str_cmp( arg1, "delete" ) )
		{
			if ( arg2[0] == '\0' )
			{
				send_to_char( "Sk�adnia:  ed delete <s�owo_kluczowe>" NL, ch );
				return;
			}
			if ( DelRExtra( pRoom, arg2 ) )
				send_to_char( "Deleted." NL, ch );
			else
				send_to_char( "Not found." NL, ch );
			return;
		}
	}


	GET_CREATE( redit_create( ch, arg2 ) );
	SET_STR( "name", 	pRoom->name, 	arg2 );
	SET_STR_EDIT( "description", pRoom->description, arg2 )

			if ( !str_prefix( arg1, "nightdescription" ) )
	{
		if ( !IS_SET( pRoom->area->flags, AFLAG_NIGHTDESCS ) )
		{
			send_to_char(
					"This areafile does not support nighttime descriptions" NL, ch );
			return;
		}
		if ( arg2[0] == '\0' )
		{
			string_append( ch, &pRoom->nightdesc );
			return;
		}
		if ( !str_prefix( arg2, "delete" ) )
		{
			if( pRoom->nightdesc && pRoom->nightdesc[0] != '\0' )
			{
				STRDUP( pRoom->nightdesc, (char *)"" );
				send_to_char("Nightdesc removed." NL, ch );
			}
			else
				send_to_char("No nightdesc." NL, ch );
			return;
		}
		send_to_char( "Sk�adnia:  nightdesc  [delete]   (edycja)" NL, ch );
		return;
	}

	if ( !str_cmp( arg1, "next" ) || !str_cmp( arg1, ">" ) )
	{
		do_redit( ch, itoa( pRoom->vnum+1 ) );
		return;
	}

	if ( !str_cmp( arg1, "prev" ) || !str_cmp( arg1, "<" ) )
	{
		do_redit( ch, itoa( pRoom->vnum-1 ) );
		return;
	}

	SET_INT( "light", 	pRoom->light,	atoi(arg2) );

	if ( !str_prefix( arg1, "copy" ) )
	{
		ROOM_INDEX_DATA *pCroom;

		if ( arg2[0] == '\0' || !is_number( arg2 ) )
		{
			send_to_char( "Sk�adnia:   copy <vnum>." NL, ch);
			return;
		}
		if( !(pCroom = get_room_index( atoi( arg2 ) ) ) )
		{
			send_to_char( "Nie ma pokoju o takim vnumie." NL, ch);
			return;
		}

		STRDUP( pRoom->name, pCroom->name );

		STRDUP( pRoom->description, pCroom->description );

		pRoom->room_flags	= pCroom->room_flags;
		pRoom->light		= pCroom->light;
		pRoom->sector_type	= pCroom->sector_type;

		return;
	}

	if ( !str_cmp( arg1, "teledelay" ) )
	{
		if ( !*arg2 )
		{
			send_to_char(
					"Ustal op�nienie teleportu. (0 = brak)." NL
					"Sk�adnia: teledelay <warto��>" NL, ch );
			return;
		}
		pRoom->tele_delay = atoi( arg2 );
		send_to_char( "Ok." NL, ch );
		return;
	}

	if ( !str_cmp( arg1, "televnum" ) )
	{
		if ( !*arg2 )
		{
			send_to_char(
					"Ustaw vnum docelowy teleportu." NL
					"Sk�adnia: televnum <vnum>" NL, ch );
			return;
		}
		pRoom->tele_vnum = atoi( arg2 );
		send_to_char( "Ok." NL, ch );
		return;
	}

	if ( !str_cmp( arg1, "tunnel" ) )
	{
		if ( arg2[0] == '\0' || !is_number( arg2 ) )
		{
			send_to_char( "Sk�adnia:  tunnel <ilo��> (0=niesko�czona)" NL, ch );
			return;
		}

		value = atoi( arg2 );

		pRoom->tunnel = URANGE( 1, value, 1000 );

		send_to_char( "Limit pojemno�ci lokacji ustalony." NL, ch );
		return;
	}




    /* 	Ok Teraz edycja drzwi.
	Niby sprawa prosta, ale:
	Istniej� ci bowiem 2 mo�liwo�ci
	- Drzwi istniej� mo�na je wi�c usun��, b�d� edytowa�
	(delete, room, name, etc)
	- Drzwi nie istniej�, mo�na je tylko stworzy� ( dig, lub link)
	Ale tylko je�li drzwi nie s� 'virtualne' - gdy s�, u�ywamy vexit
	*/

	/* tworzymy wyj�cie 'wirtualne' (gdzie�) EX_xAUTO */
	if ( !str_prefix( arg1, "vexit" ) )
	{
		argument 	= one_argument( argument, arg2 );

		if( !*arg2 || !*argument )
		{
			send_to_char( "Sk�adnia: vexit <vnum> <keyword>" NL, ch );
			return;
		}

		if( find_door( ch, argument, true ) )
		{
			send_to_char( "Wyj�cie o takiej nazwie ju� istnieje." NL, ch );
			return;
		}

		value = atoi( arg2 );

		if ( !get_room_index( value ) )
		{
			send_to_char(
					"Nie mo�esz po��czy� tego pokoju z lokacj�, kt�ra nie istnieje." NL, ch );
			return;
		}

		pExit = make_exit( pRoom, get_room_index( value ), DIR_SOMEWHERE );
		STRDUP( pExit->keyword	  , argument );
		STRDUP( pExit->description, (char *)"" );
		pExit->key			= -1;
		pExit->orig_flags		= EX_xAUTO;
		send_to_char( "Ok! ViRtUaLnE wyj�cie utworzone." NL, ch );
		return;
	}

	/* edytujemy istniej�ce wyj�cie (zar�wno standardowe jak i virtualne) */
	if( ( ( pExit = find_door( ch, arg1, true ) ) != NULL )
			   && arg2[0] != '\0' )
	{
		argument = one_argument( argument, arg1 );
		strcpy( arg2, argument );

		if ( !str_cmp( arg1, "delete" ) )
		{
			if( pExit->to_room )
			{
				for( rExit = pExit->to_room->first_exit; rExit; rExit = rExit_next )
				{
					rExit_next = rExit->next;
					if( rExit
									   &&  rExit->to_room
									   &&  rExit->to_room->vnum == pRoom->vnum
									   &&  rExit != pExit	)/* to MO�E si� zdarzy� */
						extract_exit( pExit->to_room, rExit );
				}
			}

			extract_exit( pRoom, pExit );
			send_to_char( "Przej�cia roz��czone." NL, ch );
			return;
		}

		if ( !str_cmp( arg1, "room" ) )
		{
			if ( arg2[0] == '\0' || !is_number( arg2 ) )
			{
				send_to_char( "Sk�adnia:  <kierunek> room <vnum>" NL, ch );
				return;
			}

			value = atoi( arg2 );

			if ( !get_room_index( value ) )
			{
				send_to_char( "Pok�j docelowy nie istnieje." NL, ch );
				return;
			}
			pExit->vnum = value;
			pExit->to_room = get_room_index( value );

			send_to_char( "Jednokierunkowe wyj�cie zmienione." NL, ch );
			return;
		}

		if ( !str_cmp( arg1, "key" ) )
		{
			if ( !*arg2 )
			{
				send_to_char( "Sk�adnia:  <kierunek> key <vnum>" NL
						"           <kierunek> key delete" NL, ch );
				return;
			}

			if ( !str_cmp( arg2, "delete" ) || (value = atoi( arg2 )) == -1 )
			{
				pExit->key = -1;
				send_to_char( "Klucz usuni�ty." NL, ch );
				return;
			}


			if ( value < 1 || !get_obj_index( value ) )
			{
				send_to_char( "Taki przedmiot nie istnieje." NL,    ch );
				return;
			}

			pExit->key = value;
			send_to_char( "Klucz ustalony." NL, ch );
			return;
		}

		if ( !str_cmp( arg1, "distance" ) )
		{
			if ( arg2[0] == '\0' || !is_number( arg2 ) )
			{
				send_to_char( "Sk�adnia:  <kierunek> distance <dystans>" NL, ch );
				return;
			}

			value = atoi( arg2 );

			pExit->distance = URANGE( 1, value, 50 );

			send_to_char( "Dystans ustalony." NL, ch );
			return;
		}

		if ( !str_cmp( arg1, "name" ) || !str_cmp( arg1, "keyword" ) )
		{
			STRDUP( pExit->keyword, argument );

			send_to_char( "Nazwa wyj�cia ustalona." NL, ch );
			return;
		}

		if ( ( value = flag_value( exit_flags_list, arg1 ) ) != NO_FLAG )
		{
			ROOM_INDEX_DATA *pToRoom;

			TOGGLE_BIT( pExit->orig_flags, value );

			if( ( pToRoom = pExit->to_room )
						   &&  ( can_edit( ch, pToRoom->vnum ) )
						   &&  ( rExit = get_exit( pToRoom, rev_dir[pExit->vdir] ) ) )
				TOGGLE_BIT( rExit->orig_flags, value );
			send_to_char( "Flaga wyj�cia prze��czona." NL, ch );
			return;
		}
	}

    /* Oki, a teraz je�li wyj�cie nie istnieje
	- sprawdzamy, czy edytuj�cy krain� wpisa� np. north, south itd.
	i chce takie stworzy�
	*/
	for ( door = 0; door <= MAX_DIR; door++ )
	{
		if ( !str_cmp( arg1, dir_name_orig[door] ) )
			break;
	}

	if ( door != DIR_PORTAL && arg2[0] != '\0' )
	{
	/* je�li wyj�cie istnieje - niech sie builder zainteresuje
		opcjami powy�ej */
		if( get_exit( pRoom, door ) )
		{
			send_to_char( "Takie wyj�cie ju� istnieje." NL, ch );
			return;
		}

		argument = one_argument( argument, arg1 );
		strcpy( arg2, argument );

		if ( !str_cmp( arg1, "link" ) )
		{
			if ( arg2[0] == '\0' || !is_number( arg2 ) )
			{
				send_to_char( "Sk�adnia:  <kierunek> link <vnum>" NL, ch );
				return;
			}

			value = atoi( arg2 );

			if( !can_edit( ch, value ) )
			{
				send_to_char( "Nie mo�esz edytowa� wyj�� lokacji docelowej." NL, ch );
				return;
			}

			if ( !get_room_index( value ) )
			{
				send_to_char( "Nie mo�esz po��czy� tego pokoju z lokacj�, kt�ra nie istnieje." NL, ch );
				return;
			}

			if ( get_exit( get_room_index(value), rev_dir[door] ) )
			{
				send_to_char( "Lokacja docelowa ma ju� zaj�te wyj�cie w tym kierunku." NL, ch );
				return;
			}

			/*wyj�cie w pokoju*/
			pExit = make_exit( pRoom, get_room_index( value ), door );
			STRDUP( pExit->keyword    , (char *)"" );
			STRDUP( pExit->description, (char *)"" );
			pExit->key			= -1;
			pExit->orig_flags		= 0;
			/*wyj�cie powrotne z lokacji docelowej*/
			rExit = make_exit( pExit->to_room, pRoom, rev_dir[door] );
			STRDUP( rExit->keyword    , (char *)"" );
			STRDUP( rExit->description, (char *)"" );
			rExit->key			= -1;
			rExit->orig_flags		= 0;

			send_to_char( "Przej�cie po��czone." NL, ch );
			return;
		}

		if ( !str_cmp( arg1, "dig" ) )
		{
			if ( arg2[0] == '\0' || !is_number( arg2 ) )
			{
				send_to_char( "Sk�adnia <kierunek> dig <vnum>" NL, ch );
				return;
			}

			value = atoi( arg2 );

			if( !can_edit( ch, value ) )
			{
				send_to_char( "Nie mo�esz edytowa� wyj�� lokacji docelowej." NL, ch );
				return;
			}

			if ( get_room_index( value ) )
			{
				send_to_char( "Ten pok�j ju� istnieje. U�yj 'link'." NL, ch );
				return;
			}

			sprintf( buf, "create %s %s", arg2, pRoom->area->filename );
			redit( ch->desc, buf );
			ch->desc->olc_editing   = (void *)ch->in_room;
			sprintf( buf, "%s link %s", dir_name_orig[door], arg2 );
			redit( ch->desc, buf );
			return;
		}

		if ( !str_cmp( arg1, "room" ) )
		{
			if ( arg2[0] == '\0' || !is_number( arg2 ) )
			{
				send_to_char( "Sk�adnia:  <kierunek> room <vnum>" NL, ch );
				return;
			}

			value = atoi( arg2 );

			if ( !get_room_index( value ) )
			{
				send_to_char( "Pok�j docelowy nie istnieje." NL, ch );
				return;
			}

			pExit = make_exit( pRoom, get_room_index( value ), door );
			STRDUP( pExit->keyword    , (char *)"" );
			STRDUP( pExit->description, (char *)"" );
			pExit->key			= -1;
			pExit->orig_flags		= 0;
			pExit->flags		= 0;

			send_to_char( "Jednokierunkowe wyj�cie stworzone." NL, ch );
			return;
		}
	}
	interpret( ch, arg );
	return;
}

bool aedit_create( CHAR_DATA *ch, char *argument )
{
	AREA_DATA *pArea;

	if( get_trust( ch ) < 103 )
	{
		send_to_char( "No permission. Sorry." NL, ch );
		return false;
	}

	if( argument[0] == '\0' )
	{
		send_to_char( "create <filename>" NL, ch );
		return false;
	}
	pArea		  =   new_area( );

	STRDUP( pArea->filename, argument );
	STRDUP( pArea->author, ch->name );
	LINK( pArea, first_area, last_area, next, prev );

	ch->desc->olc_editing =   (void *) pArea;

	write_area_list();
	send_to_char( "Area created. Area list saved." NL, ch );
	return true;
}

void aedit( DESCRIPTOR_DATA *d, char *argument )
{
	CHAR_DATA *ch 	= d->character;
	AREA_DATA *pArea;
	char       arg	[ MAX_STRING_LENGTH ];
	char       arg1	[ MAX_STRING_LENGTH ];
	char       arg2	[ MAX_STRING_LENGTH ];
	char       buf 	[ MAX_STRING_LENGTH ];
	int        value;

	pArea = ( AREA_DATA *) ch->desc->olc_editing;
	strcpy( arg, argument );
	smash_tilde( argument );
	argument = one_argument( argument, arg1 );
	strcpy( arg2, argument );


	if ( !is_builder( ch, pArea ) )
	{
		send_to_char( "Nieodpowiedni poziom security." NL, ch );
		edit_done( ch, (char *)"" );
		return;
	}


	if ( !str_prefix( arg1, "show" ) )
	{
		sprintf( buf, "%s", pArea->filename );
		do_astat( ch, buf );
		return;
	}


	GET_DONE( );

	GET_HELP( arg2 );

	if ( !str_cmp( arg1, "savelist" ) && get_trust( ch ) > 102 )
	{
		send_to_char( "Writing area.lst..." NL, ch );
		write_area_list( );
		return;
	}

	if ( ( value = flag_value( area_flags_list, arg ) ) != NO_FLAG )
	{
		TOGGLE_BIT( pArea->flags, value );

		send_to_char( "Flagi prze��czone." NL, ch );
		return;
	}


	GET_CREATE( aedit_create( ch, arg2 ) );
	SET_STR( "name", 		pArea->name, 		arg2 );
	SET_FILENAME( "filename", 	pArea->filename, 	arg2,	"xml" );
	SET_STR( "author", 		pArea->author, 		arg2 );
	SET_INT( "resetfrequency", 	pArea->reset_frequency,	atoi(arg2) );
	SET_INT( "age", 		pArea->age,		atoi(arg2) );


	if ( !str_cmp( arg1, "planet" ) )
	{
		PLANET_DATA *planet;

		if( arg2[0] != '\0' && !str_cmp( arg2, "NULL" ) )
		{
			if( (planet = pArea->planet) )
				UNLINK( pArea, planet->first_area, planet->last_area, next_on_planet, prev_on_planet );
			pArea->planet = NULL;
			send_to_char( "Done" NL, ch );

			return;
		}
		planet = get_planet(arg2);

		if (planet)
		{
			if (pArea->planet)
			{
				PLANET_DATA *old_planet;

				old_planet=pArea->planet;
				UNLINK(pArea, old_planet->first_area, old_planet->last_area, next_on_planet, prev_on_planet);
			}
			pArea->planet=planet;
			LINK(pArea, planet->first_area, planet->last_area, next_on_planet, prev_on_planet);
			save_planet(planet);
			send_to_char( "Planeta ustalona." NL, ch );
		}
		else
			send_to_char( "Nie ma takiej planety." NL, ch );

		return;
	}

	SET_INT( "low_economy",	pArea->low_economy,	atoi(arg2) );
	SET_INT( "high_economy",	pArea->high_economy,	atoi(arg2) );
	SET_INT_LIMIT( "low_range",	pArea->low_range,	atoi(arg2),
				   0, MAX_LEVEL );
	SET_INT_LIMIT( "high_range",	pArea->high_range,	atoi(arg2),
				   0, MAX_LEVEL );
	SET_INT_LIMIT( "security",	pArea->security,	atoi(arg2),
				   0, ch->pcdata->security );
	SET_STR_NONE( "resetmsg", pArea->resetmsg, arg2 );

	if ( !str_prefix( arg1, "builder" ) )
	{

		argument = one_argument( argument, arg2 );

		if ( arg2[0] == '\0' )
		{
			send_to_char( "Sk�adnia:  builder <imi�>" NL, ch );
			return;
		}

		toggle_string( &pArea->builder, arg2 );

		if( is_name( arg2, pArea->builder ) )
			send_to_char( "Builder added." NL, ch );
		else
			send_to_char( "Builder removed." NL, ch );

		return;
	}


	if ( !str_prefix( arg1, "lvnum" ) )
	{
		if ( !is_number( arg2 ) || arg2[0] == '\0' )
		{
			send_to_char( "Sk�adnia:  lvnum <dolny_vnum>" NL, ch );
			return;
		}

		value = atoi( arg2 );

		if ( get_vnum_area( value )
				   && get_vnum_area( value ) != pArea )
		{
			send_to_char( "Przedzia� koliduje z inn� krain�." NL, ch );
			return;
		}

		pArea->lvnum = value;

		send_to_char( "Dolny vnum ustalony." NL, ch );
		return;
	}


	if ( !str_prefix( arg1, "uvnum" )
			 ||   !str_prefix( arg1, "hivnum") )
	{
		if ( !is_number( arg2 ) || arg2[0] == '\0' )
		{
			send_to_char( "Sk�adnia:  uvnum <g�rny_vnum>" NL, ch );
			return;
		}

		value = atoi( arg2 );

		if ( get_vnum_area( value )
				   && get_vnum_area( value ) != pArea )
		{
			send_to_char( "Przedzia� koliduje z inn� krain�." NL, ch );
			return;
		}

		pArea->uvnum = value;

		send_to_char( "G�rny vnum ustalony." NL, ch );
		return;
	}


	if ( !str_prefix( arg1, "vnum" ) )
	{
		argument = one_argument( argument, arg1 );
		strcpy( arg2, argument );

		if ( !is_number( arg1 ) || arg1[0] == '\0'
					|| !is_number( arg2 ) || arg2[0] == '\0' )
		{
			send_to_char( "Sk�adnia:  vnum <dolny> <g�rny>" NL, ch );
			return;
		}

		value = atoi( arg1 );

		if ( get_vnum_area( value )
				   && get_vnum_area( value ) != pArea )
		{
			send_to_char( "Przedzia� dolny koliduje z inn� krain�." NL, ch );
			return;
		}

		pArea->lvnum = value;

		send_to_char( "Dolny vnum ustalony." NL, ch );

		value = atoi( arg2 );

		if ( get_vnum_area( value )
				   && get_vnum_area( value ) != pArea )
		{
			send_to_char( "Przedzia� g�rny koliduje z inn� krain�." NL, ch );
			return;
		}

		pArea->uvnum = value;

		send_to_char( "G�rny vnum ustalony." NL, ch );

		return;
	}

	interpret( ch, arg );
	return;
}




DEF_DO_FUN( aedit )
{
	AREA_DATA *pArea;
	int        value;
	char 	arg[MIL];

	pArea = ch->in_room->area;

	if ( is_number( argument ) )
	{
		value = atoi( argument );
		if ( !( pArea = get_vnum_area( value ) ) )
		{
			send_to_char( "That area vnum does not exist." NL, ch );
			return;
		}
	//Trog: jakby cos to tutaj trzeba wpisac sprawdzanie po area_id.
		/*Thanos: a dlaczego tutaj? Nie mo�na w find_area() ? */
	}
	else
	{
		argument = one_argument( argument, arg );

		if ( !str_cmp( arg, "reset" ) )
		{
			reset_area( pArea );
			send_to_char( "Area reset." NL, ch );
			return;
		}
		else
			if ( !str_cmp( arg, "create" ) )
		{
			if( argument[0] == '\0' )
			{
				send_to_char( "create <filename>" NL, ch );
				return;
			}
			aedit_create( ch, argument );
			ch->desc->connected = CON_AEDITOR;
			return;
		}
		else
			for ( pArea = first_area; pArea; pArea = pArea->next )
				if ( !str_cmp( pArea->filename, arg ) )
		{
			ch->desc->olc_editing = ( void * ) pArea;
			ch->desc->connected = CON_AEDITOR;
			aedit( ch->desc, (char *)"show" );
			return;
		}

	}

	if( !pArea ) pArea = ch->in_room->area;

	ch->desc->olc_editing = ( void * ) pArea;
	ch->desc->connected = CON_AEDITOR;
	aedit( ch->desc, (char *)"show" );
	return;
}


int mprog_count( MOB_INDEX_DATA * pMob )
{
	MPROG_DATA *mprg;
	int         count;

	for ( count = 0, mprg = pMob->mudprogs; mprg; mprg = mprg->next, count++ )
		;
	return count;
}

int oprog_count( OBJ_INDEX_DATA * pObj )
{
	MPROG_DATA *oprg;
	int         count;

	for ( count = 0, oprg = pObj->mudprogs; oprg; oprg = oprg->next, count++ )
		;
	return count;
}

int rprog_count( ROOM_INDEX_DATA * pRoom )
{
	MPROG_DATA *rprg;
	int         count;

	for ( count = 0, rprg = pRoom->mudprogs; rprg; rprg = rprg->next, count++ )
		;
	return count;
}


bool mpedit_create( CHAR_DATA *ch, char *argument )
{
	MOB_INDEX_DATA *pMob;
	AREA_DATA      *pArea;
	int             value;

	value = atoi( argument );
	if ( argument[0] == '\0' || value == 0 )
	{
		send_to_char( "Syntax: create vnum <script vnum>" NL, ch );
		return false;
	}

	pArea = get_vnum_area( value );
	if( !can_edit( ch, value ) )
	{
		send_to_char( "No permission. Sorry." NL, ch );
		return false;
	}

	if ( !pArea )
	{
		send_to_char( "That vnum is not assigned an area." NL, ch );
		return false;
	}

	if ( !( pMob = get_mob_index( value ) ) )
	{
		send_to_char( "Mobile vnum does not exist!" NL, ch );
		return false;
	}

	if ( pMob->mudprogs )
	{
		send_to_char( "Mobile already has mob programs!" NL, ch );
		return false;
	}

	pMob->mudprogs = new_mprog();
	pMob->progtypes = pMob->mudprogs->type;

	ch->desc->olc_editing		= (void *) pMob;
	ch->pcdata->mprog_edit	= 0;

	send_to_char( "MOBProg created." NL, ch );
	return true;
}


bool opedit_create( CHAR_DATA *ch, char *argument )
{
	OBJ_INDEX_DATA *pObj;
	MPROG_DATA     *mprg;
	int             value;

	value = atoi( argument );
	if ( argument[0] == '\0' || value == 0 )
	{
		send_to_char( "Syntax: create vnum <script vnum>" NL, ch );
		return false;
	}

	if ( !( pObj = get_obj_index( value ) ) )
	{
		send_to_char( "Object vnum does not exist!" NL, ch );
		return false;
	}
	if( !can_edit( ch, value ) )
	{
		send_to_char( "No permission. Sorry." NL, ch );
		return false;
	}

	if ( pObj->mudprogs )
	{
		send_to_char( "Obj already has programs!" NL, ch );
		return false;
	}

	CREATE( pObj->mudprogs, MPROG_DATA, 1 );
	mprg			= pObj->mudprogs;
	mprg->type			= 0|RAND_PROG;
	STRDUP( mprg->comlist, "break\n" );
	STRDUP( mprg->arglist, "0" );
	ch->desc->olc_editing	= (void *) pObj;
	ch->pcdata->mprog_edit	= 0;

	send_to_char( "OBJProg created." NL, ch );
	return true;
}

bool rpedit_create( CHAR_DATA *ch, char *argument )
{
	ROOM_INDEX_DATA *pRoom;
	MPROG_DATA     *mprg;
	int             value;

	value = atoi( argument );
	if ( argument[0] == '\0' || value == 0 )
	{
		send_to_char( "Syntax: create vnum <script vnum>" NL, ch );
		return false;
	}

	if( !can_edit( ch, value ) )
	{
		send_to_char( "No permission. Sorry." NL, ch );
		return false;
	}

	if ( !( pRoom = get_room_index( value ) ) )
	{
		send_to_char( "Room vnum does not exist!" NL, ch );
		return false;
	}

	if ( pRoom->mudprogs )
	{
		send_to_char( "Room already has programs!" NL, ch );
		return false;
	}

	CREATE( pRoom->mudprogs,MPROG_DATA,1 );
	mprg		= pRoom->mudprogs;

	mprg->type			= 0|RAND_PROG;
	pRoom->progtypes 		= pRoom->progtypes | mprg->type;
	STRDUP( mprg->comlist, "break\n" );
	STRDUP( mprg->arglist, "0" );

	ch->desc->olc_editing		= (void *) pRoom;
	ch->pcdata->mprog_edit	= 0;

	send_to_char( "ROOMProg created." NL, ch );
	return true;
}

MPROG_DATA *edit_mprog( CHAR_DATA * ch, MOB_INDEX_DATA * pMob )
{
	MPROG_DATA *mprg;
	int         mprog_num;
	int         count = 0;

	mprog_num = ch->pcdata->mprog_edit;
	for ( mprg = pMob->mudprogs; mprg && count < mprog_num; mprg = mprg->next )
		count++;

	return mprg;
}

MPROG_DATA *edit_oprog( CHAR_DATA * ch, OBJ_INDEX_DATA * pObj )
{
	MPROG_DATA *oprg;
	int         oprog_num;
	int         count = 0;

	oprog_num = ch->pcdata->mprog_edit;
	for ( oprg = pObj->mudprogs; oprg && count < oprog_num; oprg = oprg->next )
		count++;

	return oprg;
}

MPROG_DATA *edit_rprog( CHAR_DATA * ch, ROOM_INDEX_DATA * pRoom )
{
	MPROG_DATA *rprg;
	int         rprog_num;
	int         count = 0;

	rprog_num = ch->pcdata->mprog_edit;
	for ( rprg = pRoom->mudprogs; rprg && count < rprog_num; rprg = rprg->next )
		count++;

	return rprg;
}


void show_mprog( CHAR_DATA * ch, MPROG_DATA * pMobProg )
{
	char buf [ MAX_STRING_LENGTH ];
	char pbuf[3*MSL];

	buf[0] = '\0';
	strcpy( pbuf, pMobProg->comlist );

	sprintf( pbuf, "%s", str_repl( pbuf, "endif", FB_GREEN "endif" PLAIN, true ) );
	sprintf( pbuf, "%s", str_repl( pbuf, "if", FB_GREEN "if" PLAIN, true ) );
	sprintf( pbuf, "%s", str_repl( pbuf, "else", FB_GREEN "else" PLAIN, true ) );
	sprintf( pbuf, "%s", str_repl( pbuf, "$n", FB_WHITE "$n" PLAIN, true ) );
	sprintf( pbuf, "%s", str_repl( pbuf, ">", FB_WHITE ">" PLAIN, true ) );
	sprintf( pbuf, "%s", str_repl( pbuf, "<", FB_WHITE "<" PLAIN, true ) );
	sprintf( pbuf, "%s", str_repl( pbuf, ">=", FB_WHITE ">=" PLAIN, true ) );
	sprintf( pbuf, "%s", str_repl( pbuf, "<=", FB_WHITE "<=" PLAIN, true ) );
	sprintf( pbuf, "%s", str_repl( pbuf, "==", FB_WHITE "==" PLAIN, true ) );
	sprintf( pbuf, "%s", str_repl( pbuf, "!=", FB_WHITE "!=" PLAIN, true ) );
	sprintf( pbuf, "%s", str_repl( pbuf, ")", FB_CYAN ")" PLAIN, true ) );
	sprintf( pbuf, "%s", str_repl( pbuf, "(", FB_CYAN "(" PLAIN, true ) );

	sprintf( buf, FG_CYAN "Type:" PLAIN " %s " FG_CYAN "Trigger:" PLAIN " %s" EOL,
			 mprog_type_to_name( pMobProg->type ),
			 pMobProg->arglist ? pMobProg->arglist : "NULL" );
	send_to_char( buf, ch );

	sprintf( buf, FG_CYAN "Program:" EOL "%s" FG_CYAN "----" EOL,
			 pbuf );
	send_to_char( buf, ch );
}

void show_oprog( CHAR_DATA * ch, MPROG_DATA * pObjProg )
{
	char buf [ MAX_STRING_LENGTH ];

	sprintf( buf, FG_CYAN "Type:" PLAIN " %s " FG_CYAN "Trigger:" PLAIN " %s" EOL,
			 mprog_type_to_name( pObjProg->type ),
			 pObjProg->arglist ? pObjProg->arglist : "NULL" );
	send_to_char( buf, ch );

	sprintf( buf, FG_CYAN "Program:" EOL "%s" FG_CYAN "----" EOL,
			 pObjProg->comlist ? pObjProg->comlist : "NULL" NL );
	send_to_char( buf, ch );
}

void show_rprog( CHAR_DATA * ch, MPROG_DATA * pRoomProg )
{
	char buf [ MAX_STRING_LENGTH ];

	sprintf( buf, FG_CYAN "Type:" PLAIN " %s " FG_CYAN "Trigger:" PLAIN " %s" EOL,
			 mprog_type_to_name( pRoomProg->type ),
			 pRoomProg->arglist ? pRoomProg->arglist : "NULL" );
	send_to_char( buf, ch );

	sprintf( buf, FG_CYAN "Program:" EOL "%s" FG_CYAN "----" EOL,
			 pRoomProg->comlist ? pRoomProg->comlist : "NULL" NL );
	send_to_char( buf, ch );
}

void delete_mprog( CHAR_DATA * ch, int pnum )
{
	MPROG_DATA     *mprg;
	MPROG_DATA     *mprg_prev;
	MOB_INDEX_DATA *pMob;
	char            buf	[ MAX_INPUT_LENGTH ];
	int             count;

	pMob = ( MOB_INDEX_DATA * ) ch->desc->olc_editing;

	if ( pnum < 0 )
		return;

	if ( pnum == 0 )
	{
		mprg = pMob->mudprogs->next;
		free_mprog( pMob->mudprogs );
		pMob->mudprogs	= mprg;
	}
	else
	{
		mprg_prev	= pMob->mudprogs;
		mprg		= mprg_prev->next;

		for ( count = 1; mprg && count < pnum; count++ )
		{
			mprg_prev	= mprg;
			mprg	= mprg->next;
		}
		if ( mprg )
		{
			mprg_prev->next = mprg->next;
			free_mprog( mprg );
		}
	}

	pMob->progtypes = 0;
	for ( MPROG_DATA *mprg = pMob->mudprogs; mprg; mprg = mprg->next )
		pMob->progtypes |= mprg->type;


	sprintf( buf, "MOBProg %d Deleted." NL, pnum + 1 );
	send_to_char( buf, ch );
	return;
}

void delete_oprog( CHAR_DATA * ch, int pnum )
{
	MPROG_DATA     *oprg;
	MPROG_DATA     *oprg_prev;
	OBJ_INDEX_DATA *pObj;
	char            buf	[ MAX_INPUT_LENGTH ];
	int             count;

	pObj = ( OBJ_INDEX_DATA * ) ch->desc->olc_editing;

	if ( pnum < 0 )
		return;

	if ( pnum == 0 )
	{
		oprg = pObj->mudprogs->next;
		free_mprog( pObj->mudprogs );
		pObj->mudprogs	= oprg;
	}
	else
	{
		oprg_prev	= pObj->mudprogs;
		oprg		= oprg_prev->next;

		for ( count = 1; oprg && count < pnum; count++ )
		{
			oprg_prev	= oprg;
			oprg	= oprg->next;
		}
		if ( oprg )
		{
			oprg_prev->next = oprg->next;
			free_mprog( oprg );
		}
	}

	sprintf( buf, "OBJProg %d Deleted." NL, pnum + 1 );
	send_to_char( buf, ch );
	return;
}

void delete_rprog( CHAR_DATA * ch, int pnum )
{
	MPROG_DATA     *rprg;
	MPROG_DATA     *rprg_prev;
	ROOM_INDEX_DATA *pRoom;
	char            buf	[ MAX_INPUT_LENGTH ];
	int             count;

	pRoom = ( ROOM_INDEX_DATA * ) ch->desc->olc_editing;

	if ( pnum < 0 )
		return;

	if ( pnum == 0 )
	{
		rprg = pRoom->mudprogs->next;
		free_mprog( pRoom->mudprogs );
		pRoom->mudprogs = rprg;
	}
	else
	{
		rprg_prev	= pRoom->mudprogs;
		rprg		= rprg_prev->next;

		for ( count = 1; rprg && count < pnum; count++ )
		{
			rprg_prev	= rprg;
			rprg	= rprg->next;
		}
		if ( rprg )
		{
			rprg_prev->next = rprg->next;
			free_mprog( rprg );
		}
	}

	sprintf( buf, "ROOMProg %d Deleted." NL, pnum + 1 );
	send_to_char( buf, ch );
	return;
}



void mpedit_show( CHAR_DATA *ch, char *argument )
{
	MPROG_DATA     *pMobProg;
	MOB_INDEX_DATA *pMob;
	char            buf [MAX_STRING_LENGTH];

	if( !(pMob = ( MOB_INDEX_DATA * ) ch->desc->olc_editing) )
	{
		bug( "mpedit_show: null pMob." );
		edit_done( ch, (char *)"" );
		return;
	}
	pMobProg = edit_mprog( ch, pMob );

	if ( argument[0] == '\0' )
		show_mprog( ch, pMobProg );
	else
		if( is_number( argument ) )
	{
		MPROG_DATA *mprg;
		int         prg = atoi( argument );
		int         cnt = mprog_count( pMob );

		if ( prg < 1 || prg > cnt )
		{
			sprintf( buf, "Valid range is 1 to %d." NL, cnt );
			send_to_char( buf, ch );
			return;
		}

		for ( mprg = pMob->mudprogs; mprg && prg-- > 1; mprg = mprg->next )
			;
		show_mprog( ch, mprg );
	}
	else if ( !str_cmp( argument, "all" ) )
	{
		for ( pMobProg = pMob->mudprogs; pMobProg; pMobProg = pMobProg->next )
			show_mprog( ch, pMobProg );
		send_to_char( "|" NL, ch );
	}
	else
		send_to_char( "Syntax: show <all>" NL, ch );

	return;
}


void opedit_show( CHAR_DATA *ch, char *argument )
{
	MPROG_DATA     *pObjProg;
	OBJ_INDEX_DATA *pObj;
	char            buf [MAX_STRING_LENGTH];

	pObj	= (OBJ_INDEX_DATA *) ch->desc->olc_editing;
	if( !(pObj = ( OBJ_INDEX_DATA * ) ch->desc->olc_editing) )
	{
		bug( "opedit_show: null pObj." );
		edit_done( ch, (char *)"" );
		return;
	}
	pObjProg	= edit_oprog( ch, pObj );

	if( argument[0] == '\0' )
		show_oprog( ch, pObjProg );
	else
		if( is_number( argument ) )
	{
		MPROG_DATA *oprg;
		int         prg = atoi( argument );
		int         cnt = oprog_count( pObj );

		if ( prg < 1 || prg > cnt )
		{
			sprintf( buf, "Valid range is 1 to %d." NL, cnt );
			send_to_char( buf, ch );
			return;
		}

		for ( oprg = pObj->mudprogs; oprg && prg-- > 1; oprg = oprg->next )
			;
		show_oprog( ch, oprg );
	}
	else if ( !str_cmp( argument, "all" ) )
	{
		for ( pObjProg = pObj->mudprogs; pObjProg; pObjProg = pObjProg->next )
			show_oprog( ch, pObjProg );
		send_to_char( "|" NL, ch );
	}
	else
		send_to_char( "Syntax: show <all>" NL, ch );

	return;
}

void rpedit_show( CHAR_DATA *ch, char *argument )
{
	MPROG_DATA     *pRoomProg;
	ROOM_INDEX_DATA *pRoom;
	char            buf [MAX_STRING_LENGTH];

	if( !(pRoom = ( ROOM_INDEX_DATA * ) ch->desc->olc_editing) )
	{
		bug( "rpedit_show: null pRoom." );
		edit_done( ch, (char *)"" );
		return;
	}
	pRoomProg	= edit_rprog( ch, pRoom );

	if( argument[0] == '\0' )
		show_rprog( ch, pRoomProg );
	else
		if( is_number( argument ) )
	{
		MPROG_DATA *rprg;
		int         prg = atoi( argument );
		int         cnt = rprog_count( pRoom );

		if ( prg < 1 || prg > cnt )
		{
			sprintf( buf, "Valid range is 1 to %d." NL, cnt );
			send_to_char( buf, ch );
			return;
		}

		for ( rprg = pRoom->mudprogs; rprg && prg-- > 1; rprg = rprg->next )
			;
		show_rprog( ch, rprg );
	}
	else if ( !str_cmp( argument, "all" ) )
	{
		for ( pRoomProg = pRoom->mudprogs; pRoomProg; pRoomProg = pRoomProg->next )
			show_rprog( ch, pRoomProg );
		send_to_char( "|" NL, ch );
	}
	else
		send_to_char( "Syntax: show <all>" NL, ch );

	return;
}



void mpedit( DESCRIPTOR_DATA *d, char *argument )
{
	CHAR_DATA *		ch 	= d->character;
	MOB_INDEX_DATA *pMob;
	MPROG_DATA     *pMobProg;
	char 	    arg		[ MAX_STRING_LENGTH ];
	char            arg1	[ MAX_STRING_LENGTH ];
	char            arg2	[ MAX_STRING_LENGTH ];
	char            buf		[ MAX_STRING_LENGTH ];
	int             value;
	int             count;

	if ( IS_NPC( ch ) )
	{
		edit_done( ch, (char *)"" );
		return;
	}

	strcpy( arg, argument );
	smash_tilde( argument );
	argument = one_argument( argument, arg1 );
	strcpy( arg2, argument );

	if( !(pMob = ( MOB_INDEX_DATA * ) ch->desc->olc_editing) )
	{
		bug( "mpedit: null pMob." );
		edit_done( ch, (char *)"" );
		return;
	}
	pMobProg	= edit_mprog( ch, pMob );
	count	= mprog_count( pMob );


	if ( !str_prefix( arg1, "show" ) )
	{
		sprintf( buf, "%s", arg2 );
		mpedit_show( ch, buf );
		return;
	}

	if ( !str_cmp( arg1, "next" ) || !str_cmp( arg1, ">" ) )
	{
		if( ch->pcdata->mprog_edit+1 >= mprog_count( pMob ) )
		{
			send_to_char( "There is no next prog." NL, ch );
			return;
		}
		ch->pcdata->mprog_edit++;
		send_to_char( "Editing next prog." NL, ch );
		return;
	}
	if ( !str_cmp( arg1, "prev" ) || !str_cmp( arg1, "<" ) )
	{
		if( ch->pcdata->mprog_edit <= 0 )
		{
			send_to_char( "There is no prev prog." NL, ch );
			return;
		}
		ch->pcdata->mprog_edit--;
		send_to_char( "Editing prev prog." NL, ch );
		return;
	}


	GET_DONE( );

	GET_HELP( arg2 );


	if ( !str_cmp( arg1, "add" ) )
	{
		MPROG_DATA *mprg;
		MPROG_DATA *mprg2;

		if ( !pMob->mudprogs )
			send_to_char( "Mobile doesn't have mobprogs. Use create." NL, ch );

		for ( mprg = mprg2 = pMob->mudprogs; mprg; mprg2 = mprg, mprg = mprg->next )
			pMob->progtypes |= mprg->type;

		mprg2->next = new_mprog();
		pMob->progtypes |= mprg2->type;
		count++;

		ch->pcdata->mprog_edit = count - 1;

		sprintf( buf, "MOBProg %d Added." NL, count );
		send_to_char( buf, ch );
		return;
	}


	if ( !str_prefix( arg1, "delete" ) )
	{
		if ( arg2[0] == '\0' )
			delete_mprog( ch, ch->pcdata->mprog_edit );
		else if ( is_number( arg2 ) )
		{
			if ( ( value = atoi( arg2 ) ) > count )
			{
				send_to_char("Mobile does not have that many programs." NL, ch);
				return;
			}

			delete_mprog( ch, value - 1 );
		}
		else if ( !str_cmp( arg2, "all" ) )
		{
			for ( value = count - 1; value >= 0; value-- )
				delete_mprog( ch, value );
		}
		else
		{
			send_to_char( "Syntax:  delete <pnum>|all" NL, ch );
			return;
		}

		count = mprog_count( pMob );
		if ( ch->pcdata->mprog_edit >= count )
		{
			ch->pcdata->mprog_edit = count - 1;
			if ( count == 0 )
				edit_done( ch, (char *)"" );
		}

		send_to_char( "Ok." NL, ch );
		return;
	}


	if ( !str_prefix( arg1, "copy" ) )
	{
		MOB_INDEX_DATA *cMob;
		MPROG_DATA     *mprg;
		MPROG_DATA     *cprg;
		MPROG_DATA     *mprg_next;

		if ( arg2[0] == '\0' || !is_number( arg2 ) )
		{
			send_to_char( "Syntax:  copy [vnum]" NL, ch );
			return;
		}

		value = atoi( arg2 );
		if ( !( cMob = get_mob_index( value ) ) )
		{
			send_to_char( "No such mobile exists." NL, ch );
			return;
		}

		if ( cMob == pMob )
		{
			send_to_char( "You can't copy from yourself, sorry." NL, ch );
			return;
		}

		if ( !cMob->mudprogs )
		{
			send_to_char( "That mobile doesn't have mobprogs!" NL, ch );
			return;
		}

		for ( mprg = pMob->mudprogs; mprg; mprg = mprg_next )
		{
			mprg_next = mprg->next;
			free_mprog( mprg );
		}

		mprg = pMob->mudprogs = new_mprog();
		pMob->progtypes = 0;
		for ( cprg = cMob->mudprogs; cprg;
					cprg = cprg->next, mprg = mprg->next )
		{
			mprg->type = cprg->type;
			pMob->progtypes |= mprg->type;
			SET_BIT( pMob->progtypes, cprg->type );
			STRDUP( mprg->arglist, cprg->arglist );
			STRDUP( mprg->comlist, cprg->comlist );
			if ( cprg->next )
				mprg->next = new_mprog();
			else
				mprg->next = NULL;

		}

		ch->pcdata->mprog_edit = mprog_count( pMob ) - 1;

		send_to_char( "MOBProg copied." NL, ch );
		return;
	}


	if ( !str_prefix( arg1, "trigger" ) )
	{
		if ( arg2[0] == '\0' )
		{
			send_to_char( "Syntax:  trigger <trigger value(s)>" NL, ch );
			return;
		}

		STRDUP( pMobProg->arglist,arg2 );
		send_to_char( "Trigger set." NL, ch );
		return;
	}


	SET_STR_EDIT( "program", pMobProg->comlist, arg2 )

	int64 flag;
	if ( ( flag = flag_value( mprog_flags_list, arg ) ) != NO_FLAG )
	{
		pMobProg->type = flag;

		BUILD_PROG_TYPES( pMob );

		send_to_char( "MOBProg type set." NL, ch );
		return;
	}

	GET_CREATE( mpedit_create( ch, argument ) );

	interpret( ch, arg );
	return;
}

void opedit( DESCRIPTOR_DATA *d, char *argument )
{
	CHAR_DATA *		ch = d->character;
	OBJ_INDEX_DATA *pObj;
	MPROG_DATA     *pObjProg;
	char 	    arg		[ MAX_STRING_LENGTH ];
	char            arg1	[ MAX_STRING_LENGTH ];
	char            arg2	[ MAX_STRING_LENGTH ];
	char            buf		[ MAX_STRING_LENGTH ];
	int             value;
	int             count;

	if ( IS_NPC( ch ) )
	{
		edit_done( ch, (char *)"" );
		return;
	}

	strcpy( arg, argument );
	smash_tilde( argument );
	argument = one_argument( argument, arg1 );
	strcpy( arg2, argument );

	if( !(pObj = ( OBJ_INDEX_DATA * ) ch->desc->olc_editing) )
	{
		bug( "opedit: null pObj." );
		edit_done( ch, (char *)"" );
		return;
	}
	pObjProg	= edit_oprog( ch, pObj );
	count	= oprog_count( pObj );


	if ( !str_prefix( arg1, "show" ) )
	{
		sprintf( buf, "%s", arg2 );
		opedit_show( ch, buf );
		return;
	}

	GET_DONE( );

	GET_HELP( arg2 );

	if ( !str_cmp( arg1, "next" ) || !str_cmp( arg1, ">" ) )
	{
		if( ch->pcdata->mprog_edit+1 >= oprog_count( pObj ) )
		{
			send_to_char( "There is no next prog." NL, ch );
			return;
		}
		ch->pcdata->mprog_edit++;
		send_to_char( "Editing next prog." NL, ch );
		return;
	}
	if ( !str_cmp( arg1, "prev" ) || !str_cmp( arg1, "<" ) )
	{
		if( ch->pcdata->mprog_edit <= 0 )
		{
			send_to_char( "There is no prev prog." NL, ch );
			return;
		}
		ch->pcdata->mprog_edit--;
		send_to_char( "Editing prev prog." NL, ch );
		return;
	}


	if ( !str_prefix( arg1, "add" ) )
	{
		MPROG_DATA *oprg;
		MPROG_DATA *oprg2;

		if ( !pObj->mudprogs )
			send_to_char( "Object doesn't have objprogs. Use create." NL, ch );

		for ( oprg = oprg2 = pObj->mudprogs; oprg;
					oprg2 = oprg, oprg = oprg->next );

		CREATE( oprg2->next, MPROG_DATA, 1 );
		oprg		= oprg2->next;
		oprg->type	= 0|RAND_PROG;
		STRDUP( oprg->arglist,"0" );
		STRDUP( oprg->comlist,"break\n" );
		count++;

		ch->pcdata->mprog_edit = count - 1;

		sprintf( buf, "OBJProg %d Added." NL, count );
		send_to_char( buf, ch );
		return;
	}


	if ( !str_prefix( arg1, "delete" ) )
	{
		if ( arg2[0] == '\0' )
			delete_oprog( ch, ch->pcdata->mprog_edit );
		else if ( is_number( arg2 ) )
		{
			if ( ( value = atoi( arg2 ) ) > count )
			{
				send_to_char( "Object does not have that many programs." NL, ch);
				return;
			}

			delete_oprog( ch, value - 1 );
		}
		else if ( !str_cmp( arg2, "all" ) )
		{
			for ( value = count - 1; value >= 0; value-- )
				delete_oprog( ch, value );
		}
		else
		{
			send_to_char( "Syntax:  delete <pnum>|all" NL, ch );
			return;
		}

		count = oprog_count( pObj );
		if ( ch->pcdata->mprog_edit >= count )
		{
			ch->pcdata->mprog_edit = count - 1;
			if ( count == 0 )
				edit_done( ch, (char *)"" );
		}

		send_to_char( "Ok." NL, ch );
		return;
	}


	if ( !str_prefix( arg1, "copy" ) )
	{
		OBJ_INDEX_DATA *cObj;
		MPROG_DATA     *oprg;
		MPROG_DATA     *cprg;
		MPROG_DATA     *oprg_next;

		if ( arg2[0] == '\0' || !is_number( arg2 ) )
		{
			send_to_char( "Syntax:  copy [vnum]" NL, ch );
			return;
		}

		value = atoi( arg2 );
		if ( !( cObj = get_obj_index( value ) ) )
		{
			send_to_char( "No such object exists." NL, ch );
			return;
		}

		if ( cObj == pObj )
		{
			send_to_char( "You can't copy from yourself, sorry." NL, ch );
			return;
		}

		if ( !cObj->mudprogs )
		{
			send_to_char( "That object doesn't have progs!" NL, ch );
			return;
		}

		for ( oprg = pObj->mudprogs; oprg; oprg = oprg_next )
		{
			oprg_next = oprg->next;
			free_mprog( oprg );
		}

		CREATE( pObj->mudprogs,MPROG_DATA,1 );
		oprg = pObj->mudprogs;
		for ( cprg = cObj->mudprogs; cprg;
					cprg = cprg->next, oprg = oprg->next )
		{
			oprg->type = cprg->type;
			SET_BIT( pObj->progtypes, cprg->type );
			STRDUP( oprg->arglist, cprg->arglist );
			STRDUP( oprg->comlist, cprg->comlist );
			if ( cprg->next )
				CREATE( oprg->next,MPROG_DATA, 1);
			else
				oprg->next = NULL;
		}

		ch->pcdata->mprog_edit = oprog_count( pObj ) - 1;

		send_to_char( "OBJProg copied." NL, ch );
		return;
	}


	if ( !str_prefix( arg1, "trigger" ) )
	{
		if ( arg2[0] == '\0' )
		{
			send_to_char( "Syntax:  trigger <trigger value(s)>" NL, ch );
			return;
		}

		STRDUP( pObjProg->arglist, arg2 );
		send_to_char( "Trigger set." NL, ch );
		return;
	}

	SET_STR_EDIT( "program", pObjProg->comlist, arg2 )

	int64 flag;
	if ( ( flag = flag_value( mprog_flags_list, arg ) ) != NO_FLAG )
	{
		pObjProg->type = flag;
		BUILD_PROG_TYPES( pObj );

		send_to_char( "OBJProg type set." NL, ch );
		return;
	}

	GET_CREATE( opedit_create( ch, argument ) );

	interpret( ch, arg );
	return;
}

void rpedit( DESCRIPTOR_DATA *d, char *argument )
{
	CHAR_DATA *		ch	= d->character;
	ROOM_INDEX_DATA *pRoom;
	MPROG_DATA     *pRoomProg;
	char 	    arg		[ MAX_STRING_LENGTH ];
	char            arg1	[ MAX_STRING_LENGTH ];
	char            arg2	[ MAX_STRING_LENGTH ];
	char            buf		[ MAX_STRING_LENGTH ];
	int             value;
	int             count;

	if ( IS_NPC( ch ) )
	{
		edit_done( ch, (char *)"" );
		return;
	}

	strcpy( arg, argument );
	smash_tilde( argument );
	argument = one_argument( argument, arg1 );
	strcpy( arg2, argument );

	if( !(pRoom = ( ROOM_INDEX_DATA * ) ch->desc->olc_editing) )
	{
		bug( "rpedit: null pRoom." );
		edit_done( ch, (char *)"" );
		return;
	}
	pRoomProg	= edit_rprog( ch, pRoom );
	count	= rprog_count( pRoom );

	if ( !str_prefix( arg1, "show" ) )
	{
		sprintf( buf, "%s", arg2 );
		rpedit_show( ch, buf );
		return;
	}

	GET_DONE( );

	GET_HELP( arg2 );

	if ( !str_cmp( arg1, "next" ) || !str_cmp( arg1, ">" ) )
	{
		if( ch->pcdata->mprog_edit+1 >= rprog_count( pRoom ) )
		{
			send_to_char( "There is no next prog." NL, ch );
			return;
		}
		ch->pcdata->mprog_edit++;
		send_to_char( "Editing next prog." NL, ch );
		return;
	}
	if ( !str_cmp( arg1, "prev" ) || !str_cmp( arg1, "<" ) )
	{
		if( ch->pcdata->mprog_edit <= 0 )
		{
			send_to_char( "There is no prev prog." NL, ch );
			return;
		}
		ch->pcdata->mprog_edit--;
		send_to_char( "Editing prev prog." NL, ch );
		return;
	}


	if ( !str_prefix( arg1, "add" ) )
	{
		MPROG_DATA *rprg;
		MPROG_DATA *rprg2;

		if ( !pRoom->mudprogs )
			send_to_char( "Room doesn't have progs. Use create." NL, ch );

		for ( rprg = rprg2 = pRoom->mudprogs; rprg;
					rprg2 = rprg, rprg = rprg->next );

		CREATE( rprg2->next, MPROG_DATA, 1 );
		rprg		= rprg2->next;
		rprg->type	= RAND_PROG;
		pRoom->progtypes= pRoom->progtypes | rprg->type;
		STRDUP( rprg->arglist, "0" );
		STRDUP( rprg->comlist, "break\n" );
		count++;

		ch->pcdata->mprog_edit = count - 1;

		sprintf( buf, "ROOMProg %d Added." NL, count );
		send_to_char( buf, ch );
		return;
	}


	if ( !str_prefix( arg1, "delete" ) )
	{
		if ( arg2[0] == '\0' )
			delete_rprog( ch, ch->pcdata->mprog_edit );
		else if ( is_number( arg2 ) )
		{
			if ( ( value = atoi( arg2 ) ) > count )
			{
				send_to_char( "Room does not have that many programs." NL, ch
							);
				return;
			}

			delete_rprog( ch, value - 1 );
		}
		else if ( !str_cmp( arg2, "all" ) )
		{
			for ( value = count - 1; value >= 0; value-- )
				delete_rprog( ch, value );
		}
		else
		{
			send_to_char( "Syntax:  delete <pnum>|all" NL, ch );
			return;
		}

		count = rprog_count( pRoom );
		if ( ch->pcdata->mprog_edit >= count )
		{
			ch->pcdata->mprog_edit = count - 1;
			if ( count == 0 )
				edit_done( ch, (char *)"" );
		}

		send_to_char( "Ok." NL, ch );
		return;
	}


	if ( !str_prefix( arg1, "copy" ) )
	{
		ROOM_INDEX_DATA *cRoom;
		MPROG_DATA     *rprg;
		MPROG_DATA     *cprg;
		MPROG_DATA     *rprg_next;

		if ( arg2[0] == '\0' || !is_number( arg2 ) )
		{
			send_to_char( "Syntax:  copy [vnum]" NL, ch );
			return;
		}

		value = atoi( arg2 );
		if ( !( cRoom = get_room_index( value ) ) )
		{
			send_to_char( "No such room exists." NL, ch );
			return;
		}

		if ( cRoom == pRoom )
		{
			send_to_char( "You can't copy from yourself, sorry." NL, ch );
			return;
		}

		if ( !cRoom->mudprogs )
		{
			send_to_char( "That room doesn't have progs!" NL, ch );
			return;
		}

		for ( rprg = pRoom->mudprogs; rprg; rprg = rprg_next )
		{
			rprg_next = rprg->next;
			free_mprog( rprg );
		}

		CREATE( pRoom->mudprogs,MPROG_DATA,1 );
		rprg = pRoom->mudprogs;
		for ( cprg = cRoom->mudprogs; cprg;
					cprg = cprg->next, rprg = rprg->next )
		{
			rprg->type = cprg->type;
			SET_BIT( pRoom->progtypes, cprg->type );
			STRDUP( rprg->arglist, cprg->arglist );
			STRDUP( rprg->comlist, cprg->comlist );
			if ( cprg->next )
				CREATE( rprg->next, MPROG_DATA, 1 );
			else
				rprg->next = NULL;
		}

		ch->pcdata->mprog_edit = rprog_count( pRoom ) - 1;

		send_to_char( "OBJProg copied." NL, ch );
		return;
	}


	if ( !str_prefix( arg1, "trigger" ) )
	{
		if ( arg2[0] == '\0' )
		{
			send_to_char( "Syntax:  trigger <trigger value(s)>" NL, ch );
			return;
		}

		STRDUP( pRoomProg->arglist, arg2 );
		send_to_char( "Trigger set." NL, ch );
		return;
	}


	SET_STR_EDIT( "program", pRoomProg->comlist, arg2 )

	int64 flag;
	if ( ( flag = flag_value( mprog_flags_list, arg ) ) != NO_FLAG )
	{
		pRoomProg->type = flag;

		BUILD_PROG_TYPES( pRoom );

		send_to_char( "ROOMProg type set." NL, ch );
		return;
	}

	GET_CREATE( rpedit_create( ch, argument ) );

	interpret( ch, arg );
	return;
}




DEF_DO_FUN( opedit )
{
	OBJ_INDEX_DATA *pObj;
	MPROG_DATA     *oprg;
	MPROG_DATA     *cprg;
	int             value;
	char            arg1 [MAX_STRING_LENGTH];
	char            arg2 [MAX_STRING_LENGTH];

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

	if ( is_number( arg1 ) )
	{
		value = atoi( arg1 );
		if ( !( pObj = get_obj_index( value ) ) )
		{
			send_to_char( "Ten vnum nie istnieje." NL, ch );
			return;
		}
		if( !can_edit( ch, value ) )
		{
			send_to_char( "No permission. Sorry." NL, ch );
			return;
		}

		if ( arg2[0] == '\0' || atoi( arg2 ) == 0 )
		{
			if ( !pObj->mudprogs )
			{
				send_to_char( "Przedmiot nie ma objprog�w.", ch );
				send_to_char( "  U�yj create." NL, ch );
				return;
			}
			else
			{
				send_to_char( "Edycja pierwszego objproga." NL, ch );
				value = 1;
			}
		}
		else if ( ( value = atoi( arg2 ) ) > oprog_count( pObj ) )
		{
			send_to_char( "Przedmiot nie ma tak duzo objprog�w." NL, ch );
			return;
		}

		ch->desc->olc_editing = ( void * ) pObj;
		ch->pcdata->mprog_edit = value - 1;
		ch->desc->connected = CON_OPEDITOR;
		opedit( ch->desc, (char *)"show" );
		return;
	}
	else
	{
		if ( !str_cmp( arg1, "create" ) )
		{
			value = atoi( arg2 );
			if ( arg2[0] == '\0' || value == 0 )
			{
				send_to_char( "Syntax:  opedit create vnum" NL,
							  ch );
				return;
			}

			if( !can_edit( ch, value ) )
			{
				send_to_char( "No permission. Sorry." NL, ch );
				return;
			}

			if ( opedit_create( ch, arg2 ) )
				ch->desc->connected = CON_OPEDITOR;
			else
				return;

			if ( is_number( argument ) )
			{
				OBJ_INDEX_DATA *cObj, *pObj;

				if ( !( pObj = get_obj_index( value ) ) )
				{
					send_to_char( "No destination obj exists." NL, ch );
					return;
				}

				value = atoi( argument );
				if ( !( cObj = get_obj_index( value ) ) )
				{
					send_to_char( "No such source obj exists." NL, ch );
					return;
				}

				oprg = pObj->mudprogs;

				for ( cprg = cObj->mudprogs; cprg;
								  cprg = cprg->next, oprg = oprg->next )
				{
					oprg->type = cprg->type;
					STRDUP( oprg->arglist, cprg->arglist );
					STRDUP( oprg->comlist, cprg->comlist );
					if ( cprg->next )
						CREATE( oprg->next, MPROG_DATA,1 );
					else
						oprg->next = NULL;
				}

				send_to_char( "OBJProg copied." NL, ch );
			}
			return;
		}
	}

	send_to_char( "Syntax: opedit <vnum> [prog_nr]." NL, ch );
	return;
}

//Added by Ratm progi lokacji
DEF_DO_FUN( rpedit )
{
	ROOM_INDEX_DATA *pRoom;
	MPROG_DATA     *rprg;
	MPROG_DATA     *cprg;
	int             value;
	char            arg1 [MAX_STRING_LENGTH];
	char            arg2 [MAX_STRING_LENGTH];

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );


	if ( is_number( arg1 ) )
	{
		value = atoi( arg1 );
		if ( !( pRoom = get_room_index( value ) ) )
		{
			send_to_char( "Ten vnum nie istnieje." NL, ch );
			return;
		}
		if( !can_edit( ch, pRoom->vnum ) )
		{
			send_to_char( "No permission. Sorry." NL, ch );
			return;
		}

		if ( arg2[0] == '\0' || atoi( arg2 ) == 0 )
		{
			if ( !pRoom->mudprogs )
			{
				send_to_char( "Pok�j nie ma prog�w.", ch );
				send_to_char( "  U�yj create." NL, ch );
				return;
			}
			else
			{
				send_to_char( "Edycja pierwszego roomproga." NL, ch );
				value = 1;
			}
		}
		else if ( ( value = atoi( arg2 ) ) > rprog_count( pRoom ) )
		{
			send_to_char( "Pok�j nie ma tak du�o prog�w." NL, ch );
			return;
		}

		ch->desc->olc_editing = ( void * ) pRoom;
		ch->pcdata->mprog_edit = value - 1;
		ch->desc->connected = CON_RPEDITOR;
		rpedit( ch->desc, (char *)"show" );
		return;
	}
	else
	{
		if ( !str_cmp( arg1, "create" ) )
		{
			value = atoi( arg2 );
			if ( arg2[0] == '\0' || value == 0 )
			{
				send_to_char( "Syntax:  edit roomprog create vnum [svnum]" NL,
							  ch );
				return;
			}

			if( !can_edit( ch, value ) )
			{
				send_to_char( "No permission. Sorry." NL, ch );
				return;
			}

			if ( rpedit_create( ch, arg2 ) )
				ch->desc->connected = CON_RPEDITOR;
			else
				return;

			if ( is_number( argument ) )
			{
				ROOM_INDEX_DATA *cRoom, *pRoom;

				if ( !( pRoom = get_room_index( value ) ) )
				{
					send_to_char( "No destination room exists." NL, ch );
					return;
				}

				value = atoi( argument );
				if ( !( cRoom = get_room_index( value ) ) )
				{
					send_to_char( "No such source room exists." NL, ch );
					return;
				}

				rprg = pRoom->mudprogs;

				for ( cprg = cRoom->mudprogs; cprg;
								  cprg = cprg->next, rprg = rprg->next )
				{
					rprg->type = cprg->type;
					STRDUP( rprg->arglist, cprg->arglist );
					STRDUP( rprg->comlist, cprg->comlist );
					if ( cprg->next )
						CREATE( rprg->next, MPROG_DATA, 1 );
					else
						rprg->next = NULL;
				}

				send_to_char( "ROOMProg copied." NL, ch );
			}
			return;
		}
	}

	send_to_char( "Syntax: rpedit <vnum> [prog_nr]." NL, ch );
	return;
}

//done

DEF_DO_FUN( mpedit )
{
	MOB_INDEX_DATA *pMob;
	MPROG_DATA     *mprg;
	MPROG_DATA     *cprg;
	int             value;
	char            arg1 [MAX_STRING_LENGTH];
	char            arg2 [MAX_STRING_LENGTH];

	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

	if ( is_number( arg1 ) )
	{
		value = atoi( arg1 );
		if ( !( pMob = get_mob_index( value ) ) )
		{
			send_to_char( "Ten vnum nie istnieje." NL, ch );
			return;
		}
		if( !can_edit( ch, value ) )
		{
			send_to_char( "No permission. Sorry." NL, ch );
			return;
		}

		if ( arg2[0] == '\0' || atoi( arg2 ) == 0 )
		{
			if ( !pMob->mudprogs )
			{
				send_to_char( "Mob nie ma mobprogow.", ch );
				send_to_char( "  Uzyj create." NL, ch );
				return;
			}
			else
			{
				send_to_char( "Edycja pierwszego mobproga." NL, ch );
				value = 1;
			}
		}
		else if ( ( value = atoi( arg2 ) ) > mprog_count( pMob ) )
		{
			send_to_char( "Mob nie ma tak duzo mobprogow." NL, ch );
			return;
		}

		ch->desc->olc_editing = ( void * ) pMob;
		ch->pcdata->mprog_edit = value - 1;
		ch->desc->connected = CON_MPEDITOR;
		mpedit( ch->desc, (char *)"show" );
		return;
	}
	else
	{
		if ( !str_cmp( arg1, "create" ) )
		{
			value = atoi( arg2 );
			if ( arg2[0] == '\0' || value == 0 )
			{
				send_to_char( "Syntax:  edit mobprog create vnum [svnum]" NL,
							  ch );
				return;
			}

			if( !can_edit( ch, value ) )
			{
				send_to_char( "No permission. Sorry." NL, ch );
				return;
			}

			if ( mpedit_create( ch, arg2 ) )
				ch->desc->connected = CON_MPEDITOR;
			else
				return;

			if ( is_number( argument ) )
			{
				MOB_INDEX_DATA *cMob, *pMob;

				if ( !( pMob = get_mob_index( value ) ) )
				{
					send_to_char( "No destination mob exists." NL, ch );
					return;
				}

				value = atoi( argument );
				if ( !( cMob = get_mob_index( value ) ) )
				{
					send_to_char( "No such source mob exists." NL, ch );
					return;
				}
				if( !can_edit( ch, value ) )
				{
					send_to_char( "No permission. Sorry." NL, ch );
					return;
				}

				mprg = pMob->mudprogs;

				for ( cprg = cMob->mudprogs; cprg;
								  cprg = cprg->next, mprg = mprg->next )
				{
					mprg->type = cprg->type;
					STRDUP( mprg->arglist, cprg->arglist );
					STRDUP( mprg->comlist, cprg->comlist );
					if ( cprg->next )
						CREATE( mprg->next,MPROG_DATA,1 );
					else
						mprg->next = NULL;
				}

				send_to_char( "MOBProg copied." NL, ch );
			}
			return;
		}
	}

	send_to_char( "Syntax: mpedit <vnum> [prog_nr]." NL, ch );
	return;
}

DEF_DO_FUN( editinfo )
{
	if ( IS_NPC( ch ) )
	{
		send_to_char( "Jak jestes mobem to siedz cicho :P" NL, ch );
		return;
	}

	if ( argument[0] != '\0' && str_cmp( argument, "edit" ) )
	{
		send_to_char( "Sk�adnia: editinfo [edit]" NL, ch );
		return;
	}

	if ( !str_cmp( argument, "edit" ) )
	{
		string_append( ch, &ch->pcdata->editinfo );
		return;
	}

	if ( ch->pcdata->editinfo[0] == '\0' )
	{
		send_to_char(
				"Tw�j notatnik jest pusty. Aby co� napisa� wydaj komend� 'editinfo edit'." NL, ch );
		return;
	}

	pager_printf( ch, "Tw�j notatnik zawiera:" NL NL "%s" EOL,
				  ch->pcdata->editinfo );
	return;
}

void 		save_helps( );

HELPS_FILE *find_helps_file( char *name )
{
	HELPS_FILE	*fHelp;

	for( fHelp = first_helps_file; fHelp; fHelp = fHelp->next )
		if( !str_cmp( name, fHelp->name ) )
			return fHelp;
	return NULL;
}

HELP_DATA *find_help( const char *argument )
{
	HELPS_FILE	*fHelp;
	HELP_DATA	*pHelp = 0;
	int			count = 0;
	bool		found = false;

	if( argument[0] == '\0' )
		return NULL;

	for( fHelp = first_helps_file; !found && fHelp; fHelp = fHelp->next )
		for( pHelp = fHelp->first_help; pHelp; pHelp = pHelp->next )
	{
		if( !str_prefix( argument, pHelp->keyword ) ||
				   is_name( argument, pHelp->keyword ) )
		{
			count = 1;
			found = true;
			break;
		}

		if( ( is_name_prefix( argument, pHelp->keyword ) ) )
			count++;
	}

	if( !pHelp || count == 0 )
		return NULL;

	return pHelp;
}

DEF_DO_FUN( hstat )
{
	HELP_DATA *	pHelp;

	if( !(pHelp=find_help( argument )) )
	{
		send_to_char( "Cannot find help on that subject or you have to make it more clear." NL, ch );
		return;
	}

	ch_printf( ch, FG_CYAN "Filename: " PLAIN "%s" EOL, pHelp->file->name );
	ch_printf( ch, FG_CYAN "Keyword: " PLAIN "%s" EOL, pHelp->keyword );
	ch_printf( ch, FG_CYAN "Type:    " PLAIN "%s" FG_CYAN "     Level: " PLAIN "%d" NL,
			   flag_string( help_types_list, pHelp->type ), pHelp->level );

	if( pHelp->syntax[0] != '\0' )
		ch_printf( ch, FG_CYAN "Syntax:" EOL "%s" EOL, pHelp->syntax );
	else
		ch_printf( ch, FG_CYAN "Syntax:  " FG_BLACK "[no sytax]" EOL );

	ch_printf( ch, FG_CYAN "Text:" EOL "%s" EOL, pHelp->text );
	return;
}

bool hedit_create_file( CHAR_DATA *ch, char *argument )
{
	HELPS_FILE	*fHelp;
	char		arg[MIL];

	argument = one_argument( argument, arg );

	if( *arg == '\0' )
	{
		send_to_char( "Syntax: createfile <filename>.hlp" NL, ch );
		return false;
	}

	if( (fHelp = find_helps_file( arg )) )	/* new help */
	{
		send_to_char( "Helps file by that name already exists." NL, ch );
		return false;
	}

	fHelp = new_helps_file();
	STRDUP( fHelp->name, arg );
	LINK( fHelp, first_helps_file, last_helps_file, next, prev );
	top_helps_file++;

	send_to_char( "Helps file created." NL, ch );
	return true;
}

bool hedit_create( CHAR_DATA *ch, char *argument )
{
	HELPS_FILE	*fHelp;
	HELP_DATA	*pHelp;
	char		arg[MIL];

	argument = one_argument( argument, arg );

	if( !(fHelp = find_helps_file( arg )) )
	{
		send_to_char( "No such helps file." NL, ch );
		return false;
	}

	if( (pHelp = find_help( argument )) )	/* new help */
	{
		send_to_char( "There is a help with that keyword already." NL, ch );
		return false;
	}

	pHelp = new_help( fHelp );
	if( argument[0] == '\0' )
		STRDUP( pHelp->keyword,"NOKEYWORD" );
	else
		STRDUP( pHelp->keyword, strupper(argument) );

	STRDUP( pHelp->text, "Brak pomocy na ten temat." NL );
	pHelp->level = ch->top_level;
	add_help( fHelp, pHelp, true );

	ch->desc->olc_editing = ( void * ) pHelp;
	ch->desc->connected = CON_HEDITOR;
	send_to_char( "Help created." NL, ch );
	return true;
}

void hedit( DESCRIPTOR_DATA *d, char *argument )
{
	CHAR_DATA	*ch	= d->character;
	HELP_DATA	*pHelp;
	char		arg[MIL];
	char		arg1[MIL];
	char		buf[MSL];
	int			value;

	if( !(pHelp = ( HELP_DATA * ) ch->desc->olc_editing) )
	{
		bug( "hedit: null pHelp." );
		edit_done( ch, (char *)"" );
		return;
	}

	strcpy( arg, argument );
	smash_tilde( argument );
	argument = one_argument( argument, arg1 );

	if ( !str_prefix( arg1, "show" ) )
	{
		sprintf( buf, "%s", pHelp->keyword );
		do_hstat( ch, buf );
		return;
	}

	GET_CREATE( hedit_create( ch, argument ) );
	if( !str_prefix( arg1, "createfile" ) )
	{
		hedit_create_file( ch, argument );
		return;
	}

	GET_DONE( );

	if( !str_cmp( arg1, "delete" ) )
	{
		HELP_DATA	*n_help;

		UNLINK( pHelp, pHelp->file->first_help, pHelp->file->last_help, next, prev );

		n_help = pHelp->prev ? pHelp->prev : pHelp->next ? pHelp->next : NULL;
/*
		if( pHelp->prev )
		n_help = pHelp->prev;
		else
		if( pHelp->next )
		n_help = pHelp->next;
*/
		send_to_char( "Help page deleted." NL, ch );
		free_help( pHelp );

		if( n_help )
			ch->desc->olc_editing = ( void * ) n_help;
		else
			edit_done( ch, (char *)"" );

		return;
	}

	if( !str_cmp( arg1, "listfiles" ) )
	{
		HELPS_FILE	*fHelp;
		HELP_DATA	*pHelp_cnt;

		send_to_char( "Helps files: " NL, ch );
		FOREACH( fHelp, first_helps_file )
		{
			int i = 0;
			FOREACH( pHelp_cnt, fHelp->first_help )
					i++;
			ch_printf( ch, "  %s (%d)" NL, fHelp->name, i );
		}

		return;
	}

	if( !str_cmp( arg1, "deletefile" ) )
	{
		HELPS_FILE	*fHelp;
		char		arg2[MIL];
		char		arg4[MIL];

		argument = one_argument( argument, arg2 );
		argument = one_argument( argument, arg4 );

		if( *arg2 == '\0' )
		{
			send_to_char( "Syntax: deletefile <filename>.hlp" NL
					"                   <filename>.hlp moveto <filename>.hlp" NL
					"                   <filename>.hlp rename <new_name>.hlp" NL, ch );
			return;
		}

		if( !(fHelp = find_helps_file( arg2 )) )
		{
			send_to_char( "No such helps filename." NL, ch );
			return;
		}

		if( str_cmp( arg4, "sure" ) && str_cmp( arg4, "moveto" ) && str_cmp( arg4, "rename" ) )
		{
			send_to_char( "This will remove the helps file and all help pages included in it!!!" NL
					"If you are really sure type: deletefile <filename>.hlp sure" NL, ch );
			return;
		}

		if( !str_cmp( arg4, "sure" ) )
		{
			UNLINK( fHelp, first_helps_file, last_helps_file, next, prev );
			free_helps_file( fHelp );
			send_to_char( "Helps file (and all help pages included in it) deleted." NL, ch );
			edit_done( ch, (char *)"" );
			return;
		}

		if( !str_cmp( arg4, "moveto" ) )
		{
			HELPS_FILE	*fHelp_to;
			HELP_DATA	*pHelp;
			HELP_DATA	*pHelp_next;

			if( *argument == '\0' )
			{
				send_to_char( "Syntax: deletefile <filename>.hlp moveto <filename>.hlp" NL, ch );
				return;
			}

			if( !(fHelp_to = find_helps_file( argument )) )
			{
				send_to_char( "No such helps file to move to." NL, ch );
				return;
			}

			for( pHelp = fHelp->first_help; pHelp; pHelp = pHelp_next )
			{
				pHelp_next = pHelp->next;
				UNLINK( pHelp, fHelp->first_help, fHelp->last_help, next, prev );
				add_help( fHelp_to, pHelp, false );
			}

			UNLINK( fHelp, first_helps_file, last_helps_file, next, prev );
			free_helps_file( fHelp );
			ch_printf( ch, "Helps file deleted. Help pages moved to: %s." NL, fHelp_to->name );
			return;
		}

		if( !str_cmp( arg4, "rename" ) )
		{
			char		name[MIL];

			argument = one_argument( argument, name );

			if( *name == '\0' )
			{
				send_to_char( "Syntax: deletefile <filename>.hlp rename <new_name>.hlp" NL, ch );
				return;
			}

			if( find_helps_file( name ) )
			{
				send_to_char( "Helps file by that name already exists." NL, ch );
				return;
			}

			clear_helps_file( fHelp->name );
			STRDUP( fHelp->name, name );
			ch_printf( ch, "Helps file renamed to: %s." NL, name );
			return;
		}

		return;
	}

	if( !str_prefix( arg1, "moveto" ) )
	{
		HELPS_FILE	*fHelp;

		if( *argument == '\0' )
		{
			send_to_char( "Syntax: moveto <filename>.hlp" NL, ch );
			return;
		}

		if( !(fHelp = find_helps_file( argument )) )
		{
			send_to_char( "No such helps file to move to." NL, ch );
			return;
		}

		UNLINK( pHelp, pHelp->file->first_help, pHelp->file->last_help, next, prev );
		add_help( fHelp, pHelp, true );

		ch_printf( ch, "Help page moved to: %s." NL, fHelp->name );
		if( !pHelp ) /* Trog: bezpiecznik, add_help moze nam zniszczyc pHelp, jesli jest on duplikatem */
			edit_done( ch, (char *)"" );
		return;
	}

	GET_HELP( argument );
	GET_SAVE( save_helps() );

	if( !str_prefix( arg1, "foldlist" ) )
	{
		save_helps_list();
		donemsg( ch );
		return;
	}

	if( !str_cmp( arg1, "jump" ) )
	{
		HELP_DATA *	nHelp;

		if( argument[0] == '\0' )
		{
			send_to_char( "Syntax: jump <letter/keyword>" NL, ch );
			return;
		}

		if( !(nHelp=find_help( argument )) )
		{
			send_to_char( "No help on that." NL, ch );
			return;
		}
		else
		{
			ch->desc->olc_editing = ( void * ) nHelp;
			send_to_char( "Ok." NL, ch );
			return;
		}
	}

	if ( !str_prefix( arg1, "keywords" ) )
	{
		if ( argument[0] == '\0' )
		{
			send_to_char( "Syntax:   keyword <keyword(s)>" NL, ch );
			return;
		}

		if( find_help( argument ) )
		{
			send_to_char( "There is a help on that!" NL, ch );
			return;
		}

		STRDUP( pHelp->keyword, strupper(argument) );

		send_to_char( "Keyword set." NL, ch );
		return;
	}

	SET_STR_EDIT( "syntax", 	pHelp->syntax, 	argument )
			SET_STR_EDIT( "text", 	pHelp->text, 	argument )

			if ( ( value = flag_value(help_types_list, arg1 ) ) != -1 )
	{
		send_to_char( "Help type set." NL, ch );
		switch( pHelp->type = value )
		{
			case HELP_OLCHELP:
				send_to_char( "Help for OLCMEN. Setting level to 103." NL, ch );
				pHelp->level = 103;
				break;
			case HELP_WIZHELP:
				send_to_char( "Help for IMMORTALS and AVATARS. Setting level to 101." NL
						"It can be also set to 100 manually if for AVATARS." NL, ch );
				pHelp->level = 101;
				break;
		}
		return;
	}

	if( !str_cmp( arg1, "level" ) )
	{
		value = atoi( argument );
		switch( pHelp->type )
		{
			case HELP_PHELP:
				if( value < -1 || value > MAX_LEVEL )
				{
					ch_printf( ch, "Help for players level range is: -1 - %d." NL, MAX_LEVEL );
					return;
				}
				break;
			case HELP_OLCHELP:
				if( value < 103 || value > MAX_LEVEL )
				{
					ch_printf( ch, "Help for OLCMEN level range is: 103 - %d." NL, MAX_LEVEL );
					return;
				}
				break;
			case HELP_WIZHELP:
				if( value < LEVEL_AVATAR || value > MAX_LEVEL )
				{
					ch_printf( ch, "Help for IMMORTALS/AVATARS level range is: %d - %d." NL,
							   LEVEL_AVATAR, MAX_LEVEL );
					return;
				}
				break;
		}

		pHelp->level = value;
		donemsg( ch );
		return;
	}

	if ( !str_cmp( arg1, "next" ) || !str_cmp( arg1, ">" ) )
	{
		if( !pHelp->next )
		{
			send_to_char( "No next help." NL, ch );
			return;
		}
		ch->desc->olc_editing = ( void * ) pHelp->next;
		hedit( ch->desc, (char *)"show" );
		send_to_char( "Editing next Help." NL, ch );
		return;
	}

	if ( !str_cmp( arg1, "prev" ) || !str_cmp( arg1, "<" ) )
	{
		if( !pHelp->prev )
		{
			send_to_char( "No prev help." NL, ch );
			return;
		}
		ch->desc->olc_editing = ( void * ) pHelp->prev;
		hedit( ch->desc, (char *)"show" );
		send_to_char( "Editing prev Help." NL, ch );
		return;
	}

	if ( !str_cmp( arg1, "remove" ) || !str_cmp( arg1, "delete" ) )
	{
		if( pHelp->next )
			ch->desc->olc_editing = ( void * ) pHelp->next;
		else
			if( pHelp->prev )
				ch->desc->olc_editing = ( void * ) pHelp->prev;
		else
			edit_done( ch, (char *)"" );

		UNLINK( pHelp, pHelp->file->first_help, pHelp->file->last_help, next, prev );
		free_help( pHelp );
		send_to_char( "Removed." NL, ch );
		return;
	}

	interpret( ch, arg );
	return;
}


DEF_DO_FUN( hedit )
{
	HELP_DATA *	pHelp;
	char 	arg[MIL];

	if( !*argument )
	{
		send_to_char( "Syntax: hedit <keyword>|create [keyword]|foldarea" NL, ch );
		return;
	}

	argument = one_argument( argument, arg );
	if ( !str_prefix( arg, "fold" ) )
	{
		hedit( ch->desc, (char *)"fold" );
		return;
	}

	if( !str_cmp( arg, "create" ) )
	{
		hedit_create( ch, argument );
		return;
	}

	if( !str_cmp( arg, "createfile" ) )
	{
		hedit_create_file( ch, argument );
		return;
	}

	if ( (pHelp = find_help( arg )) != NULL )
	{
		ch->desc->olc_editing = ( void * ) pHelp;
		ch->desc->connected = CON_HEDITOR;
		hedit( ch->desc, (char *)"show" );
		return;
	}
	else
	{
		send_to_char( "Help not found. Use CREATE." NL, ch );
		return;
	}
	return;
}

HELP_DATA *get_help( CHAR_DATA *ch, char *argument );
/*
 * Stupid leading space muncher fix				-Thoric
 */
char *help_fix( char *text )
{
	char *fixed;

	if ( !text )
		return (char *)"";
	fixed = strip_cr(text);
	if ( fixed[0] == ' ' )
		fixed[0] = '.';
	return fixed;
}

/* Trog */
void save_helps_list()
{
	FILE		*fpout;
	HELPS_FILE	*fHelp;

	log_string_plus( "Saving helps files list...", LOG_NORMAL, LEVEL_GREATER );
	RESERVE_CLOSE;
	if( !(fpout = fopen(HELPS_LIST, "w")) )
	{
		bug( "Cannot open helps.lst for writing!" NL );
		perror( HELPS_LIST );
		return;
	}

	for( fHelp = first_helps_file; fHelp; fHelp = fHelp->next )
		fprintf( fpout, "%s\n", fHelp->name );
	fprintf( fpout, "$\n" );

	fclose( fpout );
	RESERVE_OPEN;
	return;
}

void save_helps()
{
	HELPS_FILE	*fHelp;
	HELP_DATA	*pHelp;
	FILE		*fpout;
	char		help_file[256];
	char		help_bak[256];
	char		filename[MIL];

	RESERVE_CLOSE;

	for( fHelp = first_helps_file; fHelp; fHelp = fHelp->next )
	{
		sprintf( filename, "Saving %s...", fHelp->name );
		log_string_plus( filename, LOG_NORMAL, LEVEL_GREATER );
		sprintf( help_file, "%s%s", HELPS_DIR, fHelp->name );
		sprintf( help_bak, "%s.bak", help_file );
		rename( help_file, help_bak );
		if( !(fpout = fopen(help_file, "w")) )
		{
			bug( "fopen" );
			perror( help_file );
			RESERVE_OPEN;
			return;
		}

		fprintf( fpout, "#SWHELPS\n\n" );
		for( pHelp = fHelp->first_help; pHelp; pHelp = pHelp->next )
		{
			fprintf( fpout, "%d %s~ %d\n",
					 pHelp->level, pHelp->keyword, pHelp->type );
			fprintf( fpout, "%s~\n", help_fix(pHelp->syntax) );
			fprintf( fpout, "%s~\n", help_fix(pHelp->text) );
		}
		fprintf( fpout, "0 $~\n\n" );
		fclose( fpout );
	}

	RESERVE_OPEN;
	save_helps_list();
	return;
}

DEF_DO_FUN( olcinfo )
{
	CMDTYPE * 	cmd;
	int        	cnt, hash;

	pager_printf( ch, NL FB_YELLOW "OLC Commands:" EOL "          " );
	for ( cnt = hash = 0; hash < MAX_CMD_HASH; hash++ )
	{
		for ( cmd = command_hash[hash]; cmd; cmd = cmd->next )
		{
			if( !IS_SET( cmd->flags, CMD_OLC ) )
				continue;

			pager_printf(ch," " PLAIN "%-10.10s%s",
						 cmd->name, ((++cnt)%6)? "" : NL "          ");
		}
	}
	pager_printf( ch, NL );
	return;
}

PROJECT_DATA *get_project( char *argument )
{
	PROJECT_DATA *pro;

	for( pro = first_project; pro; pro = pro->next )
	{
		if( pro->effect )
			if( atoi( argument ) == pro->effect->vnum )
				return pro;
	}

	return NULL;
}

DEF_DO_FUN( prostat )
{
	PROJECT_DATA *	pro;
	PART_DATA *		part;
	int			i;

	if( !(pro=get_project( argument )) )
	{
		send_to_char( "No such project." NL, ch );
		return;
	}

	pager_printf( ch, FG_CYAN "Effect: " FB_WHITE "%d" PLAIN " (%s" PLAIN ")" EOL,
				  pro->effect->vnum, pro->effect->name  );
	pager_printf( ch, FG_CYAN "Montage_type: " PLAIN "[%s]" NL,
				  flag_string( mont_types_list, pro->montage_type ) );

	for( i=1, part = pro->first_part; part; part = part->next, i++ )
	{
		COMPONENT_DATA *	comp;
		int			j=1;

		pager_printf( ch, FB_WHITE "Part %2d" FG_CYAN ": Quantity: " FB_CYAN "%d" EOL,
					  i, part->quantity );

		for( comp = part->first_component; comp; comp = comp->next, j++ )
		{
			pager_printf( ch, "     " FG_YELLOW "Obj " FB_YELLOW "%2d", j );
			if( comp->type == COMP_VNUM )
			{
				OBJ_INDEX_DATA *obj = get_obj_index( comp->nr );

				pager_printf( ch,
							  FG_CYAN " vnum: {" FB_WHITE "%5d" FG_CYAN "}",
							  comp->nr );
				if( obj )
				{
					pager_printf( ch, " %30s %s" EOL,
								  obj->name,
								  get_project( itoa(obj->vnum) ) ? FB_WHITE "[+]" : ""  );
				}
				else
					pager_printf( ch, " " FB_YELLOW "NO SUCH OBJECT!" EOL );
			}
			else
				if( comp->type == COMP_TYPE )
			{
				pager_printf( ch,
							  FG_CYAN " type: {" FB_WHITE "%s" FG_CYAN "}" EOL,
							  bit_name( obj_types_list, comp->nr ) );
			}
			else
				if( comp->type == COMP_WEAR )
			{
				pager_printf( ch,
							  FG_CYAN " wear: {" FB_WHITE "%s" FG_CYAN "}" EOL,
							  flag_string( wear_flags_list, comp->nr ) );
			}
			else
				pager_printf( ch, "ERROR" EOL );
		}
	}
	return;
}

bool proedit_create( CHAR_DATA *ch, char *argument )
{
	PROJECT_DATA *	pro;
	OBJ_INDEX_DATA *	obj;
	int			vnum;

	if( !argument[0] )
	{
		send_to_char( "Syntax:  create <vnum>" NL, ch );
		return false;
	}

	if ( (pro = get_project( argument )) )	/* new project */
	{
		send_to_char( "That project already exists." NL, ch );
		return false;
	}

	vnum = atoi( argument );
	if( !(obj=get_obj_index( vnum )) )
	{
		send_to_char( "Effect object deosn't exist!" NL, ch );
		return false;
	}

	CREATE( pro, PROJECT_DATA, 1 );
	pro->effect 	= obj;
	pro->montage_type 	= MONT_SCREW;
	LINK( pro, first_project, last_project, next, prev );

	ch->desc->olc_editing = ( void * ) pro;
	ch->desc->connected = CON_PROEDITOR;
	send_to_char( "Project created." NL, ch );
	return true;
}

PART_DATA *get_part( PROJECT_DATA *pro, int nr )
{
	PART_DATA *	part;
	int		i;

	for( i = 1, part = pro->first_part; part; part = part->next, i++ )
		if( i == nr )
			return part;

	return NULL;
}

COMPONENT_DATA *get_component( int nr, PART_DATA *part )
{
	COMPONENT_DATA *	comp;
	int			i=1;

	for( comp = part->first_component; comp; comp = comp->next, i++ )
		if( nr==i )
			return comp;

	return NULL;
}

void proedit( DESCRIPTOR_DATA *d, char *argument )
{
	CHAR_DATA *		ch = d->character;
	PROJECT_DATA *	pro;
	PART_DATA *		part;
	COMPONENT_DATA *	comp;
	char       	arg	[ MAX_STRING_LENGTH ];
	char       	arg1	[ MAX_STRING_LENGTH ];
	char       	arg2	[ MAX_STRING_LENGTH ];
	char       	arg3	[ MAX_STRING_LENGTH ];
	char       	buf 	[ MAX_STRING_LENGTH ];
	int             	value;

	pro = ( PROJECT_DATA *) ch->desc->olc_editing;

	strcpy( arg, argument );
	smash_tilde( argument );
	argument = one_argument( argument, arg1 );

	if ( !str_prefix( arg1, "show" ) )
	{
		sprintf( buf, "%d", pro->effect->vnum );
		do_prostat( ch, buf );
		return;
	}

	GET_CREATE( proedit_create( ch, argument ) );

	GET_DONE( );

	if( !str_cmp( arg1, "delete" ) )
	{
		PROJECT_DATA *	n_pro = NULL;

		UNLINK( pro, first_project, last_project, next, prev );

		if( pro->prev )
			n_pro = pro->prev;
		else
			if( pro->next )
				n_pro = pro->next;

		send_to_char( "Ok." NL, ch );
		free_project( pro );

		if( n_pro )
			ch->desc->olc_editing = ( void * ) n_pro;
		else
			edit_done( ch, (char *)"" );
		return;
	}

	GET_HELP( argument );

	if ( !str_prefix( arg1, "fold" ) )
	{
		save_projects( );
		send_to_char( "Done." NL, ch );
		return;
	}

	if( !str_cmp( arg1, "jump" ) )
	{
		PROJECT_DATA *	nPro;

		if ( argument[0] == '\0' )
		{
			send_to_char( "Syntax:   jump <vnum>" NL, ch );
			return;
		}

		if( !(nPro=get_project( argument )) )
		{
			send_to_char( "No such project." NL, ch );
			return;
		}
		else
		{
			ch->desc->olc_editing = ( void * ) nPro;
			send_to_char( "Ok." NL, ch );
			return;
		}
	}

	if ( !str_prefix( arg1, "effect" ) )
	{
		if ( argument[0] == '\0' )
		{
			send_to_char( "Syntax:   effect <obj vnum>" NL, ch );
			return;
		}

		if( get_project( argument ) )
		{
			send_to_char( "There is a project of this object!" NL, ch );
			return;
		}
		if( !get_obj_index( atoi(argument) ) )
		{
			send_to_char( "No such object." NL, ch );
			return;
		}
		pro->effect = get_obj_index( atoi(argument) );
		send_to_char( "Effect set." NL, ch );
		return;
	}

	if ( !str_cmp( arg1, "next" ) || !str_cmp( arg1, ">" ) )
	{
		if( !pro->next )
		{
			send_to_char( "No next project." NL, ch );
			return;
		}
		ch->desc->olc_editing = ( void * ) pro->next;
		proedit( ch->desc, (char *)"show" );
		send_to_char( "Editing next Project." NL, ch );
		return;
	}

	if ( ( value = flag_value(mont_types_list, arg ) ) != -1 )
	{
		pro->montage_type = value;
		ch_printf( ch,"Montage type set." NL );
		return;
	}


	if ( !str_cmp( arg1, "prev" ) || !str_cmp( arg1, "<" ) )
	{
		if( !pro->prev )
		{
			send_to_char( "No prev project." NL, ch );
			return;
		}
		ch->desc->olc_editing = ( void * ) pro->prev;
		proedit( ch->desc, (char *)"show" );
		send_to_char( "Editing prev Project." NL, ch );
		return;
	}

	if( !str_cmp( arg1, "part" ) )
	{
		argument = one_argument( argument, arg2 );
		argument = one_argument( argument, arg3 );

		if( arg2[0] == '\0' || arg3[0] == '\0' )
		{
			send_to_char(
					"Syntax:  part <nr> quantity <quantity>                -- set quantity of part" NL
					"         part <nr> delete                             -- delete whole part" NL
					"         part add <vnum>                              -- create a new part" NL
					"         part <nr> obj <nr> <vnum|wear|type> <value>  -- set component object" NL
					"         part <nr> obj <nr> delete                    -- delete component object" NL
					"         part <nr> obj add                            -- create component object" NL, ch );
			return;
		}

		if( !str_cmp( arg2, "add" ) )
		{
			if( !get_obj_index( atoi( arg3 ) ) )
			{
				send_to_char( "No such object." NL, ch );
				return;
			}

			CREATE( part, PART_DATA, 1 );
			part->quantity = 1;
			CREATE( comp, COMPONENT_DATA, 1 );
			comp->nr = atoi( arg3 );
			comp->type = COMP_VNUM;
			LINK( comp, part->first_component, part->last_component, next, prev );
			LINK( part, pro->first_part, pro->last_part, next, prev );

			send_to_char( "Part added." NL, ch );
			return;
		}

		if( !is_number( arg2 ) )
		{
			proedit( ch->desc, (char *)"part" );
			return;
		}

		if( !(part=get_part( pro, atoi( arg2 ) )) )
		{
			send_to_char( "No such part." NL, ch );
			return;
		}

		if( !str_cmp( arg3, "delete" ) )
		{
			UNLINK( part, pro->first_part, pro->last_part, next, prev );
			free_part( part );
			send_to_char( "Part deleted." NL, ch );
			return;
		}

		if( !str_prefix( arg3, "quantity" ) )
		{
			int i;

			if( argument[0] == '\0' )
			{
				send_to_char(
						"Syntax:  part <nr> quantity <quantity>  -- set the quantity of part" NL, ch );
				return;
			}

			if( (i=atoi( argument )) < 1 )
			{
				send_to_char( "Quantity must be larger than 1." NL, ch );
				return;
			}

			part->quantity = atoi( argument );
			send_to_char( "Part quantity set." NL, ch );
			return;
		}

		if( !str_prefix( arg3, "obj" ) )
		{
			char	arg4	[MIL];
			char	arg5	[MIL];
			int		nr;

			if( !*argument )
			{
				proedit( ch->desc, (char *)"part" );
				return;
			}

			argument = one_argument( argument, arg4 );

			if( !is_number( arg4 ) && !str_cmp( arg4, "add" ) )
			{
				CREATE( comp, COMPONENT_DATA, 1 );
				comp->nr 	= 0;
				comp->type 	= COMP_VNUM;
				LINK( comp, part->first_component, part->last_component, next, prev );
				send_to_char( "Component added." NL, ch );
				return;
			}
			nr = atoi( arg4 );
			if( !(comp=get_component( nr, part ))  )
			{
				send_to_char( "No such object." NL, ch );
				return;
			}

			if( !*argument )
			{
				proedit( ch->desc, (char *)"part" );
				return;
			}

			if( !str_cmp( argument, "delete" ) )
			{
				UNLINK( comp, part->first_component, part->last_component, next, prev );
				DISPOSE( comp );
				send_to_char( "Component deleted." NL, ch );
				return;
			}

			argument = one_argument( argument, arg5 );

			if( !str_cmp( arg5, "wear" ) )
			{
				int val;
				if( (val=flag_value( wear_flags_list, argument )) == NO_FLAG )
				{
					send_to_char( "Invalid wear flag." NL, ch );
					return;
				}
				comp->type = COMP_WEAR;
				comp->nr = val;
				send_to_char( "Object wear flag set." NL, ch );
				return;
			}

			if( !str_cmp( arg5, "type" ) )
			{
				int val;
				if( (val=flag_value( obj_types_list, argument )) == NO_FLAG )
				{
					send_to_char( "Invalid item type." NL, ch );
					return;
				}
				comp->type = COMP_TYPE;
				comp->nr = val;
				send_to_char( "Object item type set." NL, ch );
				return;
			}

			if( !str_cmp( arg5, "vnum" ) )
			{
				int val = atoi( argument );
				if( !get_obj_index( val ) )
				{
					send_to_char( "No object with that vnum." NL, ch );
					return;
				}
				comp->type = COMP_VNUM;
				comp->nr = val;
				send_to_char( "Object vnum set." NL, ch );
				return;
			}
		}

		proedit( ch->desc, (char *)"part" );
		return;
	}

	interpret( ch, arg );
	return;
}


DEF_DO_FUN( proedit )
{
	PROJECT_DATA *	pro;
	char 		arg	[MIL];

	if ( argument[0] == '\0' )
	{
		send_to_char( "Syntax: proedit <vnum> | create <vnum> | fold | list" NL, ch );
		return;
	}

	argument = one_argument( argument, arg );

	if ( !str_prefix( arg, "fold" ) )
	{
		proedit( ch->desc, (char *)"fold" );
		return;
	}

	if ( !str_cmp( arg, "create" ) )
	{
		proedit_create( ch, argument );
		return;
	}

	if ( !str_cmp( arg, "list" ) )
	{
		pager_printf( ch, FB_WHITE
				"[ vnum] Name                                [Montage Type]  Parts" EOL );

		for( pro = first_project; pro; pro = pro->next )
		{
			PART_DATA *	part;
			int 	i;

			if( !pro->effect )
				continue;

			i=0;
			for( part = pro->first_part; part; part = part->next )
				i++;

			pager_printf( ch, "[%5d] %-35s [%12s]    %d" NL,
						  pro->effect->vnum, pro->effect->name,
						  flag_string( mont_types_list, pro->montage_type ), i );
		}
		return;
	}

	if ( (pro = get_project( arg )) != NULL )
	{
		ch->desc->olc_editing = ( void * ) pro;
		ch->desc->connected = CON_PROEDITOR;
		proedit( ch->desc, (char *)"show" );
		return;
	}
	else
	{
		send_to_char( "Project not found. Use CREATE." NL, ch );
		return;
	}
}


/** Trog: LEdit */
LANG_DATA *find_lang( const char *name )
{
	LANG_DATA	*lang;

	if( !str_cmp( name, lang_base->name ) )
		return lang_base;

	FOREACH( lang, first_lang )
		if( !str_cmp( name, lang->name ) )
			return lang;

	return NULL;
}

KNOWN_LANG *find_klang( CHAR_DATA *ch, LANG_DATA *lang )
{
	KNOWN_LANG	*klang;

	FOREACH( klang, ch->first_klang )
		if( lang == klang->language )
			return klang;

	return NULL;
}

/** Trog: koniec LEdit */

/** Trog: RaEdit */
/** trzeba dopisac taka samo f-cje, ale zeby zamiast NULLa
 * zwracala base_rece */
RACE_DATA *find_race( char *name )
{
	RACE_DATA	*race;

	if( !str_cmp( name, base_race->name ) )
		return base_race;

	FOREACH( race, first_race )
			if( !str_cmp( name, race->name ) )
			return race;

	return NULL;
}

bool raedit_create( CHAR_DATA *ch, char *argument )
{
	RACE_DATA	*pRace;

	if( !*argument )
	{
		send_to_char( "Syntax: create <race_name>" NL, ch );
		return false;
	}

	if( find_race( argument ) )
	{
		send_to_char( "That race already exists." NL, ch );
		return false;
	}

	pRace = new_race();
	STRDUP( pRace->name, argument );
	LINK( pRace, first_race, last_race, next, prev );
	pRace->language = lang_base;
	ch->desc->olc_editing = (void *)pRace;
	send_to_char( "Race created." NL, ch );
	return true;
}

void raedit_show( CHAR_DATA *ch, char *argument )
{
	RACE_DATA	*pRace;

	if( *argument != '\0' )
	{
		if( !(pRace = find_race( argument )) )
		{
			send_to_char( "No such race." NL, ch );
			return;
		}
	}
	else
		if( !(pRace = (RACE_DATA *)ch->desc->olc_editing) )
	{
		bug( "raedit_show: null pRace." );
		edit_done( ch, (char *)"" );
		return;
	}

	pager_printf( ch, FG_CYAN "Name: " PLAIN "%s" FG_CYAN " Filename: " PLAIN "%s" FG_CYAN "." EOL
				FG_CYAN "Inflect0: " PLAIN "%s" FG_CYAN " Inflect1: " PLAIN "%s" FG_CYAN " Inflect2: " PLAIN "%s" EOL
				FG_CYAN "Inflect3: " PLAIN "%s" FG_CYAN " Inflect4: " PLAIN "%s" FG_CYAN " Inflect5: " PLAIN "%s" EOL
				FG_CYAN "InflectFemale0: " PLAIN "%s" FG_CYAN " InflectFemale1: " PLAIN "%s" EOL
				FG_CYAN "InflectFemale2: " PLAIN "%s" FG_CYAN "InflectFemale3: " PLAIN "%s" EOL
				FG_CYAN "InflectFemale4: " PLAIN "%s" FG_CYAN " InflectFemale5: " PLAIN "%s" EOL
				FG_CYAN "Flags: " PLAIN "[%s]" NL,
			pRace->name, pRace->filename, pRace->przypadki[0],
			pRace->przypadki[1], pRace->przypadki[2], pRace->przypadki[3], pRace->przypadki[4], pRace->przypadki[5],
			pRace->inflectsFemale[0], pRace->inflectsFemale[1], pRace->inflectsFemale[2], pRace->inflectsFemale[3],
			pRace->inflectsFemale[4], pRace->inflectsFemale[5], flag_string( race_flags_list, pRace->flags ) );
	pager_printf( ch, FG_CYAN "Affected: " PLAIN "[%s]" NL
			FG_CYAN "str_plus: " PLAIN "%3d" FG_CYAN " dex_plus: " PLAIN "%3d" FG_CYAN " con_plus: " PLAIN "%3d" FG_CYAN " lck_plus: " PLAIN "%3d" NL
					FG_CYAN "int_plus: " PLAIN "%3d" FG_CYAN " wis_plus: " PLAIN "%3d" FG_CYAN " cha_plus: " PLAIN "%3d" FG_CYAN " frc_plus: " PLAIN "%3d" NL
					FG_CYAN "hp_plus: " PLAIN "%3d" FG_CYAN " force_plus: " PLAIN "%3d" NL,
			flag_string( aff_flags_list, pRace->affected ), pRace->str_plus, pRace->dex_plus, pRace->con_plus, pRace->lck_plus,
			pRace->int_plus, pRace->wis_plus, pRace->cha_plus, pRace->frc_plus, pRace->hp_plus, pRace->force_plus );
	pager_printf( ch, FG_CYAN "Susceptible: " PLAIN "[%s]", flag_string( ris_flags_list, pRace->susceptible ) );
	pager_printf( ch, FG_CYAN " Resistant: " PLAIN "[%s]", flag_string( ris_flags_list, pRace->resistant ) );
	pager_printf( ch, FG_CYAN " Immune: " PLAIN "[%s]" NL
			FG_CYAN "Language: " PLAIN "[%s]" NL
					FG_CYAN "Description: " EOL "%s" EOL,
			flag_string( ris_flags_list, pRace->immune ), pRace->language->name, pRace->description );
}

void raedit( DESCRIPTOR_DATA *d, char *argument )
{
	CHAR_DATA	*ch	= d->character;
	RACE_DATA	*pRace;
	char		arg[MIL];
	char		arg1[MIL];
	int64		value;

	strcpy( arg, argument );
	argument = one_argument( argument, arg1 );

	if( !str_cmp( arg1, "list" ) )
	{
		int	i = 2;

		pager_printf( ch, FG_CYAN "[" MOD_BOLD "Nr " FG_CYAN "][" MOD_BOLD "Name              " FG_CYAN "]["
				MOD_BOLD "Inflect0          " FG_CYAN "][" MOD_BOLD "Language       " FG_CYAN "][" MOD_BOLD "Filename       " FG_CYAN "]" EOL );
		pager_printf( ch, FG_CYAN "[  " PLAIN "1" FG_CYAN "][" FB_YELLOW "%-18s" FG_CYAN "][" PLAIN "%-18s" FG_CYAN "][" PLAIN "%-15s" FG_CYAN "]" EOL,
					  base_race->name, base_race->przypadki[0], base_race->language->name );
		FOREACH( pRace, first_race )
				pager_printf( ch, FG_CYAN "[" PLAIN "%3d" FG_CYAN "][" PLAIN "%-18s" FG_CYAN "][" PLAIN "%-18s" FG_CYAN "][" PLAIN "%-15s" FG_CYAN "]"
						FG_CYAN "[" PLAIN "%-15s" FG_CYAN "]" EOL,
						i++, pRace->name, pRace->przypadki[0], pRace->language->name, strip_colors( pRace->filename, 15 ) );
		pager_printf( ch, NL );

		return;
	}

	if( !(pRace = (RACE_DATA *)ch->desc->olc_editing) )
	{
		bug( "raedit_show: null pRace." );
		edit_done( ch, (char *)"" );
		return;
	}

	if( !str_prefix( arg1, "show" ) )
	{
		raedit_show( ch, argument );
		return;
	}

	GET_DONE();
	GET_HELP( argument );
	GET_CREATE( raedit_create( ch, argument ) );

	if( !str_cmp( arg1, "foldlist" ) )
	{
		save_races_list();
		send_to_char( "Races list saved." NL, ch );
		return;
	}

	if( pRace == base_race )
	{
		send_to_char( "This race cannot be modified." NL, ch );
//		interpret( ch, arg );
		return;
	}

	GET_SAVE( save_race( pRace ) );

	SET_STR( "name", pRace->name, argument );
	SET_FILENAME( "filename", pRace->filename, argument, "rac" );
	SET_STR_DISP( "inf0", pRace->przypadki[0], argument, "Inflect0" );
	SET_STR_DISP( "inf1", pRace->przypadki[1], argument, "Inflect1" );
	SET_STR_DISP( "inf2", pRace->przypadki[2], argument, "Inflect2" );
	SET_STR_DISP( "inf3", pRace->przypadki[3], argument, "Inflect3" );
	SET_STR_DISP( "inf4", pRace->przypadki[4], argument, "Inflect4" );
	SET_STR_DISP( "inf5", pRace->przypadki[5], argument, "Inflect5" );
	SET_STR_DISP( "infFemale0", pRace->inflectsFemale[0], argument, "InflectFemale0" );
	SET_STR_DISP( "infFemale1", pRace->inflectsFemale[1], argument, "InflectFemale1" );
	SET_STR_DISP( "infFemale2", pRace->inflectsFemale[2], argument, "InflectFemale2" );
	SET_STR_DISP( "infFemale3", pRace->inflectsFemale[3], argument, "InflectFemale3" );
	SET_STR_DISP( "infFemale4", pRace->inflectsFemale[4], argument, "InflectFemale4" );
	SET_STR_DISP( "infFemale5", pRace->inflectsFemale[5], argument, "InflectFemale5" );
	SET_STR_EDIT( "description", pRace->description, argument );
	PSET_INT_LIMIT( "str_plus", pRace->str_plus, atoi( argument ), -10, 10 );
	PSET_INT_LIMIT( "dex_plus", pRace->dex_plus, atoi( argument ), -10, 10 );
	PSET_INT_LIMIT( "con_plus", pRace->con_plus, atoi( argument ), -10, 10 );
	PSET_INT_LIMIT( "lck_plus", pRace->lck_plus, atoi( argument ), -10, 10 );
	PSET_INT_LIMIT( "int_plus", pRace->int_plus, atoi( argument ), -10, 10 );
	PSET_INT_LIMIT( "wis_plus", pRace->wis_plus, atoi( argument ), -10, 10 );
	PSET_INT_LIMIT( "cha_plus", pRace->cha_plus, atoi( argument ), -10, 10 );
	PSET_INT_LIMIT( "frc_plus", pRace->frc_plus, atoi( argument ), -10, 10 );
	PSET_INT_LIMIT( "hp_plus", pRace->hp_plus, atoi( argument ), -100, 100 );
	PSET_INT_LIMIT( "force_plus", pRace->force_plus, atoi( argument ), -100, 100 );

	if( !str_cmp( arg1, "inflects" ) )
	{
		int	i;

		if( !*argument )
			send_to_char( "Syntax: inflects <string>" NL, ch );
		else
		{
			for( i = 0; i < 6; i++ )
				STRDUP( pRace->przypadki[i], argument );
			send_to_char( "Inflects set." NL, ch );
		}

		return;
	}

	if( !str_cmp( arg1, "inflectsFemale" ) )
	{
		int	i;

		if( !*argument )
			send_to_char( "Syntax: inflectsFemale <string>" NL, ch );
		else
		{
			for( i = 0; i < 6; i++ )
				STRDUP( pRace->inflectsFemale[i], argument );
			send_to_char( "InflectsFemale set." NL, ch );
		}

		return;
	}

	if( !str_prefix( arg1, "affected" ) )
	{
		if( (value = flag_value( aff_flags_list, argument )) != NO_FLAG )
		{
			TOGGLE_BIT( pRace->affected, value );
			send_to_char( "Affect flag toggled." NL, ch );
		}
		else
			send_to_char( "No such affect flag." NL, ch );

		return;
	}

	if( !str_prefix( arg1, "susceptible" ) )
	{
		if( (value = flag_value( ris_flags_list, argument )) == NO_FLAG )
			send_to_char( "No such susceptible flag." NL, ch );
		else
		{
			TOGGLE_BIT( pRace->susceptible, value );
			send_to_char( "Susceptible flag toggled." NL, ch );
		}

		return;
	}

	if( !str_prefix( arg1, "resistant" ) )
	{
		if( (value = flag_value( ris_flags_list, argument )) == NO_FLAG )
			send_to_char( "No such resist flag." NL, ch );
		else
		{
			TOGGLE_BIT( pRace->resistant, value );
			send_to_char( "Resist flag toggled." NL, ch );
		}

		return;
	}

	if( !str_prefix( arg1, "immune" ) )
	{
		if( (value = flag_value( ris_flags_list, argument )) == NO_FLAG )
			send_to_char( "No such immune flag." NL, ch );
		else
		{
			TOGGLE_BIT( pRace->immune, value );
			send_to_char( "Immune flag toggled." NL, ch );
		}

		return;
	}

	if( !str_prefix( arg1, "language" ) )
	{
		LANG_DATA	*lang;


		if( !(lang = find_lang( argument )) )
		{
			ch_printf( ch, "No such language: %s." NL, argument );
			return;

		}
		pRace->language = lang;
		send_to_char( "Language set." NL, ch );
		return;
	}

	if( (value = flag_value( race_flags_list, arg1 )) != NO_FLAG )
	{
		TOGGLE_BIT( pRace->flags, value );
		send_to_char( "Race flag toggled." NL, ch );
		return;
	}

	interpret( ch, arg );
}

DEF_DO_FUN( raedit )
{
	RACE_DATA	*pRace;
	char		arg[MIL];
	char		arg1[MIL];

	if( !*argument )
	{
		send_to_char( "Syntax: raedit <race_name> | create <race_name> | list" NL, ch );
		return;
	}

	strcpy( arg, argument );
	argument = one_argument( argument, arg1 );

	if( !str_cmp( arg1, "create" ) )
	{
		if( raedit_create( ch, argument ) )
		{
			ch->desc->connected = CON_RAEDITOR;
			raedit( ch->desc, (char *)"show" );
		}
	}
	else
		if( !str_cmp( arg1, "list" ) )
			raedit( ch->desc, (char *)"list" );
	else
		if( (pRace = find_race( arg )) )
	{
		ch->desc->olc_editing = (void *)pRace;
		ch->desc->connected = CON_RAEDITOR;
		raedit( ch->desc, (char *)"show" );
	}
	else
		send_to_char( "No such race." NL, ch );
}

void save_race( RACE_DATA *pRace )
{
	FILE	*fpout;
	char	filename[MFL];
	int		i;

	if( !pRace )
	{
		bug( "save_race: null pRace" );
		return;
	}

	/* Trog: rasa bazowa to stala - nie zapisujemy jej */
	if( pRace == base_race )
		return;

	if( !*pRace->filename )
	{
		bug( "save_race: race: %s does not have filename specified", pRace->name );
		return;
	}

	sprintf( filename, "%s%s", RACE_DIR, pRace->filename );
	RESERVE_CLOSE;
	if( !(fpout = fopen(filename, "w")) )
	{
		bug( "fopen" );
		perror( filename );
		RESERVE_OPEN;
		return;
	}

	fprintf( fpout, "#SWRACE\n\n" );
	fprintf( fpout, "Name        %s~\n", pRace->name );
	fprintf( fpout, "Inflects   " );
	for( i = 0; i < 6; i++ )
		fprintf( fpout, " %s~", pRace->przypadki[i] );
	fprintf( fpout, "InflectsF  " );
	for( i = 0; i < 6; i++ )
		fprintf( fpout, " %s~", pRace->inflectsFemale[i] );
	fprintf( fpout, "\nFlags       %lld\n", pRace->flags );
	fprintf( fpout, "Affected    %lld\n", pRace->affected );
	fprintf( fpout, "Str_plus    %d\n", pRace->str_plus );
	fprintf( fpout, "Dex_plus    %d\n", pRace->dex_plus );
	fprintf( fpout, "Wis_plus    %d\n", pRace->wis_plus );
	fprintf( fpout, "Int_plus    %d\n", pRace->int_plus );
	fprintf( fpout, "Con_plus    %d\n", pRace->con_plus );
	fprintf( fpout, "Cha_plus    %d\n", pRace->cha_plus );
	fprintf( fpout, "Lck_plus    %d\n", pRace->lck_plus );
	fprintf( fpout, "Frc_plus    %d\n", pRace->frc_plus );
	fprintf( fpout, "Hp_plus     %d\n", pRace->hp_plus );
	fprintf( fpout, "Force_plus  %d\n", pRace->force_plus );
	fprintf( fpout, "Susceptible %lld\n", pRace->susceptible );
	fprintf( fpout, "Resistant   %lld\n", pRace->resistant );
	fprintf( fpout, "Immune      %lld\n", pRace->immune );
	fprintf( fpout, "Language    %s~\n", pRace->language->name );
	fprintf( fpout, "Description %s~\n", pRace->description );
	fprintf( fpout, "Restrictions" );
	for( i = 0; i < MAX_DESC_TYPES; i++ )
		fprintf( fpout, " %lld", pRace->desc_restrictions[i] );
	fprintf( fpout, "\n\nEnd\n\n" );

	fclose( fpout );
	RESERVE_OPEN;
}

void save_races_list2()
{
	ILD	*first_ild = NULL;
	ILD	*last_ild = NULL;

	ILD_CREATE( RACE_DATA, race, filename, first_ild, last_ild );
	save_list( RACE_LISTXML, first_ild );
	ILD_FREE( first_ild );
}

void save_races_list()
{
	RACE_DATA	*pRace;
	FILE		*fpout;

	RESERVE_CLOSE;
	if( !(fpout = fopen(RACE_LIST, "w")) )
	{
		bug( "fopen" );
		perror( RACE_LIST );
		RESERVE_OPEN;
		return;
	}

	FOREACH( pRace, first_race )
			if( !*pRace->filename )
			bug( "save_races_list: race: %s does not have filename specified", pRace->name );
	else
		fprintf( fpout, "%s\n", pRace->filename );
	fprintf( fpout, "$\n\n" );

	fclose( fpout );
	RESERVE_OPEN;
}
/* Trog: koniec RaEdit */

/* Trog: TCEdit */
TURBOCAR *find_turbocar( char *name )
{
	TURBOCAR	*turbocar;

	FOREACH( turbocar, first_turbocar )
		if( !str_cmp( name, turbocar->name ) )
			return turbocar;

	return NULL;
}

bool tcedit_create( CHAR_DATA *ch, char *argument )
{
	TURBOCAR	*pTurbocar;

	if( !*argument )
	{
		send_to_char( "Syntax: create <turbocar_name>" NL, ch );
		return false;
	}

	if( find_turbocar( argument ) )
	{
		send_to_char( "That turbocar already exists." NL, ch );
		return false;
	}

	pTurbocar = new_turbocar();
	STRDUP( pTurbocar->name, argument );
	LINK( pTurbocar, first_turbocar, last_turbocar, next, prev );
	ch->desc->olc_editing = (void *)pTurbocar;
	send_to_char( "Turbocar created." NL, ch );
	return true;
}

void tcedit_show( CHAR_DATA *ch, char *argument )
{
	TURBOCAR	*pTurbocar;
	TC_STATION	*station;
	int			i = 0;

	if( *argument != '\0' )
	{
		if( !(pTurbocar = find_turbocar( argument )) )
		{
			send_to_char( "No such turbocar." NL, ch );
			return;
		}
	}
	else
	if( !(pTurbocar = (TURBOCAR *)ch->desc->olc_editing) )
	{
		bug( "tcedit_show: null pTurbocar." );
		edit_done( ch, (char *)"" );
		return;
	}

	pager_printf( ch, FG_CYAN "Name: " PLAIN "%s " FG_CYAN "Vnum: " PLAIN "%2d "
		FG_CYAN " Filename: " PLAIN "%s" FG_CYAN "." EOL,
		pTurbocar->name, pTurbocar->vnum, pTurbocar->filename );

	FOREACH( station, pTurbocar->first_station )
	{
		pager_printf( ch, FG_CYAN "Station: " PLAIN "%2d " FG_CYAN "Vnum: " PLAIN "%-5d "
			FG_CYAN "Name: " PLAIN "%s" FG_CYAN "." EOL, ++i, station->vnum, station->name );
	}

	pager_printf( ch, NL );
}

void tcedit( DESCRIPTOR_DATA *d, char *argument )
{
	CHAR_DATA	*ch	= d->character;
	TURBOCAR	*pTurbocar;
	TC_STATION	*station;
	TC_STATION	*station_pn;
	char		arg[MIL];
	char		arg1[MIL];
	char		arg2[MIL];

	strcpy( arg, argument );
	argument = one_argument( argument, arg1 );

	if( !str_cmp( arg1, "list" ) )
	{
		int	i = 0;

		pager_printf( ch, FG_CYAN "[" MOD_BOLD "Nr  " FG_CYAN "][" MOD_BOLD "Name              " FG_CYAN "]["
				MOD_BOLD "Vnum " FG_CYAN "][" MOD_BOLD "Filename       " FG_CYAN "]" EOL );
		FOREACH( pTurbocar, first_turbocar )
			pager_printf( ch, FG_CYAN "[" PLAIN "%4d" FG_CYAN "][" PLAIN "%-18s" FG_CYAN "][" PLAIN "%-5d" FG_CYAN "][" PLAIN "%-15s" FG_CYAN "]" EOL,
				++i, pTurbocar->name, pTurbocar->vnum, strip_colors( pTurbocar->filename, 15 ) );
		pager_printf( ch, NL );

		return;
	}

	if( !(pTurbocar = (TURBOCAR *)ch->desc->olc_editing) )
	{
		bug( "raedit_show: null pTurbocar." );
		edit_done( ch, (char *)"" );
		return;
	}

	if( !str_prefix( arg1, "show" ) )
	{
		tcedit_show( ch, argument );
		return;
	}

	GET_DONE();
	GET_HELP( argument );
	GET_CREATE( tcedit_create( ch, argument ) );

	if( !str_cmp( arg1, "foldlist" ) )
	{
		save_turbocars_list();
		send_to_char( "Turbocars list saved." NL, ch );
		return;
	}

	GET_SAVE( save_turbocar( pTurbocar ) );

	SET_STR( "name", pTurbocar->name, argument );
	SET_FILENAME( "filename", pTurbocar->filename, argument, "tur" );
	SET_INT( "vnum", pTurbocar->vnum, atoi( argument ) );

	if( !str_cmp( arg1, "addstation" ) )
	{
		argument = one_argument( argument, arg2 );
		if( !*arg2 )
			send_to_char( "Syntax: addstation <vnum> <name>" NL, ch );
		else
		{
			station = new_station();
			LINK( station, pTurbocar->first_station, pTurbocar->last_station,
				next, prev );
			station->vnum = atoi( arg2 );
			STRDUP( station->name, argument );
			send_to_char( "Station added." NL, ch );
		}

		return;
	}

	if( !str_prefix( arg1, "delstation" ) )
	{
		if( !*argument )
			send_to_char( "Syntax: delstation <vnum>" NL, ch );
		else
		{
			int	vnum = atoi( argument );

			FOREACH( station, pTurbocar->first_station )
				if( station->vnum == vnum )
				{
					if( station == pTurbocar->current_station )	/* moze sie zdarzyc, wiec... */
					{
						if( pTurbocar->current_station == pTurbocar->last_station )
							pTurbocar->current_station = pTurbocar->first_station;
						else
							pTurbocar->current_station = station->next;
					}
					UNLINK( station, pTurbocar->first_station, pTurbocar->last_station,
						next, prev );
					free_station( station );
					send_to_char( "Station deleted." NL, ch );
					return;
				}

			ch_printf( ch, "Station (vnum: %d) not found." NL, vnum );
		}

		return;
	}

	if( !str_prefix( arg1, "up" ) )
	{
		if( !*argument )
			send_to_char( "Syntax: up <vnum>" NL, ch );
		else
		{
			int	vnum = atoi( argument );

			FOREACH( station, pTurbocar->first_station )
				if( station->vnum == vnum )
				{
					if( !station->prev )
					{
						send_to_char( "This station is already on top." NL, ch );
						return;
					}

					station_pn = station->prev;
					UNLINK( station, pTurbocar->first_station, pTurbocar->last_station,
						next, prev );
					INSERT( station, station_pn, pTurbocar->first_station, next, prev );
					send_to_char( "Station moved up." NL, ch );

					return;
				}

			ch_printf( ch, "Station (vnum: %d) not found." NL, vnum );
		}

		return;
	}

	if( !str_prefix( arg1, "down" ) )
	{
		if( !*argument )
			send_to_char( "Syntax: down <vnum>" NL, ch );
		else
		{
			int	vnum = atoi( argument );

			FOREACH( station, pTurbocar->first_station )
				if( station->vnum == vnum )
				{
					if( !station->next )
					{
						send_to_char( "This station is already at bottom." NL, ch );
						return;
					}

					station_pn = station->next;
					UNLINK( station, pTurbocar->first_station, pTurbocar->last_station,
						next, prev );
					if( station_pn == pTurbocar->last_station )
						LINK( station, pTurbocar->first_station, pTurbocar->last_station,
							next, prev );
					else
					{
						station_pn = station_pn->next;
						INSERT( station, station_pn, pTurbocar->first_station, next, prev );
					}

					send_to_char( "Station moved down." NL, ch );
					return;
				}

			ch_printf( ch, "Station (vnum: %d) not found." NL, vnum );
		}

		return;
	}

	if( !str_cmp( arg1, "namestation" ) )
	{
		argument = one_argument( argument, arg2 );
		if( !*arg2 )
			send_to_char( "Syntax: namestation <vnum> <new_name>" NL, ch );
		else
		{
			int	vnum = atoi( arg2 );

			FOREACH( station, pTurbocar->first_station )
				if( station->vnum == vnum )
				{
					STRDUP( station->name, argument );
					send_to_char( "Station renamed." NL, ch );
					return;
				}

			ch_printf( ch, "Station (vnum: %d) not found." NL, vnum );
		}

		return;
	}

	if( !str_cmp( arg1, "help" ) )
	{
		ch_printf( ch, "Commands:" NL " name vnum list show create fold foldlist" NL NL
			"Station commands:" NL " addstation delstation namestation up down" NL NL );
		return;
	}

	interpret( ch, arg );
}

DEF_DO_FUN( tcedit )
{
	TURBOCAR	*pTurbocar;
	char		arg[MIL];
	char		arg1[MIL];

	if( !*argument )
	{
		send_to_char( "Syntax: tcedit <turbocar_name> | create <turbocar_name> | list" NL, ch );
		return;
	}

	strcpy( arg, argument );
	argument = one_argument( argument, arg1 );

	if( !str_cmp( arg1, "create" ) )
	{
		if( tcedit_create( ch, argument ) )
		{
			ch->desc->connected = CON_TCEDITOR;
			tcedit( ch->desc, (char *)"show" );
		}
	}
	else
	if( !str_cmp( arg1, "list" ) )
		tcedit( ch->desc, (char *)"list" );
	else
	if( (pTurbocar = find_turbocar( arg )) )
	{
		ch->desc->olc_editing = (void *)pTurbocar;
		ch->desc->connected = CON_TCEDITOR;
		tcedit( ch->desc, (char *)"show" );
	}
	else
		send_to_char( "No such turbocar." NL, ch );
}

void save_turbocar( TURBOCAR *pTurbocar )
{
	FILE		*fpout;
	char		filename[MFL];
	TC_STATION	*station;

	if( !pTurbocar )
	{
		bug( "save_turbocar: null pTurbocar" );
		return;
	}

	if( !*pTurbocar->filename )
	{
		bug( "save_turbocar: turbocar: %s does not have filename specified", pTurbocar->name );
		return;
	}

	sprintf( filename, "%s%s", TURBOCAR_DIR, pTurbocar->filename );
	RESERVE_CLOSE;
	if( !(fpout = fopen(filename, "w")) )
	{
		bug( "fopen" );
		perror( filename );
		RESERVE_OPEN;
		return;
	}

	fprintf( fpout, "#SWTURBOCAR\n\n" );
	fprintf( fpout, "Name        %s~\n", pTurbocar->name );
	fprintf( fpout, "Vnum        %d\n", pTurbocar->vnum );
	FOREACH( station, pTurbocar->first_station )
		fprintf( fpout, "Station     %d %s~\n", station->vnum, station->name );
	fprintf( fpout, "\nEnd\n\n" );

	fclose( fpout );
	RESERVE_OPEN;
}

void save_turbocars_list()
{
	TURBOCAR	*pTurbocar;
	FILE		*fpout;

	RESERVE_CLOSE;
	if( !(fpout = fopen(TURBOCAR_LIST, "w")) )
	{
		bug( "fopen" );
		perror( TURBOCAR_LIST );
		RESERVE_OPEN;
		return;
	}

	FOREACH( pTurbocar, first_turbocar )
			if( !*pTurbocar->filename )
			bug( "save_turbocars_list: turbocar: %s does not have filename specified", pTurbocar->name );
	else
		fprintf( fpout, "%s\n", pTurbocar->filename );
	fprintf( fpout, "$\n\n" );

	fclose( fpout );
	RESERVE_OPEN;
}
/* Trog: koniec TCEdit */


