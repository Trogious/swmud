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
*			 Shop and repair shop module			   *
****************************************************************************/

#include <sys/types.h>
#include <time.h>
#include "mud.h"


/*
 * Local functions
 */

#define	CD	CHAR_DATA
CD *	find_fixer	args( ( CHAR_DATA *ch ) );
int	get_cost	args( ( CHAR_DATA *ch, CHAR_DATA *keeper,
				OBJ_DATA *obj, bool fBuy ) );
int 	get_repaircost  args( ( CHAR_DATA *keeper, OBJ_DATA *obj ) );
#undef CD

/*
 * Shopping commands.
 */
CHAR_DATA *find_keeper( CHAR_DATA *ch, const char *argument )
{
    CHAR_DATA *keeper;
    SHOP_DATA *pShop;
    char       buf [ MAX_STRING_LENGTH ];

    pShop = NULL;
    for ( keeper = ch->in_room->first_person; keeper; keeper = keeper->next_in_room )
    {
	if ( IS_NPC( keeper ) && ( pShop = keeper->pIndexData->pShop )
	    && ( argument[0] == '\0'
		|| is_name( argument, keeper->name )
		|| is_name_prefix( argument, keeper->name ) ) )
	    	    break;

    }


    if ( !keeper || !pShop || ( keeper && IS_AFFECTED( keeper, AFF_CHARM ) ) )
    {
	if( argument[0] == '\0')
	    sprintf(buf, "Nie ma tu nikogo, kto móg³by z tob± handlowaæ." NL );
	else
	    sprintf(buf, "Nie ma tu takiego %s" PLAIN ", kto móg³by z tob± handlowaæ." NL,
		argument );

	send_to_char( buf, ch );
	return NULL;
    }

    if ( !knows_language( keeper, ch->speaking, ch ) )
    {
	ch_printf( ch, "Wygl±da na to, ¿e %s" PLAIN " nie mo¿e ciê zrozumieæ.", keeper->przypadki[0] );
	return NULL;
    }

    /*
     * Undesirables.
     */
    if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_KILLER ) )
    {

	sprintf( buf, "Nie obs³ugujê morderców %s" COL_TELL " !!!",
	    ch->przypadki[0] );
	ch_tell( keeper, ch, buf );

	sprintf( buf, "%s" COL_SHOUT " MORDERC%s jest tutaj!",
	    ch->przypadki[0],
	    ch->sex == SEX_FEMALE ? "ZYNI" : "A" );
	do_shout( keeper, buf );

	return NULL;
    }
/*
    if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_THIEF ) )
    {
	sprintf( buf, "Nie obs³ugujê z³odzieji %s !!!",
	    ch->przypadki[0] );
	ch_tell( keeper, ch, buf );

	sprintf( buf, "%s Z£ODZIEJ%s jest tutaj!",
	    ch->przypadki[0],
	    ch->sex == SEX_FEMALE ? "KA" : "" );
	do_shout( keeper, buf );

	return NULL;
    }
*/
    /*
     * Shop hours.
     */
    if ( (GET_PLANET( keeper ))->hour < pShop->open_hour )
    {
	do_say( keeper, (char *)"Przykro mi ale jeszcze zamkniête, przyjd¼ pó¼niej." );
	return NULL;
    }

    if ( (GET_PLANET( keeper ))->hour > pShop->close_hour )
    {
	do_say( keeper, (char *)"Przykro mi ju¿ zamkniête, przyjd¼ jutro." );
	return NULL;
    }

    /*
     * Invisible or hidden people.
     */
    if ( !can_see( keeper, ch ) )
    {
	do_say( keeper, (char *)"Nie handlujê z kim¶, kogo nie widzê." );
	return NULL;
    }

    return keeper;

}
/*
 * repair commands.
 */
CHAR_DATA *find_fixer( CHAR_DATA *ch )
{
    CHAR_DATA *keeper;
    REPAIR_DATA *rShop;

    rShop = NULL;
    for ( keeper = ch->in_room->first_person;
	  keeper;
	  keeper = keeper->next_in_room )
	if ( IS_NPC(keeper) && (rShop = keeper->pIndexData->rShop) != NULL )
	    break;

    if ( !rShop )
    {
	send_to_char( "Nie mo¿esz zrobiæ tego tutaj." NL, ch );
	return NULL;
    }


    /*
     * Shop hours.
     */
    if ( (GET_PLANET( keeper ))->hour < rShop->open_hour )
    {
	do_say( keeper, (char *)"Jeszcze zamkniête! Przyjd¼ pó¼niej." );
	return NULL;
    }

    if ( (GET_PLANET( keeper ))->hour > rShop->close_hour )
    {
	do_say( keeper, (char *)"Ju¿ zamkniête! Przyjd¼ jutro." );
	return NULL;
    }


    if ( !knows_language( keeper, ch->speaking, ch ) )
    {
	ch_printf( ch, "Wygl±da na to, ¿e %s" PLAIN " nie mo¿e ciê zrozumieæ.", keeper->przypadki[0] );
	return NULL;
    }

    return keeper;
}



int get_cost( CHAR_DATA *ch, CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy )
{
    SHOP_DATA *pShop;
    int cost;
    bool richcustomer;
    int profitmod;

    if ( !obj || ( pShop = keeper->pIndexData->pShop ) == NULL )
	return 0;

    if ( ch->gold > (ch->top_level * ch->top_level * 1000) )
	richcustomer = true;
    else
	richcustomer = false;

    if ( fBuy )
    {
		if( IS_SET( pShop->flags, SHOP_RAWCOST ) )
			cost = obj->cost * pShop->profit_buy / 100;
		else
		{
			profitmod = 13 - get_curr_cha(ch) + (richcustomer ? 15 : 0)	+ ((URANGE(5,ch->top_level,LEVEL_AVATAR)-20)/2);
			cost = (obj->cost * UMAX( (pShop->profit_sell+1), pShop->profit_buy+profitmod ) ) / 100;
		}
    }
    else
    {
		if( IS_SET( pShop->flags, SHOP_RAWCOST ) )
			cost = obj->cost * pShop->profit_sell / 100;
		else
		{
			OBJ_DATA *obj2;
			int itype;

			profitmod = get_curr_cha(ch) - 13 - (richcustomer ? 15 : 0);
			cost = 0;
			for ( itype = 0; itype < MAX_TRADE; itype++ )
			{
			    if ( obj->item_type == pShop->buy_type[itype] )
			    {
					cost = (int) (obj->cost
					* UMIN( (pShop->profit_buy-1),
					pShop->profit_sell+profitmod) ) / 100;
					break;
				}
			}
			for ( obj2 = keeper->first_carrying; obj2; obj2 = obj2->next_content )
			{
				if ( obj->pIndexData == obj2->pIndexData )
				{
					cost /= (obj2->count+1);
					break;
				}
			}

			cost = UMIN( cost , 2500 );
		}
	}

	if ( obj->item_type == ITEM_DEVICE )
		cost = (cost * obj->value[2] / obj->value[1]);

    return cost;
}

