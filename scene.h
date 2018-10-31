#pragma once

#include "gui.h"
#include "framebuffer.h"
#include "TM.h"
#include "Render.h"
#include "Light.h"
#include "ProjectiveMap.h"


class Scene {
public:


	GUI *gui;
	FrameBuffer *fb;
	PPC *ppc;
	Scene();
	void DBG();
	//void Render();
	//void Render(PPC *currPPC, FrameBuffer *currfb);
	Render *render;
	TM *tms;
	int tmsN;
	Light *lights;
	int lightsN;
	ProjectiveMap *proj_map;

private:
	void scene_1();
	void scene_2();
	void scene_3();
	void scene_4();
	void scene_5();

};

extern Scene *scene;