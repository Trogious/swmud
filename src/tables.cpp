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
 * 			Table load/save Module				   *
 ****************************************************************************/

#include <time.h>
#include "mud.h"

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

/* global variables */
int top_sn;
int top_herb;

SKILLTYPE *skill_table[MAX_SKILL];
SKILLTYPE *herb_table[MAX_HERB];

const char *const skill_tname[] =
{ "unknown", "Moc", "Umiej�tno��", "Broni", "J�zyk", "Herb" };
const char *const skill_tname_orig[] =
{ "unknown", "Spell", "Skill", "Weapon", "Tongue", "Herb" };

struct fun_type
{
	const char *name;
	DO_FUN *code;
};

const struct fun_type fun_table[] =
{
/* A */
{ "do_aedit", do_aedit },
{ "do_alia", do_alia },
{ "do_alias", do_alias },
{ "do_add_ship_title", do_add_ship_title },/*byTrog*/
{ "do_admintalk", do_admintalk },/*byTrog*/
{ "do_addbounty", do_addbounty },
{ "do_addengineer", do_addengineer },/*byAldegard*/
{ "do_addpilot", do_addpilot },
{ "do_add_patrol", do_add_patrol },
{ "do_accelerate", do_accelerate },
{ "do_advance", do_advance },
{ "do_affected", do_affected },
{ "do_afk", do_afk },
{ "do_aid", do_aid },
{ "do_alist", do_alist },
{ "do_allow", do_allow },
{ "do_allbounties", do_allbounties },
{ "do_allships", do_allships },
{ "do_allspeeders", do_allspeeders },
{ "do_ammo", do_ammo },
{ "do_ansi", do_ansi },
{ "do_answer", do_answer },
{ "do_apply", do_apply },
{ "do_appraise", do_appraise },
{ "do_appoint", do_appoint },
{ "do_areas", do_areas },
{ "do_arm", do_arm },
{ "do_ask", do_ask },
{ "do_astat", do_astat },
{ "do_astro", do_astro },/* Aldegard */
{ "do_at", do_at },
{ "do_authorize", do_authorize },
{ "do_autorecharge", do_autorecharge },
{ "do_autotrack", do_autotrack },
{ "do_autopilot", do_autopilot },
/* B */
{ "do_backstab", do_backstab },
{ "do_balzhur", do_balzhur },
{ "do_bamfin", do_bamfin },
{ "do_bamfout", do_bamfout },
{ "do_bail", do_bail },
{ "do_ban", do_ban },
{ "do_bank", do_bank },
{ "do_bargain", do_bargain },
{ "do_bash", do_bash },
{ "do_bashdoor", do_bashdoor },
{ "do_beep", do_beep },
{ "do_beg", do_beg },
{ "do_berserk", do_berserk },
{ "do_bestow", do_bestow },
{ "do_bio", do_bio },
{ "do_board", do_board },
{ "do_boards", do_boards },
{ "do_bodybag", do_bodybag },
{ "do_bounties", do_bounties },
{ "do_bribe", do_bribe },
{ "do_bset", do_bset },
{ "do_bstat", do_bstat },
{ "do_bug", do_bug },
{ "do_bury", do_bury },
{ "do_buy", do_buy },
{ "do_buyhome", do_buyhome },
{ "do_buyship", do_buyship },
{ "do_build_hq", do_build_hq },
/* C */
{ "do_calculate", do_calculate },
{ "do_capture", do_capture },
{ "do_cast", do_cast },
{ "do_cedit", do_cedit },
{ "do_chaff", do_chaff },
{ "do_camuflage", do_camuflage },/*by Aldegard*/
{ "do_interdict", do_interdict },/*by Aldegard*/
{ "do_eject", do_eject },/*by Aldegard*/
{ "do_changeattribute", do_changeattribute },/*Tanglor*/
{ "do_channels", do_channels },
{ "do_chat", do_chat },
{ "do_check_vnums", do_check_vnums },
{ "do_circle", do_circle },
{ "do_clan", do_clan }, /*byTrog*/
{ "do_clantalk", do_clantalk },
{ "do_climb", do_climb },
{ "do_crack_transponder", do_crack_transponder },/*by Aldegard*/
{ "do_close", do_close },
{ "do_clone", do_clone },
{ "do_clonescore", do_clonescore },
{ "do_closebay", do_closebay },
{ "do_closehatch", do_closehatch },
{ "do_closeshipdock", do_closeshipdock },
{ "do_cmdtable", do_cmdtable },
{ "do_codertalk", do_codertalk },
{ "do_commands", do_commands },
{ "do_compare", do_compare },
{ "do_config", do_config },
{ "do_connect", do_connect },/*Trog*/
{ "do_consider", do_consider },
{ "do_copyover", do_copyover },
{ "do_copyship", do_copyship },
{ "do_copystats", do_copystats }, /*Pixel*/
{ "do_corrupt", do_corrupt }, /*Ganis*/
{ "do_crank", do_crank }, /*byTrog*/
{ "do_credits", do_credits },
{ "do_crew", do_crew }, /*byAldegard*/
{ "do_cset", do_cset },
{ "do_clear_modules", do_clear_modules }, /*Qrwa ma� !!!*/
/*
 * Uuu, Alde. To 'Qrwa ma�' te� u nas koduje? :P
 * Czemu nic nie powiedzieli�cie?   -- Thanos
 */

/* D */
{ "do_delbounty", do_delbounty },/*Trog*/
{ "do_demote", do_demote },
{ "do_deny", do_deny },
{ "do_description", do_description },
{ "do_destro", do_destro },
{ "do_destroy", do_destroy },
{ "do_detonate", do_detonate },
{ "do_detrap", do_detrap },
{ "do_dialedit", do_dialedit }, /*Tanglor - Edytor Dialog�w*/
{ "do_diallist", do_diallist }, /*Tanglor - Lista dialog�w*/
{ "do_dig", do_dig },
{ "do_disarm", do_disarm },
{ "do_disconnect", do_disconnect },
{ "do_disguise", do_disguise },
{ "do_dismount", do_dismount },
{ "do_distres", do_distres },/*Aldegard*/
{ "do_dock", do_dock },/*Aldegard*/
{ "do_dominate", do_dominate },
{ "do_down", do_down },
{ "do_drag", do_drag },
{ "do_drink", do_drink },
{ "do_drive", do_drive },
{ "do_drop", do_drop },
{ "do_dump", do_dump },
/* E */
{ "do_east", do_east },
{ "do_eat", do_eat },
{ "do_echo", do_echo },
{ "do_echoat", do_echoat },
{ "do_editinfo", do_editinfo },/*Thanos*/
{ "do_elite_guard", do_elite_guard },
{ "do_emote", do_emote },
{ "do_empty", do_empty },
{ "do_empower", do_empower },
{ "do_enter", do_enter },
{ "do_equipment", do_equipment },
{ "do_examine", do_examine },
{ "do_exits", do_exits },
{ "do_experience", do_experience },
{ "do_exchange", do_exchange },
/* F */
{ "do_feed", do_feed },
{ "do_fill", do_fill },
{ "do_findresets", do_findresets }, /* Ganis */
/*    {	"do_findship",			do_findship		}, //Dremor*/
{ "do_fire", do_fire },
{ "do_firstaid", do_first_aid },
{ "do_fix", do_fix },/*Thanos*/
{ "do_fixchar", do_fixchar },
{ "do_flame", do_flame },
{ "do_flee", do_flee },
{ "do_fly", do_fly },
{ "do_foldarea", do_foldarea },
{ "do_follow", do_follow },
{ "do_for", do_for },
{ "do_forbid", do_forbid },
{ "do_force", do_force },
{ "do_forceclose", do_forceclose },
{ "do_forget", do_forget },
{ "do_fquit", do_fquit },
{ "do_freeze", do_freeze },
{ "do_fshow", do_fshow },
/* G */
{ "do_garota", do_garota },
{ "do_gather_intelligence", do_gather_intelligence },
{ "do_get", do_get },
{ "do_gemcutting", do_gemcutting },
{ "do_give", do_give },
{ "do_glance", do_glance },
{ "do_gold", do_gold },
{ "do_goto", do_goto },
{ "do_gouge", do_gouge },
{ "do_group", do_group },
{ "do_gtell", do_gtell },
{ "do_guildtalk", do_guildtalk },
/* H */
{ "do_hail", do_hail },
{ "do_hackshiplock", do_hackshiplock },
{ "do_hedit", do_hedit },
{ "do_help", do_help },
{ "do_hide", do_hide },
{ "do_hijack", do_hijack },
{ "do_hitall", do_hitall },
{ "do_hlist", do_hlist },
{ "do_holylight", do_holylight },
{ "do_holonet", do_holonet },
{ "do_homepage", do_homepage },
{ "do_hstat", do_hstat },
{ "do_hyperspace", do_hyperspace },
/* I */
{ "do_ide", do_ide },
{ "do_idea", do_idea },
{ "do_ignore", do_ignore },/*Thanos*/
{ "do_immadvance", do_immadvance }, /*Trog*/
{ "do_identify", do_identify },
{ "do_immtalk", do_immtalk },
{ "do_imports", do_imports },
{ "do_info", do_info },
{ "do_induct", do_induct },
{ "do_introduce", do_introduce },/*Tanglor*/
{ "do_inform", do_inform },
{ "do_inventory", do_inventory },
{ "do_invis", do_invis },
{ "do_invite", do_invite },/*Ganis*/
{ "do_invites", do_invites },/*Ganis*/
{ "do_iso", do_iso },
{ "do_isolate", do_isolate },
/* J */
/*    {	"do_jail",  	        	do_jail			},*/
/* K */
{ "do_kick", do_kick },
{ "do_kill", do_kill },
{ "do_kin", do_kin }, /*Tanglor*/
/* L */
{ "do_languages", do_languages },
{ "do_land", do_land },
{ "do_last", do_last },
{ "do_launch", do_launch },
{ "do_leave", do_leave },
{ "do_leaveship", do_leaveship },
{ "do_light", do_light },
{ "do_list", do_list },
{ "do_litterbug", do_litterbug },
{ "do_loadup", do_loadup },
{ "do_load_cargo", do_load_cargo },
{ "do_lock", do_lock },
{ "do_log", do_log },
{ "do_look", do_look },
/*    {	"do_low_purge",			do_low_purge		},*/
/* M */
{ "do_mailroom", do_mailroom },
{ "do_makearmor", do_makearmor },
{ "do_makeblade", do_makeblade },
{ "do_makeblaster", do_makeblaster },
{ "do_makelightsaber", do_makelightsaber },
{ "do_makespice", do_makespice },
{ "do_makegrenade", do_makegrenade },
{ "do_makehackdev", do_makehackdev },
{ "do_makelight", do_makelight },
{ "do_makelandmine", do_makelandmine },
{ "do_makecomlink", do_makecomlink },
{ "do_makeshield", do_makeshield },
{ "do_makecontainer", do_makecontainer },
{ "do_makejewelry", do_makejewelry },
{ "do_makeboard", do_makeboard },
{ "do_makeclan", do_makeclan },
{ "do_makemissile", do_makemissile },
{ "do_makeship", do_makeship },
{ "do_makestarsystem", do_makestarsystem },
/*    {	"do_makewizlist",		do_makewizlist		},*/
{ "do_makeplanet", do_makeplanet },
{ "do_mass_propeganda", do_mass_propeganda },
{ "do_matedit", do_matedit },
{ "do_matlist", do_matlist },
{ "do_medit", do_medit },
{ "do_memory", do_memory },
{ "do_mfind", do_mfind },
{ "do_minvoke", do_minvoke },
{ "do_mine", do_mine },
{ "do_mlist", do_mlist },
{ "do_modules", do_modules },/* Aldegard */
{ "do_mont", do_mont },/* Thanos */
{ "do_mount", do_mount },
{ "do_mp_close_passage", do_mp_close_passage },
{ "do_mp_damage", do_mp_damage },
{ "do_mp_deposit", do_mp_deposit },
{ "do_mp_open_passage", do_mp_open_passage },
{ "do_mp_practice", do_mp_practice },
{ "do_mp_restore", do_mp_restore },
{ "do_mp_slay", do_mp_slay },
{ "do_mp_withdraw", do_mp_withdraw },
{ "do_mpaddfrc", do_mpaddfrc },
{ "do_mpaddtolist", do_mpaddtolist },
{ "do_mpaddtowanted", do_mpaddtowanted },/*Thanos*/
{ "do_mpadvance", do_mpadvance },
{ "do_mpapply", do_mpapply },
{ "do_mpapplyb", do_mpapplyb },
{ "do_mpasound", do_mpasound },
{ "do_mpat", do_mpat },
{ "do_mpauth", do_mpauth },/*Trog*/
{ "do_mpclrnameslist", do_mpclrnameslist },
{ "do_mpdelay", do_mpdelay },
{ "do_mpdream", do_mpdream },
{ "do_mpecho", do_mpecho },
{ "do_mpechoaround", do_mpechoaround },
{ "do_mpechoat", do_mpechoat },
{ "do_mpedit", do_mpedit },
{ "do_mpgain", do_mpgain },
{ "do_mpforce", do_mpforce },
{ "do_mpgoto", do_mpgoto },
{ "do_mpinduct", do_mpinduct },/*byTrog*/
{ "do_mpinvis", do_mpinvis },
{ "do_mpjunk", do_mpjunk },
{ "do_mpkill", do_mpkill },
{ "do_mpmload", do_mpmload },
{ "do_mpnoecho", do_mpnoecho },/*Thanos*/
{ "do_mpnothing", do_mpnothing },
{ "do_mpoload", do_mpoload },
{ "do_mpoutcast", do_mpoutcast },/*byTrog*/
{ "do_mppurge", do_mppurge },
{ "do_mpqmload", do_mpqmload },/*Thanos*/
{ "do_mpqoload", do_mpqoload },/*Thanos*/
{ "do_mpresetroom", do_mpresetroom },/*Thanos*/
{ "do_mpstat", do_mpstat },
{ "do_mpstun", do_mpstun },/*Thanos*/
{ "do_mpsuggest", do_mpsuggest },/*Thanos*/
{ "do_mptire", do_mptire },
{ "do_mptransfer", do_mptransfer },
{ "do_mpremfromlist", do_mpremfromlist },
{ "do_mpremfromwanted", do_mpremfromwanted },
{ "do_mset", do_mset },
{ "do_mstat", do_mstat },
{ "do_murde", do_murde },
{ "do_murder", do_murder },
{ "do_music", do_music },
{ "do_mwhere", do_mwhere },
/* N */
{ "do_newbiechat", do_newbiechat },
{ "do_newbieset", do_newbieset },
{ "do_noresolve", do_noresolve },
{ "do_north", do_north },
{ "do_northeast", do_northeast },
{ "do_northwest", do_northwest },
{ "do_noteroom", do_noteroom },
{ "do_nopol", do_nopol },
/* O */
{ "do_oedit", do_oedit },
{ "do_ofind", do_ofind },
{ "do_oinvoke", do_oinvoke },
{ "do_olcinfo", do_olcinfo },
{ "do_olctalk", do_olctalk },
{ "do_oldscore", do_oldscore },
{ "do_olist", do_olist },
{ "do_ooc", do_ooc },
{ "do_opedit", do_opedit },
{ "do_open", do_open },
{ "do_openbay", do_openbay },
{ "do_openhatch", do_openhatch },
{ "do_openshipdock", do_openshipdock },
{ "do_opstat", do_opstat },
{ "do_order", do_order },
{ "do_ordertalk", do_ordertalk },
{ "do_oset", do_oset },
{ "do_ostat", do_ostat },
{ "do_outcast", do_outcast },
{ "do_owhere", do_owhere },
/* P */
{ "do_pager", do_pager },
{ "do_pardon", do_pardon },
{ "do_password", do_password },
{ "do_pazaak", do_pazaak },
{ "do_peace", do_peace },
{ "do_pedit", do_pedit },
{ "do_pick", do_pick },
{ "do_pickshiplock", do_pickshiplock },
{ "do_planets", do_planets },
{ "do_pluogus", do_pluogus },
{ "do_poison_weapon", do_poison_weapon },
{ "do_postguard", do_postguard },
{ "do_practice", do_practice },
{ "do_prefi", do_prefi },
{ "do_prefix", do_prefix },
{ "do_proedit", do_proedit },
{ "do_prompt", do_prompt },
{ "do_propeganda", do_propeganda },
{ "do_prostat", do_prostat },
{ "do_pull", do_pull },
{ "do_punch", do_punch },
{ "do_purge", do_purge },
{ "do_push", do_push },
{ "do_put", do_put },
/* Q */
{ "do_qedit", do_qedit },
{ "do_quaff", do_quaff },
{ "do_quest", do_quest },
{ "do_qstat", do_qstat },
{ "do_qui", do_qui },
{ "do_quit", do_quit },
/* R */
{ "do_radar", do_radar },
{ "do_raedit", do_raedit },
{ "do_rank", do_rank },
{ "do_rat", do_rat },
{ "do_reboo", do_reboo },
{ "do_reboot", do_reboot },
{ "do_recharge", do_recharge },
{ "do_recho", do_recho },
{ "do_recall", do_recall },
{ "do_redit", do_redit },
{ "do_refuel", do_refuel },
{ "do_reinforcements", do_reinforcements },
{ "do_regoto", do_regoto },
{ "do_remengineer", do_remengineer },
{ "do_remember", do_remember },/*Tanglor*/
{ "do_remove", do_remove },
{ "do_rempilot", do_rempilot },
{ "do_rename", do_rename },
{ "do_rent", do_rent },
{ "do_repair", do_repair },
{ "do_repairship", do_repairship },
{ "do_repairshops", do_repairshops },
{ "do_reply", do_reply },
{ "do_report", do_report },
{ "do_rescue", do_rescue },
{ "do_reset", do_reset },
{ "do_resetship", do_resetship },
{ "do_resign", do_resign },
{ "do_rest", do_rest },
{ "do_restore", do_restore },
{ "do_restoretime", do_restoretime },
{ "do_retell", do_retell },
{ "do_retire", do_retire },
{ "do_retran", do_retran },
{ "do_return", do_return },
{ "do_rlist", do_rlist },
{ "do_rpedit", do_rpedit },
{ "do_rpstat", do_rpstat },
{ "do_rset", do_rset },
{ "do_rstat", do_rstat },
/* S */
{ "do_sacrifice", do_sacrifice },
{ "do_save", do_save },
{ "do_saveall", do_saveall },
{ "do_say", do_say },
{ "do_scan", do_scan },
{ "do_score", do_score },
{ "do_scribe", do_scribe },
{ "do_search", do_search },
{ "do_sedit", do_sedit },
{ "do_seduce", do_seduce },
{ "do_sell", do_sell },
{ "do_sellship", do_sellship },
{ "do_setblaster", do_setblaster },
{ "do_set_boot_time", do_set_boot_time },
{ "do_set_password", do_set_password },
{ "do_setclan", do_setclan },
{ "do_setcourse", do_setcourse },
{ "do_setship", do_setship },
{ "do_describe_ship", do_describe_ship }, /*byTrog*/
{ "do_setstarsystem", do_setstarsystem },
{ "do_setplanet", do_setplanet },
{ "do_shgoto", do_shgoto }, /*Thanos*/
{ "do_ships", do_ships },
{ "do_shedit", do_shedit }, /*Thanos*/
{ "do_shlist", do_shlist }, /*Thanos*/
{ "do_shload", do_shload }, /*Thanos*/
{ "do_shops", do_shops },
{ "do_shout", do_shout },
{ "do_shove", do_shove },
{ "do_showclan", do_showclan },
{ "do_showship", do_showship },
{ "do_showplanet", do_showplanet },/*byTrog*/
{ "do_showstarsystem", do_showstarsystem },
{ "do_shsave", do_shsave }, /*Thanos*/
{ "do_shstat", do_shstat }, /*Thanos*/
{ "do_shutdow", do_shutdow },
{ "do_shutdown", do_shutdown },
{ "do_silence", do_silence },
{ "do_sit", do_sit },
{ "do_skedit", do_skedit },
{ "do_sla", do_sla },
{ "do_slay", do_slay },
{ "do_sleep", do_sleep },
{ "do_slice", do_slice },
{ "do_slist", do_slist },
{ "do_smalltalk", do_smalltalk },
{ "do_smoke", do_smoke },
{ "do_snipe", do_snipe },
{ "do_snoop", do_snoop },
{ "do_sober", do_sober },
{ "do_socials", do_socials },
{ "do_south", do_south },
{ "do_sound", do_sound },
{ "do_southeast", do_southeast },
{ "do_southwest", do_southwest },
{ "do_speak", do_speak },
{ "do_speeders", do_speeders },
{ "do_split", do_split },
{ "do_ssedit", do_ssedit },
{ "do_sset", do_sset },
{ "do_stand", do_stand },
{ "do_start_speak", do_start_speak },
{ "do_starsystems", do_starsystems },
{ "do_status", do_status },
{ "do_stbar", do_stbar },
{ "do_steal", do_steal },
{ "do_stun", do_stun },
{ "do_switch", do_switch },
{ "do_shiptalk", do_shiptalk },
{ "do_spacetalk", do_spacetalk },
{ "do_special_forces", do_special_forces },
{ "do_suicide", do_suicide },
{ "do_systemtalk", do_systemtalk },
/* T */
{ "do_takedrug", do_takedrug },
{ "do_tamp", do_tamp },
{ "do_target", do_target },
{ "do_tcedit", do_tcedit },
{ "do_teach", do_teach },
{ "do_tell", do_tell },
{ "do_throw", do_throw },
{ "do_time", do_time },
{ "do_timecmd", do_timecmd },
{ "do_title", do_title },
/*{	"do_thanosscore",		do_thanosscore		}, Pixel: moze kiedys*/
{ "do_torture", do_torture },
{ "do_track", do_track },
{ "do_tractorbeam", do_tractorbeam },
{ "do_train", do_train },
{ "do_trajectory", do_trajectory },
{ "do_transfer", do_transfer },
{ "do_transship", do_transship },
{ "do_trust", do_trust },
{ "do_typo", do_typo },
{ "do_transponders", do_transponders },/*by Aldegard*/
/* U */
{ "do_unalia", do_unalia },
{ "do_unalias", do_unalias },
{ "do_unload_cargo", do_unload_cargo },
{ "do_unlock", do_unlock },
{ "do_unsilence", do_unsilence },
{ "do_up", do_up },
{ "do_use", do_use },
{ "do_users", do_users },
{ "do_uninstal_module", do_uninstal_module },/*Aldegard*/
{ "do_uninvite", do_uninvite },/*Ganis*/
{ "do_unisolate", do_unisolate },
{ "do_undock", do_undock },/*Aldegard*/
/* V */
{ "do_value", do_value },
{ "do_visible", do_visible },
{ "do_vsearch", do_vsearch },
/* W */
{ "do_wake", do_wake },
{ "do_wanted", do_wanted },
{ "do_wartalk", do_wartalk },
{ "do_wear", do_wear },
{ "do_weather", do_weather },
{ "do_west", do_west },
{ "do_where", do_where },
{ "do_who", do_who },
{ "do_whois", do_whois },
{ "do_whowas", do_whowas },
{ "do_wimpy", do_wimpy },
{ "do_wizinfo", do_wizinfo },
{ "do_wizlist", do_wizlist },
{ "do_wizlock", do_wizlock },
{ "do_win", do_win },
/* Y */
{ "do_yell", do_yell },

/* !!! NIE DOTYKA� - TO MA BY� NA KO�CU !!! */
{ "reserved", skill_notfound } };

