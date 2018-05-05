/*
 * SWHome.h
 *
 *  Created on: 2010-07-15
 *      Author: Trog
 */

#ifndef SWHome_H_
#define SWHome_H_
#include <SWInt64.h>
#include <SWDbEntity.h>
#include <SWOwnerEntityId.h>
#include <SWItemHome.h>
#include <SWContainers.h>
#include <list>

namespace player
{

class SWHome: public SWDbEntity, public SWOwnerEntityId
{
	SWInt64 vnum;
	list<SWItemHome *> items;

	SWDbEntityDataMapping getDataMapping() const;
	bool loadRelated(SWDataBase *db);

public:
	SWHome()
	{
	}

	SWHome(SWInt64 vnum) :
		vnum(vnum)
	{
	}

	SWHome(SWInt64 vnum, const SWInt &ownerEntityId) :
		SWOwnerEntityId(ownerEntityId), vnum(vnum)
	{
	}

	const SWString getOwnerEntityFKey() const
	{
		return "pdata_id";
	}

	const SWString getTableName() const
	{
		return "home_pdata";
	}

	virtual ~SWHome()
	{
		SWContainers::freeContents(items);
	}

	const SWInt64 &getVnum() const
	{
		return vnum;
	}

	void setVnum(const SWInt64 &vnum)
	{
		this->vnum = vnum;
	}

	const list<SWItemHome *> &getItems() const
	{
		return items;
	}

	void setItems(const list<SWItemHome *> &items)
	{
		this->items = items;
	}

	void addItem(SWItemHome *item)
	{
		items.push_back(item);
	}

	SWItemHome *removeItem(const SWInt64 vnum)
	{
		return dynamic_cast<SWItemHome *> (SWItem::removeItem(items, vnum));
	}

	void deleteItem(const SWInt64 vnum)
	{
		delete SWItem::removeItem(items, vnum);
	}

	virtual SWString getInsertQuery() const;
	virtual SWString getUpdateQuery() const;
};

}

#endif /* SWHome_H_ */
