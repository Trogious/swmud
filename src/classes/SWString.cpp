/*
 * SWString.cpp
 *
 *  Created on: 2009-09-07
 *      Author: Trog
 */

#include "SWString.h"
#include "SWInt.h"
#include "SWInt64.h"
#include <cctype>
#include <algorithm>
#include <list>

SWString &SWString::makePersonName()
{
	iterator it = begin();
	if (it != end())
	{
		*it = toupper(*it);
		for (++it; it != end(); ++it)
		{
			*it = tolower(*it);
		}
	}
	return *this;

}

SWString &SWString::toLower()
{
	//transform(begin(), end(), begin(), std::ptr_fun(&::tolower));
	transform(begin(), end(), begin(), [](int c){return ::tolower(c);});
	return *this;
}

SWString &SWString::toUpper()
{
	//transform(begin(), end(), begin(), std::ptr_fun(&::toupper));
	transform(begin(), end(), begin(), [](int c){return ::toupper(c);});
	return *this;
}

SWString &SWString::lcFirst()
{
	if (this->length())
	{
		*begin() = tolower(*begin());
	}
	return *this;
}

SWString &SWString::ucFirst()
{
	if (this->length())
	{
		*begin() = toupper(*begin());
	}
	return *this;
}

SWString &SWString::append(const SWInt &i)
{
	return append(i.toString());
}

SWString &SWString::prepend(const SWInt &i)
{
	return prepend(i.toString());
}

SWString &SWString::append(const SWInt64 &i)
{
	return append(i.toString());
}

SWString &SWString::prepend(const SWInt64 &i)
{
	return prepend(i.toString());
}

SWString &SWString::trimLeft(char c)
{
	SWString::iterator it = begin();
	for (; it != end(); ++it)
	{
		if (*it != c)
		{
			erase(begin(), it);
			break;
		}
	}

	return *this;
}

SWString &SWString::trimRight(char c)
{
	SWString::const_reverse_iterator it = rbegin();
	for (int i = 0; it != rend(); ++it, ++i)
	{
		if (*it != c)
		{
			erase(length() - i, i);
			break;
		}
	}

	return *this;
}

vector<SWString> SWString::split(const SWString &separator) const
{
	std::list<SWString> parts;
	size_type foundAt;
	size_type startFrom = 0;
	int ss = separator.size();
	vector<SWString> partVector;

	if (!ss)
	{
		return partVector;
	}
	while ((foundAt = find(separator.c_str(), startFrom)) != npos)
	{
		SWString part;
		std::copy(begin() + startFrom, begin() + foundAt, back_inserter(part));
		parts.push_back(part);
		startFrom = foundAt + ss;
	}
	if (startFrom <= size())
	{
		SWString part;
		std::copy(begin() + startFrom, end(), back_inserter(part));
		parts.push_back(part);
	}

	std::copy(parts.begin(), parts.end(), back_inserter(partVector));

	return partVector;
}

SWString::size_type SWString::findIn(const char *str, const char *s, bool casesensitive)
{
	return casesensitive ? SWString(str).find(s) : SWString(str).toLower().find(SWString(s).toLower());
}

SWString::size_type SWString::findIn(const SWString &str, const char *s, bool casesensitive)
{
	return casesensitive ? str.find(s) : str.asLower().find(SWString(s).toLower());
}
