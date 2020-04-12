/***************************************************************************
* Star Wars Reality 1.0						           *
* copyright (c) 1997, 1998 by Sean Cooper                                  *
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
* (c) 2001, 2002            M       U        D                Ver 1.1		*
* ------------------------------------------------------------------------	*
*						Stock Exchange routines	 	 						*
****************************************************************************/

/*! \fn DEF_DO_FUN( exchange )
*	\brief zaloguj sie do terminala gieldowego
*	\param ch - loguj±cy siê gracz
*	\param argument - interesuj±ca nas aukcja
*/
/*! \todo ob³uga zakoñczonej aukcji przedmiotów sprzedawanych w wiêcej niz jednym egzemplarzu
 * \todo Rozstrzyganie kto wygra³ i ile obiektów zabierze w aukcji na wiele
 * przedmiotów
 * \todo Na zakoñczenie aukcji przekazaæ notke do uczestników
 * \todo Dodaæ obs³uge sprzeda¿y materia³ów i produktów
 * \todo Dodaæ magazyny po³¹czone z gie³dami
 * \todo Opuszczanie aukcji
 */
#include "olc.h"
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "auction.h"

#if defined(KEY)
#undef KEY
#endif

#define DETAIL_LENGTH	74
#define KEY( literal, field, value )				\
				if ( !str_cmp( word, literal ) )	\
				{									\
				    field  = value;					\
				    fMatch = true;					\
				    break;							\
				}

const char * make_stock_state_info(const char * state_name)
{
	static char buffer[MIL];
	char buf[100];
	int length=0,index=0,len=0;

	buf[0]='\0';
	len = strlen(state_name);
	length = (DETAIL_LENGTH - len)/2;
	len = DETAIL_LENGTH - length - len;
	for(index=0;index<length;index++)
		strcat(buf," ");
	buffer[0]='\0';
	strcat(buffer,buf);
	strcat(buffer,state_name);
	buf[0]='\0';
	for(index=0;index<len;index++)
		strcat(buf," ");
	strcat(buffer,buf);

	return buffer;
}

void prepare_terminal_header(CHAR_DATA *ch)
{
	if (ch==NULL)
		return;
	pager_printf(ch,EOL "   [" FG_CYAN
		"---------------------------- " PLAIN "Terminal gie³dowy"
		FG_CYAN " ---------------------------" PLAIN "]");
	pager_printf(ch, NL FG_CYAN "      Planeta  : " PLAIN "%-15s%10s" NL
			FG_CYAN "      Gie³da   : " PLAIN "%-10s" EOL NL,
			ch->in_room->area->planet ?  ch->in_room->area->planet->name : "brak"
			,"",ch->in_room->name);
}

STOCK_EXCHANGE_DATA * get_stock_exchange(CHAR_DATA * ch)
{
	if (ch==NULL)
		return NULL;
	PLANET_DATA * pPlanet = ch->in_room->area->planet;
	STOCK_EXCHANGE_DATA * pStock = NULL;
	if (!pPlanet)
		return pStock;
	FOREACH(pStock,first_stock_exchange)
		if (!strcmp(pStock->pPlanet->name,pPlanet->name))
			return pStock;
	return pStock;
}

int parse_atodate( CHAR_DATA * ch, char * argument)
{
	char  arg[MSL];
	int 	day=0;
	int		hours=0;

	argument = one_argument( argument, arg );
	//najpierw idzie liczba
	if ( !is_number(arg))
	{
		pager_printf( ch, "Sk³adnia: czas nr <dzieñ/dni> nr <godzinê/godziny/godzin>" NL);
		return -1;
	}
	day = atoi(arg);
	//teraz idzie znacznik czy to godzina czy dzien
	argument = one_argument( argument, arg );
	//jesli nie ma znacznika - to zakladamy ze to godziny
	if ( arg[0]=='\0' )
	{
		if (day < 1 || day > 23 )
		{
			pager_printf( ch, " W³aœciwy zakres godzin to od 1 godziny do 23 godzin" NL);
			return -1;
		}
		return day;
	}
	else
	{
		if( !str_prefix( arg, "days") || !str_prefix( arg, "dzien") || !str_prefix( arg, "dni") )
		{
			if ( day <1 || day > 14 )
			{
				pager_printf( ch, " W³aœciwy zakres dni trwania aukcji to od 1 do 14 dni" NL);
				return -1;
			}
		}
		else if ( !str_prefix(arg, "hours") || !str_prefix( arg, "godzine") || !str_prefix( arg, "godziny"))
		{
			if (day < 1 || day > 23 )
			{
				pager_printf( ch, " W³aœciwy zakres godzin to od 1 godziny do 23 godzin" NL);
				return -1;
			}
			return day;
		}
		else
		{
			pager_printf( ch, "Sk³adnia: czas nr <dzieñ/dni> nr <godzinê/godziny/godzin>" NL);
			return -1;
		}
	}
	argument = one_argument( argument, arg );
	if ( arg[0]=='\0' )
		return day*24+hours;
	if ( !is_number(arg))
	{
		pager_printf( ch, "Sk³adnia: czas nr <dzieñ/dni> nr <godzinê/godziny/godzin>" NL);
		return -1;
	}
	hours = atoi(arg);
	if (hours < 1 || hours > 23 )
	{
		pager_printf( ch, " W³aœciwy zakres godzin to od 1 godziny do 23 godzin" NL);
		return -1;
	}
	return day*24+hours;
}

AUCTION_DATA * auction_exist( CHAR_DATA * ch, char * arg , char * text, STOCK_EXCHANGE_DATA * pStock )
{
	AUCTION_DATA *		pAuction = NULL;
	int			number=0;

	if ( !is_number ( arg ) )
	{
		pager_printf( ch, "Podaj numer aukcji, któr¹ chcesz %s." NL NL, text);
		return NULL;
	}
	number = atoi( arg );
	pAuction = get_auction_nr( ch, number, pStock );
	if ( !pAuction )
	{
		pager_printf( ch, "Nie ma aukcji o takim numerze." NL NL);
		return NULL;
	}

	return pAuction;
}

/**
* @brief zapisz gie³de do pliku
*
* @param pStock zapisywana gie³da
*/
void save_stock_market( STOCK_EXCHANGE_DATA * pStock )
{
	FILE			* fp;
	AUCTION_DATA	* pAuction;
	BETS_PROGRES	* pBets;
	int 			bidders_count=0;
	char			buf[MIL];

	RESERVE_CLOSE;
	//tworzymy plik powiazany z dana planeta
	if ( !pStock )
	{
		bug("Stock pointer is NULL");
		return;
	}
	if ( !pStock->pPlanet )
	{
		bug("Stock isn't connected with any planet");
		return;
	}
	sprintf( buf, "%s%s.stock",STOCK_MARKET_DIR, pStock->pPlanet->filename);
	if ( (fp = fopen(buf,"w") ) == NULL)
	{
		bug("fopen");
		perror(AUCTION_FILE);
		RESERVE_OPEN;
		return;
    }

    FOREACH(pAuction,pStock->first)
    {
		fprintf(fp,"\n#AUCTION\n");
		fprintf(fp,"Seller          %s~\n",pAuction->seller_name);
		fprintf(fp,"Start_price	    %lld\n",pAuction->starting_price);
		fprintf(fp,"BuyOut          %lld\n",pAuction->buy_out_now);
		fprintf(fp,"Quantity        %lld\n",pAuction->quantity);
		fprintf(fp,"Finish          %d\n",(int)pAuction->end_time);
		fprintf(fp,"Desc            %s~\n",pAuction->desc);
		fprintf(fp,"Payment         %d\n",pAuction->payment_collected ? 1:0);
		fprintf(fp,"Item_collected  %d\n",pAuction->item_collected ? 1:0);
		fprintf(fp,"Cargo_type      %d\n",pAuction->type);
		FOREACH(pBets,pAuction->first)
		{
			bidders_count++;
			fprintf(fp,"  Bidder        %s~\n",pBets->name);
			fprintf(fp,"  Max_price     %lld\n",pBets->max_price);
			fprintf(fp,"  Bets_quantity %lld\n",pBets->quantity);
			fprintf(fp,"  Buy_out       %d\n\n",pBets->buy_out? 1:0);
		}
		//tutaj trzeba zapisaæ zró¿nicowany zapis licytowanego gad¿etu
		pAuction->bidders_count = bidders_count;
		fprintf(fp,"Item_name		%s~\n\n",pAuction->stock_name);
		switch ( pAuction->type)
		{
			case AUCTION_ITEM:
				if (pAuction->item)
					fwrite_obj_raw(pAuction->item,fp,0,OS_CARRY);
				break;
			case AUCTION_SHIP:
				break;
			case AUCTION_HOUSE:
				break;
			default:
				break;
		}
		fprintf(fp, "#END\n\n");    //dla obiektu
		bidders_count=0;
	}
	fprintf(fp, "#FILE_END\n");
	fclose(fp);
	RESERVE_OPEN;
}

/**
* @brief odczytaj gie³de z podanego pliku
*
* @param filename plik z gie³da
*
* @param pStock odczytana gie³da
*/
int fread_stock_market( char * filename, STOCK_EXCHANGE_DATA * pStock)
{
    FILE *			fp;
	AUCTION_DATA	* pAuction = 0;
	BETS_PROGRES	* pBets = 0;
	OBJ_DATA		* obj;
	OBJ_DATA		* obj_next;

	const char		* word;
	bool			fMatch;
    if ( ( fp = fopen( filename, "r" ) ) == NULL )
		return 0;

	for(;;)
	{
		word = feof( fp ) ? "#FILE_END" : fread_word( fp );
		fMatch = false;

		switch( UPPER( word[0] ) )
		{
			case '#' :
				if (!str_cmp(word,"#OBJECT"))
				{
					fread_obj(supermob,fp,OS_CARRY);
					for(obj = supermob->first_carrying;obj;obj=obj_next)
					{
						obj_next=obj->next_content;
						obj_from_char(obj);
						pAuction->item = obj;
					}
					fMatch = true;
					break;
				}
				if (!str_cmp(word, "#AUCTION" ) )
				{
					pAuction = new_auction();
					LINK( pAuction, pStock->first, pStock->last, next, prev );
					fMatch = true;
					break;
				}
				if (!str_cmp(word,"#END") )
				{
					fMatch = true;
					break;
				}
				if (!str_cmp(word,"#FILE_END"))
					return 1;
				break;
			case 'B':
				KEY("BuyOut",		pAuction->buy_out_now,	fread_number64(fp));
				KEY("Buy_out",		pBets->buy_out,			fread_number(fp));
				KEY("Bets_quantity",		pBets->quantity,			fread_number(fp));
				if ( !str_cmp(word,"Bidder"))
				{
					fMatch = true;
					pBets = new_bets();
					LINK( pBets, pAuction->first, pAuction->last, next, prev );
					STRDUP( pBets->name, fread_string( fp ) );
					pAuction->bidders_count++;
				}
				break;
			case 'C':
				KEY("Cargo_type",		pAuction->type,				(auction_type)fread_number( fp ) );
				break;
			case 'D':
				SKEY("Desc",			pAuction->desc,				fread_string( fp ) );
				break;
			case 'F':
				KEY( "Finish",			pAuction->end_time,			fread_number( fp ) );
				break;
			case 'I' :
				KEY( "Item_collected",	pAuction->item_collected,	fread_number( fp ) );
				if ( !str_cmp(word, "Item_name" ) )
				{
					STRFREE( pAuction->stock_name );
					pAuction->stock_name = fread_string( fp );
					fMatch = true;
				}
				break;
			case 'M' :
				KEY( "Max_price",		pBets->max_price,			fread_number64( fp ) );
				break;
			case 'P' :
				SKEY( "Planet",			pAuction->planet,			fread_string( fp ) );
				KEY( "Payment",			pAuction->payment_collected,	fread_number( fp ) );
				break;
			case 'Q' :
				KEY( "Quantity",		pAuction->quantity,			fread_number64( fp ) );
				KEY( "Bets_quantity",	pBets->quantity,			fread_number64( fp ) );
				break;
			case 'S' :
				SKEY( "Seller",			pAuction->seller_name,		fread_string( fp ) );
				KEY( "Start_price",		pAuction->starting_price,	fread_number64( fp ) );
				KEY( "Start_time",		pAuction->start_time,		fread_number( fp ) );
				break;
			default: break;
		}
		if ( !fMatch )
			bug( "Stock_Market %s - no match : %s", pStock->pPlanet->name, word);
	}
	return 1;
}