struct spell_type
{
	const char *name;
	SPELL_FUN *code;
};

const struct spell_type spell_table[] =
{
{ "spell_smaug", spell_smaug },
{ "spell_animate_dead", spell_animate_dead },
{ "spell_blindness", spell_blindness },
{ "spell_cause_critical", spell_cause_critical },
{ "spell_cause_light", spell_cause_light },
{ "spell_cause_serious", spell_cause_serious },
{ "spell_charm_person", spell_charm_person },
{ "spell_control_weather", spell_control_weather },
{ "spell_cure_blindness", spell_cure_blindness },
{ "spell_cure_poison", spell_cure_poison },
{ "spell_curse", spell_curse },
{ "spell_detect_poison", spell_detect_poison },
{ "spell_dispel_evil", spell_dispel_evil },
{ "spell_dispel_force", spell_dispel_force },
{ "spell_dream", spell_dream },
{ "spell_earthquake", spell_earthquake },
{ "spell_enchant_weapon", spell_enchant_weapon },
{ "spell_energy_drain", spell_energy_drain },
{ "spell_faerie_fire", spell_faerie_fire },
{ "spell_faerie_fog", spell_faerie_fog },
{ "spell_farsight", spell_farsight },
{ "spell_knock", spell_knock },
{ "spell_harm", spell_harm },
{ "spell_identify", spell_identify },
{ "spell_invis", spell_invis },
{ "spell_know_alignment", spell_know_alignment },
{ "spell_lightning_bolt", spell_lightning_bolt },
{ "spell_poison", spell_poison },
{ "spell_possess", spell_possess },
{ "spell_remove_curse", spell_remove_curse },
{ "spell_remove_invis", spell_remove_invis },
{ "spell_remove_trap", spell_remove_trap },
{ "spell_sleep", spell_sleep },
{ "spell_ventriloquate", spell_ventriloquate },
{ "spell_weaken", spell_weaken },
{ "spell_acid_breath", spell_acid_breath },
{ "spell_fire_breath", spell_fire_breath },
{ "spell_frost_breath", spell_frost_breath },
{ "spell_gas_breath", spell_gas_breath },
{ "spell_lightning_breath", spell_lightning_breath },
{ "spell_scorching_surge", spell_scorching_surge },

{ "spell_ethereal_fist", spell_ethereal_fist },
{ "spell_hand_of_darkness", spell_hand_of_darkness },
{ "spell_mind_wrack", spell_mind_wrack },
{ "spell_mind_wrench", spell_mind_wrench },
{ "spell_revive", spell_revive },
{ "spell_black_lightning", spell_black_lightning },

/* NIE DOTYKA� */
{ "reserved", NULL },
{ "spell_null", spell_null },
{ "", spell_notfound }

};

