/*
 * SWPlayerCharacter.h
 *
 *  Created on: 2010-08-07
 *      Author: Trog
 */

#ifndef SWPLAYERCHARACTER_H_
#define SWPLAYERCHARACTER_H_
#include <SWPlayerData.h>
#include <SWLanguageSkill.h>
#include <SWAbility.h>
#include <SWSkill.h>
#include <SWAffect.h>
#include <SWFriend.h>
#include <SWForceEvent.h>
#include <SWQuest.h>
#include <SWKillTrack.h>
#include <SWItemPlayer.h>
#include <SWDbEntityDataMapping.h>

namespace player
{

class SWFake;

class SWPlayerCharacter: public SWDbEntity, public SWOwnerEntityId
{
public:
	static const SWInt inflectMaxElements;

private:
	static const char *table_name;
	static const SWInt addictionMaxElements;
	static const SWInt drugLevelMaxElements;
	static const SWInt conditionMaxElements;
	static const SWInt abilityMaxElements;
	static const SWInt killtrackMaxElements;

	SWPlayerData *playerData;
	vector<SWString> inflect;
	SWString longDescr;
	SWString description;
	SWInt mainAbility;
	SWLanguageSkill *speaking;
	SWInt topLevel;
	SWInt practice;
	SWInt room;
	SWInt hit;
	SWInt maxHit;
	SWInt move;
	SWInt maxMove;
	SWInt mana;
	SWInt maxMana;
	SWInt credits;
	SWInt64 affectedBy;
	SWFake *fake;
	SWInt position;
	SWInt savingPoisonDeath;
	SWInt savingWand;
	SWInt savingParaPetri;
	SWInt savingBreath;
	SWInt savingSpellStaff;
	SWInt alignment;
	SWInt quest_points;
	SWInt hitRoll;
	SWInt damRoll;
	SWInt armor;
	SWInt wimpy;
	SWInt64 resistant;
	SWInt64 immune;
	SWInt64 susceptible;
	SWInt mentalState;
	SWString bio;
	vector<SWInt> addiction;
	vector<SWInt> drugLevel;
	SWInt permStr;
	SWInt permInt;
	SWInt permWis;
	SWInt permDex;
	SWInt permCon;
	SWInt permCha;
	SWInt permLck;
	SWInt permFrc;
	SWInt modStr;
	SWInt modInt;
	SWInt modWis;
	SWInt modDex;
	SWInt modCon;
	SWInt modCha;
	SWInt modLck;
	SWInt modFrc;
	vector<SWInt> condition;
	SWTimeStamp *cloned;
	vector<SWAbility> ability;
	list<SWSkill> skills;
	list<SWAffect *> affects;
	list<SWFriend> friends;
	list<SWForceEvent> *forceEvents;
	list<SWQuest> questsDone;
	list<SWKillTrack> killTrack;
	list<SWItemPlayer *> items;

	SWDbEntityDataMapping getDataMapping() const;
	bool loadRelated(SWDataBase *db);

public:
	SWPlayerCharacter() :
		inflect(vector<SWString> (inflectMaxElements)), speaking(0), fake(0), addiction(vector<SWInt> (
				addictionMaxElements)), drugLevel(vector<SWInt> (drugLevelMaxElements)), condition(vector<SWInt> (
				conditionMaxElements)), cloned(0), ability(vector<SWAbility> (abilityMaxElements)), forceEvents(0)
	{
	}

	SWPlayerCharacter(SWPlayerData *playerData) :
		SWOwnerEntityId(playerData->getId()), playerData(playerData), inflect(vector<SWString> (inflectMaxElements)),
				speaking(0), fake(0), addiction(vector<SWInt> (addictionMaxElements)), drugLevel(vector<SWInt> (
						drugLevelMaxElements)), condition(vector<SWInt> (conditionMaxElements)), cloned(0), ability(
						vector<SWAbility> (abilityMaxElements)), forceEvents(0)
	{
	}

	virtual ~SWPlayerCharacter();

	const SWString getOwnerEntityFKey() const
	{
		return "pdata_id";
	}

	const SWString getTableName() const
	{
		return "pchar";
	}

	const vector<SWInt> &getAddiction() const
	{
		return addiction;
	}

	const SWInt64 &getAffectedBy() const
	{
		return affectedBy;
	}

