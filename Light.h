#pragma once
#include "v3.h"
#include "Render.h"
#include "framebuffer.h"
#include "PPC.h"
#include "TM.h"

class Light
{
public:
	Light();
	Light(float _k_ambient, int maxmun, TM *tms, int tmsN);

	void AddPointLight(V3 _position, V3 _color);

	bool IsVisiable(V3 object_pos, float cos_th);
	

	V3 ColorScf(V3 object_pos, V3 normal, V3 eye_pos, int specuar_exp = 100);

	void Visualize(FrameBuffer *fb_visual, PPC *ppc_visual);

	void ChangePosition(int i, V3 new_pos);

	~Light();

private:
	void InitZB();
	V3 GetTMsCenter();
	void Reset();

	Render *render_ZB;
	FrameBuffer *fb;
	PPC *ppc;
	TM *tms;
	int tmsN;

	int lightsN, lights_maxmum;
	V3 *positions;
	V3 *colors;
	float k_ambient;

	int w, h;
};