/**
* @brief tworzy katalog oraz puste pliki z gie³dami
*/
void recreate_stock_markets_list()
{
    FILE *		fpList,* fpList1;
	char		buf[MSL];
	PLANET_DATA * pPlanet = NULL;

	if ( ( fpList = fopen( STOCKS_LIST, "r" ) ) == NULL )
	{
		if (mkdir( STOCK_MARKET_DIR , 0750) && ( fpList = fopen( STOCKS_LIST, "w" ) ) == NULL)
		{
			perror( STOCKS_LIST);
			exit( 1 );
		}
	}
	//sprawdzamy czy jest utworzony katalog stocks
	FOREACH(pPlanet, first_planet)
	{
		fprintf( fpList, "%s\n",pPlanet->name );
		sprintf( buf, "%s%s.stock",STOCK_MARKET_DIR, pPlanet->filename);
		if ( ( fpList1 = fopen( buf, "r" ) ) == NULL )
		{
			if ( ( fpList1 = fopen( buf, "w" ) ) == NULL )
			{
				perror( buf );
				RESERVE_OPEN;
				exit( 1 );
			}
			fprintf( fpList1, "#END\n\n#FILE_END\n");
			fclose( fpList1 );
		}
	}
	fprintf( fpList,"$\n" );
	fclose	( fpList );
}

/**
* @brief Oczytuje wszystkie gie³dy
*/
void load_stock_markets( )
{
    FILE *		fpList,*fpList1;
    const char *		filename;
	char		buf[MSL];
	STOCK_EXCHANGE_DATA * pStock = NULL;
	AUCTION_DATA *		pAuction= NULL;
	PLANET_DATA * pPlanet = NULL;

    RESERVE_CLOSE;
    if ( ( fpList = fopen( STOCKS_LIST, "r" ) ) == NULL )
		recreate_stock_markets_list();
    if ( ( fpList = fopen( STOCKS_LIST, "r" ) ) == NULL )
	{
		perror( STOCKS_LIST);
		exit( 1 );
	}

    for ( ; ; )
    {
		filename = feof( fpList ) ? "$" : fread_word( fpList );
		if( !sysdata.silent )
		    log_string( filename );
		if ( filename[0] == '$' )
			break;

		pStock = new_stock_exchange();
		pPlanet = get_planet(filename);
		LINK( pStock, first_stock_exchange, last_stock_exchange, next, prev );
		pStock->pPlanet=pPlanet;
		sprintf( buf, "%s%s.stock",STOCK_MARKET_DIR, pPlanet->filename);
		if ( !fread_stock_market( buf, pStock ) )
		{
			if ( ( fpList1 = fopen( buf, "r" ) ) == NULL )
			{
				if ( ( fpList1 = fopen( buf, "w" ) ) == NULL )
				{
					perror( buf );
					RESERVE_OPEN;
					exit( 1 );
				}
				fprintf( fpList1, "#END\n\n#FILE_END\n");
				fclose( fpList1 );
			}
			else
				bug( "Cannot load stock_market file: %s", buf );
		}
		else
		{
			sort_stock_market_auctions( pStock );
			FOREACH(pAuction, pStock->first)
				sort_auction_bets( pAuction );
		}
	}
	fclose( fpList );
	if( !sysdata.silent )
		log_string(" Done stock_markets" );
    RESERVE_OPEN;
    return;
}

//////////////////////////////////////////////////////////////////////////////
/**
 * @brief Poka¿ ekran terminala gie³dowego z szczegó³ami dotycz¹cymi
 * licytowanego przedmiotu
 *
 * @param ch gracz przegl¹daj¹cy terminal
 */
/*void stock_show_items_details( CHAR_DATA * ch )
{
    STOCK_EXCHANGE_DATA	*	pStock	= NULL;
	AUCTION_DATA *			pAuction= NULL;
	OBJ_DATA *				pObject = NULL;
	char					buf[MIL];

    pStock = get_stock_exchange(ch);
	pAuction = (AUCTION_DATA *)ch->desc->olc_editing;
	pObject			= pAuction->item;
	if ( !pObject && (!pAuction->item_collected || pAuction->quantity == 0))
	{
		bug("Missing auctioned pObject pointer");
		pager_printf( ch, "Nast¹pi³ b³¹d. Gdzieœ zagin¹³ licytowany obiekt. Zg³oœ to do koderów." NL);
		ch->pcdata->line_nr = STOCK_STATE_MAIN;
		stock_show_terminal( ch );
		return;
	}
	if ( auction_finished( pAuction ) && !auction_creator( ch->name, pAuction )
		   && !is_auction_member( ch->name, pAuction ) )
	{
		ch_printf( ch, "Ta aukcja jest ju¿ zakoñczona i nie masz do"
				" niej dostêpu." NL );
		ch->pcdata->line_nr = STOCK_STATE_MAIN;
		stock_show_terminal( ch );
		return;
	}
	sprintf(buf,"Gie³da->Przedmioty");
	if (!pAuction->item_collected)
	{
		strcat(buf,"->");
		strcat(buf,pObject->przypadki[0]);
	}
	pager_printf( ch, PLAIN "   [" FG_CYAN "%s" PLAIN "]" NL, make_stock_state_info(buf));
	pager_printf( ch, PLAIN "   [" FG_CYAN T_LINE PLAIN "]" NL NL);
	pager_printf( ch, FG_CYAN "    Sprzedawca " PLAIN "%s" FG_CYAN "  %s" EOL EOL,
		pAuction->seller_name,
		(auction_creator(ch,pAuction)) ? "(czyli Ty)":"");

	if ( pAuction->quantity > 1 )
		pager_printf( ch, FG_CYAN "    Iloœæ egzemplarzy tego przedmiotu : " PLAIN "%d" NL, pAuction->quantity);
	if ( !pAuction->item_collected )
		auction_show_obj_stats( ch, pObject );
	else
		pager_printf( ch, FG_CYAN "    Przedmiot licytacji zosta³ ju¿ odebrany" NL);

	if ( auction_creator( ch->name, pAuction ) && pAuction->payment_collected )
		pager_printf ( ch, FG_CYAN "    Odebra³eœ ju¿ swoj¹ wygran¹" NL);

	pager_printf( ch, NL FG_CYAN "    Opis sprzedawcy:" PLAIN "%s" EOL,pAuction->desc );

	pager_printf ( ch, NL FG_CYAN "    %s : " PLAIN "%s" FG_CYAN " kredyt%s." NL NL,
		pAuction->first ? "Aktualna stawka" : "Cena wywo³awcza to",
		advitoa( pAuction->bet, true), NUMBER_SUFF( pAuction->bet, "kê", "ki", "ek" ) );

	pager_printf( ch, NL NL );
	show_all_betters(ch, pAuction);

	if ( auction_creator( ch->name, pAuction ) )
		pager_printf( ch, NL FG_CYAN "    Twoja aukcja ");
	else    pager_printf( ch, NL FG_CYAN "    Aukcja ");
	if ( !auction_finished( pAuction ) )
		pager_printf( ch, FG_CYAN "zakoñczy siê za %s" NL,
				prepare_time(pAuction->end_time,false,false));
	else 	pager_printf( ch, FG_CYAN "zakoñczy³a siê" NL);

	pager_printf( ch, NL PLAIN "   [" FG_CYAN T_LINE PLAIN "]" NL "    " );

	if ( !auction_creator( ch->name, pAuction ) && !auction_finished( pAuction ) )
	{
		pager_printf( ch,"[" FG_CYAN "Licytuj" PLAIN "]" );
		if ( pAuction->buy_out_now )
			pager_printf( ch,"[" FG_CYAN "BuyOut" PLAIN "]" );
		if ( pAuction->quantity > 1 && is_auction_member( ch->name, pAuction ) )
			pager_printf( ch,"[" FG_CYAN "Ilosc" PLAIN "]" );
	}
	if ( IS_IMMORTAL( ch ) || ( auction_creator( ch->name, pAuction ) && !pAuction->first ) )
		pager_printf( ch,"[" FG_CYAN "Usuñ" PLAIN "]" );
	if ( auction_finished( pAuction ) &&
		( ( auction_creator( ch->name, pAuction ) && !pAuction->payment_collected )
		|| is_auction_member( ch->name, pAuction ) ) )
		pager_printf( ch,"[" FG_CYAN "Odbierz" PLAIN "]" );
	pager_printf( ch, "[" FB_YELLOW "Back" PLAIN "]" NL NL);
}
*/

void stock_show_new_offert(CHAR_DATA * ch)
{
	int number = 0;

	AUCTION_DATA *	pAuction = (AUCTION_DATA*)ch->desc->olc_editing;
	ROOM_INDEX_DATA * pRoom = NULL;

	pager_printf( ch, PLAIN "   [" FG_CYAN "%s" PLAIN "]" NL, make_stock_state_info("Gie³da->Nowa oferta") );
	pager_printf( ch, PLAIN "   [" FG_CYAN T_LINE PLAIN "]" NL );
	pager_printf( ch, NL "   " FG_CYAN "Typ aukcji" NL);
	pager_printf( ch, PLAIN "    [%s]" FG_CYAN "%-10s"
			PLAIN "[%s]" FG_CYAN "%-10s" PLAIN "[%s]" FG_CYAN "%-10s"
			PLAIN "[%s]" FG_CYAN "%-10s" PLAIN "[%s]" FG_CYAN "%-10s" NL NL,
			pAuction->type == AUCTION_ITEM ? "x":" ","Item",
			pAuction->type == AUCTION_SHIP ? "x":" ","Statek",
			pAuction->type == AUCTION_MATERIAL ? "x":"|","Materia³",
			pAuction->type == AUCTION_PRODUCT ? "x":"|","Produkt",
			pAuction->type == AUCTION_HOUSE ? "x":"|","Dom");

	if ( pAuction->type == AUCTION_HOUSE )
	{
		number = atoi( pAuction->stock_name );
		pRoom = get_room_index(  number );
		if ( !pRoom )
		{
			bug( "AUCTION - Error in finding house or wrongly stored house coords");
		}
		else
		{
			pager_printf( ch, FG_CYAN "   %-25s%-15s%-10s" NL,"Mieszkanie na planecie :"," ","Dzielnica");
			pager_printf( ch, PLAIN "    [ " FG_YELLOW "%-30s" PLAIN " ]",
					pRoom->area->planet ? pRoom->area->planet->name : " Brak " );
			pager_printf( ch,"     " PLAIN "[ " FG_CYAN "%s" PLAIN " ]" NL NL,pRoom->area->name );
		}
	}
	else
	{
		pager_printf( ch, FG_CYAN "   %-25s%-20s%-10s" NL,"Licytowany towar:"," ","Iloœæ:");
		pager_printf( ch, PLAIN "    [ " FG_YELLOW "%-30s" PLAIN " ]",pAuction->stock_name);
		pager_printf( ch,"          " PLAIN "[ " FG_CYAN "%s" PLAIN " ]" NL NL,advitoa(pAuction->quantity,true) );
	}

	pager_printf( ch, FG_CYAN "   %-25s%-20s%-20s" NL, "Cena wywo³awcza:"," ","Cena BuyOut:");
	pager_printf( ch,PLAIN "    [ " FG_CYAN "%-15s" PLAIN " ]%-25s",
		advitoa(pAuction->starting_price,true)," "	);
	pager_printf( ch,PLAIN "[ " FG_CYAN "%s" PLAIN " ]" NL NL,
		pAuction->buy_out_now ? advitoa(pAuction->buy_out_now,true) : "Brak");

	pager_printf( ch, FG_CYAN "   Czas trwania aukcji:");
	if (pAuction->duration) pager_printf( ch, "          Czyli skoñczy siê :");
	pager_printf( ch, NL PLAIN "    [ " FG_CYAN "%d d, %d h" PLAIN " ]" , pAuction->duration/24, pAuction->duration%24);
	if (pAuction->duration)
	{
		struct	tm	*tms;
      	tms = localtime(&current_time);
		tms->tm_mday+=pAuction->duration/24;
		tms->tm_hour+=pAuction->duration%24;
		pAuction->end_time = mktime(tms);
      	tms = localtime(&pAuction->end_time);
		pager_printf( ch, PLAIN "                  [ " FG_CYAN "%s%d-%s%d-%d , %s%d:%s%d " PLAIN " ]" ,
			tms->tm_mday<10? "0":"",tms->tm_mday, tms->tm_mon<10 ? "0":"", tms->tm_mon+1,
			tms->tm_year+1900,
			tms->tm_hour<10 ? "0":"", tms->tm_hour, tms->tm_min<10 ? "0":"", tms->tm_min);
	}

	pager_printf( ch, NL NL FG_CYAN "   %-25s" NL PLAIN "    [%s]" FG_CYAN "%-13s"
			PLAIN "[%s]" FG_CYAN "%-13s" NL NL,
			"Forma p³atno¶ci", " ","Przelew","x","Gotówka");

	pager_printf( ch, FG_CYAN "   %-25s" NL PLAIN "  %s" NL NL,
			"Opis sprzedawcy :", pAuction->desc);
	pager_printf( ch, NL PLAIN "   [" FG_CYAN T_LINE PLAIN "]" NL );

	pager_printf( ch,
			PLAIN "     [" FG_CYAN "Typ" PLAIN "][" FG_CYAN "Towar" PLAIN
			"][" FG_CYAN "Ilo¶æ" PLAIN "][" FG_CYAN "Cena" PLAIN "]["
			FG_CYAN "BuyOut" PLAIN "][" FG_CYAN "Czas" PLAIN "]"
			"[" FG_CYAN "P³atno¶æ" PLAIN "][" FG_CYAN "Opis" PLAIN "]["
			FG_YELLOW "Send" PLAIN "][" FG_YELLOW "Erase" PLAIN "]["
			FB_YELLOW "Back" PLAIN "]" NL NL);
}

