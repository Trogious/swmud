/***************************************************************************
* swmud.pl
* ------------------------------------------------------------------------ *
*                     ____________   __     ______                         *
*   Aldegard    Jot  (   ___   ___| /  \   |  __  )   Thanos      Trog     *
*        ______________\  \|   |  /  ^   \ |     <_______________          *
*        \________________/|___|/___/"\___\|__|\________________/          *
*                  \   \/ \/   //  \   |  __  )(  ___/`                    *
*                    \       //  ^   \ |     <__)  \                       *
*                      \_!_//___/"\___\|__|\______/TM                      *
* (c) 2009                  M       U        D                Ver 1.0      *
* ------------------------------------------------------------------------ *
*                               <module_name>                              *
****************************************************************************/


/* This file was generated by Trog's granulate_structs.pl. */

#ifndef SKILL_TYPE_H_
#define SKILL_TYPE_H_
#include "../defines.h"
#include "../typedefs.h"

struct	skill_type
{
	char				* name;			/* Name of skill		*/
	SPELL_FUN			* spell_fun;		/* Spell pointer (for spells)	*/
	DO_FUN				* skill_fun;		/* Skill pointer (for skills)	*/
	int					target;			/* Legal targets		*/
	int					minimum_position;	/* Position for caster / user	*/
	int					slot;			/* Slot for #OBJECT loading	*/
	int					min_mana;		/* Minimum mana used		*/
	int					beats;			/* Rounds required to use skill */
	char				* noun_damage;		/* Damage message		*/
	char				* msg_off;		/* Wear off message		*/
	int					guild;			/* Which guild the skill belongs to */
	int					min_level;		/* Minimum level to be able to cast */
	int					type;			/* Spell/Skill/Weapon/Tongue	*/
	int64				flags;			/* extra stuff			*/
	char				* hit_char;		/* Success message to caster	*/
	char				* hit_vict;		/* Success message to victim	*/
	char				* hit_room;		/* Success message to room	*/
	char				* miss_char;		/* Failure message to caster	*/
	char				* miss_vict;		/* Failure message to victim	*/
	char				* miss_room;		/* Failure message to room	*/
	char				* die_char;		/* Victim death msg to caster	*/
	char				* die_vict;		/* Victim death msg to victim	*/
	char				* die_room;		/* Victim death msg to room	*/
	char				* imm_char;		/* Victim immune msg to caster	*/
	char				* imm_vict;		/* Victim immune msg to victim	*/
	char				* imm_room;		/* Victim immune msg to room	*/
	char				* dice;			/* Dice roll			*/
	int					value;			/* Misc value			*/
	int					saves;			/* What saving spell applies	*/
	int					difficulty;		/* Difficulty of casting/learning */
	SMAUG_AFF			* affects;		/* Spell affects, if any	*/
	char				* components;		/* Spell components, if any	*/
	char				* teachers;		/* Skill requires a special teacher */
	int					participants;	/* # of required participants	*/
	struct	timerset	userec;			/* Usage record			*/
	int					alignment;		/* for jedi powers */
	int					forcetype;		/* Trog: rodzaj mocy (none, neutral, jedi, sith) */
};

#endif //SKILL_TYPE_H_