int get_repaircost( CHAR_DATA *keeper, OBJ_DATA *obj )
{
    REPAIR_DATA *rShop;
    int cost;
    int itype;
    bool found;

    if ( !obj || ( rShop = keeper->pIndexData->rShop ) == NULL )
	return 0;

    cost = 0;
    found = false;
    for ( itype = 0; itype < MAX_FIX; itype++ )
    {
	if ( obj->item_type == rShop->fix_type[itype] )
	{
	    cost = (int) (obj->cost * rShop->profit_fix / 1000);
	    found = true;
	    break;
	}
    }

    if ( !found )
      cost = -1;

    if ( cost == 0 )
      cost = 1;

    if ( found && cost > 0 )
    {
      switch (obj->item_type)
      {
	case ITEM_ARMOR:
	  if (obj->value[0] >= obj->value[1])
	    cost = -2;
	  else
	    cost *= (obj->value[1] - obj->value[0]);
	  break;
	case ITEM_WEAPON:
	  if (INIT_WEAPON_CONDITION == obj->value[0])
	    cost = -2;
	  else
	    cost *= (INIT_WEAPON_CONDITION - obj->value[0]);
	  break;
	case ITEM_DEVICE:
	  if (obj->value[2] >= obj->value[1])
	    cost = -2;
	  else
	    cost *= (obj->value[1] - obj->value[2]);
      }
    }

    return cost;
}


/************************************************************************
 * Nowa wersja handlu by Thanos						*
 *									*
 * wla¶ciwie doda³em tylko drugie 'kup', 'sprzedaj' i 'targuj',		*
 * ale nie zmienia to faktu, ze jest to nowa wersja ;)			*
 ************************************************************************/

bool check_trading( CHAR_DATA* ch, int type )
{

    if( !ch->pcdata->trader )
    {
        bug( "(%d): Cannot find keeper.",type );
        return false;
    }

    if( type == SELL_AN_ITEM || type == BUY_AN_ITEM )
    {
	if( !ch->pcdata->trade_item )
	{
	    bug( "(%d): Cannot find item.",type );
	    return false;
	}
    }
    else if( type == BUY_A_PET )
    {
	if( !ch->pcdata->trade_pet )
	{
	    bug( "(%d): where the heck is pet?",type );
	    return false;
	}
    }

return true;
}

/*
 * funkcja wywo³ana komend± 'targuj'
 */
DEF_DO_FUN( bargain )
{
	CHAR_DATA*		keeper;
	CHAR_DATA*		people;
	OBJ_DATA *		offer;
	OBJ_DATA *  		obj;
	CHAR_DATA * 		pet;
	ROOM_INDEX_DATA *	pRoomIndexNext;
	bool 			is_offered;
	bool 			is_there_a_keeper;
	char 			buf [ MAX_STRING_LENGTH ];
	int			bargain_profit;
	int 			profit;

    if( IS_NPC( ch ) )
    {
        send_to_char( "Przykro mi, ale jeste¶ mobem, nie mo¿esz handlowaæ ;)." NL, ch);
	return;
    }

    if( !ch->pcdata->trade_type )
    {
        send_to_char( "Nie handlujesz z nikim." NL, ch);
	return;
    }

    /* czy gracz znajduje siê w tej samej lokacji co ZYWY sprzedawca? */
    for( people = ch->in_room->first_person; people; people = people->next_in_room )
    {
	if( people == ch->pcdata->trader )
	{
	    is_there_a_keeper = true;
	    break;
	}
	else
	    is_there_a_keeper = false;
    }

    if( !is_there_a_keeper || !can_see( ch, ch->pcdata->trader ) )
    {
        send_to_char( "Przykro mi, ale twój sprzedawca gdzie¶ siê zapodzia³." NL, ch);
	ch->pcdata->trade_type = TRADE_NONE;
    return;
    }


    keeper = ch->pcdata->trader;
    obj = ch->pcdata->trade_item;

    /* sprzedawca zyje i ma siê dobrze ;) */
    switch( ch->pcdata->trade_type )
    {

	default: // na wszelki...
	    send_to_char( "Nie handlujesz z nikim." NL, ch);
	    break;

	case BUY_AN_ITEM: //gracz kupuje przedmiot(y)...

	    if( !check_trading( ch, BUY_AN_ITEM ) )
		return;

	    /* Gdyby towar nagle zosta³ wykupiony */
	    is_offered = false;
	    for( offer = keeper->first_carrying; offer; offer = offer->next_content )
	    {
		if ( offer == obj )
		{
		    is_offered = true;
		    break;
		}
		else
		    is_offered = false;
	    }

	    if ( !is_offered )
	    {
		sprintf( buf,
		    "Przykro mi, ale nie mamy ju¿ %s" COL_TELL ". Przyjd¼ jutro.",
				obj->przypadki[1] );
		ch_tell( keeper, ch, buf );
		ch->pcdata->trade_type = TRADE_NONE;
		return;
	    }

	    if( ch->pcdata->curr_bargain >= ch->pcdata->bargain_max )
	    {
		ch_tell( keeper, ch, "Co ??? Jeszcze taniej? Nie ma mowy! Wynocha!" );
		ch->reply = keeper;
		ch->pcdata->trade_type = TRADE_NONE;
	    return;
	    }

	    if( ch->pcdata->trade_price < MIN_BARGAIN_PRICE )
	    {
		ch_tell( keeper, ch, "Daj spokój... Nie bêdê ³amaæ kredytek na pó³." );
		return;
	    }

	    bargain_profit = number_range( 1, 3 );

	    profit = ch->pcdata->trade_price*bargain_profit/100;

	    if( profit == 0 )
	    {
		ch_tell( keeper, ch, "Nieee, musisz siê bardziej postaraæ..." );
		return;
	    }

	    if( number_percent( ) < MAX_BARGAIN-ch->pcdata->bargain_max)
	    {
		ch->pcdata->trade_price += profit;
	    	sprintf( buf, "Wiesz co? Rozmy¶lam siê. Chcê %d.",
		    ch->pcdata->trade_price );
		ch_tell( keeper, ch, buf );
		return;
	    }

	    ch->pcdata->curr_bargain += profit;

	    if( ch->pcdata->curr_bargain > ch->pcdata->bargain_max )
	    {
		ch_tell( keeper, ch, "Co ??? Jeszcze taniej? Nie ma mowy! Wynocha!" );
		ch->pcdata->trade_type = TRADE_NONE;
		return;
	    }
	    else
	    if( ch->pcdata->curr_bargain == ch->pcdata->bargain_max +
		number_range( -2, 1 ) )
	    {
	    	ch_tell( keeper, ch, "Nie przeginaj! Mam dzieci na utrzymaniu!" );
		return;
	    }
	    else
	    {
		sprintf( buf, "Hmm... Niech bêdzie, wezmê %d kredyt%s.",
		    ch->pcdata->trade_price - ch->pcdata->curr_bargain,
		    NUMBER_SUFF( ch->pcdata->trade_price - ch->pcdata->curr_bargain, "ka", "ki", "ek" ) );
		ch_tell( keeper, ch, buf );
		return;
	    }
	    break;

	case SELL_AN_ITEM: //gracz sprzedaje przedmiot

	    if( !check_trading( ch, SELL_AN_ITEM ) )
		return;

	    is_offered = false;
	    for( offer = ch->first_carrying; offer; offer = offer->next_content )
	    {
		if ( offer == obj )
		{
		    is_offered = true;
		    break;
		}
		else
		    is_offered = false;
		}

	    if ( !is_offered )
	    {
		sprintf( buf,
		"Przecie¿ ty nie masz %s" COL_TELL ". O co tu siê targowaæ?",
				obj->przypadki[1] );
		ch_tell( keeper, ch, buf );
		ch->pcdata->trade_type = TRADE_NONE;
		return;
	    }

	    if( ch->pcdata->trade_price < MIN_BARGAIN_PRICE )
	    {
		ch_tell( keeper, ch, "Daj spokój... Nie bêdê ³amaæ kredytek na pó³." );
		return;
	    }

	    bargain_profit = number_range( 1, 3 );

	    profit = ch->pcdata->trade_price*bargain_profit/100;

	    if( profit == 0 )
	    {
		ch_tell( keeper, ch, "Nieee, musisz siê bardziej postaraæ..." );
		return;
	    }

	    if( number_percent( ) < MAX_BARGAIN-ch->pcdata->bargain_max)
	    {
		ch->pcdata->trade_price -= profit;
	    	sprintf( buf, "Wiesz co? Rozmy¶lam siê. Dam %d.",
		    ch->pcdata->trade_price );
		ch_tell( keeper, ch, buf );
		return;
	    }

	    ch->pcdata->curr_bargain += profit;

	    if( ch->pcdata->curr_bargain > ch->pcdata->bargain_max )
	    {
	    	ch_tell( keeper, ch, "Co ??? Jeszcze dro¿ej? Nie ma mowy! Wynocha!" );
		ch->pcdata->trade_type = TRADE_NONE;
		return;
	    }
	    else
	    if( ch->pcdata->curr_bargain == ch->pcdata->bargain_max +
		number_range( -2, 1 ) )
	    {
	    	ch_tell( keeper, ch, "Nie przeginaj! Mam dzieci na utrzymaniu!" );
		return;
	    }
	    else
	    {
		sprintf( buf, "Hmm... Niech bêdzie, dam ci %d kredytek.",
		    ch->pcdata->trade_price + ch->pcdata->curr_bargain );
		ch_tell( keeper, ch, buf );
		return;
	    }

	    break;

	case BUY_A_PET: //gracz kupuje zwierzaka (a moze statek kosmiczny?)

	    if( !check_trading( ch, BUY_A_PET ) )
		return;

	    /* na wypadek, gdyby zwierzaki sie skonczyly (np. umarly) */
	    pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
	    pet = ch->pcdata->trade_pet;

	    is_offered = false;
	    for ( people = pRoomIndexNext->first_person; people; people = people->next_in_room )
	    {
		if ( people == ch->pcdata->trade_pet )
		{
		    is_offered = true;
		    STRDUP( pet->name, pet->name );
		    break;
		}
		else
		    is_offered = false;
	    }


	    if( !is_offered )
	    {
    		sprintf( buf, "Przykro mi, ale nie mamy ju¿ %s" COL_TELL ". Przyjd¼ jutro.",
		    pet->przypadki[1] );
		ch_tell( keeper, ch, buf );
		ch->pcdata->trade_type = TRADE_NONE;
		return;
	    }

	    if( ch->pcdata->curr_bargain >= ch->pcdata->bargain_max )
	    {
	    	ch_tell( keeper, ch, "Co ??? Jeszcze taniej? Nie ma mowy! Wynocha!" );
		ch->pcdata->trade_type = TRADE_NONE;
		return;
	    }

	    if( ch->pcdata->trade_price < MIN_BARGAIN_PRICE )
	    {
		ch_tell( keeper, ch, "Daj spokój... Nie bêdê ³amaæ kredytek na pó³." );
		return;
	    }

	    bargain_profit = number_range( 1, 3 );

	    profit = ch->pcdata->trade_price*bargain_profit/100;

	    if( profit == 0 )
	    {
		ch_tell( keeper, ch, "Nieee, musisz siê bardziej postaraæ..." );
		return;
	    }

	    if( number_percent( ) < MAX_BARGAIN-ch->pcdata->bargain_max)
	    {
		ch->pcdata->trade_price += profit;
	    	sprintf( buf, "Wiesz co? Rozmy¶lam siê. Chcê %d.",
		    ch->pcdata->trade_price );
		ch_tell( keeper, ch, buf );
		return;
	    }

	    ch->pcdata->curr_bargain += profit;

	    if( ch->pcdata->curr_bargain > ch->pcdata->bargain_max )
	    {
	    	ch_tell( keeper, ch, "Co ??? Jeszcze taniej? Nie ma mowy! Wynocha!" );
		ch->pcdata->trade_type = TRADE_NONE;
		return;
	    }
	    else
	    if( ch->pcdata->curr_bargain == ch->pcdata->bargain_max +
		number_range( -2, 1 ) )
	    {
	    	ch_tell( keeper, ch, "Nie przeginaj! Mam dzieci na utrzymaniu!"  );
		return;
	    }
	    else
	    {
		sprintf( buf, "Hmm... Niech bêdzie, wezmê %d kredyt%s.",
		    ch->pcdata->trade_price - ch->pcdata->curr_bargain,
		    NUMBER_SUFF(
			ch->pcdata->trade_price - ch->pcdata->curr_bargain, "ka", "ki", "ek" ) );
		ch_tell( keeper, ch, buf );
		return;
	    }
	    break;
    }
    return;
}