	const SWInt &getAlignment() const
	{
		return alignment;
	}

	const SWInt &getArmor() const
	{
		return armor;
	}

	const SWString &getBio() const
	{
		return bio;
	}

	const SWTimeStamp *getCloned() const
	{
		return cloned;
	}

	const vector<SWInt> &getCondition() const
	{
		return condition;
	}

	const SWInt &getCredits() const
	{
		return credits;
	}

	const SWInt &getDamRoll() const
	{
		return damRoll;
	}

	const SWString &getDescription() const
	{
		return description;
	}

	const vector<SWInt> &getDrugLevel() const
	{
		return drugLevel;
	}

	const SWInt &getHit() const
	{
		return hit;
	}

	const SWInt &getHitRoll() const
	{
		return hitRoll;
	}

	const SWInt64 &getImmune() const
	{
		return immune;
	}

	const vector<SWString> &getInflect() const
	{
		return inflect;
	}

	const SWString &getLongDescr() const
	{
		return longDescr;
	}

	const SWInt &getMainAbility() const
	{
		return mainAbility;
	}

	const SWInt &getMana() const
	{
		return mana;
	}

	const SWInt &getMaxHit() const
	{
		return maxHit;
	}

	const SWInt &getMaxMana() const
	{
		return maxMana;
	}

	const SWInt &getMaxMove() const
	{
		return maxMove;
	}

	const SWInt &getMentalState() const
	{
		return mentalState;
	}

	const SWInt &getModCha() const
	{
		return modCha;
	}

	const SWInt &getModCon() const
	{
		return modCon;
	}

	const SWInt &getModDex() const
	{
		return modDex;
	}

	const SWInt &getModFrc() const
	{
		return modFrc;
	}

	const SWInt &getModInt() const
	{
		return modInt;
	}

	const SWInt &getModLck() const
	{
		return modLck;
	}

	const SWInt &getModStr() const
	{
		return modStr;
	}

	const SWInt &getModWis() const
	{
		return modWis;
	}

	const SWInt &getMove() const
	{
		return move;
	}

	const SWInt &getPermCha() const
	{
		return permCha;
	}

	const SWInt &getPermCon() const
	{
		return permCon;
	}

	const SWInt &getPermDex() const
	{
		return permDex;
	}

	const SWInt &getPermFrc() const
	{
		return permFrc;
	}

	const SWInt &getPermInt() const
	{
		return permInt;
	}

	const SWInt &getPermLck() const
	{
		return permLck;
	}

	const SWInt &getPermStr() const
	{
		return permStr;
	}

	const SWInt &getPermWis() const
	{
		return permWis;
	}

	SWPlayerData *getPlayerData() const
	{
		return playerData;
	}

	const SWInt &getPosition() const
	{
		return position;
	}

	const SWInt &getPractice() const
	{
		return practice;
	}

	const SWInt &getQuestPoints() const
	{
		return quest_points;
	}

	const SWInt64 &getResistant() const
	{
		return resistant;
	}

	const SWInt &getRoom() const
	{
		return room;
	}

	const SWInt &getSavingBreath() const
	{
		return savingBreath;
	}

	const SWInt &getSavingParaPetri() const
	{
		return savingParaPetri;
	}

	const SWInt &getSavingPoisonDeath() const
	{
		return savingPoisonDeath;
	}

	const SWInt &getSavingSpellStaff() const
	{
		return savingSpellStaff;
	}

	const SWInt &getSavingWand() const
	{
		return savingWand;
	}

	SWLanguageSkill *getSpeaking() const
	{
		return speaking;
	}

	const SWInt64 &getSusceptible() const
	{
		return susceptible;
	}

	const SWInt &getTopLevel() const
	{
		return topLevel;
	}

	const SWInt &getWimpy() const
	{
		return wimpy;
	}

	const vector<SWAbility> &getAbility() const
	{
		return ability;
	}

	const list<SWSkill> &getSkills() const
	{
		return skills;
	}

	const list<SWAffect *> &getAffects() const
	{
		return affects;
	}

	const list<SWFriend> &getFriends() const
	{
		return friends;
	}

	const list<SWForceEvent> * const getForceEvents() const
	{
		return forceEvents;
	}

	const list<SWQuest> &getQuestsDone() const
	{
		return questsDone;
	}

