/*
 * SWDynamicClassTable.h
 *
 *  Created on: 2009-08-02
 *      Author: Trog
 */

#ifndef SWDYNAMICCLASSTABLE_H_
#define SWDYNAMICCLASSTABLE_H_

/* include here all classes you want to support dynamic loading */
#include "SWPlayerBounty.h"
#include "SWMobBounty.h"

class SWDynamicClassTable
{
public:
	const SWString name;
	const SWClassFactory *factory;
};

/* add here mappings to dynamic classes */
const SWDynamicClassTable dynamic_class_table[] =
{
		{ "SWMobBounty", new SWMobBountyFactory },
		{ "SWPlayerBounty", new SWPlayerBountyFactory },
		{ "", 0 }
};

#endif /* SWDYNAMICCLASSTABLE_H_ */