DO_FUN* skill_function(char *name)
{
	int fn;

	for (fn = 0; str_cmp(fun_table[fn].name, "reserved"); fn++)
	{
		if (!str_cmp(name, fun_table[fn].name))
			return fun_table[fn].code;
	}

	return skill_notfound;
}

SPELL_FUN* spell_function(char *name)
{
	int fn;

	for (fn = 0; *spell_table[fn].name; fn++)
	{
		if (!str_cmp(name, spell_table[fn].name))
			return spell_table[fn].code;
	}

	return spell_notfound;
}

const char* skill_name(DO_FUN *skill)
{
	int fn;

	for (fn = 0; str_cmp(fun_table[fn].name, "reserved"); fn++)
	{
		if (fun_table[fn].code == skill)
			return fun_table[fn].name;
	}
	return "reserved";
}

const char* spell_name(SPELL_FUN *spell)
{
	int fn;

	for (fn = 0; *spell_table[fn].name; fn++)
	{
		if (spell_table[fn].code == spell)
			return spell_table[fn].name;
	}
	return "reserved";
}

/*
 * Function used by qsort to sort skills
 */
int skill_comp(SKILLTYPE **sk1, SKILLTYPE **sk2)
{
	SKILLTYPE *skill1 = (*sk1);
	SKILLTYPE *skill2 = (*sk2);

	if (!skill1 && skill2)
		return 1;
	if (skill1 && !skill2)
		return -1;
	if (!skill1 && !skill2)
		return 0;
	if (skill1->type < skill2->type)
		return -1;
	if (skill1->type > skill2->type)
		return 1;
	return strcmp(skill1->name, skill2->name);
}