	const list<SWKillTrack> &getKillTrack() const
	{
		return killTrack;
	}

	void setAddiction(const vector<SWInt> &addiction)
	{
		this->addiction = addiction;
	}

	void setAffectedBy(const SWInt64 &affectedBy)
	{
		this->affectedBy = affectedBy;
	}

	void setAlignment(const SWInt &alignment)
	{
		this->alignment = alignment;
	}

	void setArmor(const SWInt &armor)
	{
		this->armor = armor;
	}

	void setBio(const SWString &bio)
	{
		this->bio = bio;
	}

	void setCloned(SWTimeStamp * const cloned)
	{
		this->cloned = cloned;
	}

	void setCloned(const SWTimeStamp &cloned)
	{
		this->cloned = new SWTimeStamp(cloned);
	}

	void setCondition(const vector<SWInt> &condition)
	{
		this->condition = condition;
	}

	void setCredits(const SWInt &credits)
	{
		this->credits = credits;
	}

	void setDamRoll(const SWInt &damRoll)
	{
		this->damRoll = damRoll;
	}

	void setDescription(const SWString &description)
	{
		this->description = description;
	}

	void setDrugLevel(const vector<SWInt> &drugLevel)
	{
		this->drugLevel = drugLevel;
	}

	void setHit(const SWInt &hit)
	{
		this->hit = hit;
	}

	void setHitRoll(const SWInt &hitRoll)
	{
		this->hitRoll = hitRoll;
	}

	void setImmune(const SWInt64 &immune)
	{
		this->immune = immune;
	}

	void setInflect(const vector<SWString> &inflect)
	{
		this->inflect = inflect;
	}

	void setLongDescr(const SWString &longDescr)
	{
		this->longDescr = longDescr;
	}

	void setMainAbility(const SWInt &mainAbility)
	{
		this->mainAbility = mainAbility;
	}

	void setMana(const SWInt &mana)
	{
		this->mana = mana;
	}

	void setMaxHit(const SWInt &maxHit)
	{
		this->maxHit = maxHit;
	}

	void setMaxMana(const SWInt &maxMana)
	{
		this->maxMana = maxMana;
	}

	void setMaxMove(const SWInt &maxMove)
	{
		this->maxMove = maxMove;
	}

	void setMentalState(const SWInt &mentalState)
	{
		this->mentalState = mentalState;
	}

	void setModCha(const SWInt &modCha)
	{
		this->modCha = modCha;
	}

	void setModCon(const SWInt &modCon)
	{
		this->modCon = modCon;
	}

	void setModDex(const SWInt &modDex)
	{
		this->modDex = modDex;
	}

	void setModFrc(const SWInt &modFrc)
	{
		this->modFrc = modFrc;
	}

	void setModInt(const SWInt &modInt)
	{
		this->modInt = modInt;
	}

	void setModLck(const SWInt &modLck)
	{
		this->modLck = modLck;
	}

	void setModStr(const SWInt &modStr)
	{
		this->modStr = modStr;
	}

	void setModWis(const SWInt &modWis)
	{
		this->modWis = modWis;
	}

	void setMove(const SWInt &move)
	{
		this->move = move;
	}

	void setPermCha(const SWInt &permCha)
	{
		this->permCha = permCha;
	}

	void setPermCon(const SWInt &permCon)
	{
		this->permCon = permCon;
	}

	void setPermDex(const SWInt &permDex)
	{
		this->permDex = permDex;
	}

	void setPermFrc(const SWInt &permFrc)
	{
		this->permFrc = permFrc;
	}

	void setPermInt(const SWInt &permInt)
	{
		this->permInt = permInt;
	}

	void setPermLck(const SWInt &permLck)
	{
		this->permLck = permLck;
	}

	void setPermStr(const SWInt &permStr)
	{
		this->permStr = permStr;
	}

	void setPermWis(const SWInt &permWis)
	{
		this->permWis = permWis;
	}

	void setPlayerData(SWPlayerData *playerData)
	{
		this->playerData = playerData;
	}

	void setPosition(const SWInt &position)
	{
		this->position = position;
	}

	void setPractice(const SWInt &practice)
	{
		this->practice = practice;
	}

	void setQuestPoints(const SWInt &quest_points)
	{
		this->quest_points = quest_points;
	}

