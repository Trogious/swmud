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
*       	     OLC Header Module Added by Thanos			   *
****************************************************************************/

#ifndef STOCK_MARKET_HEADER_FILE
#define STOCK_MARKET_HEADER_FILE 
#include "mud.h"


#define T_LINE "--------------------------------------------------------------------------"

/*! \fn void show_auction_data(CHAR_DATA *ch,AUCTION_DATA * pAuction, int number)
*	\brief wy�wietl szczeg�owe dane o aukcji
*	\param ch - gracz przegl�daj�cy liste
*	\param pAuction - interesuj�ca nas aukcja
*	\param number - numer porz�dkowy aukcji w li�cie
*/
/*! \fn char * prepare_time(time_t 	end_time, bool short_form, bool minus)
*	\brief wylicza czas jaki pozosta� do podanej daty
*	\brief i przedstawia go w postaci tekstu (np. 12 dni, 5 godzin, 2 minuty)
*	\param end_time - data do jakiej wyliczamy czasu
*	\param short_form - czy skr�ci� opis skr�conej skracaj�c opisy
*	\param minus - czy wstawi� minus przed tekstem
 *	\return zwraca wska�nik na tablice z przygotowanym tekstem
*/
/*! \fn bool auction_creator(char *name, AUCTION_DATA *pAuction)
*	\brief czy gracz o podanym name jest tw�rc� aukcji
*	\param name - nazwa gracza
*	\param pAuction - testowana aukcja
*/
/*! \fn bool is_auction_member(char *name, AUCTION_DATA *pAuction)
*	\brief czy gracz o podanej nazwie bierze udzia� w tej aukcji
*	\param name - nazwa gracza
*	\param pAuction - testowana aukcja
*/
/*!	\fn BETS_PROGRES * get_auction_member( char *name, AUCTION_DATA *pAuction)
*	\brief podaj strukture powi�zan� z danym graczem
*	\param name - nazwa gracza
*	\param pAuction - testowana aukcja
*/
/*!	\fn bool auction_finished(AUCTION_DATA *pAuction)
*	\brief czy aukcja jest ju� sko�czona
*	\param pAuction - testowana aukcja
*/
/*!	\fn bool won_auction(char *name, AUCTION_DATA *pAuction)
*	\brief czy gracz o podanej nazwie wygra� aukcje
*	\param name - nazwa gracza
*	\param pAuction - testowana aukcja
*/
/*!	\fn AUCTION_DATA * find_auction_nr( int nr)
*	\brief znajdz aukcje o podanym numerze porz�dkowym
*	\param nr - gracz przegl�daj�cy liste
*	\return wska�nik na struktur� poszukiwanej aukcji
*/
/*!	\fn void auction_buy_out(CHAR_DATA *ch, AUCTION_DATA *pAuction)
*	\brief gracz dokona� natychmiastowego wykupu w podanej aukcji
*	\param ch - gracz dokonuj�cy wykupu
*	\param pAuction - kt�rej aukcji dotyczy operacja wykupu
*/
/*!	\fn void auction_cleanup( AUCTION_DATA * pAuction)
*	\brief posprz�taj po zako�czonej aukcji
*	\param pAuction - wybrana do usuni�cia aukcja
*/
/*!	\fn void auction_gather(CHAR_DATA *ch, AUCTION_DATA *pAuction)
*	\brief gracze tworz�cy lub bior�cy w aukcji udzia� przyszli rozliczy� si�
*	\param ch - gracz rozliczaj�cy si�
*	\param pAuction - wybrana do rozliczenia aukcja
*/
/*!	\fn void auction_bet(CHAR_DATA *ch, int nr, int new_bet)
*	\brief gracze tworz�cy lub bior�cy w aukcji udzia� przyszli rozliczy� si�
*	\param ch - gracz podbijaj�cy stawk�
*	\param nr - numer porz�dkowy aukcji
*	\param new_bet - nowa stawka
*/
/*!	\fn void auction_start(CHAR_DATA *ch,OBJ_DATA *	pObject,SHIP_DATA *pShip)
*	\brief gracz chce rozpocz�� now� aukcje z podanym przedmiotem
*	\param ch - gracz otwieraj�cy aukcje
*	\param pObject - licytowany przedmiot
*	\param pShip - bo mo�emu ju� licytowa� statki
 */
/*!	\fn void send_auction_notes(AUCTION_DATA * pAuction,bool finished)
*	\brief mud rozsy�a notki do graczy bior�cych udzia� w danej aukcji
*	\param pAuction - aukcja kt�re wymaga
*	\param finished - czy aukcja si� zako�czy�a czy te� kto� j� przerwa�
*/
/*!	\fn void do_auction(CHAR_DATA *ch, char* argument)
*	\brief g��wna funkcja obs�uguj�ca sterowanie aukcj� przez gracza
*	\param ch - gracz wydaj�cy polecenie steruj�ce aukcj�
*	\param argument - polecenia dodatkowe
*/
/*!	\fn void show_auction_list(CHAR_DATA *ch)
*	\brief poka� graczowi list� aktualnie tocz�cych si� aukcji
*/
/*!	\fn void auction_remove( CHAR_DATA *ch, int nr )
*	\brief gracz bior�cy udzia� w licytacji rezygnuje z dalszego udzia�u w niej
*	\param ch - gracz licytuj�cy
*	\param nr - nr porz�dkowy licytacji
*/

