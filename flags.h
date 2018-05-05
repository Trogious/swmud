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
*			                  Flags header file                            *
****************************************************************************/

#ifndef FLAGS_HEADER_FILE
#define FLAGS_HEADER_FILE

/* TYPE DEFINITIONS */
#ifndef int64
typedef long long int64;
#endif

/* Command logging types. */
typedef enum
{
	LOG_NORMAL,
	LOG_ALWAYS,
	LOG_NEVER,
	LOG_BUILD,
	LOG_HIGH,
	LOG_COMM,
	LOG_PROG,
	LOG_ALL,

	LOG_BUG = 1000
} log_types;

/*
 * Return types for move_char, damage, greet_trigger, etc, etc
 * Added by Thoric to get rid of bugs
 */
typedef enum
{
	rNONE, rCHAR_DIED, rVICT_DIED, rBOTH_DIED, rCHAR_QUIT, rVICT_QUIT,
	rBOTH_QUIT, rSPELL_FAILED, rOBJ_SCRAPPED, rOBJ_EATEN, rOBJ_EXPIRED,
	rOBJ_TIMER, rOBJ_SACCED, rOBJ_QUAFFED, rOBJ_USED, rOBJ_EXTRACTED,
	rOBJ_DRUNK, rCHAR_IMMUNE, rVICT_IMMUNE, rCHAR_AND_OBJ_EXTRACTED = 128,

	rERROR = 255
} ret_types;

/* Time and weather stuff. */
typedef enum
{
	SUN_DARK, SUN_RISE, SUN_LIGHT, SUN_SET
} sun_positions;

typedef enum
{
	SKY_CLOUDLESS, SKY_CLOUDY, SKY_RAINING, SKY_LIGHTNING
} sky_conditions;

/* Connected state for a channel. */
typedef enum
{
	/* nanny */
	CON_GET_NAME					= -1,
	CON_GET_OLD_PASSWORD			= -2,
	CON_READ_MOTD					= -3,
	CON_READ_IMOTD					= -4,
	CON_CONFIRM_NEW_NAME			= -5,
	CON_GET_NEW_COLOUR				= -6,
	CON_GET_CHAR_SET				= -7,
	CON_CHECK_CHAR_SET				= -8,
	CON_GET_MSP						= -9,
	CON_WPROWADZENIE				= -10,
	CON_GET_NEW_RACE				= -11,
	CON_CONFIRM_NEW_RACE			= -12,
	CON_GET_NEW_SEX					= -13,
	CON_GET_NEW_CLASS				= -14,
	CON_CONFIRM_CLASS				= -15,
	CON_GET_DOPELNIACZ				= -16,
	CON_GET_CELOWNIK				= -17,
	CON_GET_BIERNIK					= -18,
	CON_GET_NARZEDNIK				= -19,
	CON_GET_MIEJSCOWNIK				= -20,
	CON_CHECK_ODMIANE				= -21,
	CON_ROLL_STATS					= -22,
	CON_STATS_OK					= -23,
	CON_GET_NEW_PASSWORD			= -24,
	CON_CONFIRM_NEW_PASSWORD		= -25,
	CON_SHOW_LOOK_TRAIT_GROUP		= -26,
  	CON_SHOW_LOOK_TRAIT       	    = -27,
  	CON_CONFIRM_LOOK_TRAIT_CHOICE   = -28,
  	CON_CONFIRM_ALL_LOOK_TRAIT_CHOICE   = -29,

	/* playing ma byæ 0 */
	CON_PLAYING						= 0,

	/* tutaj dodawaæ TRYBY w jakich jest gracz (po zalogowaniu siê) */
	CON_TCEDITOR,
	CON_RAEDITOR,
	CON_PROEDITOR,
	CON_CEDITOR,
	CON_SKEDITOR,
	CON_SHEDITOR,
	CON_PEDITOR,
	CON_SSEDITOR,
	CON_HEDITOR,
	CON_AEDITOR,
	CON_RPEDITOR,
	CON_OPEDITOR,
	CON_MPEDITOR,
	CON_REDITOR,
	CON_OEDITOR,
	CON_MEDITOR,
	CON_QEDITOR,
	CON_DIALOGEDIT,
	CON_MATEDITOR,            //MATERIAL EDITOR
	CON_STOCK_MARKET,
	CON_BUILD_HQ,			//budowanie HQ
	CON_SPEAKING,
	/* Thanos: Holonet	   */
	CON_HOLONET						= 100,

	CON_CONFIRM_SUICIDE				= 500,
	CON_OLD_PASS_TO_CHANGE			= 501,
	CON_NEW_PASS_TO_CHANGE			= 502,
	CON_CONFIRM_NEW_PASS_TO_CHANGE	= 503,

	CON_RENAMING					= 1000,

	CON_COPYOVER_RECOVER			= 9999
} connection_types;

/* Character substates */
typedef enum
{
	SUB_NONE, SUB_PAUSE, SUB_RESTRICTED, SUB_REPEATCMD, SUB_BAN_DESC,

	/* timer types ONLY below this point */
	SUB_TIMER_DO_ABORT = 128, SUB_TIMER_CANT_ABORT
} char_substates;

/* Trog: Aldegard's BIG stuff :-) (space) */
typedef enum
{
	SHIP_CIVILIAN, SHIP_REPUBLIC, SHIP_IMPERIAL, MOB_SHIP, SHIP_PIRATE, SHIP_YUUZHAN
} ship_types;

typedef enum
{
	SHIP_DOCKED, SHIP_READY, SHIP_BUSY_1, SHIP_BUSY_2, SHIP_BUSY_3, SHIP_BUSY_4, SHIP_BUSY_5,
	SHIP_BUSY_6, SHIP_BUSY_7, SHIP_BUSY_8, SHIP_BUSY_9, SHIP_BUSY_10, SHIP_REFUEL, SHIP_REPOSITORY,
	SHIP_LAUNCH, SHIP_LAUNCH_2, SHIP_LAND, SHIP_LAND_2, SHIP_HYPERSPACE, SHIP_DISABLED, SHIP_FLYING,
	SHIP_EXPLODE, SHIP_TRACTORED, SHIP_DOCKED2, SHIP_DOCKED2_2, SHIP_DOCKED2_FIN
} ship_states;

typedef enum
{
	MISSILE_READY, MISSILE_FIRED, MISSILE_RELOAD, MISSILE_RELOAD_2, MISSILE_DAMAGED
} missile_states;