	void setResistant(const SWInt64 &resistant)
	{
		this->resistant = resistant;
	}

	void setRoom(const SWInt &room)
	{
		this->room = room;
	}

	void setSavingBreath(const SWInt &savingBreath)
	{
		this->savingBreath = savingBreath;
	}

	void setSavingParaPetri(const SWInt &savingParaPetri)
	{
		this->savingParaPetri = savingParaPetri;
	}

	void setSavingPoisonDeath(const SWInt &savingPoisonDeath)
	{
		this->savingPoisonDeath = savingPoisonDeath;
	}

	void setSavingSpellStaff(const SWInt &savingSpellStaff)
	{
		this->savingSpellStaff = savingSpellStaff;
	}

	void setSavingWand(const SWInt &savingWand)
	{
		this->savingWand = savingWand;
	}

	void setSpeaking(SWLanguageSkill * const speaking)
	{
		this->speaking = speaking;
	}

	void setSusceptible(const SWInt64 &susceptible)
	{
		this->susceptible = susceptible;
	}

	void setTopLevel(const SWInt &topLevel)
	{
		this->topLevel = topLevel;
	}

	void setWimpy(const SWInt &wimpy)
	{
		this->wimpy = wimpy;
	}

	void setInflect(const SWInt &no, const SWString &inflect)
	{
		this->inflect[no] = inflect;
	}

	void setAddiction(const SWInt &no, const SWInt &addiction)
	{
		this->addiction[no] = addiction;
	}

	void setDrugLevel(const SWInt &no, const SWInt &level)
	{
		this->drugLevel[no] = level;
	}

	void setCondition(const SWInt &no, const SWInt &condition)
	{
		this->condition[no] = condition;
	}

	const SWFake * const getFake() const
	{
		return fake;
	}

	void setFake(SWFake * const fake)
	{
		this->fake = fake;
	}

	void setAbility(const vector<SWAbility> &ability)
	{
		this->ability = ability;
	}

	void setAbility(const SWInt &no, const SWAbility &ability)
	{
		this->ability[no] = ability;
	}

	void setSkills(const list<SWSkill> &skills)
	{
		this->skills = skills;
	}

	void addSkill(const SWSkill &skill)
	{
		skills.push_back(skill);
	}

	void removeSkill(const SWString &name);

	void setAffects(const list<SWAffect *> &affects)
	{
		this->affects = affects;
	}

	void addAffect(SWAffect *affect)
	{
		affects.push_back(affect);
	}

	/* TODO: design & implement
	 void removeAffect(const SWString &name);
	 */

	void setFriends(const list<SWFriend> &friends)
	{
		this->friends = friends;
	}

	void addFriend(const SWFriend &myFriend)
	{
		friends.push_back(myFriend);
	}

	void setForceEvents(list<SWForceEvent> * const forceEvents)
	{
		this->forceEvents = forceEvents;
	}

	void addForceEvent(const SWForceEvent &forceEvent)
	{
		if (!forceEvents)
		{
			forceEvents = new list<SWForceEvent> ();
		}
		this->forceEvents->push_back(forceEvent);
	}

	/* TODO: removeForceEvent */

	void setQuestsDone(const list<SWQuest> &questsDone)
	{
		this->questsDone = questsDone;
	}

	void addQuestDone(const SWQuest &questDone)
	{
		questsDone.push_back(questDone);
	}

	void removeQuestDone(const SWInt64 &questDone);

	void setKillTrack(const list<SWKillTrack> &killTrack)
	{
		this->killTrack = killTrack;
	}

	void addKilled(const SWInt64 &vnum);

	const list<SWItemPlayer *> &getItems() const
	{
		return items;
	}

	void setItems(const list<SWItemPlayer *> &items)
	{
		this->items = items;
	}

	void addItem(SWItemPlayer *item)
	{
		items.push_back(item);
	}

	SWItemPlayer *removeItem(const SWInt64 vnum)
	{
		return dynamic_cast<SWItemPlayer *> (SWItem::removeItem(items, vnum));
	}

	void deleteItem(const SWInt64 vnum)
	{
		delete SWItem::removeItem(items, vnum);
	}

	virtual SWString getInsertQuery() const;
	virtual SWString getUpdateQuery() const;
};

}

#endif /* SWPLAYERCHARACTER_H_ */