/** @fn STOCK_EXCHANGE_DATA * get_stock_exchange(CHAR_DATA * ch)
* @brief Zwraca wska�nik na gie�de, ktora funkcjonuje na planecie, na kt�rej jest gracz
* 
* @param ch gracz, kt�ry sprawdza dane gie�dy
* 
* @return NULL je�li gracz znajduje si� na planecie, na kt�rej nie ma dzia�aj�cej gie�dy
*       pointer do gieldy
*/
/** @fn char * make_stock_state_info(char * state_name)
* @brief Linia nag��wka terminala z nazw� stanu umieszczon� po�rodku
* 
* @param state_name nazwa stanu lub etapu przegladania terminala
*/
/** @fn int parse_atodate( CHAR_DATA * ch, char * argument)
 * @brief ma przeparsowac string w poszukiwaniu czasu i zwr�ci� ilosc
 * godzin trwania aukcji
 * 
 * @param ch gracz ktory zobaczy wyniki obliczen
 * @param argument parsowana data w postaci xx dni xx godzin
 * 
 * @return ile godzin pozostalo do podanej daty
 */
/** @fn AUCTION_DATA * auction_exist( CHAR_DATA * ch, char * arg , char * text, STOCK_EXCHANGE_DATA * pStock, int type )
* @brief Znajdz aukcje o podanym numerze na danej gieldzie 
* 
* @param ch 
* @param arg numer poszukiwanej aukcji w postaci stringa
* @param text co b�dzie robione z t� aukcja (usun��, zmodyfikowa�)
* @param pStock przeszukiwana gie�da
* @param type typ aukcji
*/

enum
{
	STOCK_STATE_MAIN,	/*!	g�owne okno terminala gie�dowego */
	STOCK_STATE_ITEMS,	/*!	okno listy przedmiot�w wystawionych na sprzedaz */
	STOCK_STATE_ITEMS_DETAIL,/*!	okno szczegolowych danych sprzedawanego przedmiotu */
	STOCK_STATE_SHIPS,	/*!	okno listy statk�w wystawionych na sprzedaz */
	STOCK_STATE_SHIPS_DETAIL,/*!	okno szczegolowych danych sprzedawanego statku */
	STOCK_STATE_HOUSE,/*!	okno listy statk�w wystawionych na sprzedaz */
	STOCK_STATE_HOUSE_DETAIL,/*!	okno szczegolowych danych sprzedawanego statku */
	STOCK_STATE_MATERIALS,/*!	okno listy statk�w wystawionych na sprzedaz */
	STOCK_STATE_MATERIALS_DETAIL,/*!	okno szczegolowych danych sprzedawanego statku */
	STOCK_STATE_PRODUCTS,/*!	okno listy statk�w wystawionych na sprzedaz */
	STOCK_STATE_PRODUCTS_DETAIL,/*!	okno szczegolowych danych sprzedawanego statku */
	STOCK_STATE_TIME_CONTRACTS,/*!	okno listy statk�w wystawionych na sprzedaz */
	STOCK_STATE_TIME_CONTRACTS_DETAIL,/*!	okno szczegolowych danych sprzedawanego statku */
	STOCK_STATE_PRIVATE_CONTRACT,/*!	okno listy statk�w wystawionych na sprzedaz */
	STOCK_STATE_PRIVATE_CONTRACT_DETAIL,/*!	okno szczegolowych danych sprzedawanego statku */
	STOCK_STATE_PLANET_INFO,/*!	okno listy statk�w wystawionych na sprzedaz */
	STOCK_STATE_NEW_OFFER/*!	okno listy statk�w wystawionych na sprzedaz */
};

const SWString prepare_time(time_t 	end_time, bool short_form, bool minus);
char *	advitoa( int val, bool coma);


bool	auction_started( AUCTION_DATA *pAuction );
bool	auction_finished(AUCTION_DATA *pAuction);
bool	is_auction_member(char *name, AUCTION_DATA *pAuction);
bool	auction_creator(char *name, AUCTION_DATA *pAuction);
void	show_1_auction_list(CHAR_DATA *ch, AUCTION_DATA * pAuction, int index);
void	stock_show_terminal(CHAR_DATA *ch);
void	show_all_betters(CHAR_DATA *ch, AUCTION_DATA * pAuction);
void	auction_show_obj_stats( CHAR_DATA * ch, OBJ_DATA *obj );
void	auction_show_ship_info( CHAR_DATA *ch, SHIP_DATA * pShip );


void	sort_stock_market_auctions(STOCK_EXCHANGE_DATA * pStock);
void	sort_auction_bets(AUCTION_DATA * pAuction);

void	auction_item_gather( CHAR_DATA *ch, AUCTION_DATA * pAuction );
BETS_PROGRES	* get_auction_member( char *name, AUCTION_DATA *pAuction);
AUCTION_DATA * get_auction_nr( CHAR_DATA * ch, int nr, STOCK_EXCHANGE_DATA * pStock);

extern time_t auction_pulse;

auction_type get_auction_type(CHAR_DATA * ch);
void process_bets(AUCTION_DATA 	*pAuction);
void process_auctions();
void send_auction_notes( AUCTION_DATA * pAuction, auction_ends typ );
char * advitoa( int val, bool coma);
extern time_t auction_pulse;
void process_bets(AUCTION_DATA 	*pAuction);
void process_auctions();
void send_auction_notes( AUCTION_DATA * pAuction, auction_ends typ );
char * advitoa( int val, bool coma);
extern time_t auction_pulse;
void process_bets(AUCTION_DATA 	*pAuction);
void process_auctions();
void send_auction_notes( AUCTION_DATA * pAuction, auction_ends typ );
char * advitoa( int val, bool coma);

#endif