/*
 * Kupujemy przedmiot(y), f-cja wywolana komenda 'kup' bez argumentu.
 */
void buy_an_item( CHAR_DATA *ch )
{
        char	buf	[MSL];
	int 		count;
	OBJ_DATA *  	obj;
	int 		price;
	OBJ_DATA *	offer;
	bool 		is_offered;
	CHAR_DATA*	keeper;
	int		maxgold;

	if( !check_trading( ch, BUY_AN_ITEM ) )
	return;

	obj = ch->pcdata->trade_item;
	count = ch->pcdata->trade_count;
	price = ch->pcdata->trade_price;
	keeper = ch->pcdata->trader;

	/* czy sprzedawca jeszcze ¿yje */
	if( !can_see( ch, keeper ) )
	{
    	    send_to_char( "Przykro mi, ale twój sprzedawca gdzie¶ siê zapodzia³." NL, ch );
	    ch->pcdata->trade_type = TRADE_NONE;
	    return;
	}

	/* Gdyby towar nagle zosta³ wykupiony */
	for( offer = keeper->first_carrying; offer; offer = offer->next_content )
	{
	    if ( offer == obj )
	    {
	        is_offered = true;
	        break;
	    }
	    else
		is_offered = false;
	}

	if ( !is_offered )
	{

	    sprintf( buf, "Przykro mi, ale nie mamy ju¿ %s" COL_TELL ". Przyjd¼ jutro.",
		obj->przypadki[1] );
	    ch_tell( keeper, ch, buf );
	    ch->pcdata->trade_type = TRADE_NONE;
	    return;
	}

	/* czy gracz ma odpowiedni poziom do poziomu zakupu? */
/*	if ( obj->level > ch->top_level )
	{
	    act( COL_TELL, "$n mówi Ci 'Nie mo¿esz jeszcze u¿ywaæ $p$1! Nie zawracaj mi g³owy.'",
		    keeper, obj, ch, TO_VICT );
	    ch->reply = keeper;
	    ch->pcdata->trade_type = TRADE_NONE;
	    return;
	}
*/
	if ( ( count > 1 )//nie ruszam (skopiowane z oryginalnego do_buy)
	    && !IS_SET( obj->extra_flags, ITEM_INVENTORY ) )
	{
	    sprintf( buf, "Niestety mam tylko jedn± sztukê %s" COL_TELL "...",
		obj->przypadki[1] );
	    ch_tell( keeper, ch, buf );
	    ch->pcdata->trade_type = TRADE_NONE;
	    return;
	}

	/* czy gracz ma pieni±dze, by powtórzyæ zamówienie? */
	if ( ch->gold < price )
	{
	    char buf [ MAX_STRING_LENGTH ];

	    if ( ( ch->gold ) <= 0 )
		sprintf( buf,"Przecie¿ ty nie masz pieniêdzy! Wyno¶ siê st±d!");
	    else
		sprintf( buf, "Nie staæ ciê na to! Wróæ jak uzbierasz!" );

	    ch_tell( keeper, ch, buf );
	    ch->pcdata->trade_type = TRADE_NONE;
	    return;
	}

	if( !ch->pcdata->trade_dest )
	{
	    /* czy pomie¶ci i ud¼wignie nastepn± partiê zakupów? */
	    if ( ch->carry_number + ( count * get_obj_number( obj ) ) >
		can_carry_n( ch ) )
	    {
		send_to_char( "Nie mo¿esz nie¶æ tylu przedmiotów." NL, ch );
		return;
	    }
    	}

	if ( ch->carry_weight + ( count * get_obj_weight( obj ) ) >
		can_carry_w( ch ) )
	{
	    send_to_char( "Nie uniesiesz takiego ciê¿aru." NL, ch );
	    return;
	}

	/* ud¼wignie - ale czy sprzedawca mo¿e mu sprzedaæ a¿ tyle? */
	if ( ( count > 1 )
	    && !IS_SET( obj->extra_flags, ITEM_INVENTORY ) )
	{
	    sprintf( buf, "Niestety mam tylko jedn± sztukê %s" COL_TELL "...",
		obj->przypadki[1] );
	    ch_tell( keeper, ch, buf );
	    ch->pcdata->trade_type = TRADE_NONE;
	    return;
	}

	ch->pcdata->trade_price-=
	    (ch->pcdata->trade_price * ch->pcdata->curr_bargain)/100;

	/* mo¿e - kupujemy ;) */
	if ( count == 1 )
	{   char buf [ MAX_STRING_LENGTH ];
	    int price = ch->pcdata->trade_price;

	    sprintf( buf, "Kupujesz od $N$1 $p$3 za %d kredyt%s.",
		    price, NUMBER_SUFF( price, "kê", "ki", "ek" ) );
	    act( PLAIN, buf, ch, obj, keeper, TO_CHAR );
	    act( COL_TELL, "$n kupuje od $N$1 $p$3.", ch, obj, keeper, TO_ROOM );
	}
	else
	{
	    char buf [ MAX_STRING_LENGTH ];

	    sprintf( buf, "$n kupuje od $N$1 %d sztuk $p$1.", count );
	    act( PLAIN, buf, ch, obj, keeper, TO_ROOM );
	    sprintf( buf, "Kupujesz od $N$1 %d sztuk $p$1 za %d kredyt%s.",
		    count, ch->pcdata->trade_price,
		    NUMBER_SUFF(ch->pcdata->trade_price, "kê", "ki", "ek" ) );
	    act( PLAIN, buf, ch, obj, keeper, TO_CHAR );
	}

	ch->gold     -= ch->pcdata->trade_price;
	maxgold       = keeper->top_level * 1000;
	keeper->gold += ch->pcdata->trade_price;

	if ( keeper->gold > maxgold )
	{
	    boost_economy( keeper->in_room->area, keeper->gold - maxgold );
	    keeper->gold = maxgold;
	    act( COL_ACTION, "$n wk³ada pieni±dze do wielkiego sejfu.",
		    keeper, NULL, NULL, TO_ROOM );
	}

	/* celowo wziête w komentarz - u nas mo¿na powtarzaæ zamówienia */
	    //ch->pcdata->trade_type = TRADE_NONE;

	if ( IS_SET( obj->extra_flags, ITEM_INVENTORY ) )
	    for ( ; count > 0; count-- )
	    {
		obj = create_object( obj->pIndexData, obj->level );
		if( !ch->pcdata->trade_dest )
		    obj_to_char( obj, ch );
		else
		    obj_to_obj( obj, ch->pcdata->trade_dest );
	    }
	else
	{
	    obj_from_char( obj );
	    if( !ch->pcdata->trade_dest )
		obj_to_char( obj, ch );
	    else
	        obj_to_obj( obj, ch->pcdata->trade_dest );
	}

    return;
}

