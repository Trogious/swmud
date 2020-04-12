/*
 * SWString.h
 *
 *  Created on: 2009-09-07
 *      Author: Trog
 */

#ifndef SWSTRING_H_
#define SWSTRING_H_
#include <string>
#include <vector>

class SWInt;
class SWInt64;

using std::string;
using std::vector;

class SWString: public string
{
public:
	SWString() :
		string()
	{
	}

	SWString(const string& s, size_type pos = 0, size_type n = npos) :
		string(s, pos, n)
	{
	}

	SWString(const SWString& s, size_type pos = 0, size_type n = npos) :
		string(s, pos, n)
	{
	}

	SWString(const char *s) :
		string(s)
	{
	}

	SWString(const char *s, size_type n) :
		string(s, n)
	{
	}

	SWString(size_type n, char c) :
		string(n, c)
	{
	}

	template<class InputIterator>
	SWString(InputIterator first, InputIterator last) :
		string(first, last)
	{
	}

	SWString asLower() const
	{
		return SWString(*this).toLower();
	}

	SWString asUpper() const
	{
		return SWString(*this).toUpper();
	}

	SWString &append(const SWString &str)
	{
		return static_cast<SWString&> (string::append(str));
	}

	SWString &prepend(const SWString &str)
	{
		return static_cast<SWString&> (insert(0, str));
	}

	SWString &trim(char c = ' ')
	{
		return trimLeft(c).trimRight(c);
	}

	SWString &operator<<(const SWString &str)
	{
		return append(str);
	}

	SWString &operator<<(const SWInt &i)
	{
		return append(i);
	}

	SWString &operator<<(const SWInt64 &i)
	{
		return append(i);
	}

	SWString &makePersonName();

	SWString &toLower();

	SWString &toUpper();

	SWString &ucFirst();

	SWString &lcFirst();

	SWString &append(const SWInt &i);

	SWString &prepend(const SWInt &i);

	SWString &append(const SWInt64 &i);

	SWString &prepend(const SWInt64 &i);

	SWString &trimLeft(char c = ' ');

	SWString &trimRight(char c = ' ');

	vector<SWString> split(const SWString &separator) const;

	template<class T>
	static SWString join(const T &container, const SWString &separator);

	static size_type findIn(const char *str, char *s, bool casesensitive = true)
	{
		return findIn(str, (const char *) s, casesensitive);
	}

	static size_type findIn(char *str, const char *s, bool casesensitive = true)
	{
		return findIn((const char *) str, s, casesensitive);
	}

	static size_type findIn(char *str, char *s, bool casesensitive = true)
	{
		return findIn((const char *) str, (const char *) s, casesensitive);
	}

	static size_type findIn(const SWString &str, char *s, bool casesensitive = true)
	{
		return findIn(str, (const char *) s, casesensitive);
	}

	static size_type findIn(SWString &str, const char *s, bool casesensitive = true)
	{
		return findIn(const_cast<const SWString&> (str), s, casesensitive);
	}

	static size_type findIn(SWString &str, char *s, bool casesensitive = true)
	{
		return findIn(const_cast<const SWString &> (str), (const char *) s, casesensitive);
	}

	static SWString fromString(const SWString &str)
	{
		return str;
	}

	static size_type findIn(const char *, const char *, bool = true);
	static size_type findIn(const SWString &, const char *, bool = true);
};

template<class T>
SWString SWString::join(const T &container, const SWString &separator)
{
	SWString joined;
	typename T::const_iterator it = container.begin();
	if (it != container.end())
	{
		joined << *it;
		for (++it; it != container.end(); ++it)
		{
			joined << separator << *it;
		}
	}

	return joined;
}

#endif /* SWSTRING_H_ */
