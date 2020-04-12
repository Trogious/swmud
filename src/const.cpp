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
*			     Mud constants module			   *
****************************************************************************/

#include <sys/types.h>
#include <time.h>
#include "mud.h"

const char* inv_pers_i[6] = {	"Kto¶",
				"Kogo¶",
				"Komu¶",
				"Kogo¶",
				"Kim¶",
				"Kim¶"	};


/* Trog: rasa bazowa (u nas czlowiek) */
const RACE_DATA human_race =
{
	NULL,
	NULL,
	(char *)"Human",
	(char *)"",
	{ (char *)"Mê¿czyzna", (char *)"Mê¿czyzny", (char *)"Mê¿czy¼nie",(char *) "Mê¿czyznê",
			(char *)"Mê¿czyzn±", (char *)"Mê¿czy¼nie"
	},
	{ (char *)"Kobieta", (char *)"Kobiety", (char *)"Kobiecie",(char *) "Kobietê",
			(char *)"Kobiet±", (char *)"Kobiecie"                                 // Pixel
	},
	(char *)"S± najliczniejsz± i najzwyklejsz± z ras zamieszkuj±cych gataktyke. Nie" NL
	"posiadaj± ¿adnych specjalnych umiejêtno¶ci czy bonusów, ale tak¿e nie maj±" NL
	"wielu wad. Ludzie ³atwo przystosowuj± sie do wszystkich warunków i s± zdolni" NL
	"do obrania wiekszo¶ci klas. Ludzie posiadaj± dwa ramiona, dwie nogi i" NL
	"okr±g³e g³owy. Zaskakuj±co ludzie maj± tendencjê wygl±dac inaczej od siebie." NL,
	FRACE_PC_AVAIL,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	(LANG_DATA *)&lang_common
  , {524286,	16776958,	1022,	1179666,	16777214,	44922488,	2145222630,	3850174,	0}
};

/** Trog: jezyk bazowy (u nas common) */
const LANG_DATA lang_common =
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	(char *)"Basic",
	(char *)"",
	(char *)"Nie mo¿esz mówiæ we wspólnym.",
	99,
	75,
	10,
	0
};

const	struct	class_type	class_table	[MAX_ABILITY]	=
{
	{	"combat", 		"Com",
		{
		    "Walka",
		    "walki",
		    "walce",
		    "walkê",
		    "walk±",
		    "walce",
		    "w walce"
		},
#if defined (ARMAGEDDON)
		{ 5, 0, 0, 0, 2, 0, 0 },
		18,
		1,
#endif
		REQ_STR
	},
	{	"piloting", 		"Pil",
		{
		    "Pilota¿",
		    "pilota¿u",
		    "pilota¿owi",
		    "pilota¿",
		    "pilota¿em",
		    "pilota¿u",
		    "w pilotowaniu"
		},
#if defined (ARMAGEDDON)
		{ 2, 2, 1, 0, 0, 0, 0 },
		18,
		4,
#endif
		REQ_WIS
	},
	{	"engineering", 		"Eng",
		{
		    "In¿ynieria",
		    "in¿ynierii",
		    "in¿ynierii",
		    "in¿ynieriê",
		    "in¿ynieri±",
		    "in¿ynierii",
		    "w in¿ynierii"
		},
#if defined (ARMAGEDDON)
		{ 0, 3, 4, 0, 0, 0, 0 },
		18,
		6,
#endif
		REQ_INT
	},
	{	"bounty hunting", 	"Bou",
		{
		    "£owienie nagród",
		    "³owienia nagród",
		    "³owieniu nagród",
		    "³owienie nagród",
		    "³owieniem nagród",
		    "³owieniu nagród",
		    "w ³owieniu nagród"
		},
#if defined (ARMAGEDDON)
		{ 3, 3, 0, 0, 1, 0, 0 },
		18,
		0,
#endif
		REQ_CON
	},
	{	"smuggling", 		"Smu",
		{
		    "Przemyt",
		    "przemytu",
		    "przemytowi",
		    "przemyt",
		    "przemytem",
		    "przemycie",
		    "w przemycie"
		},
#if defined (ARMAGEDDON)
		{ 0, 1, 0, 4, 1, 1, 0 },
		18,
		3,
#endif
		REQ_DEX
	},
	{
		"diplomacy", 		"Dip",
		{
		    "Dyplomacja",
		    "dyplomacji",
		    "dyplomacji",
		    "dyplomacjê",
		    "dyplomacj±",
		    "dyplomacji",
		    "w dyplomacji"
		},
#if defined (ARMAGEDDON)
		{ 0, 5, 1, 1, 0, 0, 0 },
		18,
		6,
#endif
		REQ_CHA
	},
	{	"leadership", 		"Lea",
		{
		    "Przywódczo¶æ",
		    "przywódczo¶ci",
		    "przywódczo¶ci",
		    "przywódczo¶æ",
		    "przywódczo¶ci±",
		    "przywódczo¶ci",
		    "w przywództwie"
		},
#if defined (ARMAGEDDON)
		{ 1, 2, 3, 0, 1, 0, 0 },
		18,
		5,
#endif
		REQ_WIS
	},
	{	"force", 		"For",
		{
		    "W³adanie Moc±",
		    "w³adania Moc±",
		    "w³adaniu Moc±",
		    "w³adanie Moc±",
		    "w³adaniem Moc±",
		    "w³adaniu Moc±",
		    "we w³adaniu Moc±"
		},
#if defined (ARMAGEDDON)
		{ 0, 5, 1, 1, 0, 0, 0 },
		18,
		2,
#endif
		REQ_FRC
	}
};