typedef enum
{
	TIE_FIGHTER_TIE_LN_SHIP, TIE_VANGUARD_TIE_RC_SHIP, TIE_FIRE_CONTROL_TIE_FC_SHIP,
	TIE_GROUND_ATTACK_TIE_GT_SHIP, TIE_BOMBER_TIE_SA_SHIP, TIE_SHUTTLE_TIE_SH_SHIP,
	TIE_BOARDING_CRAFT_TIE_BC_SHIP, TIE_INTERCEPTOR_SHIP, ASSAULT_GUNBOAT_SHIP, TIE_X1_ADVANCED_TIE_SHIP,
	TIE_AVENGER_TIE_AD_SHIP, TIE_ADVANCED_AVENGER_SHIP, XWING_SHIP, XWING_RECON_SHIP, YWING_SHIP,
	YWING_LONGPROBE_SHIP, AWING_SHIP, BWING_SHIP, TIE_RAPTOR_SHIP, TIE_DEFENDER_SHIP,
	SCIMITAR_ASSAULT_BOMBER_SHIP, A9_VIGILANCE_SHIP, I7_HOWLRUNNER_SHIP, PREYBIRD_SHIP,
	TIE_DROID_TIE_D_TIE_DR_SHIP, MISSILE_BOAT_SHIP, TIE_PHANTOM_SHIP, TWING_SHIP, EWING_SHIP,
	BWING_E_SHIP, XWING_II_SHIP, KWING_SHIP, CLOAKSHAPE_FIGHTER_SHIP, TOCSAN_8Q_FIGHTER_SHIP,
	ZEBRA_STARFIGHTER_SHIP, AUTHORITY_IRD_SHIP, C73_TRACKER_SHIP, Z95_HEADHUNTER_MK_I_SHIP,
	Z95_HEADHUNTER_SHIP, Z95_HEADHUNTER_XT_SHIP, Z95AF4_HEADHUNTER_SHIP, R41_STARCHASER_SHIP,
	TIEWING_TIEY_UGLY_SHIP, YTIE_UGLY_SHIP, XTIE_UGLY_SHIP, XORCEPTOR_XT_I_UGLY_SHIP, TRIFIGHTER_SHIP,
	DEATHSEED_CHIR_DAKI_SHIP, LAMBDA_SHUTTLE_SHIP, TYDERIAN_SHUTTLE_SHIP, MU2_SHUTTLE_SHIP,
	ESCORT_SHUTTLE_SHIP, STORMTROOPER_TRANSPORT_SHIP, GREK_TROOP_SHUTTLE_SHIP, GAMMA_ASSAULT_SHUTTLE_SHIP,
	KATARN_BOARDING_SHUTTLE_SHIP, ULIG_ABAHA_2_WARPOD_SHIP, ASSAULT_TRANSPORT_SHIP, MT_191_DROPSHIP_SHIP,
	KAPPA_TROOP_SHUTTLE_SHIP, SENTINEL_LANDING_CRAFT_SHIP, AEGIS_COMBAT_SHUTTLE_SHIP, WARLORD_LANDING_BARGE_SHIP,
	Y4_RAPTOR_SHIP, VWING_TRANSPORT_SHIP, LONE_SCOUT_A_SHIP, IMPERIAL_SCOUT_CRAFT_SHIP, GYRE_SCOUT_SHIP,
	FIRESPRAY_PATROL_CRAFT_SHIP, MANDALMOTORS_PURSUER_SHIP, CONQUEROR_ASSAULT_SHIP_SHIP,
	SKIPRAY_BLASTBOAT_SHIP, GUARDIAN_LIGHT_CRUISER_SHIP, CUSTOMS_FRIGATE_SHIP,
	IPV1_SYSTEM_PATROL_CRAFT_SHIP, PASSENGER_LINER_SHIP, FERRYBOAT_LINER_SHIP,
	KLEEQUE_TRANSPORT_SHIP, BR23_COURIER_SHIP, LUXURY_3000_YACHT_SHIP, BAUDO_YACHT_SHIP,
	YT1210_CORELLIAN_TRANSPORT_SHIP, YT1300_CORELLIAN_TRANSPORT_SHIP, YT2000_CORELLIAN_TRANSPORT_SHIP,
	YT2400_CORELLIAN_TRANSPORT_SHIP, GHTROC_FREIGHTER_SHIP, AZZ3_FREIGHTER_SHIP, Z10_SEEKER_SHIP,
	HERMES_COURIER_SHIP, MUURIAN_TRANSPORT_SHIP, BARLOZ_FREIGHTER_SHIP, HT2200_MEDIUM_FREIGHTER_SHIP,
	XIYTIAR_TRANSPORT_SHIP, BULK_BARGE_SHIP, MEDIUM_TRANSPORT_SHIP, CARGO_FERRY_SHIP,
	ACTION_IV_TRANSPORT_SHIP, BULK_FREIGHTER_SHIP, STAR_GALLEON_SHIP, SPACE_TANKER_SHIP,
	CARGO_TRAIN_SHIP, CONTAINER_SHIP_SHIP, MARAUDER_CORVETTE_SHIP, CORELLIAN_CORVETTE_SHIP,
	MODIFIED_CORVETTE_SHIP, ASSASIN_CORVETTE_SHIP, CUSTOMS_CORVETTE_SHIP, CORELLIAN_GUNSHIP_SHIP,
	VIBRE_CRUISER_SHIP, NEBULON_ESCORT_FRIGATE_SHIP, NEBULON_ESCORT_FRIGATE_LIGHT_SHIP,
	MODIFIED_ESCORT_FRIGATE_SHIP, LANCER_FRIGATE_SHIP, CALAMARI_FRIGATE_SHIP, QUASAR_FIRE_CRUISER_SHIP,
	IMPERIAL_ESCORT_CARRIER_SHIP, IMPERIAL_FLEET_CARRIER_SHIP, INTERDICTOR_CRUISER_SHIP,
	MEDICAL_FRIGATE_SHIP, MC_EVACUATION_CRUISER_SHIP, DUNGEON_SHIP_SHIP, CARRACK_CRUISER_SHIP,
	STRIKE_CRUISER_SHIP, BULK_CRUISER_SHIP, LIGHT_CALAMARI_CRUISER_SHIP, TASKFORCE_CRUISER_SHIP,
	INVINCIBLE_DREADNAUGHT_SHIP, RENDILLI_DREADNAUGHT_SHIP, ASSAULT_FRIGATE_SHIP,
	MC80_CALAMARI_CRUISER_SHIP, MC80A_CALAMARI_CRUISER_SHIP, MC80B_CALAMARI_CRUISER_SHIP,
	MC90_CALAMARI_CRUISER_SHIP, VICTORY_STAR_DESTROYER_SHIP, VICTORY_II_STAR_DESTROYER_SHIP,
	IMPERIAL_STAR_DESTROYER_SHIP, IMPERIAL_II_STAR_DESTROYER_SHIP, SUPER_STAR_DESTROYER_SHIP,
	SOVEREIGN_SUPER_STAR_DESTROYER_SHIP, ECLIPSE_SUPER_STAR_DESTROYER_SHIP, SPACE_FACTORY_STATION_SHIP,
	EMPRESS_SPACE_PLATFORM_SHIP, GOLAN_I_SPACE_DEFENSE_STATION_SHIP, GOLAN_II_SPACE_DEFENSE_STATION_SHIP,
	GOLAN_III_SPACE_DEFENSE_STATION_SHIP, TORPEDO_SPHERE_SHIP, DEATH_STAR_SHIP, SPACE_TUG_SHIP,
	UTILITY_TUG_SHIP, HEAVY_LIFTER_SHIP, SPACETROOPER_SHIP, PDV_MESSENGER_POD_SHIP,
	IMPERIAL_PROBOT_POD_SHIP, CLOUD_CAR, OCEAN_SHIP, LAND_SPEEDER, WHEELED, LAND_CRAWLER, WALKER,
	NO_SPACE_SHIP, SCOUT_SHIP, FIGHTER_SHIP, MIDSHIP_SHIP, FREIGHTER_SHIP, CAPITAL_SHIP, HUGE_SHIP, PLATFORM_SHIP,
	CORAL_FIGHTER_SHIP, CORAL_BOMBER_SHIP, SWARM_SHIP, SHRIWIRR_SHIP, JABITHA_SHIP,
	KOROS_STRONHA_SPIRAL_ARMED_WORLDSHIP_SHIP, YORIK_VEC_ASSAULT_CRUISER_SHIP, ROIK_CHUUN_MARH_FRIGATE_SHIP,
	KOR_CHOKK_GRAND_CRUISER_SHIP, YORIK_TREMA_TRANSPORT_CARRIER_SHIP,
	MIID_ROIK_WARSHIP_SHIP, PICKET_SHIP_FWSEN_CLASS_SHIP, SHNER_PLANETARY_ASSAULT_CARRIER_SHIP,
	DKEE_CLASS_PWECK_LANDING_SHIP, LWHEKK_CLASS_MANUFACTURING_SHIP, WURRIF_CLASS_CRUISER_SHIP,
	SHNER_CLASS_PLANETARY_ASSAULT_CARRIER_SHIP, SHREE_CLASS_BATTLE_CRUISER, ESCAPE_POD_SHIP, SHIP_MAX
} ship_classes;

typedef enum
{
	CONCUSSION_MISSILE, PROTON_TORPEDO, HEAVY_ROCKET, HEAVY_BOMB
} missile_types;

typedef enum
{
	LASER, IONCANNON, LAUNCHER_M, LAUNCHER_T, LAUNCHER_R, MINE_LAYER, COMPUTER, RADAR, TARGET_ARRAY,
	ASTRO_ARRAY, COMM, SPY_DEVICE, HULL, TANK, CARGO_STD, CARGO_PEP, CARGO_LQD, CARGO_OZ, CARGO_ENE,
	CARGO_RAD, SHIELD, CAMUFLAGE, INTERDICT, TRAC_BEAM, TRAW, HOMING_BICOMM, REACTOR, ENGINE_STD,
	ENGINE_MAN, HYPERDRIVE, AMMOHOLD_M, AMMOHOLD_T, AMMOHILD_R, AMMOHOLD_CH, AMMOHOLD_LM,
	AMMO_RM, MODULE_MAX
} module_types;

typedef enum
{
	PELTA, CANAON, CANAON_2, RUPIE, ALIEN, DOCK_MAX
} ship_dock_types;

typedef enum
{
	GROUP_CLAN, GROUP_COUNCIL, GROUP_GUILD
} group_types;

typedef	enum
{
	CLAN_CONSTRUCTION, CLAN_ORGANIZATION, CLAN_SUBORG, CLAN_GUILD, CLAN_ORDER, CLAN_MAXTYPE
} clan_type;

/* To jest do HQ, ale zle zrobione - bedzie inaczej! */
typedef enum
{
	HQ_NO_ROOM = -1, HQ_CORRIDOR, HQ_GENERATOR_1, HQ_GENERATOR_2, HQ_GENERATOR_3, HQ_STOREROOM_1,
	HQ_STOREROOM_2, HQ_STOREROOM_3, HQ_STOREROOM_4, HQ_STOREROOM_5, HQ_HOTEL, HQ_SPECIAL, HQ_COMPUTERROOM_1,
	HQ_COMPUTERROOM_2, HQ_COMPUTERROOM_3, HQ_ARMORY_1, HQ_ARMORY_2, HQ_ARMORY_3, HQ_DININGROOM,
	HQ_HOSPITAL_1, HQ_HOSPITAL_2, HQ_HOSPITAL_3, HQ_HANGAR_1, HQ_HANGAR_2, HQ_LANDING_1, HQ_LANDING_2,
	HQ_LANDING_3, HQ_LANDING_4, HQ_LANDING_5, HQ_WORKSHOP, HQ_REFINERY, HQ_MAX
} hq_room_type;

typedef enum
{
	HQS_PERFECT, HQS_EXHAUSTED, HQS_DAMAGED, HQS_EFFETE, HQS_HARDLY_STANDING, HQS_RUIN, HQS_MAX
} hq_room_state;

typedef enum
{
	SS_NONE, SS_POISON_DEATH, SS_ROD_WANDS, SS_PARA_PETRI,
	SS_BREATH, SS_SPELL_STAFF
} save_types;

typedef enum
{
	SD_NONE, SD_FIRE, SD_COLD, SD_ELECTRICITY, SD_ENERGY, SD_ACID,
	SD_POISON, SD_DRAIN }
spell_dam_types;

typedef enum
{
	SA_NONE, SA_CREATE, SA_DESTROY, SA_RESIST, SA_SUSCEPT,
	SA_DIVINATE, SA_OBSCURE, SA_CHANGE
} spell_act_types;

typedef enum
{
	SP_NONE, SP_MINOR, SP_GREATER, SP_MAJOR
} spell_power_types;

typedef enum
{
	SC_NONE, SC_LUNAR, SC_SOLAR, SC_TRAVEL, SC_SUMMON,
	SC_LIFE, SC_DEATH, SC_ILLUSION
} spell_class_types;

/* sex  */
typedef enum
{
	SEX_NEUTRAL, SEX_MALE, SEX_FEMALE
} sex_types;

typedef enum
{
	TRAP_TYPE_POISON_GAS = 1, TRAP_TYPE_POISON_DART,    TRAP_TYPE_POISON_NEEDLE,
	TRAP_TYPE_POISON_DAGGER,  TRAP_TYPE_POISON_ARROW,   TRAP_TYPE_BLINDNESS_GAS,
	TRAP_TYPE_SLEEPING_GAS,   TRAP_TYPE_FLAME,	      TRAP_TYPE_EXPLOSION,
	TRAP_TYPE_ACID_SPRAY,	    TRAP_TYPE_ELECTRIC_SHOCK, TRAP_TYPE_BLADE,
	TRAP_TYPE_SEX_CHANGE
} trap_types;

