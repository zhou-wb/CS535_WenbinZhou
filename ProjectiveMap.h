#pragma once
#include "v3.h"
#include "Render.h"
#include "framebuffer.h"
#include "PPC.h"
#include "TM.h"
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
using namespace cv;

class Render;

class ProjectiveMap
{
public:
	ProjectiveMap(V3 position, V3 lookat_point, V3 vpv, float hfov, char* img_file_name, TM *tms, int tmsN, int scale_x = 1, int scale_y = 1);
	//void Initialize(V3 position, V3 lookat_point, Mat *img);
	//return color if visible, return V3(-1,-1,-1) if invisible
	V3 GetColor(V3 object_pos, V3 normal);
	void SetPosFromPPC(PPC *new_ppc);
	PPC &GetPosAsPPC();
	void SetImage(char* new_img_file_name);

	~ProjectiveMap();

private:
	int scale_x, scale_y;
	V3 position, lookat_point;
	float hfov;
	Mat img;
	int w, h;
	Render *render_ZB;
	FrameBuffer *fb;
	PPC *ppc;
	TM *tms;
	int tmsN;

};

