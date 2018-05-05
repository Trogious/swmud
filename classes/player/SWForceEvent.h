/*
 * SWForceEvent.h
 *
 *  Created on: 2010-07-08
 *      Author: Trog
 */

#ifndef SWFORCEEVENT_H_
#define SWFORCEEVENT_H_
#include "../SWInt64.h"
#include "../SWDbEntity.h"
#include "../SWOwnerEntityId.h"
#include "../../flags.h"
#include <vector>

/**
 * Przestrzen nazw zwiazana z graczem.
 * Przestrzen nazw dla klas zwiazanych z graczem, jego atrybutami oraz wczytywaniem obydwu.
 */
namespace player
{

class SWForceEvent: public SWOwnerEntityId, public SWDbEntity
{
public:
	static const int MAX_ATTRIBUTES = FE_MAX_ATTR;

private:
	SWInt trigger;
	SWString stringAttr;
	vector<SWInt64> attribute;

	SWDbEntityDataMapping getDataMapping() const;

public:
	SWForceEvent()
	{
	}

	SWForceEvent(const SWInt &trigger, const SWString &stringAttr, const vector<SWInt64> &attribute) :
		trigger(trigger), stringAttr(stringAttr)
	{
		setAttributes(attribute);
	}

	SWForceEvent(const SWInt &trigger, const SWString &stringAttr, const vector<SWInt64> &attribute,
			const SWInt &ownerEntityId) :
		SWOwnerEntityId(ownerEntityId), trigger(trigger), stringAttr(stringAttr)
	{
		setAttributes(attribute);
	}

	virtual ~SWForceEvent()
	{
	}

	const SWString getOwnerEntityFKey() const
	{
		return "pchar_id";
	}

	const SWString getTableName() const
	{
		return "fevent_pchar";
	}

	void setAttributes(const vector<SWInt64> &attribute)
	{
		this->attribute = attribute;
	}

	const SWInt64 &getAttribute(const SWInt &i) const
	{
		return attribute[i];
	}

	const SWString &getStringAttr() const
	{
		return stringAttr;
	}

	const SWInt &getTrigger() const
	{
		return trigger;
	}

	void setAttribute(const SWInt &i, const SWInt64 &attr)
	{
		attribute[i] = attr;
	}

	void setStringAttr(const SWString &stringAttr)
	{
		this->stringAttr = stringAttr;
	}

	void setTrigger(const SWInt &trigger)
	{
		this->trigger = trigger;
	}

	virtual SWString getInsertQuery() const;
	virtual SWString getUpdateQuery() const;
};

}

#endif /* SWFORCEEVENT_H_ */