//Thanos: typy oznaczone '/**/' s± praktycznie nieu¿ywane przez kod, lub
//poprostu zbêdne, macie zastepowaæ stare a nie dodawaæ nowe!
typedef enum
{
	ITEM_NONE,
	ITEM_LIGHT,
	ITEM_PAZAAK_CARD,
	ITEM_PAZAAK_SIDE_DECK,
	ITEM_STAFF/**/,
	ITEM_WEAPON,
	ITEM_PART/**/,
	ITEM_MISSILE/**/,
	ITEM_TREASURE,
	ITEM_ARMOR,
	ITEM_POTION,
	ITEM_COMPUTER,
	ITEM_FURNITURE/**/,
	ITEM_TRASH,
	ITEM_OLDTRAP/**/,
	ITEM_CONTAINER,
	ITEM_NOTE/**/,
	ITEM_DRINK_CON,
	ITEM_KEY,
	ITEM_FOOD,
	ITEM_MONEY,
	ITEM_PEN/**/,
	ITEM_BOAT/**/,
	ITEM_CORPSE_NPC,
	ITEM_CORPSE_PC,
	ITEM_FOUNTAIN,
	ITEM_PILL,
	ITEM_BLOOD/**/,
	ITEM_BLOODSTAIN/**/,
	ITEM_SCRAPS,
	ITEM_PIPE,
	ITEM_HERB_CON,
	ITEM_HERB,
	ITEM_INCENSE/**/,
	ITEM_FIRE/**/,
	ITEM_BOOK/**/,
	ITEM_SWITCH,
	ITEM_LEVER,
	ITEM_PULLCHAIN,
	ITEM_BUTTON,
	ITEM_DIAL/**/,
	ITEM_RUNE/**/,
	ITEM_RUNEPOUCH/**/,
	ITEM_MATCH/**/,
	ITEM_TRAP,
	ITEM_MAP/**/,
	ITEM_PORTAL,
	ITEM_PAPER,
	ITEM_TINDER/**/,
	ITEM_LOCKPICK/**/,
	ITEM_SPIKE/**/,
	ITEM_DISEASE/**/,
	ITEM_OIL/**/,
	ITEM_FUEL/**/,
	ITEM_SHIPHCKDEV/*Trog: ship hacking device*/,
	ITEM_LONG_BOW/**/,
	ITEM_CROSSBOW/**/,
	ITEM_AMMO,
	ITEM_QUIVER/**/,
	ITEM_SHOVEL,
	ITEM_SALVE,
	ITEM_RAWSPICE,
	ITEM_LENS,
	ITEM_CRYSTAL,
	ITEM_DURAPLAST,
	ITEM_BATTERY,
	ITEM_TOOLKIT,
	ITEM_DURASTEEL,
	ITEM_OVEN,
	ITEM_MIRROR,
	ITEM_CIRCUIT,
	ITEM_SUPERCONDUCTOR,
	ITEM_COMLINK,
	ITEM_MEDPAC,
	ITEM_FABRIC,
	ITEM_RARE_METAL,
	ITEM_MAGNET/**/,
	ITEM_THREAD,
	ITEM_SPICE,
	ITEM_SMUT,
	ITEM_DEVICE,
	ITEM_SHIPMODULE/*Aldegard*/,
	ITEM_GRENADE,
	ITEM_LANDMINE,
	ITEM_GOVERNMENT/**/,
	ITEM_DROID_CORPSE,
	ITEM_BOLT,
	ITEM_CHEMICAL,
	ITEM_PIECE/*Trog*/,
	ITEM_SHIPDEVICE/*Aldegard*/
} item_types;

/* Apply types (for affects). */
typedef enum
{
	APPLY_NONE,
	APPLY_STR,
	APPLY_DEX,
	APPLY_INT,
	APPLY_WIS,
	APPLY_CON,
	APPLY_SEX,
	APPLY_NULL,/**/
	APPLY_LEVEL, /**/
	APPLY_AGE,/**/
	APPLY_HEIGHT,
	APPLY_WEIGHT,
	APPLY_MANA,
	APPLY_HIT,
	APPLY_MOVE,
	APPLY_GOLD,/**/
	APPLY_EXP,/**/
	APPLY_AC,
	APPLY_HITROLL,
	APPLY_DAMROLL,
	APPLY_SAVING_POISON,
	APPLY_SAVING_ROD,
	APPLY_SAVING_PARA,
	APPLY_SAVING_BREATH,
	APPLY_SAVING_SPELL,
	APPLY_CHA,
	APPLY_AFFECT,
	APPLY_RESISTANT,
	APPLY_IMMUNE,
	APPLY_SUSCEPTIBLE,
	APPLY_WEAPONSPELL,
	APPLY_LCK,
	APPLY_BACKSTAB,
	APPLY_PICK,
	APPLY_TRACK,
	APPLY_STEAL,
	APPLY_SNEAK,
	APPLY_HIDE,
	APPLY_SNIPE,
	APPLY_DETRAP,
	APPLY_DODGE,
	APPLY_PEEK,
	APPLY_SCAN,
	APPLY_GOUGE,
	APPLY_SEARCH,
	APPLY_MOUNT,
	APPLY_DISARM,
	APPLY_KICK,
	APPLY_PARRY,
	APPLY_BASH,
	APPLY_STUN,
	APPLY_PUNCH,
	APPLY_CLIMB,
	APPLY_GRIP,
	APPLY_SCRIBE,
	APPLY_BREW,
	APPLY_WEARSPELL,
	APPLY_REMOVESPELL,
	APPLY_EMOTION,
	APPLY_MENTALSTATE,
	APPLY_STRIPSN,
	APPLY_REMOVE,
	APPLY_DIG,
	APPLY_FULL,
	APPLY_THIRST,
	APPLY_DRUNK,
	APPLY_BLOOD,
	MAX_APPLY_TYPE
} apply_types;    //	/**/ = nieuzywane - mozna (czyt. trzeba) nadpisac

/* Directions */
typedef enum
{
	DIR_NORTH, DIR_EAST, DIR_SOUTH, DIR_WEST, DIR_UP, DIR_DOWN,
	DIR_NORTHEAST, DIR_NORTHWEST, DIR_SOUTHEAST, DIR_SOUTHWEST, DIR_SOMEWHERE
} dir_types;

/* Sector types */
typedef enum
{
	SECT_INSIDE, SECT_CITY, SECT_FIELD, SECT_FOREST, SECT_HILLS, SECT_MOUNTAIN,
	SECT_WATER_SWIM, SECT_WATER_NOSWIM, SECT_UNDERWATER, SECT_AIR, SECT_DESERT,
	SECT_DUNNO, SECT_OCEANFLOOR, SECT_UNDERGROUND,
	/*Thanos: wiêzienia*/
	SECT_JAIL_CORRIDOR, SECT_JAIL_OFFICE, SECT_JAIL_CELL, SECT_JAIL_ENTRANCE,
	SECT_MAX
} sector_types;

/* Equpiment wear locations */
typedef enum
{
	WEAR_NONE = -1, WEAR_LIGHT = 0, WEAR_FINGER_L, WEAR_FINGER_R, WEAR_NECK_1,
	WEAR_NECK_2, WEAR_BODY, WEAR_HEAD, WEAR_LEGS, WEAR_FEET, WEAR_HANDS,
	WEAR_ARMS, WEAR_SHIELD, WEAR_ABOUT, WEAR_WAIST, WEAR_WRIST_L, WEAR_WRIST_R,
	WEAR_WIELD, WEAR_HOLD, WEAR_DUAL_WIELD, WEAR_EARS, WEAR_EYES,
	WEAR_MISSILE_WIELD, MAX_WEAR
} wear_locations;

/* Board Types */
typedef enum
{
	BOARD_NOTE, BOARD_MAIL
} board_types;

/* Conditions */
typedef enum
{
	COND_DRUNK, COND_FULL, COND_THIRST, COND_BLOODTHIRST, MAX_CONDS
} conditions;

/* Positions */
typedef enum
{
	POS_DEAD, POS_MORTAL, POS_INCAP, POS_STUNNED, POS_SLEEPING, POS_RESTING,
	POS_SITTING, POS_FIGHTING, POS_STANDING, POS_MOUNTED, POS_SHOVE, POS_DRAG,
	POS_MAX
} positions;

typedef enum
{
	TIMER_NONE, TIMER_RECENTFIGHT, TIMER_SHOVEDRAG, TIMER_DO_FUN,
	TIMER_APPLIED, TIMER_PKILLED
} timer_types;

typedef	enum
{
	HAIR, BUILD, AGE, SKIN, BEARD, FACE, GENERAL, EYES, FUR, MAX_DESC_TYPES
} desc_type;

typedef enum
{
	Y_END, I_END
} desc_end;

/* Target types */
typedef enum
{
	TAR_IGNORE, TAR_CHAR_OFFENSIVE, TAR_CHAR_DEFENSIVE, TAR_CHAR_SELF,
	TAR_OBJ_INV
} target_types;

typedef enum
{
	SKILL_UNKNOWN, SKILL_SPELL, SKILL_SKILL, SKILL_WEAPON, SKILL_TONGUE,
	SKILL_HERB
} skill_types;

//jak rzadki i trudnodostepny jest ten materia³
typedef enum
{
	COMMON_MAT,		/*!< powszechny*/
	OCCASIONAL_MAT,	/*!< okazyjny*/
	RARE_MAT,		/*!< rzadki*/
	UNIQUE_MAT,		/*!< unikalny*/
	MAX_RARE_TYPE
}	material_rarity;

typedef enum
{
	U_ORE_FORM,	/*!< nierafinowana ruda	*/
	R_ORE_FORM, /*!< rafinowana ruda */
	ALLOY_FORM,	/*!< stop*/
	BAR_FORM,
	MAX_MATERIAL_FORM_TYPE
} material_type;

typedef enum
{
	ITEM_SOLD,	/*!< przedmiot licytacji zosta³ sprzedany */
	SHIP_SOLD,	/*!< statek zostal sprzedany */
	NO_BIDDERS,	/*!< brak chêtnych do zakupu */
	IMMO_IMPACT /*!< wtr±cili siê IMMOsi */
} auction_ends;

typedef enum
{
	AUCTION_ITEM,
	AUCTION_SHIP,
	AUCTION_HOUSE,
	AUCTION_MATERIAL,
	AUCTION_PRODUCT,
	AUCTION_TYPES_MAX
} auction_type;


