#ifndef SWINT64_H_
#define SWINT64_H_
#include "SWNumber.h"

class SWString;

class SWInt64: public SWNumber<int64>
{
public:
	SWInt64(SWInt64 &x) :
		SWNumber<int64> (x)
	{
	}
	SWInt64(const SWInt64 &x) :
		SWNumber<int64> (x)
	{
	}
	SWInt64() :
		SWNumber<int64> (0)
	{
	}
	SWInt64(int64 x) :
		SWNumber<int64> (x)
	{
	}
	SWInt64(double x) :
		SWNumber<int64> (static_cast<int64> (x))
	{
	}
	SWInt64(float x) :
		SWNumber<int64> (static_cast<int64> (x))
	{
	}
	SWInt64(int x) :
		SWNumber<int64> (static_cast<int64> (x))
	{
	}
	SWInt64(long x) :
		SWNumber<int64> (static_cast<int64> (x))
	{
	}
	SWInt64(unsigned int x) :
		SWNumber<int64> (static_cast<int64> (x))
	{
	}
	SWInt64(unsigned int64 x) :
		SWNumber<int64> (static_cast<int64> (x))
	{
	}

	DECLARE_OPERATORS(int64)

	SWString toString() const;

	static SWString toString(const SWInt64 &);
	static int64 fromString(const SWString &);
};

#endif //SWINT64_H_
