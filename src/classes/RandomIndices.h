/*
 * RandomIndices.h
 *
 *  Created on: 2009-04-04
 *      Author: Trog
 */

#ifndef RANDOMINDICES_H_
#define RANDOMINDICES_H_
#include "../defines.h"
#include "../typedefs.h"


class RandomIndices {

	RandomIndices();
public:
	virtual ~RandomIndices();

	static MOB_INDEX_DATA *getMobIndex();
	static ROOM_INDEX_DATA *getRoomIndex();
	static OBJ_INDEX_DATA *getObjIndex();
};

#endif /* RANDOMINDICES_H_ */