void stock_show_planet_info(CHAR_DATA * ch)
{
	PLANET_DATA	* pPlanet = ch->in_room->area->planet;
	DEPOSIT_DATA	* pDeposit = NULL;

	if (!pPlanet)
	{
		pager_printf(ch, "Ta gie³da jest niepowi¹zana z ¿adn¹ planeta" NL);
		pager_printf(ch, "Zgloœ to do Builderow" NL);
		return;
	}

	pager_printf( ch, NL PLAIN "   [" FG_CYAN T_LINE PLAIN "]" NL NL
			"   " FG_CYAN "  Planeta pod rz¹dami :" NL);
	pager_printf( ch, PLAIN "      [ " FG_CYAN "%-25s" PLAIN " ]" NL NL,
					strip_colors( pPlanet->governed_by ?
						CLANNAME(pPlanet->governed_by) : "" ,25));

	pager_printf( ch, FG_CYAN "     Iloœæ mieszkañców :" NL
			PLAIN "      [ " FG_CYAN "%-6d" PLAIN " ]" NL NL, pPlanet->population);

	pager_printf( ch, FG_CYAN "     Z³o¿a minera³ów :" NL);
	if (pPlanet->first_deposit)
		FOREACH(pDeposit,pPlanet->first_deposit)
		{

			pager_printf(ch, FG_YELLOW "        %-25s     "
						FG_CYAN "[ " PLAIN "%-6d " FG_CYAN "]" NL,
						strip_colors(pDeposit->material_name,25), pDeposit->daily_mining);
		}
	pager_printf(ch, NL);
	pager_printf( ch, NL PLAIN "   [" FG_CYAN T_LINE PLAIN "]" NL );
	pager_printf( ch, PLAIN "         [" FB_YELLOW "Back" PLAIN "]" NL NL);
}

/**
* @brief Draws terminal button with specified text
*
* @param ch Person which looks into this terminal
*
* @param text Button name
*
* @param color Button color
*
*/
void terminal_button(CHAR_DATA * ch, char * text, char * color)
{
	pager_printf( ch, FG_CYAN "[");
	pager_printf( ch, color );
	pager_printf( ch,"%s" PLAIN "]",text);
}

void stock_show_details( CHAR_DATA * ch )
{
	AUCTION_DATA *			pAuction= NULL;
	OBJ_DATA *				pObject = NULL;
	SHIP_DATA *				pShip	= NULL;
	char					buf[MIL];
	int						type	= 0;

	pAuction = (AUCTION_DATA *)ch->desc->olc_editing;
	type = pAuction->type;

	if ( auction_finished( pAuction ) && !auction_creator( ch->name, pAuction )
		   && !is_auction_member( ch->name, pAuction ) )
	{
		ch_printf( ch, "Ta aukcja jest ju¿ zakoñczona i nie masz do"
				" niej dostêpu." NL );
		if ( pAuction->type == AUCTION_ITEM )
			ch->pcdata->line_nr = STOCK_STATE_ITEMS;
		else if ( pAuction->type == AUCTION_SHIP )
			ch->pcdata->line_nr = STOCK_STATE_SHIPS;
		stock_show_terminal( ch );
		return;
	}

	switch ( type )
	{
		case AUCTION_ITEM:
			pObject	= pAuction->item;
			if ( !pObject && (!pAuction->item_collected || pAuction->quantity == 0))
			{
				bug("Missing auctioned pObject pointer");
				pager_printf( ch, "Nast¹pi³ b³¹d. Gdzieœ zagin¹³ licytowany obiekt. Zg³oœ to do koderów." NL);
				ch->pcdata->line_nr = STOCK_STATE_ITEMS;
				stock_show_terminal( ch );
				return;
			}
			sprintf(buf,"Gie³da->Przedmioty");
			if (!pAuction->item_collected)
			{
				strcat(buf,"->");
				strcat(buf,pObject->przypadki[0]);
			}
			break;
		case AUCTION_SHIP:
			pShip = get_ship( pAuction->stock_name );
			if ( !pShip && (!pAuction->item_collected || pAuction->quantity == 0))
			{
				bug("Missing auctioned pShip pointer");
				pager_printf( ch, "Nast¹pi³ b³¹d. Gdzieœ zagin¹³ licytowany obiekt. Zg³oœ to do koderów." NL);
				ch->pcdata->line_nr = STOCK_STATE_SHIPS;
				stock_show_terminal( ch );
				return;
			}
			sprintf(buf,"Gie³da->Statki");
			if (!pAuction->item_collected)
			{
				strcat(buf,"->");
				strcat(buf,pShip->name);
			}
			break;
		default:
			break;
	}
	pager_printf( ch, PLAIN "   [" FG_CYAN "%s" PLAIN "]" NL, make_stock_state_info(buf));
	pager_printf( ch, PLAIN "   [" FG_CYAN T_LINE PLAIN "]" NL NL);
	pager_printf( ch, FG_CYAN "    Sprzedawca " PLAIN "%s" FG_CYAN "  %s" EOL EOL,
		pAuction->seller_name,
		(auction_creator(ch->name,pAuction)) ? "(czyli Ty)":"");

	if ( pAuction->quantity > 1 )
		pager_printf( ch, FG_CYAN "    Ilo¶æ egzemplarzy tego przedmiotu : " PLAIN "%d" NL, pAuction->quantity);

	if ( pAuction->item_collected )
		pager_printf( ch, FG_CYAN "    Przedmiot licytacji zosta³ ju¿ odebrany" NL);
	else if ( pAuction->type == AUCTION_ITEM )
		auction_show_obj_stats( ch, pObject );
	else if ( pAuction->type == AUCTION_SHIP )
		auction_show_ship_info( ch, pShip );

	if ( auction_creator( ch->name, pAuction ) && pAuction->payment_collected )
		pager_printf ( ch, FG_CYAN "    Odebra³e¶ ju¿ swoj± wygran±" NL);

	pager_printf( ch, NL FG_CYAN "    Opis sprzedawcy:" PLAIN "%s" EOL,pAuction->desc );

	pager_printf ( ch, NL FG_CYAN "    %s : " PLAIN "%s" FG_CYAN " kredyt%s." NL NL,
		pAuction->first ? "Aktualna stawka" : "Cena wywo³awcza to",
		advitoa( pAuction->bet, true), NUMBER_SUFF( pAuction->bet, "kê", "ki", "ek" ) );

	pager_printf( ch, NL NL );
	show_all_betters(ch, pAuction);

	if ( auction_creator( ch->name, pAuction ) )
		pager_printf( ch, NL FG_CYAN "    Twoja aukcja ");
	else    pager_printf( ch, NL FG_CYAN "    Aukcja ");
	if ( !auction_finished( pAuction ) )
		pager_printf( ch, FG_CYAN "zakoñczy siê za %s" NL,
				prepare_time(pAuction->end_time,false,false));
	else 	pager_printf( ch, FG_CYAN "zakoñczy³a siê" NL);

	pager_printf( ch, NL PLAIN "   [" FG_CYAN T_LINE PLAIN "]" NL "    " );

	if ( !auction_creator( ch->name, pAuction ) && !auction_finished( pAuction ) )
	{
		pager_printf( ch,"[" FG_CYAN "Licytuj" PLAIN "]" );
		if ( pAuction->buy_out_now )
			pager_printf( ch,"[" FG_CYAN "BuyOut" PLAIN "]" );
		if ( pAuction->quantity > 1 && is_auction_member( ch->name, pAuction ) )
			pager_printf( ch,"[" FG_CYAN "Ilosc" PLAIN "]" );
	}
	if ( IS_IMMORTAL( ch ) || ( auction_creator( ch->name, pAuction ) && !pAuction->first ) )
		pager_printf( ch,"[" FG_CYAN "Usuñ" PLAIN "]" );
	if ( auction_finished( pAuction ) &&
		( ( auction_creator( ch->name, pAuction ) && !pAuction->payment_collected )
		|| is_auction_member( ch->name, pAuction ) ) )
		pager_printf( ch,"[" FG_CYAN "Odbierz" PLAIN "]" );
	pager_printf( ch, "[" FB_YELLOW "Back" PLAIN "]" NL NL);
}