/*
 * Sort the skill table with qsort
 */
void sort_skill_table()
{
	log_string("Sorting skill table");
	qsort(&skill_table[1], top_sn - 1, sizeof(SKILLTYPE*),
			(int (*)(const void*, const void*)) skill_comp);
}

/*
 * Write skill data to a file
 */
void fwrite_skill(FILE *fpout, SKILLTYPE *skill)
{
	SMAUG_AFF *aff;

	fprintf(fpout, "Name         %s~\n", skill->name);
	//changed by Thanos (tname na tname_orig )
	fprintf(fpout, "Type         %s\n", skill_tname_orig[skill->type]);
	fprintf(fpout, "Flags        %lld\n", skill->flags);
	if (skill->target)
		fprintf(fpout, "Target       %d\n", skill->target);
	if (skill->minimum_position)
		fprintf(fpout, "Minpos       %d\n", skill->minimum_position);
	if (skill->saves)
		fprintf(fpout, "Saves        %d\n", skill->saves);
	if (skill->slot)
		fprintf(fpout, "Slot         %d\n", skill->slot);
	if (skill->min_mana)
		fprintf(fpout, "Mana         %d\n", skill->min_mana);
	if (skill->beats)
		fprintf(fpout, "Rounds       %d\n", skill->beats);
	if (skill->guild != -1)
		fprintf(fpout, "Guild        %d\n", skill->guild);
	if (skill->skill_fun)
		fprintf(fpout, "Code         %s\n", skill_name(skill->skill_fun));
	else if (skill->spell_fun)
		fprintf(fpout, "Code         %s\n", spell_name(skill->spell_fun));
	fprintf(fpout, "Dammsg       %s~\n", skill->noun_damage);
	if (skill->msg_off && skill->msg_off[0] != '\0')
		fprintf(fpout, "Wearoff      %s~\n", skill->msg_off);

	if (skill->hit_char && skill->hit_char[0] != '\0')
		fprintf(fpout, "Hitchar      %s~\n", skill->hit_char);
	if (skill->hit_vict && skill->hit_vict[0] != '\0')
		fprintf(fpout, "Hitvict      %s~\n", skill->hit_vict);
	if (skill->hit_room && skill->hit_room[0] != '\0')
		fprintf(fpout, "Hitroom      %s~\n", skill->hit_room);

	if (skill->miss_char && skill->miss_char[0] != '\0')
		fprintf(fpout, "Misschar     %s~\n", skill->miss_char);
	if (skill->miss_vict && skill->miss_vict[0] != '\0')
		fprintf(fpout, "Missvict     %s~\n", skill->miss_vict);
	if (skill->miss_room && skill->miss_room[0] != '\0')
		fprintf(fpout, "Missroom     %s~\n", skill->miss_room);

	if (skill->die_char && skill->die_char[0] != '\0')
		fprintf(fpout, "Diechar      %s~\n", skill->die_char);
	if (skill->die_vict && skill->die_vict[0] != '\0')
		fprintf(fpout, "Dievict      %s~\n", skill->die_vict);
	if (skill->die_room && skill->die_room[0] != '\0')
		fprintf(fpout, "Dieroom      %s~\n", skill->die_room);

	if (skill->imm_char && skill->imm_char[0] != '\0')
		fprintf(fpout, "Immchar      %s~\n", skill->imm_char);
	if (skill->imm_vict && skill->imm_vict[0] != '\0')
		fprintf(fpout, "Immvict      %s~\n", skill->imm_vict);
	if (skill->imm_room && skill->imm_room[0] != '\0')
		fprintf(fpout, "Immroom      %s~\n", skill->imm_room);

	if (skill->dice && skill->dice[0] != '\0')
		fprintf(fpout, "Dice         %s~\n", skill->dice);
	if (skill->value)
		fprintf(fpout, "Value        %d\n", skill->value);
	if (skill->difficulty)
		fprintf(fpout, "Difficulty   %d\n", skill->difficulty);
	if (skill->participants)
		fprintf(fpout, "Participants %d\n", skill->participants);
	if (skill->components && skill->components[0] != '\0')
		fprintf(fpout, "Components   %s~\n", skill->components);
	if (skill->teachers && skill->teachers[0] != '\0')
		fprintf(fpout, "Teachers     %s~\n", skill->teachers);
	for (aff = skill->affects; aff; aff = aff->next)
		fprintf(fpout, "Affect       '%s' %d '%s' %d\n", aff->duration,
				aff->location, aff->modifier, aff->bitvector);
	if (skill->alignment)
		fprintf(fpout, "Alignment   %d\n", skill->alignment);

	if (skill->type != SKILL_HERB)
	{
		fprintf(fpout, "Minlevel     %d\n", skill->min_level);
	}

	if (skill->forcetype)
		fprintf(fpout, "ForceType     %d\n", skill->forcetype);

	fprintf(fpout, "End\n\n");
}