/*
 * Kupujemy zwierzaka f-cja wywolana komenda 'kup' bez argumentu.
 */
void buy_a_pet( CHAR_DATA *ch )
{
    CHAR_DATA * 	pet;
    char             	buf [ MAX_STRING_LENGTH ];
    CHAR_DATA * 	offer;
    ROOM_INDEX_DATA *	pRoomIndexNext;
    bool		is_offered;
    int			price;
    CHAR_DATA *		keeper;
    int			maxgold;

    if( !check_trading( ch, BUY_A_PET ) )
    return;

    keeper = ch->pcdata->trader;

    /* czy sprzedawca jeszcze zyje? */
    if( !can_see( ch, keeper ) )
    {
        send_to_char( "Przykro mi, ale twój sprzedawca gdzie¶ siê zapodzia³" NL, ch );
	ch->pcdata->trade_type = TRADE_NONE;
	return;
    }

    pet = ch->pcdata->trade_pet;

    ch->pcdata->trade_price-=
	    (ch->pcdata->trade_price * ch->pcdata->curr_bargain)/100;

    price = ch->pcdata->trade_price;

    /* na wypadek, gdyby zwierzaki sie sonczyly (np. umarly) */
    pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
    is_offered = false;

    for ( offer = pRoomIndexNext->first_person; offer; offer = offer->next_in_room )
    {
	if ( offer == pet )
	{
	    is_offered = true;
	    STRDUP( pet->name, pet->name );
	    break;
	}
	else
	    is_offered = false;
    }


    if( !is_offered )
    {
        sprintf( buf, "Przykro mi, ale nie mamy ju¿ %s" COL_TELL ". Przyjd¼ jutro.",
		pet->przypadki[1] );
	ch_tell( keeper, ch, buf );
	ch->pcdata->trade_type = TRADE_NONE;
	return;
    }

    /* czy gracz ma jeszcze pieni±dze na zwierzaka? */
    if ( ch->gold < price )
    {
        if ( ( ch->gold ) <= 0 )
		sprintf( buf,"Przecie¿ ty nie masz pieniêdzy! Wyno¶ siê st±d!");
	else
		sprintf( buf, "Nie staæ ciê na to! Wróæ jak uzbierasz!" );

	ch_tell( keeper, ch, buf );
	ch->pcdata->trade_type = TRADE_NONE;
	return;
    }


    /* zwierzatka cudem nie umarly (sprzedawca tez), a gracz ma pieniadze */

	ch->gold -= price;
	pet	  = create_mobile( pet->pIndexData);
	SET_BIT( ch->act,          PLR_BOUGHT_PET );
	SET_BIT( pet->act,         ACT_PET        );
	SET_BIT( pet->affected_by, AFF_CHARM      );

	sprintf( buf, "Na obro¿y %s" PLAIN " jest napisane: W³asno¶æ %s" PLAIN "'." NL,
		pet->przypadki[1], ch->przypadki[1] );
	STRDUP( pet->description, buf );

	char_to_room( pet, ch->in_room );
	add_follower( pet, ch );
	send_to_char( "Kupujesz sobie zwierzaka!." NL, ch );
	act( PLAIN, "$n kupuje sobie zwierz±tko: $N$3.", ch, NULL, pet, TO_ROOM );

	maxgold = 10 * pet->top_level * pet->top_level;

	if ( keeper->gold > maxgold )
	{
	    boost_economy( keeper->in_room->area, keeper->gold - maxgold/2 );
	    keeper->gold = maxgold/2;
	    act( COL_ACTION, "$n wk³ada pieni±dze do wielkiego sejfu.",
		    keeper, NULL, NULL, TO_ROOM );
	}
	/* tu celowo konczymy zakupy (tylko 1_zwierzak/level) */
	ch->pcdata->trade_type = TRADE_NONE;
	return;

}


