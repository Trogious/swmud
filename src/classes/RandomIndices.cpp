/*
 * RandomIndices.cpp
 *
 *  Created on: 2009-04-04
 *      Author: Trog
 */

#include "RandomIndices.h"
#include <stdlib.h>
#include "../fun_decls.h"

RandomIndices::RandomIndices() {
}

RandomIndices::~RandomIndices() {
}

MOB_INDEX_DATA *RandomIndices::getMobIndex()
{

	MOB_INDEX_DATA *pMobIndex;
	int count = 0;

	for( int key = 0; key < MAX_KEY_HASH; key++ )
		for( pMobIndex  = mob_index_hash[key]; pMobIndex; pMobIndex = pMobIndex->next )
			count++;

	int rnd = number_range( 0, count );

	for( int key = 0, count = 0; key < MAX_KEY_HASH; key++ )
		for( pMobIndex = mob_index_hash[key]; pMobIndex; pMobIndex = pMobIndex->next )
			if( count++ == rnd )
				return pMobIndex;


//	bug( "RandomIndices::getMobIndex: random mob not found" );
	return NULL;
}

ROOM_INDEX_DATA *RandomIndices::getRoomIndex()
{
	ROOM_INDEX_DATA *pRoomIndex;
	int count = 0;

	for( int key = 0; key < MAX_KEY_HASH; key++ )
		for( pRoomIndex  = room_index_hash[key]; pRoomIndex; pRoomIndex = pRoomIndex->next )
			count++;

	int rnd = number_range( 0, count );

	for( int key = 0, count = 0; key < MAX_KEY_HASH; key++ )
		for( pRoomIndex = room_index_hash[key]; pRoomIndex; pRoomIndex = pRoomIndex->next )
			if( count++ == rnd )
				return pRoomIndex;


	return NULL;
}

OBJ_INDEX_DATA *RandomIndices::getObjIndex()
{
	OBJ_INDEX_DATA *pObjIndex;
	int count = 0;

	for( int key = 0; key < MAX_KEY_HASH; key++ )
		for( pObjIndex  = obj_index_hash[key]; pObjIndex; pObjIndex = pObjIndex->next )
			count++;

	int rnd = number_range( 0, count );

	for( int key = 0, count = 0; key < MAX_KEY_HASH; key++ )
		for( pObjIndex = obj_index_hash[key]; pObjIndex; pObjIndex = pObjIndex->next )
			if( count++ == rnd )
				return pObjIndex;


	return NULL;
}
