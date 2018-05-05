/*
 * pazaak.cpp
 *
 *  Created on: Feb 23, 2010
 *      Author: Trog
 */
#include "mud.h"
#include "classes/SWPazaak.h"

DEF_DO_FUN( pazaak )
{
	char arg1[MIL] =
	{ 0 };
	char arg2[MIL] =
	{ 0 };

	if (!*argument)
	{
		send_to_char("Sk³adnia: pazaak <komenda> [opcje]" NL NL
		"Komendy:" NL
		" setpassword challenge accept withdraw list" NL, ch);
		return;
	}

	argument = one_argument(argument, arg1);

	if (!str_cmp(arg1, "setpassword"))
	{
		if (!*argument)
		{
			send_to_char("Sk³adnia: pazaak setpassowrd <has³o>" NL, ch);
			return;
		}

		argument = one_argument(argument, arg2);

		if (reg_cmp("^[A-Za-z0-9_]+$", arg2))
		{
			send_to_char("Has³o mo¿e zawieraæ jedynie znaki alfanumeryczne (litery i cyfry)." NL, ch);
			return;
		}

		bool changed = SWPazaak::setPassword(ch->name, arg2);
		if (!changed)
		{
			send_to_char("Ustawienie has³a nie powiod³o siê. Mo¿e has³o zawiera niedozwolone znaki?" NL, ch);
			return;
		}

		send_to_char("Has³o zosta³o ustawione." NL, ch);
		return;
	}

	if (!str_cmp(arg1, "withdraw"))
	{
		if (!ch->in_room || !IS_SET(ch->in_room->room_flags, ROOM_BANK))
		{
			send_to_char("Musisz byæ w banku aby to zrobiæ." NL, ch);
			return;
		}

		int creditsWithdrawn = SWPazaak::withdrawCredits(ch->name);
		if (creditsWithdrawn < 1)
		{
			send_to_char("Nie masz ¿adnych funduszy na koncie Pazaak'a." NL, ch);
			return;
		}

		ch_printf(ch, "Wyp³acasz %d kredytek z konta Pazaak'a." NL, creditsWithdrawn);
		ch->gold += creditsWithdrawn;
		save_char_obj(ch);
		return;
	}

	if (!str_cmp(arg1, "challenge"))
	{
		if (!ch->in_room || !IS_SET(ch->in_room->room_flags, ROOM_BANK))
		{
			send_to_char("Musisz byæ w banku aby to zrobiæ." NL, ch);
			return;
		}

		argument = one_argument(argument, arg2);
		if (!*arg2 || !*argument)
		{
			send_to_char("Sk³adnia: pazaak challenge <gracz> <stawka>" NL, ch);
			return;
		}

		CHAR_DATA *victim = get_char_world(ch, arg2);
		if (!victim)
		{
			ch_printf(ch,
					"Szukasz w intergalaktycznym holonecie, ale nigdzie nie mo¿esz znale¼æ kogo¶ takiego jak: %s." NL,
					arg2);
			return;
		}

		int reward = SWInt::fromString(argument);
		if (reward < 1)
		{
			send_to_char("Mo¿e jednak podasz jaka¶ rozs±dn± kwotê?" NL, ch);
			return;
		}

		if (ch->gold < reward)
		{
			send_to_char("Nie masz tyle!" NL, ch);
			return;
		}

		if (static_cast<unsigned int> (reward) < SWPazaak::MIN_REWARD)
		{
			ch_printf(ch, "Minimalna stawka to: %d kredyt%s." NL, SWPazaak::MIN_REWARD,
					NUMBER_SUFF((int)SWPazaak::MIN_REWARD, "ka", "ek", "ki"));
			return;
		}

		if (!SWPazaak::challenge(ch->name, victim->name, reward))
		{
			ch_printf(ch, "Z jakiego¶ powodu wyzwanie na pojedynek Pazaak'owy nie zosta³o zaakceptowane." NL
			"Upewnij siê, ¿e ty, jak i twój przeciwnik oboje spe³niacie wszystkie warunki." NL
			"Mo¿e najzwyczajniej w galaktyce ju¿ wyzwa³%s¶ %s?" NL, SEX_SUFFIX_EAO(ch), victim->przypadki[1]);
			return;
		}

		ch->gold -= reward;
		ch_printf(ch, "Wyzywasz %s na pojedynek Pazaak'owy." NL, victim->przypadki[1]);
		ch_printf(victim, "%s wyzywa ciê na pojedynek Pazaak'owy." NL, ch->przypadki[0]);
		save_char_obj(ch);
		return;
	}

	if (!str_cmp(arg1, "accept"))
	{
		if (!*argument)
		{
			send_to_char("Sk³adnia: pazaak accept <gracz>" NL, ch);
			return;
		}

		if (!ch->in_room || !IS_SET(ch->in_room->room_flags, ROOM_BANK))
		{
			send_to_char("Musisz byæ w banku aby to zrobiæ." NL, ch);
			return;
		}

		CHAR_DATA *victim = get_char_world(ch, argument);
		strncpy(arg2, victim ? victim->name : argument, MIL - 1);

		SWPazaak::SWPazaakChallenge *challenge = SWPazaak::findChallenge(arg2, ch->name);
		if (!challenge)
		{
			ch_printf(ch, "Nikt taki jak: %s nie wyzwa³ ciê na pojedynek Pazaak'owy." NL
			"Chyba jeste¶ nadwra¿liw%s." NL, arg2, SEX_SUFFIX_YAE(ch));
			return;
		}

		int reward = challenge->getReward();
		if (ch->gold < reward)
		{
			ch_printf(ch, "Stawka wynosi: %d. Nie masz tyle!" NL, reward);
			return;
		}

		if (!SWPazaak::acceptChallenge(challenge))
		{
			ch_printf(ch, "Z jakiego¶ powodu nie mo¿esz zaakceptowaæ takiego wyzwania." NL
			"Mo¿e nikt taki jak: %s nie wyzwa³ ciê na pojedynek Pazaak'owy" NL
			"a ty jeste¶ nadwra¿liw%s." NL, arg2, SEX_SUFFIX_YAE(ch));
			return;
		}

		ch->gold -= reward;
		send_to_char("Przyjmujesz wyzwanie Pazaak'owe." NL, ch);
		save_char_obj(ch);

		if (victim)
		{
			ch_printf(victim, "%s przyjmuje Twoje wyzwanie na pojedynek Pazaak'owy." NL, ch->przypadki[0]);
		}

		return;
	}

	if (!str_cmp(arg1, "list"))
	{
		if (!*argument)
		{
			send_to_char("Sk³adnia: pazaak list challenges|games|cards" NL, ch);
			return;
		}

		if (!str_cmp(argument, "challenges"))
		{
			list<SWPazaak::SWPazaakChallenge*> challenges = SWPazaak::getChallenges();
			challenges.sort(SWPazaak::SWPazaakChallenge::DefenderComparator(ch->name));

			char buf[MIL] =
			{ 0 };
			SWString buf2("Wyzwania na pojedynki Pazaak'owe, których jeste¶ stron±:" NL);
			for (list<SWPazaak::SWPazaakChallenge*>::const_iterator it = challenges.begin(); it != challenges.end(); it++)
			{
				if ((*it)->getAggressor() == ch->name || (*it)->getDefender() == ch->name)
				{
					snprintf(buf, MIL - 1, " [%s] %s vs. %s" NL, static_cast<SWString> ((*it)->getChallenged()).c_str(),
							(*it)->getAggressor().c_str(), (*it)->getDefender().c_str());
					buf2 << buf;
				}
			}

			send_to_pager(buf2.c_str(), ch);

			return;
		}

		if (!str_cmp(argument, "games"))
		{
			char buf[MIL] =
			{ 0 };
			char buf2[MIL] =
			{ 0 };
			const list<SWPazaakGame*> &games = SWPazaak::getGames();
			snprintf(buf, MIL - 1, FG_YELLOW "%s" PLAIN, ch->name);

			SWString buf3("Obecne pojedynki Pazaak'owe:" NL);
			for (list<SWPazaakGame*>::const_iterator it = games.begin(); it != games.end(); it++)
			{
				snprintf(buf2, MIL - 1, " [%s] ", static_cast<SWString> ((*it)->getChallenged()).c_str());
				buf3 << buf2;
				if ((*it)->getAggressor() == ch->name)
				{
					snprintf(buf2, MIL - 1, "%s vs. %s" NL, buf, (*it)->getDefender().c_str());
					buf3 << buf2;
				}
				else if ((*it)->getDefender() == ch->name)
				{
					snprintf(buf2, MIL - 1, "%s vs. %s" NL, (*it)->getAggressor().c_str(), buf);
					buf3 << buf2;
				}
				else
				{
					buf3 << (*it)->getAggressor() << " vs. " << (*it)->getDefender() << NL;
				}
			}

			send_to_pager(buf3.c_str(), ch);

			return;
		}

		if (!str_cmp(argument, "cards"))
		{
			send_to_pager(SWPazaak::listCards(ch->name).c_str(), ch);
			return;
		}
	}

	do_pazaak(ch, (char *) "");
}