int get_max_bargain( CHAR_DATA *ch, CHAR_DATA *keeper )
{
    int 	base;

    base = (get_curr_cha(ch) * 80 + get_curr_int(ch) * 20)/100;
    base+= (get_curr_lck(ch) > 10) ? 1 : 0;

    if( ch->top_level > keeper->top_level )
	base++;
    else if( ch->top_level < keeper->top_level )
	base--;

    if( ch->alignment >= 0 && keeper->alignment < 0 )
	base--;
    else
    if( ch->alignment < 0 && keeper->alignment >= 0 )
	base--;
    else
    if((ch->alignment >= 0 && keeper->alignment >= 0)
    || (ch->alignment <  0 && keeper->alignment <  0) )
	base++;

    if( get_curr_cha( keeper ) > 15 )
	base--;

    if( get_curr_lck( ch ) > 15 )
	base++;


    base = number_fuzzy( base );
    base/= (ch->race==keeper->race) ? 1 : 2;
    base = URANGE( 0, base, MAX_BARGAIN );
    return base;
}

DEF_DO_FUN( buy )
{
    char arg  [ MAX_INPUT_LENGTH ];
    char arg2 [ MAX_INPUT_LENGTH ];
    char arg3 [ MAX_INPUT_LENGTH ];
    CHAR_DATA *keeper;

    if( IS_NPC( ch ) )
	return;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg2);
    one_argument( argument, arg3 );

    //arg3 bo: buy <ile> <czego> <od_kogo>
    //		     ^      ^       ^
    //		    arg   arg2    arg3

    if ( !( keeper = find_keeper( ch, arg3 ) ) )
	    return;


    if ( arg[0] == '\0' )
    {
	if( keeper != ch->pcdata->trader )
	{
	    ch_printf( ch, "Przecie¿ nie handlujesz z %s." NL,
		keeper->przypadki[4] );
	    return;
	}

	if (  ch->pcdata->trade_type )
	{
	    if( ch->pcdata->trade_type == BUY_AN_ITEM )
	    {
		ch->pcdata->trade_dest = NULL;
		buy_an_item( ch );
	    }
	    else
	    if( ch->pcdata->trade_type == BUY_A_PET )
		buy_a_pet( ch );
	}
	else
	{
	    send_to_char( "Kupiæ co?" NL, ch );
	    ch->pcdata->trade_type = TRADE_NONE;
	}
	return;

    }
    if( ch->pcdata->trade_type == BUY_AN_ITEM && ch->pcdata->trade_item
    && arg2[0] != '\0' && !str_cmp( arg, "to" ) )
    {
	OBJ_DATA *	obj;

	if( !( obj = get_obj_carry( ch, arg2 ) ) )
	{
	    send_to_char( "Nie masz takiego przedmiotu." NL, ch );
	    return;
	}
	if ( obj->item_type != ITEM_CONTAINER )
	{
	    send_to_char( "To nie jest pojemnik." NL, ch );
	    return;
	}
	if ( IS_SET(obj->value[1], CONT_CLOSED) )
	{
	    send_to_char( "Mo¿e najpierw to otwórz!" NL, ch );
	    return;
	}
	if ( (IS_OBJ_STAT(obj, ITEM_COVERING)
	&&   (get_obj_weight( ch->pcdata->trade_item ) / ch->pcdata->trade_count )
	  > ((get_obj_weight( obj ) / obj->count)
	  -   obj->weight)) )
	{
	    send_to_char( "To siê tam nie zmie¶ci!" NL, ch );
	    return;
	}
	if ( (get_obj_weight( ch->pcdata->trade_item ) / ch->pcdata->trade_count)
	   + (get_obj_weight( obj ) / obj->count)
	   >  obj->value[0] )
	{
	    send_to_char( "To siê tam nie zmie¶ci!" NL, ch );
	    return;
	}
	ch->pcdata->trade_dest = obj;
	buy_an_item( ch );
	return;
    }

    {
	OBJ_DATA  *obj;
	CHAR_DATA *keeper;
	int        cost;
	int        item_count = 1; /* buy only one by default */
	char buf [ MAX_STRING_LENGTH ];

	if ( is_number( arg ) )
	{
	    item_count = atoi( arg );
	    strcpy( arg, arg2 );
	    strcpy( arg2, arg3 );
	}

	if ( !( keeper = find_keeper( ch, arg2 ) ) )
	    return;

	obj  = get_obj_carry( keeper, arg );
	cost = get_cost( ch, keeper, obj, true );

	if ( cost <= 0 || !can_see_obj( ch, obj ) )
	{
	    ch_tell( keeper, ch, "Nie sprzedajê niczego takiego. Spróbuj 'list'." );
	    return;
	}

	if ( item_count < 1 )
	{
	    ch_tell( keeper, ch, "A¿ tak ma³o ci nie sprzedam." );
	    return;
	}

	if ( IS_SET(obj->extra_flags, ITEM_PROTOTYPE)
             && get_trust( ch ) < LEVEL_IMMORTAL )
	{
	    ch_tell( keeper, ch, "To jeszcze prototyp! Nie mogê tego sprzedawaæ..." );
	    return;
	}

	if ( ( item_count > 1 )//tego ifa nie ruszam
	    && !IS_SET( obj->extra_flags, ITEM_INVENTORY ) )
	{
	    sprintf( buf, "Niestety, mam tylko jedn± sztukê %s" COL_TELL "...",
		obj->przypadki[1] );
	    ch_tell( keeper, ch, buf );
	    return;
	}

	if( item_count == 1 )
	    sprintf( buf, "Hmm. Chcesz %s" COL_TELL "... To wyniesie ciê %d kredyt%s",
		     obj->przypadki[3], cost, NUMBER_SUFF(cost, "kê", "ki", "ek" ) );
	else
	    sprintf( buf, "Hmm. Chcesz %d sztuk%s %s" COL_TELL "... To wyniesie ciê %d kredyt%s",
	    item_count,
	    item_count == 2 || item_count == 3 || item_count == 4 ? "i" : "",
	    obj->przypadki[1],
	    item_count * cost,
	    NUMBER_SUFF(item_count * cost, "kê", "ki", "ek" )  );
	ch_tell( keeper, ch, buf );

	ch->pcdata->trade_type	 = BUY_AN_ITEM;
	ch->pcdata->trader	 = keeper;
	ch->pcdata->trade_item   = obj;
	ch->pcdata->trade_price  = cost * item_count;
	ch->pcdata->trade_count  = item_count;
	ch->pcdata->trade_pet    = NULL;
        ch->pcdata->bargain_max  = get_max_bargain( ch, keeper );
	ch->pcdata->curr_bargain = 0;
	return;
    }
}


