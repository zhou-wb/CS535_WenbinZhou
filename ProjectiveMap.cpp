#include "ProjectiveMap.h"



ProjectiveMap::ProjectiveMap(V3 position, V3 lookat_point, V3 vpv, float hfov, char* img_file_name, TM *tms, int tmsN, int scale_x, int scale_y): 
	hfov(hfov), position(position), lookat_point(lookat_point), tms(tms), tmsN(tmsN)
{
	img = imread(img_file_name);
	w = img.cols;
	h = img.rows;
	fb = new FrameBuffer(0, 0, w*scale_x, h*scale_y);
	ppc = new PPC(hfov, w*scale_x, h*scale_y);
	ppc->PositionAndOrient(position, lookat_point, vpv);
	render_ZB = new Render(fb, ppc, tms, tmsN);
	render_ZB->RenderZBuffer();
}

/*
void ProjectiveMap::Initialize(V3 _position, V3 _lookat_point, Mat *_img)
{
	position = _position;
	lookat_point = _lookat_point;
	img = _img;
	render_ZB = new Render()
}*/


V3 ProjectiveMap::GetColor(V3 object_pos, V3 normal)
{
	V3 pp;
	ppc->Project(object_pos, pp);
	float cos_th = normal.UnitVector()*(position - object_pos).UnitVector();
	if (!fb->Visible(pp[0], pp[1], pp[2]+0.3*tan(acos(cos_th))))
	{
		return V3(-1, -1, -1);
	}
	int x = (int)pp[0] % w;
	int y = (int)pp[1] % h;
	Vec3b intensity = img.at<Vec3b>(y, x);
	uchar blue = intensity.val[0];
	uchar green = intensity.val[1];
	uchar red = intensity.val[2];
	return V3((float)red/255.f, (float)green/255.f, (float)blue/255.f);
}


void ProjectiveMap::SetPosFromPPC(PPC *new_ppc)
{
	ppc->SetPosFromAnotherPPC(new_ppc);
	position = ppc->C;
	fb->ClearZB(0.f);
	render_ZB->RenderZBuffer();
}

PPC &ProjectiveMap::GetPosAsPPC()
{
	return *ppc;
}

//void SetImage(char* new_img_file_name);


ProjectiveMap::~ProjectiveMap()
{
	delete render_ZB;
	delete ppc;
	delete fb;
}