OD *pazaak_create_card_obj(const SWPazaakCard &card)
{
	OID *objIndex = get_obj_index(OBJ_VNUM_PAZAAK_CARD);
	if (!objIndex)
	{
		bug("obj index vnum %d for pazaak card not found!", OBJ_VNUM_PAZAAK_CARD);
		return 0;
	}

	OD *obj = create_object(objIndex, 1);
	char buf[MIL] =
	{ 0 };

	snprintf(buf, MIL - 1, "Karta pazaak: %s", card.description().c_str());
	STRDUP(obj->name, buf);
	STRDUP(obj->przypadki[0], buf);

	snprintf(buf, MIL - 1, "Karty pazaak: %s", card.description().c_str());
	STRDUP(obj->przypadki[1], buf);

	snprintf(buf, MIL - 1, "Karcie pazaak: %s", card.description().c_str());
	STRDUP(obj->przypadki[2], buf);

	snprintf(buf, MIL - 1, "Kartê pazaak: %s", card.description().c_str());
	STRDUP(obj->przypadki[3], buf);

	snprintf(buf, MIL - 1, "Kart± pazaak: %s", card.description().c_str());
	STRDUP(obj->przypadki[4], buf);

	snprintf(buf, MIL - 1, "Karcie pazaak: %s", card.description().c_str());
	STRDUP(obj->przypadki[5], buf);

	obj->item_type = ITEM_PAZAAK_CARD;
	obj->value[0] = card.getType();
	obj->value[1] = card.getSign();
	obj->value[2] = card.getVariant();
	obj->value[3] = card.isDoubleSigned();

	/*
	 if (CAN_WEAR(obj, ITEM_TAKE))
	 {
	 obj = obj_to_char(obj, ch);
	 }
	 else
	 {
	 obj = obj_to_room(obj, ch->in_room);
	 act(COL_IMMORT, "$n stworzy³$o $p$3!", ch, obj, NULL, TO_ROOM);
	 }
	 */

	return obj;
}