typedef enum
{
	WAREHOUSE,
	BUILDING_COUNT
} planet_buildings;

typedef	enum
{
	FE_FIND_ROOM,
	FE_FIND_OBJECT,
	FE_FIND_MOB,
	FE_FIND_PLAYER,
	FE_FIND_SHIP,
	FE_KILL_MOB,
	FE_KILL_PLAYER,
	FE_KILL_SHIP,
	FE_GAIN_STAT,
	FE_START_QUEST,
	FE_FAIL_QUEST,
	FE_FINISH_QUEST,
	FE_ENTER_SHIP,
	FE_LAUNCH_SHIP,
	FE_LAND_SHIP,
	FE_LEVEL,
	FE_SKLVL,
	FE_POSITION,
	FE_MKILLS,
	FE_PKILLS,
	FE_SKILL,
	FE_TRIGGER_MAX
} fe_trigger;


/* PREPROCESSOR DEFINITIONS */

#define	INT64_1		((int64)1)

/* bitvector definitions */
#define BV00		(INT64_1 <<  0)
#define BV01		(INT64_1 <<  1)
#define BV02		(INT64_1 <<  2)
#define BV03		(INT64_1 <<  3)
#define BV04		(INT64_1 <<  4)
#define BV05		(INT64_1 <<  5)
#define BV06		(INT64_1 <<  6)
#define BV07		(INT64_1 <<  7)
#define BV08		(INT64_1 <<  8)
#define BV09		(INT64_1 <<  9)
#define BV10		(INT64_1 << 10)
#define BV11		(INT64_1 << 11)
#define BV12		(INT64_1 << 12)
#define BV13		(INT64_1 << 13)
#define BV14		(INT64_1 << 14)
#define BV15		(INT64_1 << 15)
#define BV16		(INT64_1 << 16)
#define BV17		(INT64_1 << 17)
#define BV18		(INT64_1 << 18)
#define BV19		(INT64_1 << 19)
#define BV20		(INT64_1 << 20)
#define BV21		(INT64_1 << 21)
#define BV22		(INT64_1 << 22)
#define BV23		(INT64_1 << 23)
#define BV24		(INT64_1 << 24)
#define BV25		(INT64_1 << 25)
#define BV26		(INT64_1 << 26)
#define BV27		(INT64_1 << 27)
#define BV28		(INT64_1 << 28)
#define BV29		(INT64_1 << 29)
#define BV30		(INT64_1 << 30)
#define BV31		(INT64_1 << 31)
#define BV32		(INT64_1 << 32)
#define BV33		(INT64_1 << 33)
#define BV34		(INT64_1 << 34)
#define BV35		(INT64_1 << 35)
#define BV36		(INT64_1 << 36)
#define BV37		(INT64_1 << 37)
#define BV38		(INT64_1 << 38)
#define BV39		(INT64_1 << 39)
#define BV40		(INT64_1 << 40)
#define BV41		(INT64_1 << 41)
#define BV42		(INT64_1 << 42)
#define BV43		(INT64_1 << 43)
#define BV44		(INT64_1 << 44)
#define BV45		(INT64_1 << 45)
#define BV46		(INT64_1 << 46)
#define BV47		(INT64_1 << 47)
#define BV48		(INT64_1 << 48)
#define BV49		(INT64_1 << 49)
#define BV50		(INT64_1 << 50)
#define BV51		(INT64_1 << 51)
#define BV52		(INT64_1 << 52)
#define BV53		(INT64_1 << 53)
#define BV54		(INT64_1 << 54)
#define BV55		(INT64_1 << 55)
#define BV56		(INT64_1 << 56)
#define BV57		(INT64_1 << 57)
#define BV58		(INT64_1 << 58)
#define BV59		(INT64_1 << 59)
#define BV60		(INT64_1 << 60)
#define BV61		(INT64_1 << 61)
#define BV62		(INT64_1 << 62)
#define BV63		(INT64_1 << 63)
/* max. 64 on IA32 */

/* Thanos - last chat itd. */
#define	INVIS_NONE			0
#define	INVIS_INVIS			BV00
#define	INVIS_DEFEL			BV01
#define	INVIS_HIDE			BV02
#define	INVIS_FORCE			BV03

/* Quest OLC -- Thanos */
#define	CMD_INIT			1
#define	CMD_EVENT			2
#define	CMD_ACTION			3
#define	CMD_NOTFOUND		-1

/* Ban Types by Thanos (musi byæ ujemny) */
#define	BAN_SOFT			-2

/* Echo types for echo_to_all */
#define	ECHOTAR_ALL			0
#define	ECHOTAR_PC			1
#define	ECHOTAR_IMM			2

/* defines for new do_who */
#define	WT_MORTAL			0
#define	WT_IMM				2
#define	WT_AVATAR			1
#define	WT_NEWBIE			3

/* Rodzaj interesu. (potrzebne do handlu z mobami) */
#define TRADE_NONE			0
#define BUY_AN_ITEM			1
#define BUY_A_PET			2
#define SELL_AN_ITEM		3

/* ability classes */
#define ABILITY_NONE		-1
#define COMBAT_ABILITY		0
#define PILOTING_ABILITY	1
#define ENGINEERING_ABILITY	2
#define HUNTING_ABILITY		3
#define SMUGGLING_ABILITY	4
#define DIPLOMACY_ABILITY	5
#define LEADERSHIP_ABILITY	6
#define FORCE_ABILITY		7

/* TO types for act. */
#define TO_ROOM				0
#define TO_NOTVICT			1
#define TO_VICT				2
#define TO_CHAR				3
#define TO_SHIP				4	/* Thanos */

/* CMD flags  -- Thanos */
#define CMD_HIDDEN			BV00   /* np. 'sla' 'qui' 	*/
#define CMD_OLC				BV01   /* komendy olca 		*/
#define CMD_ADMIN_ONLY		BV02   /* admintalk etc.	*/
#define CMD_NH				BV03   /* komendy NH		*/
#define CMD_NOORDER			BV04   /* nie mozna rozkazac uzycia tej komendy (order) */
#define	CMD_HELP			BV05   /* Trog: komendy helpmasterow */

/* Boot (reboot/shutdown/copyover) types -- Thanos */
#define REB_REBOOT			0
#define REB_COPYOVER		1
#define REB_SHUTDOWN		2

/* do f-cji shrtime -- Thanos */
#define	TIME_HMS			1
#define	TIME_DMY			2
#define	TIME_DMYHMS			3

/* Added by Thanos: Ratm do lini statusu */
#define VT_SAVECURSOR		"\e7"  	/* Save cursor and attrib */
#define VT_RESTORECURSOR	"\e8"  	/* Restore cursor pos and attribs */
#define VT_SETWIN_CLEAR		"\e[r" 	/* Clear scrollable window size */
#define VT_CLEAR_SCREEN		"\e[2J" /* Clear screen */
#define VT_CLEAR_LINE		"\e[2K" /* Clear this whole line */
#define VT_RESET_TERMINAL	"\ec"
#define STATUS_WIDTH		80	/* standardowa szeroko¶æ terminala */
#define STATUS_HEIGHT		23	/* linia w której jest status */
#define MAX_STATUS_WIDTH	160	/* maks szeroko¶æ i wysoko¶æ terminala */
#define MAX_STATUS_HEIGHT	60
#define STATUS_TYPE			1	/* domyslny typ statusu */
#define STATUS_TYPE_NONE	0
#define STATUS_TYPE_ONE		1
#define STATUS_TYPE_TWO		2
#define STATUS_TYPE_THREE	3
#define MAX_STATUS_TYPES	3

/*
 * Thanos: Kolorki z Ciapka -- zrobi³em jeszcze
 * NL i EOL
 * Lepiej siê ich u¿ywa przy send_to_char itd.
 */
#define __FGMOD				"&"		//"ß"
#define __BGMOD				"^"		//"Û"

#define MOD_BOLD			__FGMOD"W"	/*"[1m"*/
#define MOD_FAINT			__FGMOD"W"	/*"[2m"*/
#define MOD_UNDERLINE		__FGMOD"W"	/*"[4m"*/
#define MOD_REVERSE			__FGMOD"W"	/*"[7m"*/

#define FG_BLACK			__FGMOD"z"	/*"[30m"*//* Foreground Colours  	*/
#define FG_RED				__FGMOD"r"	/*"[31m"*/
#define FG_GREEN			__FGMOD"g"	/*"[32m"*/
#define FG_YELLOW			__FGMOD"O"	/*"[33m"*/
#define FG_BLUE				__FGMOD"b"	/*"[34m"*/
#define FG_MAGENTA			__FGMOD"p"	/*"[35m"*/
#define FG_PINK				__FGMOD"p"	/*"[35m"*/
#define FG_CYAN				__FGMOD"c"	/*"[36m"*/
//#define FG_WHITE			__FGMOD"x"	/*"[37m"*/
#define FG_WHITE			__FGMOD"w"	/*"[37m"*/
#define FG_GREY				__FGMOD"w"	/*"[37m"*/

#define FB_BLACK			__FGMOD"z"	/*"[1;30m"*//* Bold Foreground Colours */
#define FB_RED				__FGMOD"R"	/*"[1;31m"*/
#define FB_GREEN			__FGMOD"G"	/*"[1;32m"*/
#define FB_YELLOW			__FGMOD"Y"	/*"[1;33m"*/
#define FB_BLUE				__FGMOD"B"	/*"[1;34m"*/
#define FB_MAGENTA			__FGMOD"P"	/*"[1;35m"*/
#define FB_PINK				__FGMOD"P"	/*"[1;35m"*/
#define FB_CYAN				__FGMOD"C"	/*"[1;36m"*/
#define FB_WHITE			__FGMOD"W"	/*"[1;37m"*/
#define FB_FAINT			__FGMOD"W"	/*"[1;30m"*/

#define MOD_BLINK			__BGMOD"z"	/*"[5m"*/
#define MOD_NORMAL			__BGMOD"x"	/*"[5m"*/

#define BG_BLACK			__BGMOD"x"	/*"[40m"*//* Background Colours 	*/
#define BG_RED				__BGMOD"r"	/*"[41m"*/
#define BG_GREEN			__BGMOD"g"	/*"[42m"*/
#define BG_YELLOW			__BGMOD"O"	/*"[43m"*/
#define BG_BLUE				__BGMOD"b"	/*"[44m"*/
#define BG_MAGENTA			__BGMOD"p"	/*"[45m"*/
#define BG_CYAN				__BGMOD"c"	/*"[46m"*/
#define BG_WHITE			__BGMOD"w"	/*"[47m"*/
#define RESET				__BGMOD"x"

