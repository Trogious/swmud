/*
 * SWClassLoader.cpp
 *
 *  Created on: 2009-08-02
 *      Author: Trog
 */

#include "SWClassLoader.h"
#include "SWDynamicClassTable.h"
#include "SWException.h"

const SWClassFactory *SWClassLoader::getFactory(const SWString &name) throw(SWException)
{
	for( int i = 0; dynamic_class_table[i].factory; i++ )
	{
		if( !dynamic_class_table[i].name.compare(name) )
		{
			return dynamic_class_table[i].factory;
		}
	}

	throw new SWException((SWString("SWClassLoader cannot find class: ") + name).append(" to load"));
}
