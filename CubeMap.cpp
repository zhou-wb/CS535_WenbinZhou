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
	int origin[6][2] = { {cube_length, 0}, {0, cube_length}, {cube_length, cube_length}, \
							{2 * cube_length, cube_length},{cube_length, 2 * cube_length},{cube_length, 3 * cube_length} };
	int direction[6][2] = { {1,1},{1,1},{1,1},{1,1},{1,1},{} };
}

CubeMap::~CubeMap()
{
}