#define CLRSCR				"\033[2J"
#define NL					"\n\r"
#define PLAIN				FG_BLACK FG_GREY
#define EOL					PLAIN NL

/* A to juz mo¿na nawet modyfikowaæ :) */
#define COL_ACTION			PLAIN
#define COL_SAY				FG_CYAN
#define COL_YELL			FB_YELLOW
#define COL_SHOUT			COL_YELL
#define COL_TELL			FB_BLUE
#define COL_HIT				PLAIN
#define COL_HITME			PLAIN
#define COL_OOC				FB_PINK
#define COL_IMMORT			FB_WHITE
#define COL_ADMINTALK		FB_RED
#define COL_HURT			FB_RED
#define COL_DANGER			FB_RED
#define COL_FORCE			FB_BLUE
#define COL_POISON			FG_GREEN
#define COL_SOCIAL			FG_CYAN
#define COL_DYING			FB_YELLOW
#define COL_DEAD			FB_RED
#define COL_RMNAME			FB_BLUE
#define COL_OBJECT			FG_CYAN
#define COL_HUNGRY			FG_YELLOW
#define COL_THIRSTY			FB_BLUE
#define COL_WEAROFF			FB_RED
#define COL_SCORE			FG_CYAN
#define COL_DIEMSG			FB_WHITE
#define COL_INVITE			FB_GREEN
#define COL_BANK			FB_YELLOW

#define PROMPT_COL_N		FG_GREEN
#define PROMPT_COL_B		FB_GREEN

/* Thanos */
#define HELP_PHELP			1
#define HELP_OLCHELP		2
#define HELP_WIZHELP		3

/* shop types */
#define MAX_TRADE			5

/* Trog: flagi sklepow */
#define SHOP_RAWCOST		BV00

#define SHOP_FIX			1
#define SHOP_RECHARGE		2
#define MAX_FIX				3

/* Trog: race flags */
#define	FRACE_PROTOTYPE		BV00
#define	FRACE_PC_AVAIL		BV01
#define FRACE_MUTE			BV02 /*Ganis*/

/* Trog: some part of space :-) */
#define LASER_DAMAGED		-1
#define LASER_READY			0

#define TURRET_DESTROYED	-1
#define TURRET_READY		0
#define TURRET_DAMAGED		1

#define STARS_EVENT_ION		BV00
#define STARS_EVENT_METEOR	BV01
#define STARS_FLAG_TEMP		BV02
#define STARS_FLAG_EMP		BV03

/* planet flags */
#define PLANET_NOCAPTURE	BV00
#define PLANET_NOTSHOWN		BV01

/* statusy klanowe */
#define	CLAN_WAITING		-1
#define	CLAN_MEMBER			0
/* 0..9 - to beda RANKi ale 0..13 - tez maja swoje odpowiedniki */
#define	CLAN_DIPLOMAT		10
#define	CLAN_SECOND			11
#define	CLAN_FIRST			12
#define	CLAN_LEADER			13

/* ship flags */
#define SHIP_NOWANDER		BV00
#define SHIP_WIMPY			BV01
#define SHIP_AGGRESSIVE		BV02
#define SHIP_HIDDEN			BV03
#define SHIP_METAAGGRESSIVE	BV04
#define SHIP_DESCLESS		BV05
#define SHIP_DONE			BV06

/* turrets types -- Pixel */
#define LASER_TURRET 				1
#define DOUBLE_TURRET 				2
#define TRIPLE_TURRET				3
#define QUAD_TURRET					4
#define TURBOLASER_TURRET			5
#define HEAVY_TURBOLASER_TURRET 	6
#define SUPERLASER_TURRET			7

/* Temporary Ship Flags -- Thanos */
#define xSHIP_SAVED			BV00
#define xSHIP_RESET			BV01


/* act flags */
#define ACT_IS_NPC			BV00		/* Auto set for mobs	*/
#define ACT_SENTINEL		BV01		/* Stays in one room	*/
#define ACT_SCAVENGER		BV02		/* Picks up objects	*/
#define ACT_AGGRESSIVE		BV05		/* Attacks PC's		*/
#define ACT_STAY_AREA		BV06		/* Won't leave area	*/
#define ACT_WIMPY			BV07		/* Flees when hurt	*/
#define ACT_PET				BV08		/* Auto set for pets	*/
#define ACT_TRAIN			BV09		/* Can train PC's	*/
#define ACT_PRACTICE		BV10		/* Can practice PC's	*/
#define ACT_IMMORTAL		BV11		/* Cannot be killed	*/
#define ACT_DEADLY			BV12		/* Has a deadly poison  */
#define ACT_BOUNTYOFFICER	BV13		/* Trog: u takiego moba mozna skladac bounty */
#define ACT_META_AGGR		BV14		/* Extremely aggressive */
#define ACT_GUARDIAN		BV15		/* Protects master	*/
#define ACT_RUNNING			BV16		/* Hunts quickly	*/
#define ACT_NOWANDER		BV17		/* Doesn't wander	*/
#define ACT_MOUNTABLE		BV18		/* Can be mounted	*/
#define ACT_MOUNTED			BV19		/* Is mounted		*/
#define ACT_SCHOLAR			BV20        /* Can teach languages  */
#define ACT_SECRETIVE		BV21		/* actions aren't seen	*/
#define ACT_POLYMORPHED		BV22		/* Mob is a ch		*/
#define ACT_MOBINVIS		BV23		/* Like wizinvis	*/
#define ACT_NOASSIST		BV24		/* Doesn't assist mobs	*/
#define ACT_NOKILL			BV25        /* Mob can't die 	*/
#define ACT_DROID			BV26        /* mob is a droid 	*/
#define ACT_NOCORPSE		BV27
#define ACT_QUEST			BV28  		/*za³adowany przez quest*/
#define ACT_PROTOTYPE		BV30		/* A prototype mob	*/

/* Body parts -- Thanos (z Envy) */
#define PART_NONE			0
#define PART_HEAD			BV00		/* glowa	*/
#define PART_ARMS			BV01		/* ramiona	*/
#define PART_LEGS			BV02		/* nogi		*/
#define PART_HEART			BV03		/* serce	*/
#define PART_BRAINS			BV04		/* mozgowin	*/
#define PART_GUTS			BV05		/* flaki??	*/
#define PART_HANDS			BV06		/* rece		*/
#define PART_FEET			BV07		/* stopy	*/
#define PART_FINGERS		BV08		/* palce	*/
#define PART_EAR			BV09		/* uszy		*/
#define PART_EYE			BV10		/* oko		*/
#define PART_LONG_TONGUE	BV11		/* jezoor	*/
#define PART_EYESTALKS		BV12		/* szypulki na oczy, np. rasa gran ma takie*/
#define PART_TENTACLES		BV13		/* macki  */
#define PART_FINS			BV14		/* pletwy	*/
#define PART_WINGS			BV15		/* skrzydla	*/
#define PART_TAIL			BV16		/* ogon		*/
#define PART_LEKU			BV17		/* Dla Twi'leków */

/* body parts used in combat */
#define PART_CLAWS			BV27		/* pazury	*/
#define PART_FANGS			BV28		/* nie_wiem_co	*/
#define PART_HORNS			BV29		/* rogi		*/
#define PART_SCALES			BV30		/* luski	*/
#define PART_TUSKS			BV31		/* kly		*/

/* affected flags */
#define AFF_NONE			0
#define AFF_BLIND			BV00
#define AFF_INVISIBLE		BV01
#define AFF_DETECT_EVIL		BV02
#define AFF_DETECT_INVIS	BV03
#define AFF_DETECT_FORCE	BV04
#define AFF_DETECT_HIDDEN	BV05
#define AFF_WEAKEN			BV06
#define AFF_SANCTUARY		BV07
#define AFF_FAERIE_FIRE		BV08
#define AFF_INFRARED		BV09
#define AFF_CURSE			BV10
#define AFF_DOMINATED		BV11		/* byTrog	*/
#define AFF_POISON			BV12
#define AFF_PROTECT			BV13
#define AFF_PARALYSIS		BV14
#define AFF_SNEAK			BV15
#define AFF_HIDE			BV16
#define AFF_SLEEP			BV17
#define AFF_CHARM			BV18
#define AFF_FLYING			BV19
#define AFF_PASS_DOOR		BV20
#define AFF_FLOATING		BV21
#define AFF_TRUESIGHT		BV22
#define AFF_DETECTTRAPS		BV23
#define AFF_SCRYING			BV24
#define AFF_FORCEINVIS		BV25	/* byTrog	*/
#define AFF_SHOCKSHIELD		BV26
#define AFF_DISGUISE		BV27        /* Thanos */
#define AFF_ICESHIELD		BV28
#define AFF_POSSESS			BV29
#define AFF_BERSERK			BV30
#define AFF_AQUA_BREATH		BV31

/* resist flags */
#define RIS_FIRE			BV00
#define RIS_COLD			BV01
#define RIS_ELECTRICITY		BV02
#define RIS_ENERGY			BV03
#define RIS_BLUNT			BV04
#define RIS_PIERCE			BV05
#define RIS_SLASH			BV06
#define RIS_ACID			BV07
#define RIS_POISON			BV08
#define RIS_DRAIN			BV09
#define RIS_SLEEP			BV10
#define RIS_CHARM			BV11
#define RIS_HOLD			BV12
#define RIS_NONFORCE		BV13
#define RIS_PLUS1			BV14
#define RIS_PLUS2			BV15
#define RIS_PLUS3			BV16
#define RIS_PLUS4			BV17
#define RIS_PLUS5			BV18
#define RIS_PLUS6			BV19
#define RIS_FORCE			BV20
#define RIS_PARALYSIS		BV21

