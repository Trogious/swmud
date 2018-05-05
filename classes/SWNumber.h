/*
 * SWNumber.h
 *
 *  Created on: 2010-08-07
 *      Author: Trog
 */

#ifndef SWNUMBER_H_
#define SWNUMBER_H_

#ifndef int64
#define int64 long long
#endif

#define DECLARE_OP2( op, T )\
T operator op ( T &t )\
{\
	return value op t;\
}\
\
T operator op ( const T &t )\
{\
	return value op t;\
}

#define DECLARE_OP1( op, T )\
T operator op ()\
{\
	return op value;\
}\
\
T operator op ( int )\
{\
	return value op;\
}

#define DECLARE_OPL( op, T )\
bool operator op ( T &t )\
{\
	return value op t;\
}\
\
bool operator op ( const T &t )\
{\
	return value op t;\
}

#define DECLARE_OP1N( op, T )\
T operator op ()\
{\
	return op value;\
}

#define DECLARE_OP1NM( op, T )\
T operator op ()\
{\
	return value;\
}

#define DECLARE_OPC( T )\
operator T ()\
{\
	return static_cast<T>(value);\
}\
operator T () const\
{\
	return static_cast<T>(value);\
}\

#define DECLARE_OPS( T )\
T operator= ( T &t )\
{\
	return value = t;\
}\
T operator= ( const T &t )\
{\
	return value = t;\
}

#define DECLARE_OPERATORS( T )\
DECLARE_OPC(T)		\
					\
					\
DECLARE_OP1N(+,T)	\
DECLARE_OP1N(-,T)	\
DECLARE_OP1N(!,T)	\
DECLARE_OP1N(~,T)	\
DECLARE_OP1N(&,T*)	\
					\
DECLARE_OP1NM(*,T)	\
					\
DECLARE_OP2(+,T)	\
DECLARE_OP2(+=,T)	\
DECLARE_OP2(-,T)	\
DECLARE_OP2(-=,T)	\
DECLARE_OP2(*,T)	\
DECLARE_OP2(*=,T)	\
DECLARE_OP2(/,T)	\
DECLARE_OP2(/=,T)	\
DECLARE_OP2(%,T)	\
DECLARE_OP2(%=,T)	\
DECLARE_OP2(<<=,T)	\
DECLARE_OP2(<<,T)	\
DECLARE_OP2(>>=,T)	\
DECLARE_OP2(>>,T)	\
DECLARE_OP2(&,T)	\
DECLARE_OP2(&=,T)	\
DECLARE_OP2(|,T)	\
DECLARE_OP2(|=,T)	\
DECLARE_OP2(^,T)	\
DECLARE_OP2(^=,T)	\
					\
DECLARE_OP1(--,T)	\
DECLARE_OP1(++,T)	\
					\
DECLARE_OPL(==,T)	\
DECLARE_OPL(!=,T)	\
DECLARE_OPL(>,T)	\
DECLARE_OPL(>=,T)	\
DECLARE_OPL(<,T)	\
DECLARE_OPL(<=,T)

template<typename T>
class SWNumber
{
protected:
	T value;

public:
	SWNumber() :
		value(0)
	{
	}
	SWNumber(T &x) :
		value(x)
	{
	}
	SWNumber(const T &x) :
		value(x)
	{
	}
	virtual ~SWNumber()
	{
	}

	void setValue(const T &value)
	{
		this->value = value;
	}
};

#endif /* SWNUMBER_H_ */
