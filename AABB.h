#pragma once
#include "v3.h"

class AABB
{
public:
	AABB();
	AABB(V3 first_point);
	void AddPoint(V3 p);
	float GetDiagonal();
	V3 GetCenter();
	void Initialize();
	//notice if you use a 2D AABB on a image plane, the default setting for y axis is the larger, the lower
	//which means, top < bottom
	int Clip2D(float left, float right, float top, float bottom);
	~AABB();
	
	V3 c0, c1;
};