void stock_show_list(CHAR_DATA * ch )
{
    STOCK_EXCHANGE_DATA	*	pStock	= NULL;
	AUCTION_DATA *			pAuction= NULL;
	int						vis_auct= 0; //ilosc aukcji ktore widzimy
	int						my_auct = 0;
	int						fin_auct = 0; //aukcje zakoñczone i nale¿¹ce do gracza
	int index = 0;

    pStock = get_stock_exchange(ch);
	pager_printf( ch, PLAIN "   [" FG_CYAN "%s" PLAIN "]" NL, make_stock_state_info("Gie³da->Lista licytowanych statków") );
	pager_printf( ch, PLAIN "   [" FG_CYAN T_LINE PLAIN "]" NL NL NL );
	FOREACH( pAuction, pStock->first)
	{
		if ( !IS_IMMORTAL( ch ) )
		{
			if	( auction_finished( pAuction ) &&
				!(is_auction_member( ch->name, pAuction ) || auction_creator( ch->name, pAuction ) ) )
					continue;
		}
		switch( ch->pcdata->line_nr )
		{
			case STOCK_STATE_SHIPS:
				if ( pAuction->type != AUCTION_SHIP )
					continue;
				break;
			case STOCK_STATE_ITEMS:
				if ( pAuction->type != AUCTION_ITEM )
					continue;
				break;
			default:
				break;
		}
		index++;
		if ( auction_creator( ch->name, pAuction ) && !auction_finished( pAuction ) )
			my_auct++;
		if ( auction_finished( pAuction ) &&
				( ( auction_creator( ch->name, pAuction ) && !pAuction->payment_collected )
				|| ( is_auction_member( ch->name, pAuction ) ) ) )
			fin_auct++;
		vis_auct++;
		//wyswiatlamy nag³ówek je¶li widzimy przynajmniej jedn± aukcjê
		if ( vis_auct == 1 )
		{
			pager_printf( ch, "     " PLAIN "[" FG_CYAN "%-2s" PLAIN "]["
					FG_CYAN "%-20s" PLAIN "][" FG_CYAN "%-3s" PLAIN "]["
					FG_CYAN "%-3s" PLAIN "][" FG_CYAN "%-11s" PLAIN "]["
					FG_CYAN "%-1s" PLAIN "][" FG_CYAN "%-12s" PLAIN "]"
					FG_CYAN NL "    --------------------------"
					"------------------------------------------------" EOL,
					"Nr","Nazwa przedmiotu","Ilo","Lic","Cena","B","Do koñca");
		}
		show_1_auction_list(ch, pAuction, index);
	}

	if ( !vis_auct || !pStock->first)	//poniewa¿ niektórych aukcji nie widzimy, bo siê skoñczy³y, a nie s± nasze
	{
		pager_printf( ch,NL NL NL NL NL PLAIN "%-26s%s" EOL NL NL NL ,"","Nie toczy siê ¿adna licytacja" NL );
		pager_printf( ch, NL PLAIN "   [" FG_CYAN T_LINE PLAIN "]" NL );
	}
	else if (vis_auct<10)
	{
		for(index=0;index<10-vis_auct;index++)
			pager_printf( ch, NL);
		pager_printf( ch, PLAIN "   [" FG_CYAN T_LINE PLAIN "]" NL );
	}
	pager_printf( ch, "     " );
	if (vis_auct)
		pager_printf( ch, PLAIN "[" FG_CYAN "Pokaz" PLAIN "]" );
	if ( fin_auct )
		pager_printf( ch,"[" FG_CYAN "Odbierz" PLAIN "]" );
	if ( my_auct || ( IS_IMMORTAL( ch ) && vis_auct ) )
	{
		pager_printf( ch,"[" FG_CYAN "Zakoñcz" PLAIN "]" );
		pager_printf( ch,"[" FG_CYAN "Usuñ" PLAIN "]" );
	}
	pager_printf( ch, "[" FB_YELLOW "Back" PLAIN "]" NL NL);
}


void stock_show_terminal(CHAR_DATA *ch)
{
	int state = ch->pcdata->line_nr;
	prepare_terminal_header(ch);

	if ( ch->pcdata->line_nr ==  STOCK_STATE_MAIN)
	{
		pager_printf( ch, PLAIN "   [" FG_CYAN "%s" PLAIN "]" NL, make_stock_state_info("G³ówny ekran") );
		pager_printf( ch, PLAIN "   [" FG_CYAN T_LINE PLAIN "]" NL );
		pager_printf( ch, NL PLAIN "    [ " MOD_BOLD "1" FG_CYAN "  : %-30s" PLAIN "]" NL
				"    [ " MOD_BOLD "2" FG_CYAN "  : %-30s" PLAIN "]" NL
				"    [ " MOD_BOLD "3" FG_CYAN "  : %-30s" PLAIN "]" NL
				"    [ " MOD_BOLD "4" FG_CYAN "  : %-30s" PLAIN "]" NL
				"    [ " MOD_BOLD "5" FG_CYAN "  : %-30s" PLAIN "]" NL
				"    [ " MOD_BOLD "6" FG_CYAN "  : %-30s" PLAIN "]" NL
				"    [ " MOD_BOLD "7" FG_CYAN "  : %-30s" PLAIN "]" NL
				"    [ " MOD_BOLD "8" FG_CYAN "  : %-30s" PLAIN "]" NL
				"    [ " MOD_BOLD "9" FG_CYAN "  : %-30s" PLAIN "]" NL
				"    [ " MOD_BOLD "10" FG_CYAN " : %-30s" PLAIN "]" NL
				"    [ " MOD_BOLD "11" FG_CYAN " : %-30s" PLAIN "]" NL NL,
				"Oferty sprzeda¿y przedmiotów",
				"Oferty sprzeda¿y statków",
				"Oferty sprzeda¿y materia³ów",
				"Oferty sprzedazy produktów",
				"Oferty sprzedazy mieszka¿",
				"Oferty kontraktów terminowych",
				"Oferty zleceñ prywatnych",
				"Przygotuj now± oferte",
				"Informacje o planecie",
				"Moje aukcje",
				"Wyjd¿"
				);
		pager_printf( ch, PLAIN "   [" FG_CYAN T_LINE PLAIN "]" NL );
	}
	else if ( state ==  STOCK_STATE_ITEMS ||
			 state ==  STOCK_STATE_SHIPS )
		stock_show_list(ch);
	else if ( state ==  STOCK_STATE_SHIPS_DETAIL ||
			state ==  STOCK_STATE_ITEMS_DETAIL )
		stock_show_details(ch);
	else if ( state ==  STOCK_STATE_NEW_OFFER)
		stock_show_new_offert(ch);
	else if ( state ==  STOCK_STATE_PLANET_INFO)
		stock_show_planet_info(ch);
}

void stock_prepare_new_offert(CHAR_DATA * ch)
{
	STOCK_EXCHANGE_DATA *	pStock = get_stock_exchange(ch);
	AUCTION_DATA *			pAuction = NULL;
	//najpierw sprawdzamy czy edytuje jakas oferte
	if ( ch->desc->olc_editing )
		return;

	//teraz sprawdzamy czy nie ma przypadkiem innej nie opublikowanej oferty
	FOREACH(pAuction, pStock->first_new_offert)
		if (auction_creator(ch->name,pAuction))
		{
			ch->desc->olc_editing	= (void*)pAuction;
			return;
		}

	pAuction = new_auction();
	pAuction->end_time=0;
	STRDUP( pAuction->seller_name, ch->name);
	ch->desc->olc_editing	= (void*)pAuction;
	LINK( pAuction, pStock->first_new_offert, pStock->last_new_offert, next, prev );
}

//////////////////////////////////////////////////////////////////////////////

void stock_main_interpret( CHAR_DATA * ch, char * argument)
{
	if ( !str_prefix( argument, "" ) )
	{
		stock_show_terminal(ch);
		return;
	}
	else if (  is_number(argument) && atoi(argument) == 1)
	{
		ch->pcdata->line_nr = STOCK_STATE_ITEMS;
		stock_show_terminal(ch);
		return;
	}
	else if (  is_number(argument) && atoi(argument) == 2)
	{
		ch->pcdata->line_nr = STOCK_STATE_SHIPS;
		stock_show_terminal(ch);
		return;
	}
	else if (  is_number(argument) && atoi(argument) == 3)
	{
//		ch->pcdata->line_nr = STOCK_STATE_SELL_MATERIALS;
		stock_show_terminal(ch);
		return;
	}
	else if (  is_number(argument) && atoi(argument) == 4)
	{
//		ch->pcdata->line_nr = STOCK_STATE_BUY_MATERIALS;
		stock_show_terminal(ch);
		return;
	}
	else if ( !/*spaybycaltr*/str_prefix( argument, "nowa") || ( is_number(argument) && atoi(argument) == 8))
	{
		stock_prepare_new_offert(ch);
		ch->pcdata->line_nr = STOCK_STATE_NEW_OFFER;
		stock_show_terminal(ch);
		return;
	}
	else if ( !str_prefix( argument, "planet") || ( is_number(argument) && atoi(argument) == 6))
	{
		ch->pcdata->line_nr = STOCK_STATE_PLANET_INFO;
		stock_show_terminal(ch);
		return;
	}
	else if ( is_number(argument) && atoi(argument) == 7 )
	{
		stock_show_terminal(ch);
		return;
	}
	else if ( ( is_number(argument) && atoi(argument) == 11 ) || !str_prefix( argument, "done" ) )
    {
		ch->desc->olc_editing	= NULL;
		ch->desc->connected		= CON_PLAYING;
		send_to_char( "Wylogowujesz si¿ z terminala gie¿dowego." NL, ch );
        return;
    }
	interpret( ch, argument);

}

bool stock_new_offert_test_object( CHAR_DATA * ch, OBJ_DATA * pObject)
{
	if (pObject->timer > 0)
	{
		send_to_char ("Nie mo¿esz wystawiæ na aukcjê tak "
				" nietrwa³ego przedmiotu." NL, ch);
		return false;
	}
	if( IS_OBJ_STAT( pObject, ITEM_PERSONAL ) )
	{
		send_to_char( "Zaufaj. Nie chcesz siê tego pozbyæ" NL, ch );
		return false;
	}

	if ( pObject->inquest)
	{
		send_to_char( "Zaufaj. Bêdziesz potrzebowa³ tego przedmiotu"
				"by ukoñczyæ swój quest" NL, ch );
		return false;
	}
	return true;
}