/*
 * Attribute bonus tables.
 */
const	struct	str_app_type	str_app		[26]		=
{
    { -5, -4,   0,  0 },  /* 0  */
    { -5, -4,   3,  1 },  /* 1  */
    { -3, -2,   3,  2 },
    { -3, -1,  10,  3 },  /* 3  */
    { -2, -1,  25,  4 },
    { -2, -1,  55,  5 },  /* 5  */
    { -1,  0,  80,  6 },
    { -1,  0,  90,  8 },
    {  0,  0, 100, 10 },
    {  0,  0, 100, 12 },
    {  0,  0, 115, 14 }, /* 10  */
    {  0,  0, 115, 15 },
    {  0,  0, 140, 16 },
    {  0,  0, 140, 17 }, /* 13  */
    {  0,  1, 170, 18 },
    {  1,  1, 170, 19 }, /* 15  */
    {  1,  2, 195, 20 },
    {  2,  3, 220, 22 },
    {  2,  4, 250, 25 }, /* 18  */
    {  3,  5, 400, 30 },
    {  3,  6, 500, 35 }, /* 20  */
    {  4,  7, 600, 40 },
    {  5,  7, 700, 45 },
    {  6,  8, 800, 50 },
    {  8, 10, 900, 55 },
    { 10, 12, 999, 60 }  /* 25   */
};



const	struct	int_app_type	int_app		[26]		=
{
    {  3 },	/*  0 */
    {  5 },	/*  1 */
    {  7 },
    {  8 },	/*  3 */
    {  9 },
    { 10 },	/*  5 */
    { 11 },
    { 12 },
    { 13 },
    { 15 },
    { 17 },	/* 10 */
    { 19 },
    { 22 },
    { 25 },
    { 28 },
    { 31 },	/* 15 */
    { 34 },
    { 37 },
    { 40 },	/* 18 */
    { 44 },
    { 49 },	/* 20 */
    { 55 },
    { 60 },
    { 70 },
    { 85 },
    { 99 }	/* 25 */
};



