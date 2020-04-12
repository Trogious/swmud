/*
 * SWAffect.h
 *
 *  Created on: Jul 9, 2010
 *      Author: Trog
 */

#ifndef SWAFFECT_H_
#define SWAFFECT_H_
#include "../SWInt64.h"
#include "../SWDbEntity.h"
#include "../SWOwnerEntityId.h"
#include "../../flags.h"

namespace player
{

class SWAffect: public SWOwnerEntityId, public SWDbEntity
{
	SWInt type;
	SWInt duration;
	SWInt modifier;
	SWInt location;
	SWInt64 bitvector;

	SWDbEntityDataMapping getDataMapping() const;

public:
	SWAffect()
	{
	}

	SWAffect(const SWInt &type, const SWInt &duration, const SWInt &modifier, const SWInt &location,
			const SWInt64 &bitvector) :
		type(type), duration(duration), modifier(modifier), location(location), bitvector(bitvector)
	{
	}

	SWAffect(const SWInt &type, const SWInt &duration, const SWInt &modifier, const SWInt &location,
			const SWInt64 &bitvector, const SWInt &ownerEntityId) :
		SWOwnerEntityId(ownerEntityId), type(type), duration(duration), modifier(modifier), location(location),
				bitvector(bitvector)
	{
	}

	virtual ~SWAffect() = 0;

	bool isAffectData() const
	{
		return type >= 0 && type < TYPE_PERSONAL;
	}

	const SWInt64 &getBitvector() const
	{
		return bitvector;
	}

	const SWInt &getDuration() const
	{
		return duration;
	}

	const SWInt &getLocation() const
	{
		return location;
	}

	const SWInt &getModifier() const
	{
		return modifier;
	}

	void setBitvector(const SWInt64 &bitvector)
	{
		this->bitvector = bitvector;
	}

	void setDuration(const SWInt &duration)
	{
		this->duration = duration;
	}

	void setLocation(const SWInt &location)
	{
		this->location = location;
	}

	void setModifier(const SWInt &modifier)
	{
		this->modifier = modifier;
	}

	const SWInt &getType() const
	{
		return type;
	}

	void setType(const SWInt &type)
	{
		this->type = type;
	}

	virtual SWString getInsertQuery() const;
	virtual SWString getUpdateQuery() const;
};

}

#endif /* SWAFFECT_H_ */