void stock_new_offert_interpret( CHAR_DATA * ch, char * argument)
{
	AUCTION_DATA *		pAuction =NULL;
	OBJ_DATA *			pObject = NULL;
	SHIP_DATA *			pShip = NULL;
	ROOM_INDEX_DATA *	pRoom = NULL;
	STOCK_EXCHANGE_DATA * pStock= NULL;
	int64				price=0;
	int					number=0;
	int64				quantity=0;
	char				arg[MSL];
	char				arg1[MSL];
	char				arg2[MSL];
	pAuction = (AUCTION_DATA*)ch->desc->olc_editing;
	pStock = get_stock_exchange( ch );
	if ( !pStock )
	{
		bug("This auction room isn't connected with stockmarket [%d, %s, %s]",ch->in_room->vnum,
			ch->in_room->area->name, ch->in_room->area->planet ?  ch->in_room->area->planet->name: "No planet" );
		return;
	}

	strcpy( arg, argument );
	smash_tilde( argument );
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );

	if ( !str_prefix( arg1, "typ") )
	{
		if ( !str_prefix ( arg2, ""))
		{
			pager_printf( ch, "Musisz wybraæ typ licytowanego przedmiotu" NL );
			pager_printf( ch, "Na dzieñ dzisiejszy funckjonuje tylko handel" NL );
			pager_printf( ch, "przedmiotami" NL NL );
			return;
		}
		else if ( !str_prefix( arg2, "przedmiot" ) || !str_prefix( arg2, "item" ))
		{
			//przy zmienianiu typu tocz?cej si? aukcji musimy te? usun? nazwy
			//obiektów, które ju¿ zosta?y na niej wystawione
			pAuction->type = AUCTION_ITEM;
			STRDUP(pAuction->stock_name, "");
			pager_printf( ch, "Zmieniasz typ utworzonej aukcji" NL);
			return;
		}
		else if ( !str_prefix( arg2, "statek" ) )
		{
		//przy zmienianiu typu tocz¹cej siê aukcji musimy te¿ usun¹æ nazwy
		//obiektów, które ju¿ zosta³y na niej wystawione

			pAuction->type = AUCTION_SHIP;
			STRDUP(pAuction->stock_name, "");
			pager_printf( ch, "Zmieniasz typ utworzonej aukcji" NL);
			return;
		}
		else if ( !str_prefix( arg2, "dom" ) )
		{
		//przy zmienianiu typu tocz¹cej siê aukcji musimy te¿ usun¹æ nazwy
		//obiektów, które ju¿ zosta³y na niej wystawione

			pAuction->type = AUCTION_HOUSE;
			STRDUP(pAuction->stock_name, "");
			pager_printf( ch, "Zmieniasz typ utworzonej aukcji" NL);
			return;
		}
		else if ( !str_prefix( arg2, "material" ) )
		{
		//przy zmienianiu typu tocz¹cej siê aukcji musimy te¿ usun¹æ nazwy
		//obiektów, które ju¿ zosta³y na niej wystawione

			pAuction->type = AUCTION_MATERIAL;
			STRDUP(pAuction->stock_name, "");
			pager_printf( ch, "Zmieniasz typ utworzonej aukcji" NL);
			return;
		}
		else if ( !str_prefix( arg2, "produkt" ) )
		{
		//przy zmienianiu typu tocz¹cej siê aukcji musimy te¿ usun¹æ nazwy
		//obiektów, które ju¿ zosta³y na niej wystawione

			pAuction->type = AUCTION_PRODUCT;
			STRDUP(pAuction->stock_name, "");
			pager_printf( ch, "Zmieniasz typ utworzonej aukcji" NL);
			return;
		}
	}
	else if ( !str_prefix( arg1, "towar") )
	{
		//przejsc trzeba po wszystkich przedmiotach znajdujacych sie
		//w posiadaniu gracza
		switch(pAuction->type)
		{
			case AUCTION_ITEM:
			{
				if (arg2[0]=='\0')
				{
					pager_printf( ch, "Co chcesz licytowac ?" NL);
					return;
				}
				pObject = get_obj_carry (ch, arg2); // does char have the item ?
				if (!pObject)
				{
					pager_printf( ch, "Nie posiadasz takiego przedmiotu." NL);
					return;
				}
				if ( stock_new_offert_test_object(ch, pObject ) )
				{
					pAuction->quantity = 1;
					STRDUP( pAuction->stock_name, pObject->name );
					pAuction->starting_price	= pObject->cost;
					pAuction->bet				= pAuction->starting_price;
					pAuction->item				= pObject;
					pager_printf(ch, "Wystawiasz na aukcje %s" NL,pAuction->stock_name);
				}
				return;
			}
			case AUCTION_MATERIAL:
			{
				pager_printf( ch, "Licytowanie materia?ów jest na razie wy??czone." NL);
				break;
			}
			case AUCTION_HOUSE:
			{
				//
				if ( ch->plr_home )
				{
					if ( ch->plr_home->area->planet )
					{
						pager_printf( ch, " Twoje mieszkanie znajduje siê na planecie %s i w dzielnicy %s." NL,
							ch->plr_home->area->planet->name, ch->plr_home->area->name);
					}
					else
					{
						pager_printf( ch, " Twoje mieszkanie znajduje siê w dzielnicy %s." NL,
							ch->plr_home->area->name);
					}
					STRDUP( pAuction->stock_name, "        ");
					sprintf( pAuction->stock_name, "%d",ch->plr_home->vnum);
					pager_printf( ch, "Wystawiasz na licytacje swoje mieszkanie." NL);
					return;
				}
				else
				{
					pager_printf( ch, "Nie jesteœ w³aœcicielem ¿adnego mieszkania" NL);
					return;
				}
				break;
			}
			case AUCTION_PRODUCT:
			{
				pager_printf( ch, "Licytowanie produktów jest na razie wy??czone." NL);
				break;
			}
			case AUCTION_SHIP:
			{
				pAuction->quantity = 1;
				if (arg2[0]=='\0')
				{
					number =0;
					send_to_pager( "Oto lista statków posiadanych przez ciebie lub twoj± organiacjê:" NL, ch );
					send_to_pager( FB_WHITE "Statek                              W³a¶ciciel" NL,ch);

					for ( pShip = first_ship; pShip; pShip = pShip->next )
					{
						if ( str_cmp(pShip->owner, ch->name) )
							continue;
						if (pShip->type == MOB_SHIP) sprintf( arg1, PLAIN );
						else if (pShip->type == SHIP_REPUBLIC) sprintf( arg1, FG_RED );
						else if (pShip->type == SHIP_IMPERIAL) sprintf( arg1, FG_GREEN );
						else if (pShip->type == SHIP_YUUZHAN) sprintf( arg1, FG_YELLOW );
						else if (pShip->type == SHIP_PIRATE) sprintf( arg1, FG_MAGENTA );
						else sprintf( arg1, FB_BLUE );

						if  ( pShip->in_room )
					 		pager_printf( ch, "%s%s - %s" EOL, arg1,SHIPNAME( pShip ), pShip->in_room->name );
						else
							pager_printf( ch, "%s%s" EOL, arg1,SHIPNAME( pShip ) );
						number++;
					}

					if ( !number )
						send_to_pager( "Nie posiadasz ¿adnego statku." NL, ch );
					return;
				}

				pShip = get_ship(arg2);
				if (!pShip)
				{
					pager_printf( ch, "Niestety, nie posiadasz takiego statku." NL);
					return;
				}
				STRDUP( pAuction->stock_name, pShip->name);
				price = get_ship_value(pShip);
				pAuction->starting_price = price;
				pager_printf( ch, "Wystawiasz na licytatcje statek %s." NL, pShip->name);
				return;
			}
			default:
				break;
		}
	}
	else if ( !str_prefix( arg1, "ilosc" ) )
	{
		if ( !is_number(arg2) )
		{
			pager_printf( ch, "Podaj ile sztuk tego towaru chcesz sprzedaæ ( od 0 w góre)" NL NL);
			return;
		}
		number = atoi(arg2);
		switch(pAuction->type)
		{
			case AUCTION_ITEM:
			{
				pObject = get_obj_carry (ch, pAuction->stock_name); // does char have the item ?
				//najpierw - czy posiada ten przedmiot
				if (!pObject)
				{
					//przedmiot jakos 'zagin??' - trzeba uporz?dkowa? aukcje
					pager_printf( ch, "Wystaw najpierw przedmiot, który chcesz sprzedaæ, a dopiero"
					" potem ustal ile" NL " jego sztuk chcesz sprzedaæ." NL NL);
					return;
				}
				if ( number < 1 )
				{
					pager_printf( ch, "Musisz wystawiæ do sprzedania przynajmniej jednæ sztukê %d" NL NL,
						pAuction->item->przypadki[1]);
					return;
				}
				quantity = pObject->count; //ile obiektów jeszcze jest w posiadaniu gracza
				if (quantity < number )
				{
					pager_printf( ch, "Masz tylko %lld sztuk%s %s." NL,
						quantity,
						NUMBER_SUFF(quantity,"?","i",""),
						pAuction->item->przypadki[1]);
					return;
				}
				if ( pAuction->quantity > number)
					pager_printf( ch, "Zmniejszasz iloœæ przedmiotów na sprzeda¿" NL);
				else if ( pAuction->quantity < number)
					pager_printf( ch, "Zwiêkszasz iloœæ przedmiotów na sprzeda¿" NL);
				pAuction->quantity = number;
				return;
			}
			case AUCTION_MATERIAL:
			{
				pager_printf( ch, "Licytowanie wiêkszych iloœci materia³ów jest na razie wy³¹czone." NL);
				break;
			}
			case AUCTION_HOUSE:
			{
				pager_printf( ch, "Licytowanie wiêkszych iloœci mieszkañ jest na razie wy³¹czone." NL);
				break;
			}
			case AUCTION_PRODUCT:
			{
				pager_printf( ch, "Licytowanie wiêkszych iloœci produktów jest na razie wy³¹czone." NL);
				break;
			}
			case AUCTION_SHIP:
			{
				pager_printf( ch, "Licytowanie wiêkszych iloœci statków jest na razie wy³¹czone." NL);
				break;
			}
			default:
				break;
		}
		return;
	}
	else if ( !str_prefix( arg1, "cena" ) )
	{

		switch(pAuction->type)
		{
			case AUCTION_ITEM:
			{
				pObject = get_obj_carry (ch, pAuction->stock_name); // does char have the item ?
				//najpierw - czy posiada ten przedmiot
				if (!pObject)
				{
					//przedmiot jakos 'zagin??' - trzeba uporz?dkowa? aukcje
					pager_printf( ch, "Wystaw najpierw przedmiot, który chcesz sprzedaæ, a dopiero"
							" potem ustal ile" NL " za niego chcesz." NL NL);
					return;
				}

				if ( !is_number(arg2) )
				{
					pager_printf( ch, "Podaj cene wywo³awcz¹." NL);
					return;
				}
				number = atoi(arg2);
				if (number <= 0)
				{
					pager_printf( ch, "Chcesz to odda? za darmo ?\n");
					return;
				}
				else if (pAuction->starting_price == number)
				{
					pager_printf( ch, "Przecie? w?a¶nie taka jest cena "
							" wywo?awcza \n");
					return;
				}
				else
				{
					pAuction->starting_price = number;
					pager_printf( ch, "Zmieniasz cen? wywo?awcz± na"
							" %d kredyt%s.\n",
							number,NUMBER_SUFF(number,"ke","ki","ek"));
					return;
				}
			}
			case AUCTION_MATERIAL:
			{
				pager_printf( ch, "Licytowanie wiêkszych iloœci materia³ów jest na razie wy³¹czone." NL);
				break;
			}
			case AUCTION_HOUSE:
			{
				pager_printf( ch, "Licytowanie wiêkszych iloœci mieszkañ jest na razie wy³¹czone." NL);
				break;
			}
			case AUCTION_PRODUCT:
			{
				pager_printf( ch, "Licytowanie wiêkszych iloœci produktów jest na razie wy³¹czone." NL);
				break;
			}
			case AUCTION_SHIP:
			{
				pShip = get_ship(pAuction->stock_name);
				//najpierw - czy posiada ten przedmiot
				if (!pShip)
				{
					//przedmiot jakos 'zagin??' - trzeba uporz?dkowa? aukcje
					pager_printf( ch, "Wystaw najpierw statek, który chcesz sprzedaæ, a dopiero"
							" potem ustal ile" NL " za niego chcesz." NL NL);
					return;
				}

				if ( !is_number(arg2) )
				{
					pager_printf( ch, "Podaj cene wywo³awcz¹." NL);
					return;
				}
				number = atoi(arg2);
				if (number <= 0)
				{
					pager_printf( ch, "Chcesz to odda? za darmo ?\n");
					return;
				}
				else if (pAuction->starting_price == number)
				{
					pager_printf( ch, "Przecie? w?a¶nie taka jest cena "
							" wywo?awcza \n");
					return;
				}
				else
				{
					pAuction->starting_price = number;
					pager_printf( ch, "Zmieniasz cen? wywo?awcz± na"
							" %d kredyt%s.\n",
							number,NUMBER_SUFF(number,"ke","ki","ek"));
					return;
				}
			}
			default:
				break;
		}
	}
	else if ( !str_prefix( arg1, "buyout" ) )
	{
		if (is_number(arg2))
		{
			number = atoi(arg2);
			if (number<=pAuction->starting_price)
			{
				pager_printf( ch, "Kwota wykupu musi byæ wy¿sza od"
						" ceny wywo³awczej." NL);
				return;
			}
			else
			{
				pAuction->buy_out_now = number;
				pager_printf(ch, "Ustawiasz kwote natychmiastowego wykupu "
						"na %d kredyt%s\n",number,
						NUMBER_SUFF(number,"ke","ki","ek"));
				return;
			}
		}
		else
		{
			pager_printf( ch, "Jako parametr podaj twoj¹ kwote wykupu.\n");
			return;
		}
		return;
	}
	else if ( !str_prefix( arg1, "czas" ) )
	{
		argument = one_argument( arg, arg1 );
		number = parse_atodate(ch,argument);
		if (number > 0 )
		{
			pAuction->duration = number;
			pager_printf( ch, "Ustawiasz czas trwania aukcji na %d d%s i %d godzin%s" NL,
					number/24,NUMBER_SUFF(number/24,"zieñ","ni","ni"),
					number%24,NUMBER_SUFF(number%24,"ê","y","") );
			return;
		}
		else
			return;
	}
	else if ( !str_prefix( arg1, "opis" ) )
	{
		string_append( ch, &pAuction->desc );
		return;
	}
	else if ( !str_prefix( arg1, "send" ) )
	{
		if (!pAuction->duration)
		{
			pager_printf( ch, "Ustaw czas trwania aukcji." NL );
			return;
		}
		switch( pAuction->type)
		{
			case AUCTION_ITEM:
			{
				//najpierw testy potwierdzaj¹ce prawid³owoœæ aukcji
				pObject = get_obj_carry (ch, pAuction->stock_name); // does char have the item ?
				//najpierw - czy posiada ten przedmiot
				if (!pObject)
				{
					//przedmiot jakos 'zagin¹³' - trzeba uporz?dkowa? aukcje
					pager_printf( ch, "Obiekt, który wystawi³eœ na licytacje NIE znajduje siê w Twoim posiadaniu." NL
							"Odzyskaj obiekt, który chcesz licytowaæ lub wybierz inny." NL NL );
					return;
				}
				if ( pAuction->starting_price == 0)
				{
					pager_printf( ch, "Cena wywo³awcza musi wynosiæ przynajmniej 1 kredytkê." NL);
					return;
				}
				quantity = pObject->count; //ile obiektów jeszcze jest w posiadaniu gracza
				if (quantity < pAuction->quantity )
				{
					pager_printf( ch, "Masz tylko %lld sztuk%s %s, a zadeklarowa³e¶ chêæ sprzedania %d sztuk%s." NL,
						pAuction->quantity,
						NUMBER_SUFF(pAuction->quantity,"ê","i",""),
						pAuction->item->przypadki[1],
						NUMBER_SUFF(pAuction->quantity,"ê","i",""));
					return;
				}

				pAuction->bet = pAuction->starting_price;
				for(number=0;number<pAuction->quantity;number++)
				{
					pObject = get_obj_carry (ch, pAuction->stock_name);
					separate_obj( pObject );
					obj_from_char( pObject );
					if (number <pAuction->quantity-1)
						extract_obj( pObject );
				}
				if ( IS_SET( sysdata.save_flags, SV_AUCTION ) )
					save_char_obj( ch );
				break;
			}
			case AUCTION_SHIP:
			{
				pShip = get_ship(pAuction->stock_name);
				if ( !pShip )
				{
					//przedmiot jakos 'zagin¹³' - trzeba uporz?dkowa? aukcje
					pager_printf( ch, "Statek, który wystawi³eœ na licytacje NIE znajduje siê w Twoim posiadaniu." NL
							"Odzyskaj ten statek, który chcesz licytowaæ lub wybierz inny." NL NL );
					return;
				}
				if ( pAuction->starting_price == 0)
				{
					pager_printf( ch, "Cena wywo³awcza musi wynosiæ przynajmniej 1 kredytkê." NL);
					return;
				}
//				quantity = pObject->count; //ile obiektów jeszcze jest w posiadaniu gracza
				if (quantity < pAuction->quantity )
				{
//					pager_printf( ch, "Masz tylko %lld sztuk%s %s, a zadeklarowa³eœ chêæ sprzedania %d sztuk%s." NL,
//						pAuction->quantity,
//						NUMBER_SUFF(pAuction->quantity,"ê","i",""),
//						pAuction->item->przypadki[1],
//						NUMBER_SUFF(pAuction->quantity,"ê","i",""));
//					return;
				}

				if (number <= 0)
				{
					pager_printf( ch, "Chcesz to odda? za darmo ?\n");
					return;
				}
				else if (pAuction->starting_price == number)
				{
					pager_printf( ch, "Przecie? w?a¶nie taka jest cena "
							" wywo?awcza \n");
					return;
				}
				else
				{
					pAuction->starting_price = number;
					pager_printf( ch, "Zmieniasz cen? wywo?awcz± na"
							" %d kredyt%s.\n",
							number,NUMBER_SUFF(number,"ke","ki","ek"));
					return;
				}
				break;
			}
			case AUCTION_HOUSE:
			{
				number = atoi( pAuction->stock_name );
				pRoom = get_room_index(  number );
				if ( !pRoom )
				{
					//przedmiot jakos 'zagin¹³' - trzeba uporz?dkowa? aukcje
					pager_printf( ch, "Musisz wskazaæ, które mieszkanie chcesz sprzedaæna." NL
							"Odzyskaj ten statek, który chcesz licytowaæ lub wybierz inny." NL NL );
					return;
				}
			}
			default:
				return;
		}

		UNLINK( pAuction, pStock->first_new_offert, pStock->last_new_offert, next, prev );
		LINK( pAuction, pStock->first, pStock->last, next, prev);
		struct	tm	*tms;
      	tms = localtime(&current_time);
		tms->tm_mday+=pAuction->duration/24;
		tms->tm_hour+=pAuction->duration%24;
		pAuction->end_time = mktime(tms);
		pAuction->start_time = current_time-1;
		pager_printf( ch, "Wysy³asz dane dotycz¹ce aukcji do g³ównego terminala gie³dy" NL
				"Od teraz jest ona dostêpna dla ka¿dego innego aukcjonariusza." NL NL);
		ch->desc->olc_editing = NULL;
		sort_stock_market_auctions( pStock );
		save_stock_market( pStock );
		switch( pAuction->type)
		{
			case AUCTION_ITEM:
				ch->pcdata->line_nr = STOCK_STATE_ITEMS;
				break;
			case AUCTION_HOUSE:
//				ch->pcdata->line_nr = STOCK_STATE_ITEMS;
				break;
			case AUCTION_SHIP:
				ch->pcdata->line_nr = STOCK_STATE_SHIPS;
				break;
			case AUCTION_PRODUCT:
//				ch->pcdata->line_nr = STOCK_STATE_ITEMS;
				break;
			case AUCTION_MATERIAL:
//				ch->pcdata->line_nr = STOCK_STATE_ITEMS;
				break;
			default:
				break;
		}
		stock_show_terminal(ch);
		return;
	}
	else if ( !str_prefix( arg1, "back" ) )
	{
		ch->pcdata->line_nr = STOCK_STATE_MAIN;
		stock_show_terminal(ch);
		return;
	}
	else if ( !str_prefix( arg1, "erase" ) )
	{
		if (pAuction)
		{
			UNLINK( pAuction, pStock->first_new_offert, pStock->last_new_offert, next, prev );
			free_auction( pAuction );
		}
		ch->desc->olc_editing = NULL;
		pager_printf( ch, "Usuwasz dane dotycz¹ce aukcji z pamiêci terminala" NL NL);
		ch->pcdata->line_nr = STOCK_STATE_MAIN;
		stock_show_terminal(ch);
		return;
	}
	else if ( !str_prefix( arg1, "done" ) )
    {
		edit_done( ch, (char *)"" );
        return;
    }
	interpret( ch, arg);

}

