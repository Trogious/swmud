/*
 * SWPlayerCharacter.cpp
 *
 *  Created on: 2010-08-07
 *      Author: Trog
 */

#include <SWPlayerCharacter.h>
#include <defines.h>
#include <SWForceSkill.h>
#include <SWFake.h>
#include <SWContainers.h>
#include <SWDbArray.h>

namespace player
{

const char *SWPlayerCharacter::table_name = "pchar";
const SWInt SWPlayerCharacter::inflectMaxElements = 6;
const SWInt SWPlayerCharacter::addictionMaxElements = 9;
const SWInt SWPlayerCharacter::drugLevelMaxElements = 9;
const SWInt SWPlayerCharacter::conditionMaxElements = 4;
const SWInt SWPlayerCharacter::abilityMaxElements = MAX_ABILITY;
const SWInt SWPlayerCharacter::killtrackMaxElements = MAX_KILLTRACK;

SWPlayerCharacter::~SWPlayerCharacter()
{
	if (speaking)
	{
		delete speaking;
	}

	if (fake)
	{
		delete fake;
	}

	if (cloned)
	{
		delete cloned;
	}

	if (forceEvents)
	{
		delete forceEvents;
	}

	SWContainers::freeContents(affects);
	SWContainers::freeContents(items);
}

#define CREATE_PROXY(methodNamePart)	CREATE_PROXY2(methodNamePart,SWPlayerCharacter)

SWDbEntityDataMapping SWPlayerCharacter::getDataMapping() const
{
	SWDbEntityDataMapping mapping;

	mapping.addMapping(getOwnerEntityFKey(), getOwnerEntityId(), OWNER_PROXY);
	mapping.addMapping("inflect", getInflect(), CREATE_PROXY(Inflect));
	mapping.addMapping("long_descr", getLongDescr(), CREATE_PROXY(LongDescr));
	mapping.addMapping("description", getDescription(), CREATE_PROXY(Description));
	mapping.addMapping("main_ability", getMainAbility(), CREATE_PROXY(MainAbility));
	mapping.addMapping("top_level", getTopLevel(), CREATE_PROXY(TopLevel));
	mapping.addMapping("practice", getPractice(), CREATE_PROXY(Practice));
	mapping.addMapping("room", getRoom(), CREATE_PROXY(Room));
	mapping.addMapping("hit", getHit(), CREATE_PROXY(Hit));
	mapping.addMapping("max_hit", getMaxHit(), CREATE_PROXY(MaxHit));
	mapping.addMapping("\"move\"", getMove(), CREATE_PROXY(Move));
	mapping.addMapping("max_move", getMaxMove(), CREATE_PROXY(MaxMove));
	mapping.addMapping("perm_frc", getPermFrc(), CREATE_PROXY(PermFrc));
	mapping.addMapping("mod_frc", getModFrc(), CREATE_PROXY(ModFrc));
	mapping.addMapping("mana", getMana(), CREATE_PROXY(Mana));
	mapping.addMapping("max_mana", getMaxMana(), CREATE_PROXY(MaxMana));
	mapping.addMapping("credits", getCredits(), CREATE_PROXY(Credits));
	mapping.addMapping("affected_by", getAffectedBy(), CREATE_PROXY(AffectedBy));
	mapping.addMapping("\"position\"", getPosition(), CREATE_PROXY(Position));
	mapping.addMapping("saving_poison_death", getSavingPoisonDeath(), CREATE_PROXY(SavingPoisonDeath));
	mapping.addMapping("saving_wand", getSavingWand(), CREATE_PROXY(SavingWand));
	mapping.addMapping("saving_para_petri", getSavingParaPetri(), CREATE_PROXY(SavingParaPetri));
	mapping.addMapping("saving_breath", getSavingBreath(), CREATE_PROXY(SavingBreath));
	mapping.addMapping("saving_spell_staff", getSavingSpellStaff(), CREATE_PROXY(SavingSpellStaff));
	mapping.addMapping("alignment", getAlignment(), CREATE_PROXY(Alignment));
	mapping.addMapping("quest_points", getQuestPoints(), CREATE_PROXY(QuestPoints));
	mapping.addMapping("hitroll", getHitRoll(), CREATE_PROXY(HitRoll));
	mapping.addMapping("damroll", getDamRoll(), CREATE_PROXY(DamRoll));
	mapping.addMapping("armor", getArmor(), CREATE_PROXY(Armor));
	mapping.addMapping("wimpy", getWimpy(), CREATE_PROXY(Wimpy));
	mapping.addMapping("resistant", getResistant(), CREATE_PROXY(Resistant));
	mapping.addMapping("immune", getImmune(), CREATE_PROXY(Immune));
	mapping.addMapping("susceptible", getSusceptible(), CREATE_PROXY(Susceptible));
	mapping.addMapping("mental_state", getMentalState(), CREATE_PROXY(MentalState));
	mapping.addMapping("bio", getBio(), CREATE_PROXY(Bio));
	mapping.addMapping("addiction", getAddiction(), CREATE_PROXY(Addiction));
	mapping.addMapping("drug_level", getDrugLevel(), CREATE_PROXY(DrugLevel));
	mapping.addMapping("perm_str", getPermStr(), CREATE_PROXY(PermStr));
	mapping.addMapping("perm_int", getPermInt(), CREATE_PROXY(PermInt));
	mapping.addMapping("perm_wis", getPermWis(), CREATE_PROXY(PermWis));
	mapping.addMapping("perm_dex", getPermDex(), CREATE_PROXY(PermDex));
	mapping.addMapping("perm_con", getPermCon(), CREATE_PROXY(PermCon));
	mapping.addMapping("perm_cha", getPermCha(), CREATE_PROXY(PermCha));
	mapping.addMapping("perm_lck", getPermLck(), CREATE_PROXY(PermLck));
	mapping.addMapping("mod_str", getModStr(), CREATE_PROXY(ModStr));
	mapping.addMapping("mod_int", getModInt(), CREATE_PROXY(ModInt));
	mapping.addMapping("mod_wis", getModWis(), CREATE_PROXY(ModWis));
	mapping.addMapping("mod_dex", getModDex(), CREATE_PROXY(ModDex));
	mapping.addMapping("mod_con", getModCon(), CREATE_PROXY(ModCon));
	mapping.addMapping("mod_cha", getModCha(), CREATE_PROXY(ModCha));
	mapping.addMapping("mod_lck", getModLck(), CREATE_PROXY(ModLck));
	mapping.addMapping("condition", getCondition(), CREATE_PROXY(Condition));
	cloned ? mapping.addMapping("cloned", *getCloned(), CREATE_PROXY(Cloned)) : mapping.addMapping("cloned",
			CREATE_PROXY(Cloned));

	return mapping;
}

bool SWPlayerCharacter::loadRelated(SWDataBase *db)
{
	/*	fake ? mapping.addMapping("fake_id", getFake()->getId(), CREATE_PROXY(Fake)) : mapping.addMapping("fake_id",
	 CREATE_PROXY(Fake)); TODO: -> related */
	//	mapping.addMapping("speaking", getSpeaking()->getId(), CREATE_PROXY(Speaking)); TODO: -> related
	return true;//TODO: implement
}

SWString SWPlayerCharacter::getInsertQuery() const
{
	if (isModified())
	{
		/* podstawowe atrybuty */
		SWString queries = getDataMapping().getInsertQuery(getTableName()) + ";\n\n";

		/* reszta z tabel powiazanych */
		queries.append(SWDbEntity::getInsertQueries(ability)).append("\n");
		queries.append(SWDbEntity::getInsertQueries(skills)).append("\n");
		queries.append(SWDbEntity::getInsertQueriesPtr(affects)).append("\n");
		queries.append(SWDbEntity::getInsertQueries(friends)).append("\n");
		if (forceEvents)
		{
			queries.append(SWDbEntity::getInsertQueries(*forceEvents)).append("\n");
		}
		queries.append(SWDbEntity::getInsertQueries(questsDone)).append("\n");
		queries.append(SWDbEntity::getInsertQueries(killTrack)).append("\n");
		queries.append(SWDbEntity::getInsertQueriesPtr(items)).append("\n");

		return queries;
	}
	else
	{
		return "";
	}
}

SWString SWPlayerCharacter::getUpdateQuery() const
{
	if (isModified())
	{
		/* podstawowe atrybuty */
		SWString queries = getDataMapping().getUpdateQuery(getTableName(), getId()) + ";\n\n";

		/* reszta z tabel powiazanych */
		queries.append(SWDbEntity::getUpdateQueries(ability)).append("\n");
		queries.append(SWDbEntity::getUpdateQueries(skills)).append("\n");
		queries.append(SWDbEntity::getUpdateQueriesPtr(affects)).append("\n");
		queries.append(SWDbEntity::getUpdateQueries(friends)).append("\n");
		if (forceEvents)
		{
			queries.append(SWDbEntity::getUpdateQueries(*forceEvents)).append("\n");
		}
		queries.append(SWDbEntity::getUpdateQueries(questsDone)).append("\n");
		queries.append(SWDbEntity::getUpdateQueries(killTrack)).append("\n");
		queries.append(SWDbEntity::getUpdateQueriesPtr(items)).append("\n");

		return queries;
	}
	else
	{
		return "";
	}
}

void SWPlayerCharacter::removeSkill(const SWString &name)
{
	for (list<SWSkill>::iterator it = skills.begin(); it != skills.end(); ++it)
	{
		if (it->getName() == name)
		{
			skills.erase(it);
			break;
		}
	}
}

void SWPlayerCharacter::removeQuestDone(const SWInt64 &questDone)
{
	for (list<SWQuest>::iterator it = questsDone.begin(); it != questsDone.end(); ++it)
	{
		if (it->getQuestId() == questDone)
		{
			questsDone.erase(it);
			break;
		}
	}
}

void SWPlayerCharacter::addKilled(const SWInt64 &vnum)
{
	for (list<SWKillTrack>::iterator it = killTrack.begin(); it != killTrack.end(); ++it)
	{
		if (it->getVnum() == vnum)
		{
			++(*it);
			return;
		}
	}

	if (static_cast<SWInt> ((int) killTrack.size()) == killtrackMaxElements)
	{
		killTrack.pop_front();
		killTrack.push_back(SWKillTrack(vnum, 1, getId()));
	}
}

}
