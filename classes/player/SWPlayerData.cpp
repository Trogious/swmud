/*
 * SWPlayerData.cpp
 *
 *  Created on: 2010-08-05
 *      Author: Trog
 */

#include "SWPlayerData.h"
#include <SWDbArray.h>

namespace player
{

const SWInt SWPlayerData::stBarMaxElements = 3;

SWPlayerData::~SWPlayerData()
{
	if (homes)
	{
		delete homes;
	}
}

#define CREATE_PROXY(methodNamePart)	CREATE_PROXY2(methodNamePart,SWPlayerData)

SWDbEntityDataMapping SWPlayerData::getDataMapping() const
{
	SWDbEntityDataMapping mapping;

	mapping.addMapping("name", getName(), CREATE_PROXY(Name));
	mapping.addMapping("created", getCreated(), CREATE_PROXY(Created));
	mapping.addMapping("last_played", getLastPlayed(), CREATE_PROXY(LastPlayed));
	mapping.addMapping("played", getPlayed(), CREATE_PROXY(Played));
	mapping.addMapping("site", getSite(), CREATE_PROXY(Site));
	mapping.addMapping("wiz_invis", getWizInvis(), CREATE_PROXY(WizInvis));
	mapping.addMapping("email", getEmail(), CREATE_PROXY(Email));
	mapping.addMapping("trust", getTrust(), CREATE_PROXY(Trust));
	mapping.addMapping("bank", getBank(), CREATE_PROXY(Bank));
	mapping.addMapping("title", getTitle(), CREATE_PROXY(Title));
	mapping.addMapping("security", getSecurity(), CREATE_PROXY(Security));
	mapping.addMapping("act", getAct(), CREATE_PROXY(Act));
	mapping.addMapping("st_bar", getStBar(), CREATE_PROXY(StBar));
	mapping.addMapping("outcast_time", getOutcastTime(), CREATE_PROXY(OutcastTime));
	mapping.addMapping("restore_time", getRestoreTime(), CREATE_PROXY(RestoreTime));
	mapping.addPasswordMapping("password", getPassword()); //hasel nie zczytujemy => nie ustawiamy tu proxy
	mapping.addMapping("bamfin", getBamfin(), CREATE_PROXY(Bamfin));
	mapping.addMapping("bamfout", getBamfout(), CREATE_PROXY(Bamfout));
	mapping.addMapping("rank", getRank(), CREATE_PROXY(Rank));
	mapping.addMapping("authed_by", getAuthedBy(), CREATE_PROXY(AuthedBy));
	mapping.addMapping("backed_up", getBackedUp(), CREATE_PROXY(BackedUp));

	return mapping;
}

bool SWPlayerData::loadRelated(SWDataBase *db)
{
	return true;//TODO: implement
}

SWString SWPlayerData::getInsertQuery() const
{
	if (isModified())
	{
		/* podstawowe atrybuty */
		SWString queries = getDataMapping().getInsertQuery(getTableName()) + ";\n\n";

		/* reszta z tabel powiazanych */
		queries.append(SWDbEntity::getInsertQueries(bestowments)).append("\n");
		queries.append(SWDbEntity::getInsertQueries(forbiddenCmds)).append("\n");
		if (homes)
		{
			queries.append(SWDbEntity::getInsertQueries(*homes)).append("\n");
		}

		return queries;
	}
	else
	{
		return "";
	}
}

SWString SWPlayerData::getUpdateQuery() const
{
	if (isModified())
	{
		/* podstawowe atrybuty */
		SWString queries = getDataMapping().getUpdateQuery(getTableName(), getId()) + ";\n\n";

		/* reszta z tabel powiazanych */
		queries.append(SWDbEntity::getUpdateQueries(bestowments)).append("\n");
		queries.append(SWDbEntity::getUpdateQueries(forbiddenCmds)).append("\n");
		if (homes)
		{
			queries.append(SWDbEntity::getUpdateQueries(*homes)).append("\n");
		}

		return queries;
	}
	else
	{
		return "";
	}
}

void SWPlayerData::removeBestowment(const SWString &bestowment)
{
	for (list<SWBestow>::iterator it = bestowments.begin(); it != bestowments.end(); ++it)
	{
		if (it->getCmd() == bestowment)
		{
			bestowments.erase(it);
			break;
		}
	}
}

void SWPlayerData::removeForbiddenCmd(const SWString &forbiddenCmd)
{
	for (list<SWForbidden>::iterator it = forbiddenCmds.begin(); it != forbiddenCmds.end(); ++it)
	{
		if (it->getCmd() == forbiddenCmd)
		{
			forbiddenCmds.erase(it);
			break;
		}
	}
}

void SWPlayerData::removeHome(const SWInt64 &vnum)
{
	if (homes)
	{
		for (list<SWHome>::iterator it = homes->begin(); it != homes->end(); ++it)
		{
			if (it->getVnum() == vnum)
			{
				homes->erase(it);
				break;
			}
		}
		if (homes->size() < 1)
		{
			delete homes;
			homes = 0;
		}
	}
}

SWPlayerData *SWPlayerData::loadPlayer(const SWString &name, const SWString &password)
{
	SWPlayerData *playerData = new SWPlayerData;
	SWString query("SELECT * FROM ");
	query << playerData->getTableName() << " WHERE active='true' AND LOWER(name)=LOWER('" << name << "') AND password="
			<< SWDataBase::preEncodePassword(password) << " ORDER BY backed_up DESC NULLS LAST LIMIT 1";

	SWDataBase db;
	if (playerData->load(query, &db))
	{
		return playerData;
	}

	delete playerData;
	return 0;
}

}