/*
 * Save the skill table to disk
 */
void save_skill_table()
{
	int x;
	FILE *fpout;

	if ((fpout = fopen( SKILL_FILE, "w")) == NULL)
	{
		bug("Cannot open skills.dat for writting", 0);
		perror( SKILL_FILE);
		return;
	}

	for (x = 0; x < top_sn; x++)
	{
		if (!skill_table[x]->name || skill_table[x]->name[0] == '\0')
			break;
		fprintf(fpout, "#SKILL\n");
		fwrite_skill(fpout, skill_table[x]);
	}
	fprintf(fpout, "#END\n");
	fclose(fpout);
}

/*
 * Save the herb table to disk
 */
void save_herb_table()
{
	int x;
	FILE *fpout;

	if ((fpout = fopen( HERB_FILE, "w")) == NULL)
	{
		bug("Cannot open herbs.dat for writting", 0);
		perror( HERB_FILE);
		return;
	}

	for (x = 0; x < top_herb; x++)
	{
		if (!herb_table[x]->name || herb_table[x]->name[0] == '\0')
			break;
		fprintf(fpout, "#HERB\n");
		fwrite_skill(fpout, herb_table[x]);
	}
	fprintf(fpout, "#END\n");
	fclose(fpout);
}

/*
 * Save the socials to disk
 */