/* attack types */
#define ATCK_BITE			BV00
#define ATCK_CLAWS			BV01
#define ATCK_TAIL			BV02
#define ATCK_STING			BV03
#define ATCK_PUNCH			BV04
#define ATCK_KICK			BV05
#define ATCK_TRIP			BV06
#define ATCK_BASH			BV07
#define ATCK_STUN			BV08
#define ATCK_GOUGE			BV09
//Trog: nie ma nazw do tego ponizej.!!! (we flags.c)
#define ATCK_BACKSTAB		BV10
#define ATCK_FEED			BV11
#define ATCK_DRAIN			BV12
#define ATCK_FIREBREATH		BV13
#define ATCK_FROSTBREATH	BV14
#define ATCK_ACIDBREATH		BV15
#define ATCK_LIGHTNBREATH	BV16
#define ATCK_GASBREATH		BV17
#define ATCK_POISON			BV18
#define ATCK_NASTYPOISON	BV19
#define ATCK_GAZE			BV20
#define ATCK_BLINDNESS		BV21
#define ATCK_CAUSESERIOUS	BV22
#define ATCK_EARTHQUAKE		BV23
#define ATCK_CAUSECRITICAL	BV24
#define ATCK_CURSE			BV25
#define ATCK_FLAMESTRIKE	BV26
#define ATCK_HARM			BV27
#define ATCK_FIREBALL		BV28
#define ATCK_COLORSPRAY		BV29
#define ATCK_WEAKEN			BV30
#define ATCK_SPIRALBLAST	BV31

/* defense types */
#define DFND_PARRY			BV00
#define DFND_DODGE			BV01
//Trog: to samo, co przy atakach.
#define DFND_HEAL			BV02
#define DFND_CURELIGHT		BV03
#define DFND_CURESERIOUS	BV04
#define DFND_CURECRITICAL	BV05
#define DFND_DISPELFORCE	BV06
#define DFND_DISPELEVIL		BV07
#define DFND_SANCTUARY		BV08
#define DFND_FIRESHIELD		BV09
#define DFND_SHOCKSHIELD	BV10
#define DFND_SHIELD			BV11
#define DFND_BLESS			BV12
#define DFND_STONESKIN		BV13
#define DFND_TELEPORT		BV14
#define DFND_MONSUM1		BV15
#define DFND_MONSUM2		BV16
#define DFND_MONSUM3		BV17
#define DFND_MONSUM4		BV18
#define DFND_DISARM			BV19
#define DFND_ICESHIELD		BV20
#define DFND_GRIP			BV21

/* autosave flags */
#define SV_DEATH			BV00
#define SV_KILL				BV01
#define SV_PASSCHG			BV02
#define SV_DROP				BV03
#define SV_PUT				BV04
#define SV_GIVE				BV05
#define SV_AUTO				BV06
#define SV_ZAPDROP			BV07
#define SV_AUCTION			BV08
#define SV_GET				BV09
#define SV_RECEIVE			BV10
#define SV_IDLE				BV11
#define SV_BACKUP			BV12

/* pipe flags  */
#define PIPE_TAMPED			BV01
#define PIPE_LIT			BV02
#define PIPE_HOT			BV03
#define PIPE_DIRTY			BV04
#define PIPE_FILTHY			BV05
#define PIPE_GOINGOUT		BV06
#define PIPE_BURNT			BV07
#define PIPE_FULLOFASH		BV08

/* skill/spell flags	The minimum BV *MUST* be 11! */
/* Trog: nazwy smierdza fantasy... */
#define SF_WATER			BV11
#define SF_EARTH			BV12
#define SF_AIR				BV13
#define SF_ASTRAL			BV14
#define SF_AREA				BV15  /* is an area spell		*/
#define SF_DISTANT			BV16  /* affects something far away	*/
#define SF_REVERSE			BV17
#define SF_SAVE_HALF_DAMAGE	BV18  /* save for half damage		*/
#define SF_SAVE_NEGATES		BV19  /* save negates affect		*/
#define SF_ACCUMULATIVE		BV20  /* is accumulative		*/
#define SF_RECASTABLE		BV21  /* can be refreshed		*/
#define SF_NOSCRIBE			BV22  /* cannot be scribed		*/
#define SF_NOBREW			BV23  /* cannot be brewed		*/
#define SF_GROUPSPELL		BV24  /* only affects group members	*/
#define SF_OBJECT			BV25	/* directed at an object	*/
#define SF_CHARACTER		BV26  /* directed at a character	*/
#define SF_SECRETSKILL		BV27	/* hidden unless learned	*/
#define SF_PKSENSITIVE		BV28	/* much harder for plr vs. plr	*/
#define SF_STOPONFAIL		BV29	/* stops spell on first failure */

#define ALL_BITS			BV63
#define SDAM_MASK			ALL_BITS & ~(BV00 | BV01 | BV02)
#define SACT_MASK			ALL_BITS & ~(BV03 | BV04 | BV05)
#define SCLA_MASK			ALL_BITS & ~(BV06 | BV07 | BV08)
#define SPOW_MASK			ALL_BITS & ~(BV09 | BV10)

/* P³eæ przedmiotu by Thanos */
#define GENDER_NEUTRAL		0
#define GENDER_MALE			1
#define GENDER_FEMALE		2
/*dla liczby mnogiej*/
#define GENDER_PLURAL		3

#define MAX_TRAPTYPE		TRAP_TYPE_SEX_CHANGE
#define TRAP_ROOM			BV00
#define TRAP_OBJ			BV01
#define TRAP_ENTER_ROOM		BV02
#define TRAP_LEAVE_ROOM		BV03
#define TRAP_OPEN			BV04
#define TRAP_CLOSE			BV05
#define TRAP_GET			BV06
#define TRAP_PUT			BV07
#define TRAP_PICK			BV08
#define TRAP_UNLOCK			BV09
#define TRAP_N				BV10
#define TRAP_S				BV11
#define TRAP_E				BV12
#define TRAP_W				BV13
#define TRAP_U				BV14
#define TRAP_D				BV15
#define TRAP_EXAMINE		BV16
#define TRAP_NE				BV17
#define TRAP_NW				BV18
#define TRAP_SE				BV19
#define TRAP_SW				BV20

// Thanos
#define	GRANADE_EXPLOSIVE	0
#define	GRANADE_POISON		1
#define	GRANADE_STUN		2
#define	GRANADE_BLIND		3

#define MAX_ITEM_TYPE		ITEM_PIECE

/*
 * Extra flags.
 * Used in #OBJECTS.
 */
#define ITEM_GLOW			BV00
#define ITEM_HUM			BV01
#define ITEM_QUEST			BV02 /*Thanos - dark by³o nieu¿ywane, da³em quest*/
#define ITEM_HUTT_SIZE		BV03
#define ITEM_CONTRABAND		BV04
#define ITEM_INVIS			BV05
#define ITEM_FORCE			BV06
#define ITEM_NODROP			BV07
#define ITEM_BLESS			BV08
#define ITEM_ANTI_GOOD		BV09
#define ITEM_ANTI_EVIL		BV10
#define ITEM_ANTI_NEUTRAL	BV11
#define ITEM_NOREMOVE		BV12
#define ITEM_INVENTORY		BV13
#define ITEM_ANTI_SOLDIER	BV14
#define ITEM_ANTI_THIEF		BV15
#define ITEM_ANTI_HUNTER	BV16
#define ITEM_ANTI_JEDI		BV17
#define ITEM_SMALL_SIZE		BV18
#define ITEM_LARGE_SIZE		BV19
#define ITEM_DONATION		BV20
#define ITEM_CLANOBJECT		BV21
#define ITEM_ANTI_CITIZEN	BV22
#define ITEM_ANTI_SITH		BV23
#define ITEM_ANTI_PILOT		BV24
#define ITEM_HIDDEN			BV25
#define ITEM_POISONED		BV26
#define ITEM_COVERING		BV27
#define ITEM_PERSONAL		BV28	/* Thanos    4    Alde */
#define ITEM_BURRIED		BV29	/* item is underground */
#define ITEM_PROTOTYPE		BV30
#define ITEM_HUMAN_SIZE		BV31

/* force flags - extra extra_flags for objects that are used in spells */
#define ITEM_RETURNING		BV00
#define ITEM_BACKSTABBER	BV01
#define ITEM_BANE			BV02
#define ITEM_LOYAL			BV03
#define ITEM_HASTE			BV04
#define ITEM_DRAIN			BV05
#define ITEM_LIGHTNING_BLADE	BV06

/* Blaster settings - only saves on characters */
#define BLASTER_NORMAL		0
#define BLASTER_HALF		2
#define BLASTER_FULL		5
#define BLASTER_LOW			1
#define	BLASTER_STUN		3
#define BLASTER_HIGH		4

/* Weapon Types */
#define WEAPON_NONE			0
#define WEAPON_VIBRO_AXE	1
#define WEAPON_VIBRO_BLADE	2
#define WEAPON_LIGHTSABER	3
#define WEAPON_WHIP			4
#define WEAPON_CLAW			5
#define WEAPON_BLASTER		6
#define WEAPON_BLUDGEON		8
#define WEAPON_BOWCASTER	9
#define WEAPON_FORCE_PIKE	11

/* cargo types */
#define CARGO_NONE			0
#define CARGO_ALL			1
#define CARGO_LOMMITE		2
#define CARGO_MELEEN		3
#define CARGO_NEUTRON		4
#define CARGO_ZERSIUM		5
#define CARGO_STEEL			3
#define CARGO_RYLL			7
#define CARGO_ALAZHI		8
#define CARGO_MAX			9

/* astro types by Alde */
#define ASTRO_WRECK			0
#define ASTRO_ASTEROID		1
#define ASTRO_CARGO			2
#define ASTRO_MINE			3
#define ASTRO_LIFE_PAD		4
#define ASTRO_UNKNOWN		5
#define ASTRO_MAX			6