const	struct	wis_app_type	wis_app		[26]		=
{
    { 0 },	/*  0 */
    { 0 },	/*  1 */
    { 0 },
    { 0 },	/*  3 */
    { 0 },
    { 1 },	/*  5 */
    { 1 },
    { 1 },
    { 1 },
    { 2 },
    { 2 },	/* 10 */
    { 2 },
    { 2 },
    { 2 },
    { 2 },
    { 3 },	/* 15 */
    { 3 },
    { 4 },
    { 5 },	/* 18 */
    { 5 },
    { 5 },	/* 20 */
    { 6 },
    { 6 },
    { 6 },
    { 6 },
    { 7 }	/* 25 */
};



const	struct	dex_app_type	dex_app		[26]		=
{
    {   60 },   /* 0 */
    {   50 },   /* 1 */
    {   50 },
    {   40 },
    {   30 },
    {   20 },   /* 5 */
    {   10 },
    {    0 },
    {    0 },
    {    0 },
    {    0 },   /* 10 */
    {    0 },
    {    0 },
    {    0 },
    {    0 },
    { - 10 },   /* 15 */
    { - 15 },
    { - 20 },
    { - 30 },
    { - 40 },
    { - 50 },   /* 20 */
    { - 60 },
    { - 75 },
    { - 90 },
    { -105 },
    { -120 }    /* 25 */
};



const	struct	con_app_type	con_app		[26]		=
{
    { -4, 20 },   /*  0 */
    { -3, 25 },   /*  1 */
    { -2, 30 },
    { -2, 35 },	  /*  3 */
    { -1, 40 },
    { -1, 45 },   /*  5 */
    { -1, 50 },
    {  0, 55 },
    {  0, 60 },
    {  0, 65 },
    {  0, 70 },   /* 10 */
    {  0, 75 },
    {  0, 80 },
    {  0, 85 },
    {  0, 88 },
    {  1, 90 },   /* 15 */
    {  2, 95 },
    {  2, 97 },
    {  3, 99 },   /* 18 */
    {  3, 99 },
    {  4, 99 },   /* 20 */
    {  4, 99 },
    {  5, 99 },
    {  6, 99 },
    {  7, 99 },
    {  8, 99 }    /* 25 */
};


const	struct	cha_app_type	cha_app		[26]		=
{
    { - 60 },   /* 0 */
    { - 50 },   /* 1 */
    { - 50 },
    { - 40 },
    { - 30 },
    { - 20 },   /* 5 */
    { - 10 },
    { -  5 },
    { -  1 },
    {    0 },
    {    0 },   /* 10 */
    {    0 },
    {    0 },
    {    0 },
    {    1 },
    {    5 },   /* 15 */
    {   10 },
    {   20 },
    {   30 },
    {   40 },
    {   50 },   /* 20 */
    {   60 },
    {   70 },
    {   80 },
    {   90 },
    {   99 }    /* 25 */
};

/* Have to fix this up - not exactly sure how it works (Scryn) */
const	struct	lck_app_type	lck_app		[26]		=
{
    {   60 },   /* 0 */
    {   50 },   /* 1 */
    {   50 },
    {   40 },
    {   30 },
    {   20 },   /* 5 */
    {   10 },
    {    0 },
    {    0 },
    {    0 },
    {    0 },   /* 10 */
    {    0 },
    {    0 },
    {    0 },
    {    0 },
    { - 10 },   /* 15 */
    { - 15 },
    { - 20 },
    { - 30 },
    { - 40 },
    { - 50 },   /* 20 */
    { - 60 },
    { - 75 },
    { - 90 },
    { -105 },
    { -120 }    /* 25 */
};

const	struct	frc_app_type	frc_app		[26]		=
{
    {    0 },   /* 0 */
    {    0 },   /* 1 */
    {    0 },
    {    0 },
    {    0 },
    {    0 },   /* 5 */
    {    0 },
    {    0 },
    {    0 },
    {    0 },
    {    0 },   /* 10 */
    {    0 },
    {    0 },
    {    0 },
    {    0 },
    {    0 },   /* 15 */
    {    0 },
    {    0 },
    {    0 },
    {    0 },
    {    0 },   /* 20 */
    {    0 },
    {    0 },
    {    0 },
    {    0 },
    {    0 }    /* 25 */
};