void save_socials()
{
	FILE *fpout;
	SOCIALTYPE *social;
	int x;

	if ((fpout = fopen( SOCIAL_FILE, "w")) == NULL)
	{
		bug("Cannot open socials.dat for writting", 0);
		perror( SOCIAL_FILE);
		return;
	}

	for (x = 0; x < 27; x++)
	{
		for (social = social_index[x]; social; social = social->next)
		{
			if (!social->name || social->name[0] == '\0')
			{
				bug("blank social in hash bucket %d", x);
				continue;
			}
			fprintf(fpout, "#SOCIAL\n");
			fprintf(fpout, "Name        %s~\n", social->name);
			if (social->char_no_arg)
				fprintf(fpout, "CharNoArg   %s~\n", social->char_no_arg);
			else
				bug("NULL char_no_arg in hash bucket %d", x);
			if (social->others_no_arg)
				fprintf(fpout, "OthersNoArg %s~\n", social->others_no_arg);
			if (social->char_found)
				fprintf(fpout, "CharFound   %s~\n", social->char_found);
			if (social->others_found)
				fprintf(fpout, "OthersFound %s~\n", social->others_found);
			if (social->vict_found)
				fprintf(fpout, "VictFound   %s~\n", social->vict_found);
			if (social->char_auto)
				fprintf(fpout, "CharAuto    %s~\n", social->char_auto);
			if (social->others_auto)
				fprintf(fpout, "OthersAuto  %s~\n", social->others_auto);
			fprintf(fpout, "End\n\n");
		}
	}
	fprintf(fpout, "#END\n");
	fclose(fpout);
}