/* Lever/dial/switch/button/pullchain flags */
#define TRIG_UP				BV00
#define TRIG_UNLOCK			BV01
#define TRIG_LOCK			BV02
#define TRIG_D_NORTH		BV03
#define TRIG_D_SOUTH		BV04
#define TRIG_D_EAST			BV05
#define TRIG_D_WEST			BV06
#define TRIG_D_UP			BV07
#define TRIG_D_DOWN			BV08
#define TRIG_DOOR			BV09
#define TRIG_CONTAINER		BV10
#define TRIG_OPEN			BV11
#define TRIG_CLOSE			BV12
#define TRIG_PASSAGE		BV13
#define TRIG_OLOAD			BV14
#define TRIG_MLOAD			BV15
#define TRIG_TELEPORT		BV16
#define TRIG_TELEPORTALL	BV17
#define TRIG_TELEPORTPLUS	BV18
#define TRIG_DEATH			BV19
#define TRIG_CAST			BV20
#define TRIG_FAKEBLADE		BV21
#define TRIG_RAND4			BV22
#define TRIG_RAND6			BV23
#define TRIG_TRAPDOOR		BV24
#define TRIG_ANOTHEROOM		BV25
#define TRIG_USEDIAL		BV26
#define TRIG_ABSOLUTEVNUM	BV27
#define TRIG_SHOWROOMDESC	BV28
#define TRIG_AUTORETURN		BV29

#define TELE_SHOWDESC		BV00
#define TELE_TRANSALL		BV01
#define TELE_TRANSALLPLUS	BV02

/* drug types */
#define SPICE_GLITTERSTIM	0
#define SPICE_CARSANUM		1
#define SPICE_RYLL			2
#define SPICE_ANDRIS		3

/* crystal types */
#define GEM_NON_ADEGEN		0
#define GEM_KATHRACITE		1
#define GEM_RELACITE		2
#define GEM_DANITE			3
#define GEM_MEPHITE			4
#define GEM_PONITE			5
#define GEM_ILLUM			6
#define GEM_CORUSCA			7

/*
 * Wear flags.
 * Used in #OBJECTS.
 */
#define ITEM_TAKE			BV00
#define ITEM_WEAR_FINGER	BV01
#define ITEM_WEAR_NECK		BV02
#define ITEM_WEAR_BODY		BV03
#define ITEM_WEAR_HEAD		BV04
#define ITEM_WEAR_LEGS		BV05
#define ITEM_WEAR_FEET		BV06
#define ITEM_WEAR_HANDS		BV07
#define ITEM_WEAR_ARMS		BV08
#define ITEM_WEAR_SHIELD	BV09
#define ITEM_WEAR_ABOUT		BV10
#define ITEM_WEAR_WAIST		BV11
#define ITEM_WEAR_WRIST		BV12
#define ITEM_WIELD			BV13
#define ITEM_HOLD			BV14
#define ITEM_DUAL_WIELD		BV15
#define ITEM_WEAR_EARS		BV16
#define ITEM_WEAR_EYES		BV17
#define ITEM_MISSILE_WIELD	BV18

#define REVERSE_APPLY		1000

#define	REQ_NONE			0
#define	REQ_STR				1
#define	REQ_DEX				2
#define	REQ_INT				3
#define	REQ_WIS				4
#define	REQ_CON				5
#define	REQ_CHA				6
#define	REQ_LCK				7
#define	REQ_FRC				8
#define	REQ_SEX				9
#define	REQ_GOLD			10
#define	REQ_AGE				11
#define	REQ_HIT				12
#define	REQ_FORCE			13
#define	REQ_MOVE			14
#define	REQ_LEVEL			15
#define	REQ_AC				16
#define	REQ_HITROLL			17
#define	REQ_DAMROLL			18
#define	REQ_FULL			19
#define	REQ_THIRST			20
#define	REQ_DRUNK			21
#define	REQ_RACE			22
#define	REQ_SKILL			23
#define	REQ_AFFECT			24
#define	REQ_CLASS			25
#define	MAX_REQ				26

/*
 * Values for containers (value[1]).
 * Used in #OBJECTS.
 */
#define CONT_CLOSEABLE		BV00
#define CONT_PICKPROOF		BV01
#define CONT_CLOSED			BV02
#define CONT_LOCKED			BV03

/* Thanos: test */
#define PART_FINISHED		FALSE
#define PART_UNFINISHED		TRUE

#define DIAL_UP				BV00
#define DIAL_UNLOCK			BV01
#define DIAL_LOCK			BV02
#define DIAL_D_NORTH		BV03
#define DIAL_D_SOUTH		BV04
#define DIAL_D_EAST			BV05
#define DIAL_D_WEST			BV06
#define DIAL_D_UP			BV07
#define DIAL_D_DOWN			BV08
#define DIAL_DOOR			BV09
#define DIAL_CONTAINER		BV10
#define DIAL_OPEN			BV11
#define DIAL_CLOSE			BV12
#define DIAL_PASSAGE		BV13
#define DIAL_OLOAD			BV14
#define DIAL_MLOAD			BV15
#define DIAL_TELEPORT		BV16
#define DIAL_TELEPORTALL	BV17
#define DIAL_TELEPORTPLUS	BV18
#define DIAL_DEATH			BV19
#define DIAL_CAST			BV20
#define DIAL_FAKEBLADE		BV21
#define DIAL_RAND4			BV22
#define DIAL_RAND6			BV23
#define DIAL_TRAPDOOR		BV24

/* Quest flags by Trog */
#define QUEST_PROTOTYPE		BV00
#define QUEST_HIDDEN		BV01	/* Questu nie widaæ na quest list */
#define QUEST_NF_COMBAT		BV02	/* Ograniczenia dla klas */
#define QUEST_NF_ENGINEER	BV03
#define QUEST_NF_PILOT		BV04
#define QUEST_NF_BOUNTY		BV05
#define QUEST_NF_LEADER		BV06
#define QUEST_NF_DIPLOMAT	BV07
#define QUEST_NF_SMUGGLER	BV08
#define QUEST_NF_FORCEUSER	BV09
#define QUEST_NORESIGN		BV10	/* z questu nie mo¿na zrezygnowaæ */
#define QUEST_REPEATABLE	BV11	/* quest mo¿na powtarzaæ */

/* Thanos: typy komend questowych */
#define INIT_LOAD_MOB		1
#define INIT_LOAD_OBJ		2
#define INIT_PUT_OBJ		3
#define INIT_EQUIP_MOB		4
#define INIT_PURGE			5
#define INIT_TRANSFER		6
#define INIT_ECHO			7
#define INIT_HINT			8
#define INIT_INVITE_MOB		9
#define	INIT_GIVE_OBJ		10

#define EVENT_KILL_MOB		1
#define EVENT_FIND_OBJ		2
#define EVENT_BRING_OBJ		3
#define EVENT_GIVE_OBJ		4
#define EVENT_VISIT_MOB		5
#define EVENT_VISIT_ROOM	6
#define EVENT_LOOK_OBJ		7
#define EVENT_LOOK_MOB		8

#define ACTION_STOPQUEST	0
#define ACTION_PROG			1
#define ACTION_ECHO			2
#define ACTION_TELL			3
#define ACTION_NONE			4
#define ACTION_RUNCHPT		5
#define ACTION_REWARD		6

/* Thanos: typy krytyczno¶ci rozdzia³ów */
#define CHAPTER_UNCRITICAL		0
#define CHAPTER_CRITICAL		1
#define CHAPTER_VERY_CRITICAL	2
#define CHAPTER_VERY_UNCRITICAL	3

/* Clan flags by Trog */
#define CLAN_NOQUIT			BV00
#define CLAN_PENALTYQUIT	BV01
#define CLAN_NOINFO			BV02

/* Politics types by Trog */
#define POLITICS_ENEMY		-1
#define POLITICS_NEUTRAL	0
#define POLITICS_ALLY		1
#define POLITICS_MAX		2

/*
 * Room flags.           Holy cow!  Talked about stripped away..
 * Used in #ROOMS.       Those merc guys know how to strip code down.
 *			 Lets put it all back... ;)
 */
#define ROOM_DARK			BV00
#define ROOM_NO_FIND		BV01	/* Trog */
#define ROOM_NO_MOB			BV02
#define ROOM_INDOORS		BV03
#define ROOM_CAN_LAND		BV04
#define ROOM_CAN_FLY		BV05
#define ROOM_NO_DRIVING		BV06
#define ROOM_NO_FORCE		BV07
#define ROOM_BANK			BV08
#define ROOM_PRIVATE		BV09
#define ROOM_SAFE			BV10
#define ROOM_IMPORT			BV11
#define ROOM_STORAGE		BV12
#define ROOM_BACTA			BV13	/* Thanos */
#define ROOM_DONATION		BV14
#define ROOM_NODROPALL		BV15
#define ROOM_SILENCE		BV16
#define ROOM_LOGSPEECH		BV17
#define ROOM_NODROP			BV18
#define ROOM_STOCK_MARKET	BV19	/* Tanglor - gielda*/
#define ROOM_PLR_HOME		BV20
#define ROOM_EMPTY_HOME		BV21
#define ROOM_TELEPORT		BV22
#define ROOM_HOTEL			BV23
#define ROOM_NOFLOOR		BV24
#define ROOM_REFINERY		BV25
#define ROOM_FACTORY		BV26
#define ROOM_KILLARENA		BV27
#define ROOM_SAFEARENA		BV28
#define ROOM_SPACECRAFT		BV29
#define ROOM_PROTOTYPE		BV30
#define ROOM_AUCTION		BV31

#define MAX_DIR				DIR_SOUTHWEST	/* max for normal walking */
#define DIR_PORTAL			DIR_SOMEWHERE	/* portal direction	  */

/* Exit flags */
#define EX_ISDOOR			BV00
#define EX_CLOSED			BV01
#define EX_LOCKED			BV02
#define EX_SECRET			BV03
#define EX_SWIM				BV04
#define EX_PICKPROOF		BV05
#define EX_FLY				BV06
#define EX_CLIMB			BV07
#define EX_DIG				BV08
#define EX_RES1				BV09	/* are these res[1-4] important? */
#define EX_NOPASSDOOR		BV10
#define EX_HIDDEN			BV11
#define EX_PASSAGE			BV12
#define EX_PORTAL			BV13
#define EX_RES2				BV14
#define EX_RES3				BV15
#define EX_xCLIMB			BV16
#define EX_xENTER			BV17
#define EX_xLEAVE			BV18
#define EX_xAUTO			BV19
#define EX_RES4				BV20
#define EX_xSEARCHABLE		BV21
#define EX_BASHED			BV22
#define EX_BASHPROOF		BV23
#define EX_NOMOB			BV24
#define EX_WINDOW			BV25
#define EX_xLOOK			BV26
#define MAX_EXFLAG			26

/* Auth Flags */
#define FLAG_WRAUTH			1
#define FLAG_AUTH			2

