/*
 * SWPlayerData.h
 *
 *  Created on: 2010-08-05
 *      Author: Trog
 */

#ifndef SWPLAYERDATA_H_
#define SWPLAYERDATA_H_
#include <SWInt64.h>
#include <SWDbEntity.h>
#include <SWTimeStamp.h>
#include "../../flags.h"
#include <SWBestow.h>
#include <SWForbidden.h>
#include <SWHome.h>
#include <vector>
#include <SWDbEntityDataMappingProxy.h>

namespace player
{

class SWPlayerData: public SWDbEntity
{
	static const SWInt stBarMaxElements;

	SWString name;
	SWTimeStamp created;
	SWTimeStamp lastPlayed;
	SWInt played;
	SWString site;
	SWInt wizInvis;
	SWString email;
	SWInt trust;
	SWInt64 bank;
	SWString title;
	SWInt security;
	SWInt64 act;
	vector<SWInt> stBar;
	SWInt64 outcastTime;
	SWInt64 restoreTime;
	SWString password;
	SWString bamfin;
	SWString bamfout;
	SWString rank;
	SWString authedBy;
	SWTimeStamp backedUp;
	bool active;
	list<SWBestow> bestowments;
	list<SWForbidden> forbiddenCmds;
	list<SWHome> *homes;

	SWDbEntityDataMapping getDataMapping() const;
	bool loadRelated(SWDataBase *db);

public:
	SWPlayerData() :
		stBar(vector<SWInt> (stBarMaxElements)), active(true), homes(0)
	{
	}

	~SWPlayerData();

	const SWString getTableName() const
	{
		return "pdata";
	}

	const SWInt64 &getAct() const
	{
		return act;
	}

	bool getActive() const
	{
		return active;
	}

	const SWString &getAuthedBy() const
	{
		return authedBy;
	}

	const SWTimeStamp &getBackedUp() const
	{
		return backedUp;
	}

	const SWString &getBamfin() const
	{
		return bamfin;
	}

	const SWString &getBamfout() const
	{
		return bamfout;
	}

	const SWInt64 &getBank() const
	{
		return bank;
	}

	const SWString &getEmail() const
	{
		return email;
	}

	const SWTimeStamp &getCreated() const
	{
		return created;
	}

	const SWTimeStamp &getLastPlayed() const
	{
		return lastPlayed;
	}

	const SWString &getName() const
	{
		return name;
	}

	const SWInt64 &getOutcastTime() const
	{
		return outcastTime;
	}

	const SWString &getPassword() const
	{
		return password;
	}

	const SWInt &getPlayed() const
	{
		return played;
	}

	const SWString &getRank() const
	{
		return rank;
	}

	const SWInt64 &getRestoreTime() const
	{
		return restoreTime;
	}

	const SWInt &getSecurity() const
	{
		return security;
	}

	const SWString &getSite() const
	{
		return site;
	}

	const vector<SWInt> &getStBar() const
	{
		return stBar;
	}

	const SWString &getTitle() const
	{
		return title;
	}

	const SWInt &getTrust() const
	{
		return trust;
	}

	const SWInt &getWizInvis() const
	{
		return wizInvis;
	}

	void setAct(const SWInt64 &act)
	{
		this->act = act;
	}

	void setActive(bool active)
	{
		this->active = active;
	}

	void setAuthedBy(const SWString &authedBy)
	{
		this->authedBy = authedBy;
	}

	void setBackedUp(const SWTimeStamp &backedUp)
	{
		this->backedUp = backedUp;
	}

	void setBamfin(const SWString& bamfin)
	{
		this->bamfin = bamfin;
	}

	void setBamfout(const SWString &bamfout)
	{
		this->bamfout = bamfout;
	}

	void setBank(const SWInt64 &bank)
	{
		this->bank = bank;
	}

	void setEmail(const SWString &email)
	{
		this->email = email;
	}

	void setCreated(const SWTimeStamp &created)
	{
		this->created = created;
	}

	void setLastPlayed(const SWTimeStamp &lastPlayed)
	{
		this->lastPlayed = lastPlayed;
	}

	void setName(const SWString &name)
	{
		this->name = name;
	}

	void setOutcastTime(const SWInt64 &outcastTime)
	{
		this->outcastTime = outcastTime;
	}

	void setPassword(const SWString &password)
	{
		this->password = password;
	}

	void setPlayed(const SWInt &played)
	{
		this->played = played;
	}

	void setRank(const SWString &rank)
	{
		this->rank = rank;
	}

	void setRestoreTime(const SWInt64 &restoreTime)
	{
		this->restoreTime = restoreTime;
	}

	void setSecurity(const SWInt &security)
	{
		this->security = security;
	}

	void setSite(const SWString &site)
	{
		this->site = site;
	}

	void setStBar(const vector<SWInt> &stBar)
	{
		this->stBar = stBar;
	}

	void setStBar(const SWInt &no, const SWInt &what)
	{
		this->stBar[no] = what;
	}

	void setTitle(const SWString &title)
	{
		this->title = title;
	}

	void setTrust(const SWInt &trust)
	{
		this->trust = trust;
	}

	void setWizInvis(const SWInt &wizInvis)
	{
		this->wizInvis = wizInvis;
	}

	const list<SWBestow> &getBestowments() const
	{
		return bestowments;
	}

	const list<SWForbidden> &getForbiddenCmds() const
	{
		return forbiddenCmds;
	}

	void setBestowments(const list<SWBestow> &bestowments)
	{
		this->bestowments = bestowments;
	}

	void addBestowment(const SWString &bestowment)
	{
		this->bestowments.push_back(SWBestow(bestowment));
	}

	void removeBestowment(const SWString &bestowment);

	void setForbiddenCmds(const list<SWForbidden> &forbiddenCmds)
	{
		this->forbiddenCmds = forbiddenCmds;
	}

	void addForbiddenCmd(const SWString &forbiddenCmd)
	{
		this->forbiddenCmds.push_back(SWForbidden(forbiddenCmd));
	}

	void removeForbiddenCmd(const SWString &forbiddenCmd);

	const list<SWHome> *getHomes() const
	{
		return homes;
	}

	void setHomes(list<SWHome> *homes)
	{
		this->homes = homes;
	}

	void addHome(const SWInt64 &vnum)
	{
		if (!homes)
		{
			homes = new list<SWHome> ();
		}
		homes->push_back(SWHome(vnum, getId()));
	}

	void removeHome(const SWInt64 &vnum);

	virtual SWString getInsertQuery() const;
	virtual SWString getUpdateQuery() const;

	static SWPlayerData *loadPlayer(const SWString &name, const SWString &password);
};

}

#endif /* SWPLAYERDATA_H_ */