/*void stock_items_details_interpret( CHAR_DATA * ch, char * argument )
{
	AUCTION_DATA *		pAuction = NULL;
	STOCK_EXCHANGE_DATA * pStock = NULL;
	BETS_PROGRES *		pBets = NULL;
	char				arg[MSL];
	char				arg1[MSL];
	char				arg2[MSL];
	int64				new_bet = 0;
	int					number = 0;

	strcpy( arg, argument );
	smash_tilde( argument );
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	pStock = get_stock_exchange( ch );
	pAuction = (AUCTION_DATA*)ch->desc->olc_editing;

	if ( !str_prefix( arg1, "back") || !str_prefix( arg1, "powrot" ) )
	{
		ch->pcdata->line_nr = STOCK_STATE_ITEMS;
		ch->desc->olc_editing	= NULL;
		stock_show_terminal(ch);
		return;
	}
	else if ( !str_prefix( arg1, "zakoncz" ) || !str_prefix( arg1, "finish" ) )
    {
		if ( auction_finished( pAuction ) )
		{
			huh( ch );
			return;
		}
		if ( IS_IMMORTAL( ch ) || auction_creator( ch->name, pAuction ) )
		{
			if ( IS_IMMORTAL( ch ) )
				pager_printf( ch, "Korzystaj¹c z przys³uguj¹cych Ci mocy doprowadzasz do zakoñczenia aukcji." NL NL);
			else if ( !IS_IMMORTAL( ch ) && auction_creator( ch->name, pAuction ) )
			{
				if ( !pAuction->first )
					pager_printf( ch, "Jako twórca tej aukcji mo¿esz j¹ zakoñczyæ. I tak te¿ robisz." NL);
				else
				{
					pager_printf( ch, "Ju¿ nie mo¿esz zakoñczyæ aukcji. Bierze w niej udzia³ %d licytatn%s." NL,
							pAuction->bidders_count,NUMBER_SUFF( pAuction->bidders_count, "","ów","ów") );
					return;
				}
			}
			pAuction->end_time = current_time-1;
		}
		else
		{
			huh( ch );
			return;
		}
		stock_show_terminal( ch );
		return;
    }
	else if ( !str_prefix( arg1, "usun" ) || !str_prefix( arg1, "delete" ) )
    {
		if ( !IS_IMMORTAL( ch ) )
		{
			interpret( ch, arg);
			return;
		}
		if ( !(pAuction = auction_exist( ch, arg2 , "usun¹æ", pStock) ) )
			return;
		//tutaj mamy usun¹æ aukcje
		ch->desc->olc_editing = NULL;
		ch->pcdata->line_nr =  STOCK_STATE_ITEMS;
		UNLINK( pAuction, pStock->first_new_offert, pStock->last_new_offert, next, prev );
		free_auction( pAuction );
		pager_printf( ch, "Korzystaj¹c z przys³uguj¹cych Ci mocy doprowadzasz do usuniêcia aukcji." NL NL);
		stock_show_terminal( ch );
		return;
    }
	else if ( !str_prefix( arg1, "licytuj") )
	{
		if ( auction_finished( pAuction ) || auction_creator( ch->name, pAuction ) )
		{
			huh(ch);
			return;
		}
		if ( !is_number ( arg2 ) )
		{
			pager_printf( ch, "Podaj kwotê za jak¹ chcesz licytowaæ. Ta kwota bêdzie oznacza³a górny limit" NL
					"ile pieniêdzy chcesz wydaæ na zakup tego przedmiotu" NL );
			return;
		}
		new_bet = atoi( arg2 );
		if (pAuction->first && new_bet < pAuction->first->max_price+5 )
		{
			pager_printf( ch, "Przykro mi, ale musisz postawiæ wiêcej, by przebic innych graczy." NL);
			return;
		}
		else if ( !pAuction->first && pAuction->starting_price > new_bet)
		{
			pager_printf( ch, "Musisz postawiæ przynajmniej %lld" NL ,pAuction->starting_price);
			return;
		}
		if ( ch->gold < new_bet)
		{
			pager_printf( ch, "Nie staæ ciê na taki wydatek." NL);
			return;
		}

		if (is_auction_member(ch,pAuction))
		{
			pBets = get_auction_member(ch->name, pAuction);
			if (pBets->max_price > new_bet)
			{
				pager_printf(ch, "Ta stawka jest mniejsza od Twojej wczeœniejszej." NL);
				return;
			}
			else
			{
				int diff = new_bet - pBets->max_price;
				pBets->max_price = new_bet;
				pager_printf(ch, "Twoja nowa stawka to %d kredyt%s." NL NL,
						new_bet,NUMBER_SUFF(new_bet,"ka","ki","ek"));
				ch->gold-= diff;
			}
		}
		else
		{
			pBets = new_bets();
			LINK(pBets,pAuction->first,pAuction->last,next,prev);
			STRDUP(pBets->name,ch->name);
			pBets->max_price = new_bet;
			pager_printf( ch, "Do³¹czasz do tej licytacji ze stawk¹ %d kredyt%s" NL,
					new_bet, NUMBER_SUFF(new_bet,"ki","ek","ek") );
			ch->gold -= new_bet;
		}
		sort_auction_bets(pAuction);
		save_stock_market( pStock );
		stock_show_terminal(ch);
		return;
	}
	else if ( !str_prefix( arg1, "buyout" ) )
	{
		if ( auction_finished( pAuction ) || auction_creator( ch->name, pAuction ) || !pAuction->buy_out_now )
		{
			huh( ch );
			return;
		}
		if ( pAuction->bet > pAuction->buy_out_now ||
				( pAuction->first && pAuction->first->max_price > pAuction->buy_out_now ) )
		{
			pager_printf( ch, "Licytowana cena przekroczy³a ju¿ kwote okreœlon¹ jako wykup natychmiastowy." NL );
			return;
		}
		if ( ch->gold < pAuction->buy_out_now)
		{
			pager_printf( ch, "Nie staæ ciê na taki wydatek." NL);
			return;
		}
		if (is_auction_member(ch,pAuction))
		{
			pBets = get_auction_member(ch->name, pAuction);
			int diff = pAuction->buy_out_now - pBets->max_price;
			pBets->max_price = pAuction->buy_out_now;
			pager_printf(ch, "Dokonujesz natychmiastowego wykupu." NL NL,
				pAuction->buy_out_now ,NUMBER_SUFF(pAuction->buy_out_now,"ka","ki","ek"));
			ch->gold-= diff;
		}
		else
		{
			pBets = new_bets();
			LINK(pBets,pAuction->first,pAuction->last,next,prev);
			STRDUP(pBets->name,ch->name);
			pBets->max_price = pAuction->buy_out_now;
			pBets->quantity=1;
			pager_printf( ch, "Do³¹czasz do tej licytacji ze stawk¹ %d kredyt%s" NL,
					pAuction->buy_out_now, NUMBER_SUFF(pAuction->buy_out_now,"ki","ek","ek") );
			ch->gold -= pAuction->buy_out_now;
		}

		pAuction->end_time = current_time-1;
		sort_auction_bets(pAuction);
		save_stock_market( pStock );
		stock_show_terminal(ch);
		return;
	}
	else if ( !str_prefix( arg1, "odbierz" ) )
	{
		//jesli aukcja sie nie zakonczyla lub
		//zakonczyla sie ale nie jestes jej tworca ani czlonkiem
		if ( !auction_finished( pAuction ) ||
			( auction_finished( pAuction )
			&& !auction_creator( ch->name, pAuction ) && !is_auction_member( ch->name, pAuction ) ) )
			{
				huh(ch);
				return;
			}
		//jesli to tworca aukcji
		if ( auction_creator( ch->name, pAuction ) && pAuction->payment_collected )
		{
			huh( ch );
			return;
		}
		auction_item_gather( ch, pAuction );
		//jesli kasa i przedmiot odebrany mozna aukcje usunac
		if (pAuction->payment_collected && pAuction->item_collected && !pAuction->first)
		{
			UNLINK(pAuction,pStock->first,pStock->last,next,prev);
			free_auction(pAuction);
		}
		save_stock_market( pStock );
		ch->desc->olc_editing = NULL;
		ch->pcdata->line_nr =  STOCK_STATE_ITEMS;
		stock_show_terminal(ch);
		return;
	}
	else if ( !str_prefix( arg1, "ilosc" ) )
	{
		//jesli aukcja sie nie zakonczyla lub nie jesteœ cz³onkiem - to wybieranie ilosci nie ma sensu
		if ( auction_finished( pAuction ) || !is_auction_member( ch->name, pAuction ) || pAuction->quantity == 1)
		{
			huh(ch);
			return;
		}
		if ( !is_number( arg2 ) )
		{
			pager_printf( ch, "Wybierz ile egzemplarzy chcesz kupiæ." NL );
			return;
		}
		number = atoi( arg2 );
		if ( number < 1 )
		{
			pager_printf( ch, "Skoro zadeklarowa³eœ chêc licytowania - licytuj przynajmniej jeden egzemplarz." NL );
			return;
		}
		if ( number > pAuction->quantity )
		{
			pager_printf( ch, "Nie mo¿esz kupiæ wiêcej ni¿ %d egzemplarz%s." NL,
					pAuction->quantity, NUMBER_SUFF( pAuction->quantity, "","e","y" ) );
			return;
		}
		pBets = get_auction_member( ch->name, pAuction );
		pBets->quantity = number;
		pager_printf( ch, "Deklarujesz chêæ zakupienia %d egzemplarz%s." NL,
				pBets->quantity, NUMBER_SUFF( pBets->quantity, "a","y","y" ) );
		sort_auction_bets( pAuction );
		save_stock_market( pStock );
		return;
	}
	else if ( !str_prefix( arg1, "done" ) )
    {
		edit_done( ch, "" );
        return;
    }
	interpret( ch, arg);
}
*/
void stock_details_interpret( CHAR_DATA * ch, char * argument )
{
	AUCTION_DATA *		pAuction = NULL;
	STOCK_EXCHANGE_DATA * pStock = NULL;
	BETS_PROGRES *		pBets = NULL;
	char				arg[MSL];
	char				arg1[MSL];
	char				arg2[MSL];
	int64				new_bet = 0;
	int					number = 0,type = 0;

	strcpy( arg, argument );
	smash_tilde( argument );
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	pStock = get_stock_exchange( ch );
	pAuction = (AUCTION_DATA*)ch->desc->olc_editing;
	type = pAuction->type;

	if ( !str_prefix( arg1, "back") || !str_prefix( arg1, "powrot" ) )
	{
		if ( type == AUCTION_ITEM )
			ch->pcdata->line_nr = STOCK_STATE_ITEMS;
		else if ( type == AUCTION_SHIP )
			ch->pcdata->line_nr = STOCK_STATE_SHIPS;
		ch->desc->olc_editing	= NULL;
		stock_show_terminal(ch);
		return;
	}
	else if ( !str_prefix( arg1, "zakoncz" ) || !str_prefix( arg1, "finish" ) )
    {
		if ( auction_finished( pAuction ) )
		{
			huh( ch );
			return;
		}
		if ( IS_IMMORTAL( ch ) || auction_creator( ch->name, pAuction ) )
		{
			if ( IS_IMMORTAL( ch ) )
				pager_printf( ch, "Korzystaj¹c z przys³uguj¹cych Ci mocy doprowadzasz do zakoñczenia aukcji." NL NL);
			else if ( !IS_IMMORTAL( ch ) && auction_creator( ch->name, pAuction ) )
			{
				if ( !pAuction->first )
					pager_printf( ch, "Jako twórca tej aukcji mo¿esz j¹ zakoñczyæ. I tak te¿ robisz." NL);
				else
				{
					pager_printf( ch, "Ju¿ nie mo¿esz zakoñczyæ aukcji. Bierze w niej udzia³ %d licytatn%s." NL,
							pAuction->bidders_count,NUMBER_SUFF( pAuction->bidders_count, "","ów","ów") );
					return;
				}
			}
			pAuction->end_time = current_time-1;
		}
		else
		{
			huh( ch );
			return;
		}
		stock_show_terminal( ch );
		return;
    }
	else if ( !str_prefix( arg1, "usun" ) || !str_prefix( arg1, "delete" ) )
    {
		if ( !IS_IMMORTAL( ch ) )
		{
			interpret( ch, arg);
			return;
		}
		if ( !(pAuction = auction_exist( ch, arg2 , (char *)"usun¹æ", pStock) ) )
			return;
		//tutaj mamy usun¹æ aukcje
		if ( type == AUCTION_ITEM )
			ch->pcdata->line_nr = STOCK_STATE_ITEMS;
		else if ( type == AUCTION_SHIP )
			ch->pcdata->line_nr = STOCK_STATE_SHIPS;
		ch->desc->olc_editing = NULL;
		UNLINK( pAuction, pStock->first_new_offert, pStock->last_new_offert, next, prev );
		free_auction( pAuction );
		pager_printf( ch, "Korzystaj¹c z przys³uguj¹cych Ci mocy doprowadzasz do usuniêcia aukcji." NL NL);
		stock_show_terminal( ch );
		return;
    }
	else if ( !str_prefix( arg1, "licytuj") )
	{
		if ( auction_finished( pAuction ) || auction_creator( ch->name, pAuction ) )
		{
			huh(ch);
			return;
		}
		if ( !is_number ( arg2 ) )
		{
			pager_printf( ch, "Podaj kwotê za jak¹ chcesz licytowaæ. Ta kwota bêdzie oznacza³a górny limit" NL
					"ile pieniêdzy chcesz wydaæ na zakup tego przedmiotu" NL );
			return;
		}
		new_bet = atoi( arg2 );
		if (pAuction->first && new_bet < pAuction->first->max_price+5 )
		{
			pager_printf( ch, "Przykro mi, ale musisz postawiæ wiêcej, by przebic innych graczy." NL);
			return;
		}
		else if ( !pAuction->first && pAuction->starting_price > new_bet)
		{
			pager_printf( ch, "Musisz postawiæ przynajmniej %lld" NL ,pAuction->starting_price);
			return;
		}
		if ( ch->gold < new_bet)
		{
			pager_printf( ch, "Nie staæ ciê na taki wydatek." NL);
			return;
		}

		if (is_auction_member(ch->name,pAuction))
		{
			pBets = get_auction_member(ch->name, pAuction);
			if (pBets->max_price > new_bet)
			{
				pager_printf(ch, "Ta stawka jest mniejsza od Twojej wczeœniejszej." NL);
				return;
			}
			else
			{
				int diff = new_bet - pBets->max_price;
				pBets->max_price = new_bet;
				pager_printf(ch, "Twoja nowa stawka to %d kredyt%s." NL NL,
						new_bet,NUMBER_SUFF(new_bet,"ka","ki","ek"));
				ch->gold-= diff;
			}
		}
		else
		{
			pBets = new_bets();
			LINK(pBets,pAuction->first,pAuction->last,next,prev);
			STRDUP(pBets->name,ch->name);
			pBets->max_price = new_bet;
			pager_printf( ch, "Do³¹czasz do tej licytacji ze stawk¹ %d kredyt%s" NL,
					new_bet, NUMBER_SUFF(new_bet,"ki","ek","ek") );
			ch->gold -= new_bet;
		}
		sort_auction_bets(pAuction);
		save_stock_market( pStock );
		stock_show_terminal(ch);
		return;
	}
	else if ( !str_prefix( arg1, "buyout" ) )
	{
		if ( auction_finished( pAuction ) || auction_creator( ch->name, pAuction ) || !pAuction->buy_out_now )
		{
			huh( ch );
			return;
		}
		if ( pAuction->bet > pAuction->buy_out_now ||
				( pAuction->first && pAuction->first->max_price > pAuction->buy_out_now ) )
		{
			pager_printf( ch, "Licytowana cena przekroczy³a ju¿ kwote okreœlon¹ jako wykup natychmiastowy." NL );
			return;
		}
		if ( ch->gold < pAuction->buy_out_now)
		{
			pager_printf( ch, "Nie staæ ciê na taki wydatek." NL);
			return;
		}
		if (is_auction_member(ch->name,pAuction))
		{
			pBets = get_auction_member(ch->name, pAuction);
			int diff = pAuction->buy_out_now - pBets->max_price;
			pBets->max_price = pAuction->buy_out_now;
			pager_printf(ch, "Dokonujesz natychmiastowego wykupu." NL NL,
				pAuction->buy_out_now ,NUMBER_SUFF(pAuction->buy_out_now,"ka","ki","ek"));
			ch->gold-= diff;
		}
		else
		{
			pBets = new_bets();
			LINK(pBets,pAuction->first,pAuction->last,next,prev);
			STRDUP(pBets->name,ch->name);
			pBets->max_price = pAuction->buy_out_now;
			pBets->quantity=1;
			pager_printf( ch, "Do³¹czasz do tej licytacji ze stawk¹ %d kredyt%s" NL,
					pAuction->buy_out_now, NUMBER_SUFF(pAuction->buy_out_now,"ki","ek","ek") );
			ch->gold -= pAuction->buy_out_now;
		}

		pAuction->end_time = current_time-1;
		sort_auction_bets(pAuction);
		save_stock_market( pStock );
		stock_show_terminal(ch);
		return;
	}
	else if ( !str_prefix( arg1, "odbierz" ) )
	{
		//jesli aukcja sie nie zakonczyla lub
		//zakonczyla sie ale nie jestes jej tworca ani czlonkiem
		if ( !auction_finished( pAuction ) ||
			( auction_finished( pAuction )
			&& !auction_creator( ch->name, pAuction ) && !is_auction_member( ch->name, pAuction ) ) )
			{
				huh(ch);
				return;
			}
		//jesli to tworca aukcji
		if ( auction_creator( ch->name, pAuction ) && pAuction->payment_collected )
		{
			huh( ch );
			return;
		}
		auction_item_gather( ch, pAuction );
		//jesli kasa i przedmiot odebrany mozna aukcje usunac
		if (pAuction->payment_collected && pAuction->item_collected && !pAuction->first)
		{
			UNLINK(pAuction,pStock->first,pStock->last,next,prev);
			free_auction(pAuction);
		}
		save_stock_market( pStock );
		ch->desc->olc_editing = NULL;
		ch->pcdata->line_nr =  STOCK_STATE_ITEMS;
		stock_show_terminal(ch);
		return;
	}
	else if ( !str_prefix( arg1, "ilosc" ) )
	{
		//jesli aukcja sie nie zakonczyla lub nie jesteœ cz³onkiem - to wybieranie ilosci nie ma sensu
		if ( auction_finished( pAuction ) || !is_auction_member( ch->name, pAuction ) || pAuction->quantity == 1)
		{
			huh(ch);
			return;
		}
		if ( !is_number( arg2 ) )
		{
			pager_printf( ch, "Wybierz ile egzemplarzy chcesz kupiæ." NL );
			return;
		}
		number = atoi( arg2 );
		if ( number < 1 )
		{
			pager_printf( ch, "Skoro zadeklarowa³eœ chêc licytowania - licytuj przynajmniej jeden egzemplarz." NL );
			return;
		}
		if ( number > pAuction->quantity )
		{
			pager_printf( ch, "Nie mo¿esz kupiæ wiêcej ni¿ %d egzemplarz%s." NL,
					pAuction->quantity, NUMBER_SUFF( pAuction->quantity, "","e","y" ) );
			return;
		}
		pBets = get_auction_member( ch->name, pAuction );
		pBets->quantity = number;

		pager_printf( ch, "Deklarujesz chêæ zakupienia %d egzemplarz%s." NL,
				pBets->quantity, NUMBER_SUFF( pBets->quantity, "a","y","y" ) );
		sort_auction_bets( pAuction );
		save_stock_market( pStock );
		return;
	}
	else if ( !str_prefix( arg1, "done" ) )
    {
		edit_done( ch, (char *)"" );
        return;
    }
	interpret( ch, arg);
}