DEF_DO_FUN( list )
{
	char 		arg	[ MAX_INPUT_LENGTH ];
	char       	arg2 	[ MAX_INPUT_LENGTH ];
	CHAR_DATA *	keeper;
	OBJ_DATA *	obj;
	int 		cost;
	int 		oref = 0;
	bool 		found;

	argument = one_argument( argument, arg );
	one_argument( argument, arg2 );

	if ( ( keeper = find_keeper( ch,arg2 ) ) == NULL )
	    return;

	found = false;
	for ( obj = keeper->last_carrying; obj; obj = obj->prev_content )
	{
	    if ( obj->wear_loc == WEAR_NONE
	    &&   can_see_obj( ch, obj ) )
	    {
	       oref++;
	       if ( ( cost = get_cost( ch, keeper, obj, true ) ) > 0
	       && ( ( arg[0] == '\0' || nifty_is_name( arg, obj->name ) )
	    			     || !str_cmp( arg, "all" ) ) )
	       {
		if ( !found )
		{
		    found = true;
		    send_to_pager( "       ________________________________________________.----------.  " NL, ch );
		    send_to_pager( "      |         .--(_________Witamy_________)            __ [] X  |-." NL, ch );
		    send_to_pager( "      |         |                                                 | |" NL, ch );
		    send_to_pager( "      `\\        `----> Tu kupisz:" MOD_BLINK "_" RESET "                                | |" NL, ch );
		    send_to_pager( "       |                                                          | |" NL, ch );
		    send_to_pager( "       | [Lv  Cena] Towar                                         | |" NL, ch );
		}
		    pager_printf( ch, "       | [%2d %5d] %s" PLAIN "%-18s| |" NL,
			obj->level,
			cost,
			strip_colors(capitalize( obj->przypadki[0] ),28),
			IS_SET(obj->extra_flags, ITEM_HUTT_SIZE) ? "(huttcko wielkie) " :
		    	( IS_SET(obj->extra_flags, ITEM_LARGE_SIZE) ? "(du¿e) " :
		    	( IS_SET(obj->extra_flags, ITEM_HUMAN_SIZE) ? "(¶rednie) " :
		    	( IS_SET(obj->extra_flags, ITEM_SMALL_SIZE) ? "(ma³e) " : "" ) ) ) );
	       }
	    }
	}
	if( found )
	{
	    send_to_pager( "      .'                                                          | |" NL, ch );
	    send_to_pager( "      |__________________________________________________________/' |" NL, ch  );
	    send_to_pager( "        |__________________________________________________________/'" NL, ch  );
	    return;
	}

	if ( !found )
	{
	    if ( arg[0] == '\0' )
		send_to_char( "Nie mo¿esz tutaj nic kupiæ." NL, ch );
	    else
		send_to_char( "Nie mo¿esz kupiæ tego tutaj." NL, ch );
	}
	return;
}

/*
 * Sprzedajemy przedmiot. f-cja wywolana komenda 'sell' bez argumentu.
 */
void sell_an_item( CHAR_DATA *ch )
{
    OBJ_DATA *	obj;
    int		cost;
    OBJ_DATA *	offer;
    bool 	is_offered;
    CHAR_DATA*	keeper;

    if( !check_trading( ch, SELL_AN_ITEM ) )
	return;

    obj  = 	ch->pcdata->trade_item;
    keeper = ch->pcdata->trader;

    /* czy sprzedawca jeszcze zyje? */
    if( !can_see( ch, keeper ) )
    {
        send_to_char( "Przykro mi, ale twój sprzedawca gdzie¶ siê zapodzia³." NL, ch );
	ch->pcdata->trade_type = TRADE_NONE;
	return;
    }

    /* mo¿liwe, ¿e przedmiot zniknie graczowi podczas handlu */
    for( offer = ch->first_carrying; offer; offer = offer->next_content )
    {
        if ( offer == obj )
        {
            is_offered = true;
            break;
        }
        else
	    is_offered = false;
    }

    if ( !is_offered )
    {
	ch_tell( keeper, ch, "Nie masz takiego przedmiotu." );
	ch->pcdata->trade_type = TRADE_NONE;
	return;
    }

    /* albo zniknie sprzedawcy sprzed oczu*/
    if ( !can_see_obj( keeper, obj ) )
    {
	ch_tell( keeper, ch, "Nie widzê u ciebie takiego przedmiotu." );
	ch->pcdata->trade_type = TRADE_NONE;
	return;
    }

    /* je¶li w³a¶ciwo¶ci przedmiotu zmieni± siê, przerywamy handel */
    if ( ( cost = get_cost( ch, keeper, obj, false ) ) <= 0 )
//	|| obj->level > LEVEL_HERO ) Trog: ke?
    {
	act( PLAIN, "$n nie przejawia zainteresowania kupnem $p$1.", keeper, obj, ch, TO_VICT );
	ch->pcdata->trade_type = TRADE_NONE;
	return;
    }

    if ( IS_SET( obj->extra_flags, ITEM_POISONED ) )
    {
	ch_tell( keeper, ch, "Nie kupujê zatrutych rzeczy! Wyno¶ siê st±d!" );
	ch->pcdata->trade_type = TRADE_NONE;
	return;
    }

    cost = keeper->gold < cost ?
	keeper->gold : cost;
    cost -= (cost * ch->pcdata->curr_bargain)/100;
    separate_obj( obj );
    /* ale, gdyby siê nie zmieni³y */
    act( PLAIN, "Sprzedajesz $N$2 $p$3.", ch, obj, keeper, TO_CHAR );
    act( PLAIN, "$n sprzedaje $N$2 $p$3.", ch, obj, keeper, TO_ROOM );

    ch->gold     += cost;
    keeper->gold -= cost;

    if ( keeper->gold < 0 )
	keeper->gold = 0;

    if ( obj->item_type == ITEM_TRASH )
	extract_obj( obj );
    else  if ( IS_SET( obj->extra_flags , ITEM_CONTRABAND) )
    {
	long ch_exp;

        ch_exp = UMIN( obj->cost*10 , ( exp_level( ch->skill_level[SMUGGLING_ABILITY]+1) - exp_level( ch->skill_level[SMUGGLING_ABILITY])  ) / 10  );
        ch_printf( ch, "Zdobywasz %ld punktów do¶wiadczenia przemytniczego za kontrabandê." NL , ch_exp );
        gain_exp( ch, ch_exp , SMUGGLING_ABILITY );

	if( obj->item_type == ITEM_SPICE || obj->item_type == ITEM_RAWSPICE )
	    extract_obj( obj );
        else
        {
			/* Trog: chyba BUG. */
    	    REMOVE_BIT( obj->extra_flags , ITEM_CONTRABAND );
            obj_from_char( obj );
            obj_to_char( obj, keeper );
        }
    }
    else if ( obj->item_type == ITEM_SPICE || obj->item_type == ITEM_RAWSPICE )
	extract_obj( obj );
    else
    {
	obj_from_char( obj );
	obj_to_char( obj, keeper );
    }

    /*
     * konczymy handel, nastepny raz, gdy gracz bedzie chcial
     * sprzedac taki sam przedmiot, dostanie za niego mniej
     */
    ch->pcdata->trade_type = TRADE_NONE;
    return;
}

