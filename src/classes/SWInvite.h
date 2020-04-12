#ifndef INVITE_H_
#define INVITE_H_
#include "SWString.h"
#include "../structs/room_index_data.h"
#include "../structs/typedefs.h"

class SWInvite 
{
	public:
	const ROOM_INDEX_DATA * const home;
	const SWString name;

	SWInvite(const ROOM_INDEX_DATA * home, const SWString &name) 
			: home(home), name(name)
	{
	}
};

#endif /*INVITE_H_*/
