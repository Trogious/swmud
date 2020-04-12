/*
 * SWPoint.h
 *
 *  Created on: 17-07-2011
 *      Author: Trog
 */

#ifndef SWPOINT_H_
#define SWPOINT_H_

namespace math
{

class SWPoint
{
protected:
	float x;
	float y;
	float z;

public:
	SWPoint() :
		x(0), y(0), z(0)
	{
	}

	SWPoint(float x, float y, float z) :
		x(x), y(y), z(z)
	{
	}

	virtual ~SWPoint()
	{
	}

	float getX() const
	{
		return x;
	}

	float getY() const
	{
		return y;
	}

	float getZ() const
	{
		return z;
	}

	void setX(float x)
	{
		this->x = x;
	}

	void setY(float y)
	{
		this->y = y;
	}

	void setZ(float z)
	{
		this->z = z;
	}

	bool operator==(const SWPoint &) const;
	bool operator!=(const SWPoint &p) const
	{
		return !(*this == p);
	}

	float distance(const SWPoint &) const;
};

}

#endif /* SWPOINT_H_ */