int get_skill(char *skilltype)
{
	if (!str_cmp(skilltype, "Spell"))
		return SKILL_SPELL;
	if (!str_cmp(skilltype, "Skill"))
		return SKILL_SKILL;
	if (!str_cmp(skilltype, "Weapon"))
		return SKILL_WEAPON;
	if (!str_cmp(skilltype, "Tongue"))
		return SKILL_TONGUE;
	if (!str_cmp(skilltype, "Herb"))
		return SKILL_HERB;
	return SKILL_UNKNOWN;
}

/*
 * Save the commands to disk
 */
void save_commands()
{
	FILE *fpout;
	CMDTYPE *command;
	int x;

	if ((fpout = fopen( COMMAND_FILE, "w")) == NULL)
	{
		bug("Cannot open commands.dat for writing", 0);
		perror( COMMAND_FILE);
		return;
	}

	for (x = 0; x < MAX_CMD_HASH; x++)
	{
		for (command = command_hash[x]; command; command = command->next)
		{
			if (!command->name || command->name[0] == '\0')
			{
				bug("blank command in hash bucket %d", x);
				continue;
			}
			fprintf(fpout, "#COMMAND\n");
			fprintf(fpout, "Name        %s~\n", command->name);
			if (*command->alias)
				fprintf(fpout, "NamePl      %s~\n", command->alias);
			fprintf(fpout, "Code        %s\n", skill_name(command->do_fun));
			fprintf(fpout, "Position    %d\n", command->position);
			fprintf(fpout, "Level       %d\n", command->level);
			fprintf(fpout, "Log         %d\n", command->log);
			fprintf(fpout, "Flags       %lld\n", command->flags);
			fprintf(fpout, "End\n\n");
		}
	}
	fprintf(fpout, "#END\n");
	fclose(fpout);
}

SKILLTYPE* fread_skill(FILE *fp)
{
	const char *word;
	bool fMatch;
	SKILLTYPE *skill;

	CREATE(skill, SKILLTYPE, 1);

	skill->guild = -1;

	for (;;)
	{
		word = feof(fp) ? "End" : fread_word(fp);
		fMatch = false;

		switch (UPPER(word[0]))
		{
		case '*':
			fMatch = true;
			fread_to_eol(fp);
			break;

		case 'A':
			KEY("Alignment", skill->alignment, fread_number(fp))
			;
			if (!str_cmp(word, "Affect"))
			{
				SMAUG_AFF *aff;

				CREATE(aff, SMAUG_AFF, 1);
				STRDUP(aff->duration, fread_word(fp));
				aff->location = fread_number(fp);
				STRDUP(aff->modifier, fread_word(fp));
				aff->bitvector = fread_number(fp);
				aff->next = skill->affects;
				skill->affects = aff;
				fMatch = true;
				break;
			}
			break;

		case 'C':
			if (!str_cmp(word, "Code"))
			{
				SPELL_FUN *spellfun;
				DO_FUN *dofun;
				char *w = fread_word(fp);

				fMatch = true;
				if ((spellfun = spell_function(w)) != spell_notfound)
					skill->spell_fun = spellfun;
				else if ((dofun = skill_function(w)) != skill_notfound)
					skill->skill_fun = dofun;
				else
				{
					bug("unknown skill/spell %s", w);
					skill->spell_fun = spell_null;
				}
				break;
			}
			KEY("Code", skill->spell_fun, spell_function(fread_word(fp)))
			;
			SKEY("Components", skill->components, fread_string(fp))
			;
			break;

		case 'D':
			SKEY("Dammsg", skill->noun_damage, fread_string(fp))
			;
			SKEY("Dice", skill->dice, fread_string(fp))
			;
			SKEY("Diechar", skill->die_char, fread_string(fp))
			;
			SKEY("Dieroom", skill->die_room, fread_string(fp))
			;
			SKEY("Dievict", skill->die_vict, fread_string(fp))
			;
			KEY("Difficulty", skill->difficulty, fread_number(fp))
			;
			break;

		case 'E':
			if (!str_cmp(word, "End"))
				return skill;
			break;

		case 'F':
			KEY("Flags", skill->flags, fread_number(fp))
			;
			KEY("ForceType", skill->forcetype, fread_number(fp))
			;
			break;

		case 'G':
			KEY("Guild", skill->guild, fread_number(fp))
			;
			break;

		case 'H':
			SKEY("Hitchar", skill->hit_char, fread_string(fp))
			;
			SKEY("Hitroom", skill->hit_room, fread_string(fp))
			;
			SKEY("Hitvict", skill->hit_vict, fread_string(fp))
			;
			break;

		case 'I':
			SKEY("Immchar", skill->imm_char, fread_string(fp))
			;
			SKEY("Immroom", skill->imm_room, fread_string(fp))
			;
			SKEY("Immvict", skill->imm_vict, fread_string(fp))
			;
			break;

		case 'M':
			KEY("Mana", skill->min_mana, fread_number(fp))
			;
			KEY("Minlevel", skill->min_level, fread_number(fp))
			;
			KEY("Minpos", skill->minimum_position, fread_number(fp))
			;
			SKEY("Misschar", skill->miss_char, fread_string(fp))
			;
			SKEY("Missroom", skill->miss_room, fread_string(fp))
			;
			SKEY("Missvict", skill->miss_vict, fread_string(fp))
			;
			break;

		case 'N':
			SKEY("Name", skill->name, fread_string(fp))
			;
			break;

		case 'P':
			KEY("Participants", skill->participants, fread_number(fp))
			;
			break;

		case 'R':
			KEY("Rounds", skill->beats, fread_number(fp))
			;
			break;

		case 'S':
			KEY("Slot", skill->slot, fread_number(fp))
			;
			KEY("Saves", skill->saves, fread_number(fp))
			;
			break;

		case 'T':
			KEY("Target", skill->target, fread_number(fp))
			;
			SKEY("Teachers", skill->teachers, fread_string(fp))
			;
			KEY("Type", skill->type, get_skill(fread_word(fp)))
			;
			break;

		case 'V':
			KEY("Value", skill->value, fread_number(fp))
			;
			break;

		case 'W':
			SKEY("Wearoff", skill->msg_off, fread_string(fp))
			;
			break;
		}

		if (!fMatch)
		{
			bug("no match: %s", word);
		}
	}
}

