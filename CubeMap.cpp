#include "CubeMap.h"



CubeMap::CubeMap()
{
}

// load a tiff image to pixel buffer
bool CubeMap::LoadCubeImage(char* fname) 
{
	cube_image = imread(fname);
	int w = cube_image.cols;
	int h = cube_image.rows;
	if (w % 3 != 0 || h % 4 != 0 || w / 3 != h / 4)
		return false;
	cube_length = w / 3;
	return true;
}

void CubeMap::Locate(int face_index, int local_u, int local_v, int &cube_u, int &cube_v)
{
	float origin[6][2] = { {1.5f*cube_length, 0.5f*cube_length}, {0.5f*cube_length, 1.5f*cube_length}, \
	                       {1.5f*cube_length, 1.5f*cube_length}, {2.5f*cube_length, 1.5f*cube_length}, \
	                       {1.5f*cube_length, 2.5f*cube_length}, {1.5f*cube_length, 3.5f*cube_length} };
	float xyz2u[6][3] = { {-1,0,0},{0,0,-1},{-1,0,0},{0,0,1},{-1,0,0},{-1,0,0} };
	float xyz2v[6][3] = { {0,0,-1},{0,-1,0},{0,-1,0},{0,-1,0},{0,0,1},{0,1,0} };



	for (int face_i=0; face_i<6; face_i++)
	{
		if ()
		{
		}
	}
}

CubeMap::~CubeMap()
{
}