DEF_DO_FUN( sell )
{
    OBJ_DATA  *obj;
    CHAR_DATA *keeper;
    char       buf  [ MAX_STRING_LENGTH ];
    char       arg  [ MAX_INPUT_LENGTH  ];
    char       arg2 [ MAX_INPUT_LENGTH  ];
    int        cost;

    if( IS_NPC( ch ) )
	return;

    argument = one_argument( argument, arg );
    one_argument( argument, arg2 );

    if ( !( keeper = find_keeper( ch, arg2 ) ) )
    return;

    if ( arg[0] == '\0' )
    {
	if( keeper != ch->pcdata->trader )
	{
	    ch_printf( ch, "Przecie¿ nie handlujesz z %s." NL,
		keeper->przypadki[4] );
	    return;
	}

	if( ch->pcdata->trade_type == SELL_AN_ITEM )
	    sell_an_item( ch );
	else
	    send_to_char( "Sprzedaæ co?" NL, ch );
	return;
    }


    if ( !( obj = get_obj_carry( ch, arg ) ) )
    {
	ch_tell( keeper, ch, "Nie widzê u ciebie takiego przedmiotu." );
	return;
    }

    if ( IS_OBJ_STAT( obj, ITEM_PERSONAL ) )
    {
	ch_printf( ch, "No co¶ ty, %s" PLAIN " chcesz sprzedawaæ? TW%s %s" PLAIN " ???" EOL,
	    capitalize( obj->przypadki[3] ),
	    obj->gender == GENDER_MALE ? "ÓJ" :
	    obj->gender == GENDER_FEMALE ? "OJ¡" : "OJE",
	    all_capitalize( obj->przypadki[3] ) );
	return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "Nie mo¿esz tego odrzuciæ." NL, ch );
	return;
    }

    if ( obj->timer > 0 )
    {
	sprintf( buf, "Warto¶æ %s" COL_TELL " spada zbyt szybko...",
	    obj->przypadki[1] );
	ch_tell( keeper, ch, buf );
	return;
    }

    if ( !can_see_obj( keeper, obj ) )
    {
	ch_tell( keeper, ch, "Nie widzê u ciebie takiego przedmiotu." );
	return;
    }

    if ( ( cost = get_cost( ch, keeper, obj, false ) ) <= 0 )
//	|| obj->level > LEVEL_HERO ) Trog: ke?
    {
	act( PLAIN, "$n nie przejawia zainteresowania kupnem $p$1.", keeper, obj, ch, TO_VICT );
	return;
    }

    if ( IS_SET( obj->extra_flags, ITEM_POISONED ) )
    {
	ch_tell( keeper, ch, "Nie kupujê zatrutych rzeczy! Wyno¶ siê st±d!" );
        return;
    }

	if( keeper->gold < 1 )
	{
		ch_tell( keeper, ch, "Nie mam czym Ci zap³aciæ." );
		return;
	}

	if( keeper->gold < cost )
	{
		sprintf( buf, "%s jest wart%c" COL_TELL " %d kredyt%s.",
		    capitalize( obj->przypadki[0] ), obj->gender == GENDER_FEMALE ? 'a' :
			obj->gender == GENDER_MALE ? 'y' : 'e',
	    	cost, NUMBER_SUFF( cost, "kê", "ki", "ek" ) );
		ch_tell( keeper, ch, buf );
		sprintf( buf, "Niestety ja mam tylko %d i tyle jestem w stanie Ci zap³aciæ. Zgadzasz siê?",
			keeper->gold );
	}
	else
	{
		sprintf( buf, "Za %s" COL_TELL " dam ci %d kredyt%s. Zgadzasz siê?",
	    	obj->przypadki[3],
		    cost, NUMBER_SUFF( cost, "kê", "ki", "ek" ) );
    }
    ch_tell( keeper, ch, buf );

	ch->pcdata->trade_type	 = SELL_AN_ITEM;
	ch->pcdata->trader	 = keeper;
	ch->pcdata->trade_item   = obj;
	ch->pcdata->trade_pet    = NULL;
	ch->pcdata->trade_price  = cost;
	ch->pcdata->trade_count  = 1;
        ch->pcdata->bargain_max  = get_max_bargain( ch, keeper );
	ch->pcdata->curr_bargain = 0;

    return;
}
/************************************************************************
 * done by Thanos 							*
 ************************************************************************/


DEF_DO_FUN( value )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *keeper;
    OBJ_DATA *obj;
    int cost;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Co chcesz wyceniæ?" NL, ch );
	return;
    }

    if ( ( keeper = find_keeper( ch, "" ) ) == NULL )
	return;

    if ( ( obj = get_obj_carry( ch, argument ) ) == NULL )
    {
	ch_tell( keeper, ch, "Nie masz niczego takiego." );
	return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "Jako¶ nie mo¿esz siê tego pozbyæ!" NL, ch );
	return;
    }

    if ( ( cost = get_cost( ch, keeper, obj, false ) ) <= 0 )
    {
	act( COL_ACTION, "$n nie przejawia zainteresowania $p$4.", keeper, obj, ch, TO_VICT );
	return;
    }

    sprintf( buf, "Mogê zaoferowaæ ci %d kredyt%s za %s" COL_TELL ".",
	cost, NUMBER_SUFF( cost, "kê", "ki", "ek" ), obj->przypadki[3] );
    ch_tell( keeper, ch, buf );
    return;
}

/*
 * Repair a single object. Used when handling "repair all" - Gorog
 */
void repair_one_obj( CHAR_DATA *ch, CHAR_DATA *keeper, OBJ_DATA *obj,
                 char *arg, int maxgold, const char *fixstr, const char*fixstr2 )
{
   char buf[MAX_STRING_LENGTH];
   int cost;

/* by Ratm: a to niby czemu? personal nie personal naprawiæ mo¿na,
    a nawet trzeba, personale sa po to zeby ktos obcy ich nie zabral
    nie po to zeby nie mozna ich bylo odrzucic czy naprawic.
    if ( IS_OBJ_STAT( obj, ITEM_PERSONAL )
    && *obj->owner_name && !str_cmp( obj->owner_name, ch->name ) )
	ch_printf( ch, "Nie oddasz nikomu %s nawet na moment!" NL,
	    obj->przypadki[1] );
    else */
   if ( !can_drop_obj( ch, obj ) )
       ch_printf( ch, "Jako¶ nie mo¿esz oddaæ %s" PLAIN "!" NL, obj->przypadki[1] );
   else
   if ( ( cost = get_repaircost( keeper, obj ) ) < 0 )
   {
       if (cost != -2)
       act( COL_TELL, "$n mówi ci 'Przykro mi, ale nie da siê chyba nic zrobiæ z $p$4.'",
            keeper, obj, ch, TO_VICT );
       else
	  act( COL_TELL, "$n mówi ci '$p wygl±da dobrze jak na mój gust!'", keeper, obj, ch, TO_VICT );
   }
               /* "repair all" gets a 10% surcharge - Gorog */

   else if ( (cost = str_cmp("all",arg) ? cost : 11*cost/10) > ch->gold )
   {
      sprintf( buf,
       "$N mówi ci '%s %s" COL_TELL " bêdzie ciê kosztowa³a %d kredyt%s...'",
	fixstr, obj->przypadki[1],
        cost, NUMBER_SUFF( cost, "kê", "ki", "ek" ) );
      act( COL_ACTION, buf, ch, obj, keeper, TO_CHAR );
      ch_tell( keeper, ch, "Których chyba nie posiadasz..." );
   }
   else
   {
      sprintf( buf, "$n daje $p$3 $N$2 do %s.", fixstr2 );
      act( PLAIN, buf, ch, obj, keeper, TO_ROOM );
      sprintf( buf, "$N pobiera od ciebie %d kredyt%s za %s $p$1.",
          cost, NUMBER_SUFF( cost, "kê", "ki", "ek" ), fixstr );
      act( COL_ACTION, buf, ch, obj, keeper, TO_CHAR );
      ch->gold     -= cost;
      keeper->gold += cost;
      if ( keeper->gold < 0 )
          keeper->gold = 0;
      else
      if ( keeper->gold > maxgold )
      {
          boost_economy( keeper->in_room->area, keeper->gold - maxgold/2 );
          keeper->gold = maxgold/2;
          act( COL_ACTION, "$n wk³ada kredytki do wielkiego sejfu.", keeper,
		NULL, NULL, TO_ROOM );
      }

      switch ( obj->item_type )
      {
          default:
            ch_printf( ch, "Hmmm... Co¶ mówi ci, ¿e da³%s¶ siê nabraæ..." NL,
		SEX_SUFFIX_EAE( ch ) );
            break;
          case ITEM_ARMOR:
            obj->value[0] = obj->value[1];
            break;
          case ITEM_WEAPON:
            obj->value[0] = INIT_WEAPON_CONDITION;
            break;
          case ITEM_DEVICE:
            obj->value[2] = obj->value[1];
            break;
      }

      oprog_repair_trigger( ch, obj );
   }
}

