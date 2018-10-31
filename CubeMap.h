#pragma once
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
using namespace cv;
class CubeMap
{
public:
	CubeMap();
	//cube image must have width:height = 3:4
	bool LoadCubeImage(char* fname);
	//up=1, left=2, front=3, right=4, down=5, back=6
	void Locate(int face_index, int local_u, int local_v, int &cube_u, int &cube_v);

	~CubeMap();

private:
	Mat cube_image;
	int cube_length;
};