/*
 * Liquid properties.
 * Used in #OBJECT section of area file.
 */
const	struct	liq_type	liq_table	[LIQ_MAX]	=
{
    { "wodê",			"prze¼roczyst±",{  0, 1, 10 }	},  /*  0 */
    { "piwo",			"br±zow±",	{  3, 2,  5 }	},
    { "wino",			"ró¿owaw±",	{  5, 2,  5 }	},
    { "ale",			"br±zow±",	{  2, 2,  5 }	},
    { "ciemne ale",		"ciemnobr±zow±",{  1, 2,  5 }	},

    { "whisky",			"z³otaw±",	{  6, 1,  4 }	},  /*  5 */
    { "lemoniadê",		"ró¿ow±",	{  0, 1,  8 }	},
    { "gorza³ê",			"mêtn±",	{ 10, 0,  0 }	},
    { "lokalny specja³",	"mêtniaw±",	{  3, 3,  3 }	},
    { "sok ¿urawinowy",		"zielon±",	{  0, 4, -8 }	},

    { "mleko",			"bia³±",	{  0, 3,  6 }	},  /* 10 */
    { "herbatê",		"zielonobr±zow±",{  0, 1,  6 }	},
    { "kawê",			"czarn±",	{  0, 1,  6 }	},
    { "krew",			"czerwon±",	{  0, 2, -1 }	},
    { "morsk± wodê",		"prze¼roczyst±",{  0, 1, -2 }	},

    { "colê",			"wi¶niow±",	{  0, 1,  5 }	},  /* 15 */
    { "poncz",			"jasnobr±zow±",	{  4, 2,  5 }	},  /* 16 */
    { "grog",			"br±zow±",	{  3, 2,  5 }	},  /* 17 */
    { "shake'a",              	"kremow±",      {  0, 8,  5 }   },   /* 18 */
    { "drinka",              	"przezroczyst±",{  0, 8,  5 }   }   /* 19 */
};

const char *	const	attack_table	[13] =
{
    "uderzenie",
    "ciêcie",  "pchniêcie",  "palniêcie", "smagniêcie", "darpniêcie",
    "buchniêcie",  "r±bniêcie", "trza¶niêcie", "postrzelenie", "ugryzienie",
    "d¼gniêcie", "ssanie"
};