void load_skill_table()
{
	FILE *fp;

	if ((fp = fopen( SKILL_FILE, "r")) != NULL)
	{
		top_sn = 0;
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
			if (!str_cmp(word, "SKILL"))
			{
				if (top_sn >= MAX_SKILL)
				{
					bug("more skills than MAX_SKILL %d", MAX_SKILL);
					fclose(fp);
					return;
				}
				skill_table[top_sn++] = fread_skill(fp);
				continue;
			}
			else if (!str_cmp(word, "END"))
				break;
			else
			{
				bug("bad section.", 0);
				continue;
			}
		}
		fclose(fp);
	}
	else
	{
		bug("Cannot open skills.dat", 0);
		perror( SKILL_FILE);
		exit(0);
	}
}

void load_herb_table()
{
	FILE *fp;

	if ((fp = fopen( HERB_FILE, "r")) != NULL)
	{
		top_herb = 0;
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
				bug("# not found.", 0);
				break;
			}

			word = fread_word(fp);
			if (!str_cmp(word, "HERB"))
			{
				if (top_herb >= MAX_HERB)
				{
					bug("more herbs than MAX_HERB %d", MAX_HERB);
					fclose(fp);
					return;
				}
				herb_table[top_herb++] = fread_skill(fp);
				if (herb_table[top_herb - 1]->slot == 0)
					herb_table[top_herb - 1]->slot = top_herb - 1;
				continue;
			}
			else if (!str_cmp(word, "END"))
				break;
			else
			{
				bug("bad section.", 0);
				continue;
			}
		}
		fclose(fp);
	}
	else
	{
		bug("Cannot open herbs.dat", 0);
		perror( HERB_FILE);
		exit(0);
	}
}

void fread_social(FILE *fp)
{
	const char *word;
	bool fMatch;
	SOCIALTYPE *social;

	CREATE(social, SOCIALTYPE, 1);

	for (;;)
	{
		word = feof(fp) ? "End" : fread_word(fp);
		fMatch = false;

		switch (UPPER(word[0]))
		{
		case '*':
			fMatch = true;
			fread_to_eol(fp);
			break;

		case 'C':
			SKEY("CharNoArg", social->char_no_arg, fread_string(fp))
			;
			SKEY("CharFound", social->char_found, fread_string(fp))
			;
			SKEY("CharAuto", social->char_auto, fread_string(fp))
			;
			break;

		case 'E':
			if (!str_cmp(word, "End"))
			{
				if (!social->name)
				{
					bug("Name not found", 0);
					free_social(social);
					return;
				}
				if (!social->char_no_arg)
				{
					bug("CharNoArg not found", 0);
					free_social(social);
					return;
				}
				add_social(social);
				return;
			}
			break;

		case 'N':
			SKEY("Name", social->name, fread_string(fp))
			;
			break;

		case 'O':
			SKEY("OthersNoArg", social->others_no_arg, fread_string(fp))
			;
			SKEY("OthersFound", social->others_found, fread_string(fp))
			;
			SKEY("OthersAuto", social->others_auto, fread_string(fp))
			;
			break;

		case 'V':
			SKEY("VictFound", social->vict_found, fread_string(fp))
			;
			break;
		}

		if (!fMatch)
		{
			bug("no match: %s", word);
		}
	}
}

void load_socials()
{
	FILE *fp;

	if ((fp = fopen( SOCIAL_FILE, "r")) != NULL)
	{
		top_sn = 0;
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
				bug("# not found.", 0);
				break;
			}

			word = fread_word(fp);
			if (!str_cmp(word, "SOCIAL"))
			{
				fread_social(fp);
				continue;
			}
			else if (!str_cmp(word, "END"))
				break;
			else
			{
				bug("bad section.", 0);
				continue;
			}
		}
		fclose(fp);
	}
	else
	{
		bug("Cannot open socials.dat", 0);
		perror( SOCIAL_FILE);
		exit(0);
	}
}

void fread_command(FILE *fp)
{
	const char *word;
	bool fMatch;
	CMDTYPE *command;

	CREATE(command, CMDTYPE, 1);

	command->flags = 0; /* na wszelki -- Thanos */

	for (;;)
	{
		word = feof(fp) ? "End" : fread_word(fp);
		fMatch = false;

		switch (UPPER(word[0]))
		{
		case '*':
			fMatch = true;
			fread_to_eol(fp);
			break;

		case 'C':
			KEY("Code", command->do_fun, skill_function(fread_word(fp)))
			;
			break;

		case 'E':
			if (!str_cmp(word, "End"))
			{
				if (!command->name)
				{
					bug("Name not found", 0);
					free_command(command);
					return;
				}
				if (!command->do_fun)
				{
					bug("Function not found", 0);
					free_command(command);
					return;
				}

				if (!command->alias)
					STRDUP(command->alias, "");

				add_command(command);
				return;
			}
			break;

		case 'F': /* Thanos */
			KEY("Flags", command->flags, fread_number(fp))
			;
			break;

		case 'L':
			KEY("Level", command->level, fread_number(fp))
			;
			KEY("Log", command->log, fread_number(fp))
			;
			break;

		case 'N':
			SKEY("Name", command->name, fread_string(fp))
			;
			SKEY("NamePl", command->alias, fread_string(fp))
			;
			break;

		case 'P':
			KEY("Position", command->position, fread_number(fp))
			;
			break;
		}

		if (!fMatch)
		{
			bug("no match: %s", word);
		}
	}
}

void load_commands()
{
	FILE *fp;

	if ((fp = fopen( COMMAND_FILE, "r")) != NULL)
	{
		top_sn = 0;
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
				bug("# not found.", 0);
				break;
			}

			word = fread_word(fp);
			if (!str_cmp(word, "COMMAND"))
			{
				fread_command(fp);
				continue;
			}
			else if (!str_cmp(word, "END"))
				break;
			else
			{
				bug("bad section.", 0);
				continue;
			}
		}
		fclose(fp);
	}
	else
	{
		bug("Cannot open commands.dat", 0);
		perror( COMMAND_FILE);
		exit(0);
	}

}
