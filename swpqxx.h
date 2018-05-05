/*
 * swpqxx.h
 *
 *  Created on: 2009-09-06
 *      Author: Trog
 */

#ifndef SWPQXX_H_
#define SWPQXX_H_

namespace pqxx
{
template<> void from_string(const char *str, SWInt &i)
{
	i.setValue(SWInt::fromString(str));
}
template<> void from_string(const char *str, SWString &s)
{
	s = SWString(str);
}
}

#endif /* SWPQXX_H_ */