const char *	const	sith_hatred	[10] =
{
    FG_RED "Czujesz wzrastaj±c± w sobie nienawi¶æ!" EOL,
    FG_RED "Zaczynasz coraz bardziej nienawidziæ ¿ywych istot!" EOL,
    FG_RED "Twoja nienawi¶æ staje siê coraz wiêksza!" EOL,
    FG_RED "Strach prowadzi Ciê prosto do Ciemnej Strony Mocy!" EOL,
    FG_RED "Z ka¿d± chwil± Twój gniew staje siê silniejszy!" EOL,
    FG_RED "Strach staje siê Twoim sprzymierzeñcem!" EOL,
    FG_RED "Powoli odczuwasz potêgê Ciemnej Strony Mocy!" EOL,
    FG_RED "Gniew przyczynia siê do Twojej si³y!" EOL,
    FG_RED "Czujesz jak Ciemna Strona Mocy wspomaga Ciê!" EOL,
    FG_RED "Dziêki nienawi¶ci zyskujesz si³ê!" EOL
};
const char *  male_desc_y[6]={"y","ego","emu","ego","ym","ym" };
const char *	male_desc_i[6]={"i","iego","iemu","iego","im","im"};
const char *  female_desc_y[6]={"a","ej","ej","±","±","ej"};
const char *  female_desc_i[6]={"a","iej","iej","±","±","iej"};
const char *  category_desc[]={"W³osy","Budowa","Wiek","Skóra","Zarost","Twarz","Ogólne","Oczy","Futro","Empty"};
const	struct	char_desc	desc_table1[]=
{
	{	HAIR,"bia³ow³os",				Y_END	},
	{	HAIR,"ciemnow³os",				Y_END	},
	{	HAIR,"czarnow³os",				Y_END	},
	{	HAIR,"czerwonow³os",			Y_END	},
	{	HAIR,"d³ugow³os",				Y_END	},
	{	HAIR,"jasnow³os",				Y_END	},
	{	HAIR,"kêdzierzaw",				Y_END	},
	{	HAIR,"krótkow³os",				Y_END	},
	{	HAIR,"kud³at",					Y_END	},
	{	HAIR,"³ysiej±c",				Y_END	},
	{	HAIR,"³ys",						Y_END	},
	{	HAIR,"niebieskowlos",			Y_END	},
	{	HAIR,"ognistow³os",				Y_END	},
	{	HAIR,"pomarañczowow³os",		Y_END	},
	{	HAIR,"rudow³os",				Y_END	},
	{	HAIR,"rud",						Y_END	},
	{	HAIR,"ry¿",						Y_END	},
	{	HAIR,"siw",						Y_END	},
	{	HAIR,"siwow³os",				Y_END	},
	{ (desc_type)0 , "",Y_END }
};
const	struct	char_desc	desc_table2[]=
{
	{	BUILD,"barczyst",			Y_END	}, //20
	{	BUILD,"bary³kowat",			Y_END	},
	{	BUILD,"beczu³kowat",		Y_END	},
	{	BUILD,"brzuchat",			Y_END	},
	{	BUILD,"chud",				Y_END	},
	{	BUILD,"d³ugonog",			I_END	},
	{	BUILD,"grub",				Y_END	},
	{	BUILD,"insektoidaln",		Y_END	},
	{	BUILD,"krêp",				Y_END	},
	{	BUILD,"krótkonog",			I_END	},
	{	BUILD,"krzywonog",			I_END	}, //30
	{	BUILD,"masywn",				Y_END	},
	{	BUILD,"muskularn",			Y_END	},
	{	BUILD,"niewysok",			I_END	},
	{	BUILD,"nisk",				I_END	},
	{	BUILD,"okaza³",				Y_END	},
	{	BUILD,"pêkat",				Y_END	},
	{	BUILD,"przysadzist",		Y_END	},
	{	BUILD,"szczup³",			Y_END	},
	{	BUILD,"t³u¶ciutk",			I_END	},
	{	BUILD,"umiê¶nion",			Y_END	}, //40
	{	BUILD,"wychudzon",			Y_END	},
	{	BUILD,"wysok",				I_END	},
	{ (desc_type)0 , "",Y_END }
};
const	struct	char_desc	desc_table3[]=
{
	{	AGE,"dojrza³",            Y_END		},
	{	AGE,"leciw",              Y_END		},
	{	AGE,"ma³oletn",           I_END		}, //40
	{	AGE,"m³od",               Y_END		},
	{	AGE,"podrastaj±c",        Y_END		},
	{	AGE,"podstarza³",         Y_END		},
	{	AGE,"star",               Y_END		},
	{	AGE,"szczawikowat",       Y_END		},
	{	AGE,"wiekow",             Y_END		},
	{ (desc_type)0 , "",Y_END }
};
const	struct	char_desc	desc_table4[]=
{
	{	SKIN,"bladoskór",			Y_END	},
	{	SKIN,"br±zowoskór",			Y_END 	},
	{	SKIN,"czerwonoskór",		Y_END 	},
	{	SKIN,"ciemnoskór",			Y_END 	},
	{	SKIN,"fioletowoskór",		Y_END 	},
	{	SKIN,"jaskrawoskór",		Y_END 	},
	{	SKIN,"jasnoskór",			Y_END 	},
	{	SKIN,"gadoskór",			Y_END 	},
	{	SKIN,"karmazynowoskór",		Y_END 	},
	{	SKIN,"kosmat",				Y_END 	},
	{	SKIN,"lethañsk",			I_END 	},
	{	SKIN,"³ososiowoskór",		Y_END 	},
	{	SKIN,"niebieskoskór",		Y_END 	},
	{	SKIN,"pomarañczowoskór",	Y_END 	},
	{	SKIN,"rutiañsk",			I_END 	},
	{	SKIN,"turkosowoskór",		Y_END 	},
	{	SKIN,"opalon",				Y_END 	},
	{	SKIN,"stalowoskór",			Y_END 	},
	{	SKIN,"szaroskór",			Y_END 	},
	{	SKIN,"¶niad",				Y_END 	},
	{	SKIN,"w³ochat",				Y_END 	},
	{	SKIN,"zielonoskór",			Y_END 	},
	{	SKIN,"¿ó³toskór",			Y_END 	},
	{ (desc_type)0 , "",Y_END }
  };
