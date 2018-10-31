#include "AABB.h"



AABB::AABB()
{
	for (int idx = 0; idx < 3; idx++)
	{
		c0[idx] = FLT_MAX;
		c1[idx] = FLT_MIN;
	}
}

AABB::AABB(V3 first_point)
{
	c0 = first_point;
	c1 = first_point;
}

void AABB::Initialize()
{
	for (int idx = 0; idx < 3; idx++)
	{
		c0[idx] = FLT_MAX;
		c1[idx] = -FLT_MAX;
	}
}

void AABB::AddPoint(V3 p)
{
	for (int idx=0; idx<3; idx++)
	{
		//c0[idx] = c0[idx] < p[idx] ? c0[idx] : p[idx];
		//c1[idx] = c1[idx] > p[idx] ? c1[idx] : p[idx];
		if (c0[idx] > p[idx])
		{
			c0[idx] = p[idx];
		}
		if (c1[idx] < p[idx])
		{
			c1[idx] = p[idx];
		}
	}
}

float AABB::GetDiagonal()
{
	return (c1 - c0).Length();
}

V3 AABB::GetCenter()
{
	return (c1 + c0) / 2;
}

int AABB::Clip2D(float left, float right, float top, float bottom)
{
	if (c0[0]>right || c0[1]>bottom || c1[0]<left || c1[1]<top)
	{
		return 0;
	}

	if (c0[0]<left)
	{
		c0[0] = left;
	}
	if (c0[1] < top)
	{
		c0[1] = top;
	}
	if (c1[0] > right)
	{
		c1[0] = right;
	}
	if (c1[1] > bottom)
	{
		c1[1] = bottom;
	}
	return 1;
}





AABB::~AABB()
{
}