void stock_list_interpret( CHAR_DATA * ch, char * argument)
{
	AUCTION_DATA *		pAuction = NULL;
	STOCK_EXCHANGE_DATA * pStock = NULL;
	char				arg[MSL];
	char				arg1[MSL];
	char				arg2[MSL];
	int					number=0;

	strcpy( arg, argument );
	smash_tilde( argument );
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	pStock = get_stock_exchange( ch );

	if ( is_number(arg1) || !str_prefix( arg1, "pokaz" ) || !str_prefix( arg1, "show" ) )
	{
		if ( !is_number(arg1))
			number = atoi( arg2 ); else number = atoi( arg1 );
		pAuction = get_auction_nr( ch, number, pStock );
		if ( !pAuction )
		{
			pager_printf( ch, "Nie ma aukcji o takim numerze." NL NL);
			return;
		}
		switch ( ch->pcdata->line_nr )
		{
			case STOCK_STATE_SHIPS:
				ch->pcdata->line_nr=STOCK_STATE_SHIPS_DETAIL;
				break;
			case STOCK_STATE_ITEMS:
				ch->pcdata->line_nr=STOCK_STATE_ITEMS_DETAIL;
				break;
			default:
				break;
		}
		ch->desc->olc_editing	= (void*)pAuction;
		stock_show_terminal( ch );
		return;
	}
	else if ( !str_prefix( arg1, "back") || !str_prefix( arg1, "powrot" ) )
	{
		ch->pcdata->line_nr = STOCK_STATE_MAIN;
		ch->desc->olc_editing	= NULL;
		stock_show_terminal(ch);
		return;
	}
	else if ( !str_prefix( arg1, "zakoncz" ) || !str_prefix( arg1, "finish" ) )
    {
		if ( !is_number ( arg2 ) )
		{
			pager_printf( ch, "Podaj numer aukcji, któr¹ chcesz zakoñczyæ." NL NL);
			return;
		}
		number = atoi( arg2 );
		pAuction = get_auction_nr( ch, number, pStock );
		if ( !pAuction )
		{
			pager_printf( ch, "Nie ma aukcji o takim numerze." NL NL);
			return;
		}
		if ( IS_IMMORTAL( ch ) || auction_creator( ch->name, pAuction ) )
		{
			if ( IS_IMMORTAL( ch ) )
				pager_printf( ch, "Korzystaj¹c z przys³uguj¹cych Ci mocy doprowadzasz do zakoñczenia aukcji." NL NL);
			else if ( !IS_IMMORTAL( ch ) && auction_creator( ch->name, pAuction ) )
			{
				if ( !pAuction->first )
					pager_printf( ch, "Jako twórca tej aukcji mo¿esz j¹ zakoñczyæ. I tak te¿ robisz." NL);
				else
				{
					pager_printf( ch, "Ju¿ nie mo¿esz zakoñczyæ aukcji. Bierze w niej udzia³ %d licytatn%s." NL,
							pAuction->bidders_count,NUMBER_SUFF( pAuction->bidders_count, "","ów","ów") );
					return;
				}
			}
			pAuction->end_time = current_time-1;
		}
		else
		{
			pager_printf( ch, "Nie mo¿esz zakoñczyæ tej aukcji. Nie nale¿y do Ciebie." NL NL);
			return;
		}
		stock_show_terminal( ch );
		return;
    }
	else if ( !str_prefix( arg1, "usun" ) || !str_prefix( arg1, "delete" ) )
    {
		if ( !is_number ( arg2 ) )
		{
			pager_printf( ch, "Podaj numer aukcji, któr¹ chcesz usun¹æ." NL NL);
			return;
		}
		number = atoi( arg2 );
		pAuction = get_auction_nr( ch, number, pStock );
		if ( !pAuction )
		{
			pager_printf( ch, "Nie ma aukcji o takim numerze." NL NL);
			return;
		}
		if ( IS_IMMORTAL( ch ) )
			pager_printf( ch, "Korzystaj¹c z przys³uguj¹cych Ci mocy usuwasz aukcje." NL NL);
		else if ( !IS_IMMORTAL( ch ) && auction_creator( ch->name, pAuction ) )
		{
			if ( !pAuction->first )
				pager_printf( ch, "Jako twórca tej aukcji mo¿esz j¹ usun¹æ. I tak te¿ robisz." NL);
			else
			{
				pager_printf( ch, "Ju¿ nie mo¿esz usun¹æ aukcji. Bierze w niej udzia³ %d licytatn%s." NL,
						pAuction->bidders_count,NUMBER_SUFF( pAuction->bidders_count, "","ów","ów") );
				return;
			}
		}
		else
		{
			huh(ch);
			return;
		}

		//tutaj mamy usun¹æ aukcje
		ch->desc->olc_editing = NULL;
		UNLINK( pAuction, pStock->first, pStock->last, next, prev );
		free_auction( pAuction );
		save_stock_market( pStock );
		stock_show_terminal( ch );
		return;
    }
	else if ( !str_prefix( arg1, "odbierz" ) )
	{
		if ( !is_number( arg2 ) )
		{
			pager_printf( ch, "Podaj numer aukcji z której chcesz odebraæ gotówkê lub przedmiot" NL);
			return;
		}
		number = atoi( arg2 );
		pAuction = get_auction_nr( ch, number, pStock );
		if ( !pAuction )
		{
			pager_printf( ch, "Nie ma aukcji o takim numerze." NL NL);
			return;
		}
		//jesli aukcja sie nie zakonczyla lub
		//zakonczyla sie ale nie jestes jej tworca ani czlonkiem
		//lub tez jestes jej autorem i juz wczesniej odebrales swoja nagrode
		if ( !auction_finished( pAuction ) ||
			( auction_finished( pAuction )
			&& !auction_creator( ch->name, pAuction ) && !is_auction_member( ch->name, pAuction ) ) ||
			 ( auction_creator( ch->name, pAuction ) && pAuction->payment_collected ) )
			{
				huh(ch);
				return;
			}
		auction_item_gather( ch, pAuction );
		//jesli kasa i przedmiot odebrany mozna aukcje usunac
		if (pAuction->payment_collected && pAuction->item_collected && !pAuction->first)
		{
			UNLINK(pAuction,pStock->first,pStock->last,next,prev);
			free_auction(pAuction);
		}
		save_stock_market( pStock );
		ch->desc->olc_editing = NULL;
		stock_show_terminal(ch);
		return;
	}
	else if ( !str_prefix( arg1, "done" ) )
    {
		edit_done( ch, (char *)"" );
        return;
    }
	interpret( ch, arg);
}