const	struct	char_desc	desc_table5[]=
{
	{	BEARD,"bia³obrod",				Y_END	},
	{	BEARD,"b³êkitnobrod",			Y_END	},
	{	BEARD,"brodat",					Y_END	},
	{	BEARD,"ciemnobrod",				Y_END	},
	{	BEARD,"czarnobrod",				Y_END	},
	{	BEARD,"czerwonobrod",			Y_END	},
	{	BEARD,"d³ugobrod",				Y_END	},
	{	BEARD,"jasnobrod",				Y_END	},
	{	BEARD,"krótkobrod",				Y_END	}, //60
	{	BEARD,"kruczobrod",				Y_END	},
	{	BEARD,"krzaczast",				Y_END	},
	{	BEARD,"ognistobrod",			Y_END	},
	{	BEARD,"p³omiennobrod",			Y_END	},
	{	BEARD,"pomarañczowobrod",		Y_END	},
	{	BEARD,"rudobrod",				Y_END	},
	{	BEARD,"sinobrod",				Y_END	},
	{	BEARD,"siwobrod",				Y_END	},
	{	BEARD,"sko³tunion",				Y_END	},
	{	BEARD,"¶nie¿nobrod",			Y_END	}, //70
	{	BEARD,"w±sat",					Y_END	},
	{	BEARD,"zaro¶niêt",				Y_END	},
	{	BEARD,"zielonobrod",			Y_END	},
	{	BEARD,"¿ó³tobrod",				Y_END	},
	{ (desc_type)0 , "",Y_END }
  };
const	struct	char_desc	desc_table6[]=
{
	{	FACE,"beznos",				Y_END	},
	{	FACE,"bezw³os",				Y_END	},
	{	FACE,"czerwononos",			Y_END	},
	{	FACE,"d³ugonos",			Y_END	},
	{	FACE,"d³ugouch",			Y_END	},
	{	FACE,"dziobat",				Y_END	},
	{	FACE,"g³adkolic",			Y_END	},
	{	FACE,"jednorog",			I_END	},
	{	FACE,"krótkonos",			Y_END	},
	{	FACE,"krzywonos",			Y_END	}, //80
	{	FACE,"mackog³ow",			Y_END	},
	{	FACE,"nakrapian",			Y_END	},
	{	FACE,"ogorza³",				Y_END	},
	{	FACE,"ospowat",				Y_END	},
	{	FACE,"ostronos",			Y_END	},
	{	FACE,"piegowat",			Y_END	},
	{	FACE,"p³askonos",			Y_END	},
	{	FACE,"pryszczat",			Y_END	},
	{	FACE,"pucu³owat",			Y_END	},
	{	FACE,"rogat",				Y_END	},
	{	FACE,"rumian",				Y_END	},
	{	FACE,"szpiczastouch",		Y_END	},
	{	FACE,"szczerbat",			Y_END	},
	{	FACE,"wielkonos",			Y_END	},
	{	FACE,"wielkog³ow",			Y_END	},
	{ (desc_type)0 , "",Y_END }
  };