DEF_DO_FUN( repair )
{
    CHAR_DATA *keeper;
    OBJ_DATA *obj;
    const char *fixstr;
    const char *fixstr2;
    int maxgold;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Naprawiæ co?" NL, ch );
	return;
    }

    if ( ( keeper = find_fixer( ch ) ) == NULL )
	return;

    maxgold = keeper->top_level * 10;
    switch( keeper->pIndexData->rShop->shop_type )
    {
	default:
	case SHOP_FIX:
	  fixstr  = "naprawienie";
	  fixstr2 = "naprawy";
	  break;
	case SHOP_RECHARGE:
	  fixstr  = "na³adowanie";
	  fixstr2 = "na³adowania";
	  break;
    }

    if ( !str_cmp( argument, "all" ) )
    {
	for ( obj = ch->first_carrying; obj ; obj = obj->next_content )
	{
           if ( obj->wear_loc  == WEAR_NONE
           &&   can_see_obj( ch, obj )
	   && ( obj->item_type == ITEM_ARMOR
	   ||   obj->item_type == ITEM_WEAPON
	   ||   obj->item_type == ITEM_DEVICE ) )
                repair_one_obj( ch, keeper, obj, argument, maxgold,
                                fixstr, fixstr2);
	}
    return;
    }

    if ( ( obj = get_obj_carry( ch, argument ) ) == NULL )
    {
	ch_tell( keeper, ch, "Nie masz niczego takiego przecie¿!" );
	return;
    }

    repair_one_obj( ch, keeper, obj, argument, maxgold, fixstr, fixstr2);

    return;
}

void appraise_all( CHAR_DATA *ch, CHAR_DATA *keeper, const char *fixstr )
{
    OBJ_DATA *obj;
    char buf[MAX_STRING_LENGTH], *pbuf=buf;
    int cost, total=0;

    for ( obj = ch->first_carrying; obj != NULL ; obj = obj->next_content )
    {
        if ( obj->wear_loc  == WEAR_NONE
        &&   can_see_obj( ch, obj )
        && ( obj->item_type == ITEM_ARMOR
        ||   obj->item_type == ITEM_WEAPON
        ||   obj->item_type == ITEM_DEVICE ) )
	{
            if ( !can_drop_obj( ch, obj ) )
            ch_printf( ch, "Nie mo¿esz pozbyæ siê %s!" NL, obj->przypadki[1] );
            else if ( ( cost = get_repaircost( keeper, obj ) ) < 0 )
            {
               if (cost != -2)
               act( COL_TELL,
                    "$n mówi ci 'Przykro mi, ale nie mogê nic zrobiæ z $p$4.'",
                    keeper, obj, ch, TO_VICT );
               else
	       act( COL_TELL, "$n mówi ci '$p wygl±da dobrze jak na mój gust!'",
	    	    keeper, obj, ch, TO_VICT );
            }
            else
            {
    	    sprintf( buf,
    		"$N mówi ci '%s %s" FB_BLUE " bêdzie ciê kosztowa³a %d kredyt%s...'",
		fixstr, obj->przypadki[1],
    		cost,
    		NUMBER_SUFF( cost, "kê", "ki", "ek" ) );
            act( COL_TELL, buf, ch, NULL, keeper, TO_CHAR );
            total += cost;
            }
	}
    }
    if ( total > 0 )
    {
       send_to_char ("" NL, ch);
       sprintf( buf,
          "$N mówi ci '£±cznie wyniesie ciê to %d kredyt%s.'",
          total, NUMBER_SUFF( total, "kê", "ki", "ek" ) );
       act( COL_TELL, buf, ch, NULL, keeper, TO_CHAR );
       strcpy( pbuf,
       "$N mówi ci 'Pamiêtaj o promocji! 10% zni¿ki je¶li reperujesz wszystko! ;-)'");
       act( COL_TELL, buf, ch, NULL, keeper, TO_CHAR );
    }
}


DEF_DO_FUN( appraise )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *keeper;
    OBJ_DATA *obj;
    int cost;
    const char *fixstr;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Naprawê czego chcesz wyceniæ?" NL, ch );
	return;
    }

    if ( ( keeper = find_fixer( ch ) ) == NULL )
	return;

    switch( keeper->pIndexData->rShop->shop_type )
    {
	default:
	case SHOP_FIX:
	  fixstr  = "naprawa";
	  break;
	case SHOP_RECHARGE:
	  fixstr  = "na³adowanie";
	  break;
    }

    if ( !str_cmp( arg, "all") )
    {
    appraise_all( ch, keeper, fixstr );
    return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	act( COL_TELL, "$n mówi ci 'Nie masz niczego takiego.'",
		keeper, NULL, ch, TO_VICT );
	ch->reply = keeper;
	return;
    }

    if ( !can_drop_obj( ch, obj ) )
    {
	send_to_char( "Nie mo¿esz tego odrzuciæ.\n\r", ch );
	return;
    }

    if ( ( cost = get_repaircost( keeper, obj ) ) < 0 )
    {
      if (cost != -2)
        act( COL_TELL, "$n mówi ci 'Przykro mi, ale nie da siê chyba nic zrobiæ z $p$4.'",
            keeper, obj, ch, TO_VICT );
      else
	act( COL_TELL, "$n mówi ci '$p wygl±da dobrze jak na mój gust!'", keeper, obj, ch, TO_VICT );
      return;
    }

      sprintf( buf,
       "$N mówi ci '%s %s" FB_BLUE " bêdzie ciê kosztowa³a %d kredyt%s...'",
	fixstr, obj->przypadki[1],
        cost,
        NUMBER_SUFF( cost, "kê", "ki", "ek" ) );
    act( COL_TELL, buf, ch, NULL, keeper, TO_CHAR );
    if ( cost > ch->gold )
      act( COL_TELL, "$N mówi ci 'Których chyba nie posiadasz...'", ch,
              NULL, keeper, TO_CHAR );

    return;
}


DEF_DO_FUN( shops )
{
    SHOP_DATA *shop;

    if ( !first_shop )
    {
	send_to_char( "There are no shops." NL, ch );
	return;
    }

    for ( shop = first_shop; shop; shop = shop->next )
	ch_printf( ch, "Keeper: %5d Buy: %3d Sell: %3d Open: %2d Close: %2d Buy: %2d %2d %2d %2d %2d" NL,
		shop->keeper,	   shop->profit_buy, shop->profit_sell,
		shop->open_hour,   shop->close_hour,
		shop->buy_type[0], shop->buy_type[1],
		shop->buy_type[2], shop->buy_type[3], shop->buy_type[4] );
    return;
}


DEF_DO_FUN( repairshops )
{
    REPAIR_DATA *repair;

    if ( !first_repair )
    {
	send_to_char( "There are no repair shops." NL, ch );
	return;
    }

    for ( repair = first_repair; repair; repair = repair->next )
	ch_printf( ch, "Keeper: %5d Profit: %3d Type: %d Open: %2d Close: %2d Fix: %2d %2d %2d" NL,
		repair->keeper,	     repair->profit_fix, repair->shop_type,
		repair->open_hour,   repair->close_hour,
		repair->fix_type[0], repair->fix_type[1], repair->fix_type[2] );
    return;
}