#define NO_FLAG				-1 /*Thanos*/

/* ACT bits for players */
#define PLR_IS_NPC			BV00	  /* Don't EVER set! */
#define PLR_BOUGHT_PET		BV01
/*#define PLR_SHOVEDRAG		   BV02  NIEUZYWANE 		*/
#define PLR_MAPEXITS		BV02
#define PLR_AUTOEXIT		BV03
#define PLR_AUTOLOOT		BV04
#define PLR_KNOWN4ALL		BV05
#define PLR_BLANK			BV06
#define PLR_OUTCAST			BV07
#define PLR_BRIEF			BV08
#define PLR_COMBINE			BV09
#define PLR_PROMPT			BV10
#define PLR_TELNET_GA		BV11
#define PLR_HOLYLIGHT		BV12
#define PLR_WIZINVIS		BV13
#define PLR_ROOMVNUM		BV14
/*#define	PLR_SILENCE		   BV15  Trog: NIEUZYWANE */
#define PLR_TICK			BV16
#define PLR_ATTACKER		BV17
#define PLR_NOTE_WRITE		BV18     /* Thanos */
#define PLR_LOG				BV19
#define PLR_DENY			BV20
#define PLR_FREEZE			BV21
#define PLR_KILLER			BV22
#define PLR_STATUS_BAR		BV23	    /* added by Thanos */
#define PLR_LITTERBUG		BV24
#define PLR_ANSI			BV25
#define PLR_SOUND			BV26
#define PLR_NICE			BV27
#define PLR_FLEE			BV28
#define PLR_AUTOGOLD		BV29
#define PLR_LONGEXITS		BV30	   /* Thanos 4 Alde */
#define PLR_AFK				BV31

/* Bits for pc_data->flags. */
#define PCFLAG_R1			BV00
#define PCFLAG_NH			BV01     /* Trog NewbieHelper */
#define PCFLAG_UNAUTHED		BV02
/*#define PCFLAG_NORECALL         BV03  NIEUZYWANE */
#define PCFLAG_NOINTRO		BV04
#define PCFLAG_GAG			BV05
#define PCFLAG_RETIRED		BV06
#define PCFLAG_GUEST		BV07
/*#define PCFLAG_NOSUMMON	  BV08  NIEUZYWANE */
#define PCFLAG_PAGERON		BV09
#define PCFLAG_2			BV10
#define PCFLAG_ROOM			BV11
#define PCFLAG_SHIPSTAT		BV12 /* added by Aldegard */

//added by Thanos (polfonty Ratma)
#define CHARSET_NOPOL		01
#define CHARSET_WIN			02
#define CHARSET_ISO			03
#define INNE_LITERY_WIN		6 	/* Ilo¶æ literek którymi ró¿ni siê ISO i CP */
#define INNE_LITERY_NOPL	18	/* 	- | | -		ISO i polskawy*/

/* typy monta¿u */
#define MONT_SCREW			'A'	/*wkrêcamy*/
#define MONT_SOLDER			'B'	/*lutujemy*/
#define MONT_WELD			'C'	/*spawamy*/
#define MONT_STICK			'D'	/*sklejamy*/
#define MONT_PLUG			'E'	/*wtykamy*/
#define MONT_RIVET			'F'	/*nitujemy*/
#define MONT_SEW			'G'	/*szyjemy*/
#define MONT_COOK			'H'	/*gotujemy*/
#define MONT_TOOL			'I'	/*obrabiamy*/

/* rodzaje komponentów */
#define COMP_VNUM			'v'	/* potrzebny przedmiot o danym vnumie */
#define COMP_WEAR			'w'	/* potrzebny przedmiot o danej wear_fladze */
#define COMP_TYPE			't'	/* potrzebny przedmiot danego typu */

/* Channel bits */
#define	CHANNEL_AUCTION		BV00
#define	CHANNEL_CHAT		BV01
#define	CHANNEL_QUEST		BV02
#define	CHANNEL_IMMTALK		BV03
#define	CHANNEL_MUSIC		BV04
#define	CHANNEL_ASK			BV05
#define	CHANNEL_SHOUT		BV06
#define	CHANNEL_YELL		BV07
#define CHANNEL_MONITOR		BV08
#define CHANNEL_LOG			BV09
#define CHANNEL_PROGBUG		BV10 /* Thanos */
#define CHANNEL_CLAN		BV11
#define CHANNEL_BUILD		BV12
#define CHANNEL_CODERTALK	BV13 /*  Trog  */
#define CHANNEL_PROG		BV14 /*  Trog  */
#define CHANNEL_PRAY		BV15
#define CHANNEL_COUNCIL		BV16
#define CHANNEL_GUILD		BV17
#define CHANNEL_COMM		BV18
#define CHANNEL_TELLS		BV19
#define CHANNEL_ORDER		BV20
#define CHANNEL_NEWBIE		BV21
#define CHANNEL_WARTALK		BV22
#define CHANNEL_OOC			BV23
#define CHANNEL_SHIP		BV24
#define CHANNEL_SYSTEM		BV25
#define CHANNEL_SPACE		BV26
#define CHANNEL_103			BV27 /* Trog: to tez mozna wywalic. */
#define CHANNEL_ADMINTALK	BV28
#define CHANNEL_QUESTCHAT	BV29	/*Thanos*/
#define	CHANNEL_FLAME		BV30 /*Thanos*/
#define CHANNEL_OLCTALK		BV31 /* Trog */

#define CHANNEL_CLANTALK	CHANNEL_CLAN

/* Area defines - Scryn 8/11 */
#define AREA_DELETED		BV00
#define AREA_LOADED			BV01
/* Area flags - Narn Mar/96 */
#define AFLAG_NOPKILL		BV00
#define AFLAG_NIGHTDESCS	BV01 /* Trog */
#define AFLAG_NOHAIL		BV02 /* Thanos */
#define AFLAG_HIDDEN		BV03 /* Thanos */
#define AFLAG_DONTSHOWPLANET	BV04 /* Thanos */
#define	AFLAG_PROTOTYPE		BV05
#define	AFLAG_FOR_SHIPS		BV06 /* Trog */

/* typy przestêpstw */
#define	CRIME_ANY			-1
#define	CRIME_SWINDLE		BV00
#define	CRIME_HACKING		BV01
#define	CRIME_CONTRABAND	BV02
#define	CRIME_STEAL			BV03
#define	CRIME_MURDER		BV04

#define	SUS_NEW				0
#define	SUS_FINED			1
#define	SUS_CRITICAL		2

/* ograniczenia dla poszczegolnych ras */
#ifdef FAMILIARITY
#define RESTRICT_HAIR		BV00
#define RESTRICT_BUILD		BV01
#define RESTRICT_AGE		BV02
#define RESTRICT_SKIN		BV03
#define RESTRICT_BEARD		BV04
#define RESTRICT_FACE		BV05
#define RESTRICT_GENERAL	BV06
#define RESTRICT_EYES		BV07
#define RESTRICT_FUR		BV08
#endif

/* Liquids */
#define LIQ_WATER			0
#define LIQ_MAX				20

/* Constants for arg2 of 'B' resets. */
#define	BIT_RESET_DOOR		0
#define BIT_RESET_OBJECT	1
#define BIT_RESET_MOBILE	2
#define BIT_RESET_ROOM		3
#define BIT_RESET_TYPE_MASK	0xFF	/* 256 should be enough */
#define BIT_RESET_DOOR_THRESHOLD	8
#define BIT_RESET_DOOR_MASK	0xFF00	/* 256 should be enough */
#define BIT_RESET_SET		BV30
#define BIT_RESET_TOGGLE	BV31
#define BIT_RESET_FREEBITS	0x3FFF0000	/* For reference */

#define DOWN_REBOOT			0
#define DOWN_SHUTDOWN		1
#define DOWN_COPYOVER		2
#define DOWN_KILLED			3
#define DOWN_CRASHED		4

/*
 * Types of skill numbers.  Used to keep separate lists of sn's
 * Must be non-overlapping with spell/skill types,
 * but may be arbitrary beyond that.
 */
#define TYPE_UNDEFINED		-1
#define TYPE_HIT			1000  /* allows for 1000 skills/spells */
#define TYPE_HERB			2000  /* allows for 1000 attack types  */
#define TYPE_PERSONAL		3000  /* allows for 1000 herb types    */

/* object saving defines for fread/write_obj. -- Altrag */
#define OS_CARRY			0
#define OS_CORPSE			1

/* mud prog defines */
#define ERROR_PROG			-1
#define IN_FILE_PROG		0
#define ACT_PROG			BV00
#define SPEECH_PROG			BV01
#define RAND_PROG			BV02
#define FIGHT_PROG			BV03
#define RFIGHT_PROG			BV03
#define DEATH_PROG			BV04
#define RDEATH_PROG			BV04
#define HITPRCNT_PROG		BV05
#define ENTRY_PROG			BV06
#define ENTER_PROG			BV06
#define GREET_PROG			BV07
#define RGREET_PROG			BV07
#define OGREET_PROG			BV07
#define ALL_GREET_PROG		BV08
#define GIVE_PROG			BV09
#define BRIBE_PROG			BV10
#define HOUR_PROG			BV11
#define TIME_PROG			BV12
#define WEAR_PROG			BV13
#define REMOVE_PROG			BV14
#define	SAC_PROG			BV15
#define TELL_PROG			BV16 /* Thanos */
#define EXA_PROG			BV17
#define ZAP_PROG			BV18
#define GET_PROG			BV19
#define DROP_PROG			BV20
#define DAMAGE_PROG			BV21
#define REPAIR_PROG			BV22
#define QUEST_PROG			BV23  /* Thanos */
#define SPEECHIW_PROG		BV24  /**/
#define PULL_PROG			BV25
#define PUSH_PROG			BV26
#define SLEEP_PROG			BV27
#define REST_PROG			BV28
#define LEAVE_PROG			BV29
#define SCRIPT_PROG			BV30
#define USE_PROG			BV31
#define DIALOG_PROG			BV32
#define EXTRADESC_PROG		BV33

#define	FE_MAX_ATTR			5

#endif

/* Specyficzne VNUM'y -- Pixel */
#define WAYLAND_CLONING 2238	// Klonownia na Wayland
