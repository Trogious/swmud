/*
 * SWContainers.h
 *
 *  Created on: 2010-09-08
 *      Author: Trog
 */

#ifndef SWCONTAINERS_H_
#define SWCONTAINERS_H_

class SWContainers
{
public:
	template<class T>
	static void freeContents(T &container)
	{
		for (typename T::iterator it = container.begin(); it != container.end(); ++it)
		{
			delete *it;
		}
	}

	template<class T>
	static void freeSparseContents(T &container);

	template<class T, class P>
	static void freeSparseContents(T &container, const P &predicate);
};

template<class T>
void SWContainers::freeSparseContents(T &container)
{
	for (typename T::iterator it = container.begin(); it != container.end(); ++it)
	{
		if (*it)
		{
			delete *it;
		}
	}
}

template<class T, class P>
void SWContainers::freeSparseContents(T &container, const P &predicate)
{
	for (typename T::iterator it = container.begin(); it != container.end(); ++it)
	{
		if (predicate(*it))
		{
			delete predicate(*it);
		}
	}
}

#endif /* SWCONTAINERS_H_ */