//////////////////////////////////////////////////////////////////////////////

void exchange( DESCRIPTOR_DATA * d, char * argument )
{
	CHAR_DATA		* ch = d->character;
	char			arg[MSL];
	char			arg1[MIL];

	strcpy( arg, argument );
	argument = one_argument( argument, arg1 );

	if (IS_NPC(ch))
		return;

	if (!IS_SET( ch->in_room->room_flags , ROOM_AUCTION ))
	{
		send_to_char( "Musisz byæ na sali aukcyjnej by to zrobic!" NL, ch );
		if ( ch->desc->connected == CON_STOCK_MARKET)
			edit_done( ch, (char *)"" );
		return;
	}

	if (arg1[0]=='\0' || !str_prefix(arg1,"show"))
	{
		stock_show_terminal(ch);
		return;
	}
	if ( ch->pcdata->line_nr ==  STOCK_STATE_MAIN)
	{
		stock_main_interpret( ch, arg);
		return;
	}
	else if ( ch->pcdata->line_nr ==  STOCK_STATE_NEW_OFFER)
	{
		stock_new_offert_interpret( ch, arg);
		return;
	}
	else if ( ch->pcdata->line_nr ==  STOCK_STATE_ITEMS ||
			ch->pcdata->line_nr ==  STOCK_STATE_SHIPS )
	{
		stock_list_interpret( ch, arg);
		return;
	}
	else if ( ch->pcdata->line_nr ==  STOCK_STATE_ITEMS_DETAIL ||
			ch->pcdata->line_nr == STOCK_STATE_SHIPS_DETAIL )
	{
		stock_details_interpret( ch, arg );
		return;
	}
	else if ( ch->pcdata->line_nr ==  STOCK_STATE_PLANET_INFO)
	{
		if (!str_prefix(arg1,"back"))
		{
			ch->pcdata->line_nr = STOCK_STATE_MAIN;
			stock_show_terminal(ch);
			return;
		}
	}
	GET_DONE();

	interpret( ch, arg );
}

DEF_DO_FUN( exchange )
{
	if (IS_NPC(ch))
	{
		huh( ch );
		return;
	}

	STOCK_EXCHANGE_DATA *	pStock = NULL;
	pStock = get_stock_exchange( ch );
	if ( !pStock )
	{
        pager_printf( ch, "Na tej planecie nie funkcjonuje ¿adna gie³da" NL);
		bug("This auction room isn't connected with stockmarket [%d, %s, %s]",ch->in_room->vnum,
			ch->in_room->area->name, ch->in_room->area->planet ?  ch->in_room->area->planet->name: "No planet" );
        	return;
	}

	ch->pcdata->line_nr		= STOCK_STATE_MAIN;
	ch->desc->connected		= CON_STOCK_MARKET;
	ch->desc->olc_editing	= NULL;

	exchange(ch->desc,(char *)"");
}
