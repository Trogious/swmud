/*
 * SWClassLoader.h
 *
 *  Created on: 2009-08-02
 *      Author: Trog
 */

#ifndef SWCLASSLOADER_H_
#define SWCLASSLOADER_H_

#include "SWClassFactory.h"
#include "SWString.h"
#include "SWException.h"

using namespace std;

class SWClassLoader
{
public:
	SWClassLoader()
	{
	}
	virtual ~SWClassLoader()
	{
	}

	static const SWClassFactory *getFactory(const SWString &) throw (SWException);
};

/**
 * Makro for making class dynamic.
 * This macro must be used to make a specific class dynamic.
 * What is more the base of the class you want to be dynamic
 * must inherit from SWDynamicClass. Next, an appropriate entry
 * must be added to dynamic_class_table - a mapping of class names
 * to their factories.
 * \sa SWDynamicClassTable
 */
#define MAKE_DYNAMIC( clazz ) \
class clazz##Factory: public SWClassFactory\
{\
public:\
	SWDynamicClass *createInstance() const\
	{\
		return new clazz;\
	}\
}

#endif /* SWCLASSLOADER_H_ */
