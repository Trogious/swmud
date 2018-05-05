#ifndef SWINT_H_
#define SWINT_H_
#include "SWNumber.h"

class SWString;

class SWInt: public SWNumber<int>
{
public:
	SWInt(SWInt &x) :
		SWNumber<int>(x)
	{
	}
	SWInt(const SWInt &x) :
		SWNumber<int>(x)
	{
	}
	SWInt() :
		SWNumber<int>(0)
	{
	}
	SWInt(int x) :
		SWNumber<int>(x)
	{
	}
	SWInt(double x) :
		SWNumber<int>(static_cast<int> (x))
	{
	}
	SWInt(float x) :
		SWNumber<int>(static_cast<int> (x))
	{
	}
	SWInt(int64 x) :
		SWNumber<int>(static_cast<int> (x))
	{
	}
	SWInt(long x) :
		SWNumber<int>(static_cast<int> (x))
	{
	}
	SWInt(unsigned int x) :
		SWNumber<int>(static_cast<int> (x))
	{
	}
	SWInt(unsigned int64 x) :
		SWNumber<int>(static_cast<int> (x))
	{
	}

	DECLARE_OPERATORS(int)

	SWString toString() const;

	static SWString toString(const SWInt &);
	static int fromString(const SWString &);
};

#endif //SWINT_H_