const	struct	char_desc	desc_table7[]=
{
	{	GENERAL,"bezwzglêdn",		Y_END	}, //90
	{	GENERAL,"brudn",			Y_END	},
	{ 	GENERAL,"chciw",			Y_END	},
	{ 	GENERAL,"cwan",				Y_END	},
	{	GENERAL,"czujn",			Y_END	},
	{	GENERAL,"dostojn",			Y_END	},
	{	GENERAL,"dumn",				Y_END	},
	{	GENERAL,"energiczn",		Y_END	},
	{	GENERAL,"gro¼n",			Y_END	},
	{	GENERAL,"hard",				Y_END	},
	{ 	GENERAL,"inteligentn",		Y_END	},
	{	GENERAL,"ma³omówn",			Y_END	},
	{	GENERAL,"melancholijn",		Y_END	},
	{	GENERAL,"nerwow",			Y_END	}, //100
	{ 	GENERAL,"oble¶n",			Y_END	},
	{	GENERAL,"opanowan",			Y_END	},
	{	GENERAL,"paskudn",			Y_END	},
	{	GENERAL,"ponur",			Y_END	},
	{	GENERAL,"porywcz",			Y_END	},
	{	GENERAL,"powa¿n",			Y_END	},
	{	GENERAL,"przyg³up",			I_END	},
	{	GENERAL,"przyjacielsk",		I_END	},
	{	GENERAL,"rozmown",			Y_END	},
	{	GENERAL,"spokojn",			Y_END	},
	{	GENERAL,"¶mierdz±c",		Y_END	},
	{	GENERAL,"têp",				Y_END	},
	{	GENERAL,"weso³",			Y_END	}, //110
	{	GENERAL,"wynios³",			Y_END	},
	{	GENERAL,"zawadiack",		I_END	},
	{	GENERAL,"z³o¶liw",			Y_END	},
	{ (desc_type)0 , "",Y_END }
  };
const	struct	char_desc	desc_table8[]=
{
	{	EYES,"b³êkitnook",			I_END	},
	{	EYES,"br±zowook",			I_END	},
	{	EYES,"ciemnook",			I_END	},
	{	EYES,"czarnook",			I_END	},
	{	EYES,"czerwonook",			I_END	},
	{	EYES,"gadook",				I_END	},
	{	EYES,"jasnook",				I_END	},
	{	EYES,"jednook",				I_END	}, //120
	{	EYES,"krwistook",			I_END	},
	{	EYES,"mêtnook",				I_END	},
	{	EYES,"niebieskook",			I_END	},
	{	EYES,"pomarañczowook",		I_END	},
	{	EYES,"piwnook",				I_END	},
	{	EYES,"rybiook",				I_END	},
	{	EYES,"szarook",				I_END	},
	{	EYES,"¶wiñskook",			I_END	},
	{	EYES,"wielkook",			I_END	},
	{	EYES,"wy³upiastook",		I_END	},
	{	EYES,"zezowat",				Y_END	},
	{	EYES,"zielonook",			I_END	},
	{	EYES,"zó³took",				I_END	},
	{ (desc_type)0 , "",Y_END }
  };
const	struct	char_desc	desc_table9[]=
{
	{	FUR,"br±zowym",				Y_END	},
	{	FUR,"ciemnobr±zowym",		Y_END	},
	{	FUR,"czarnym",				Y_END	},
	{	FUR,"jasnobr±zowym",		Y_END	},
	{	FUR,"kosmatym",				Y_END	},
	{	FUR,"rozczochranym",		Y_END	},
	{	FUR,"rudym",				Y_END	},
	{	FUR,"srebrzystym",			Y_END	},
	{	FUR,"siwym",				Y_END	},
	{	(desc_type)0,"",Y_END	}
  };

//dodawac kolejne pozycje na koñcu, nie sortowac
const	struct	char_desc	 *desc_table [MAX_DESC_TYPES]=
//const	struct	char_desc	 desc_table [MAX_DESC_TYPES][]=
{
	desc_table1,
	desc_table2,
	desc_table3,
	desc_table4,
	desc_table5,
	desc_table6,
	desc_table7,
	desc_table8,
	desc_table9
};
