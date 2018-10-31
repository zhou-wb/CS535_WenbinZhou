#include "scene.h"
#include "v3.h"
#include "m33.h"
#include <stdlib.h>
#include "AABB.h"
#include "TM.h"
#include "Light.h"
#include <stdio.h>

Scene *scene;

using namespace std;

#include <fstream>

#include <iostream>

Scene::Scene() {

	gui = new GUI();
	gui->show();

	//init FrameBuffer
	int u0 = 20;
	int v0 = 20;
	int w = 640;
	int h = 480;

	fb = new FrameBuffer(u0, v0, w, h);
	fb->label("SW Frame buffer");
	fb->show();


	gui->uiw->position(u0, v0 + fb->h + 60);

	scene_1();

}

void Scene::scene_1()
{
	//init camera
	float hfov = 55.0f;
	ppc = new PPC(hfov, fb->w, fb->h);

	//init triangle mesh
	tmsN = 2;
	tms = new TM[tmsN];
	tms[0].LoadBin("geometry/teapot1K.bin");
	tms[1].SetRectangle(V3(0, 0, 0), 100, 100);
	tms[1].RotateAboutArbitraryAxis(V3(0, 0, 0), V3(1, 0, 0), 90);

	//put the camera in an appropriate position
	/*ppc->PositionAndOrient(tms[0].GetAABB().GetCenter() - V3(0, 0, -tms[0].GetAABB().GetDiagonal()) , tms[0].GetCenter(), V3(0.0f, 1.0f, 0.0f));*/
	tms[0].PositionAndSize(V3(0, -68, 200), 50.f);
	tms[1].PositionAndSize(V3(0, -80, 200), 50.f);
	ppc->PositionAndOrient(V3(0, 0, 0), V3(0, -60, 200), V3(0, 1, 0));

	lightsN = 2;
	lights = new Light(0.3, lightsN, tms, tmsN);
	V3 light_pos(30, -40, 120);
	lights->AddPointLight(light_pos, V3(1, 1, 1));
	//lights->AddPointLight(V3(-200, 0, 100), V3(1, 1, 1));

	render = new Render(fb, ppc, tms, tmsN, lights);

	for (int i = 0; i <= 360; i += 5)
	{
	//	cerr << i << endl;
		light_pos = light_pos.RotateThisPointAboutArbitraryAxis(V3(0, 0, 200), V3(0, 1, 0), 5);
		lights->ChangePosition(0, light_pos);
		render->Clear(V3(0.5, 0.5, 0.5));
		render->RenderOriginalColor();
		fb->redraw();
		Fl::check();

		/*char filename[10];
		sprintf_s(filename, "%03d.tiff", i / 5);
		fb->SaveAsTiff(filename);*/
	}

}

void Scene::scene_2()
{
	//init camera
	float hfov = 55.0f;
	ppc = new PPC(hfov, fb->w, fb->h);

	//init triangle mesh
	tmsN = 2;
	tms = new TM[tmsN];
	//tms[0].LoadBin("geometry/auditorium.bin");
	tms[0].LoadBin("geometry/teapot1K.bin");
	tms[1].SetRectangle(V3(0, 0, 0), 100, 100);
	tms[1].RotateAboutArbitraryAxis(V3(0, 0, 0), V3(1, 0, 0), 90);

	//put the camera in an appropriate position
	/*ppc->PositionAndOrient(tms[0].GetAABB().GetCenter() - V3(0, 0, -tms[0].GetAABB().GetDiagonal()) , tms[0].GetCenter(), V3(0.0f, 1.0f, 0.0f));*/
	tms[0].PositionAndSize(V3(0, -68, 200), 50.f);
	tms[1].PositionAndSize(V3(0, -80, 200), 200.f);
	
	ppc->PositionAndOrient(V3(0, -40, 0), V3(0, -80, 200), V3(0, 1, 0));

	//proj_map = new ProjectiveMap(V3(0, 0, 0), V3(0, -80, 200), V3(0, 1, 0), 40.f, "proj_1_952_640.png", tms, tmsN);
	proj_map = new ProjectiveMap(V3(0, 0, 0), tms[0].GetCenter(), V3(0, 1, 0), 20.f, "proj_2_800_800.jpg", tms, tmsN, 2, 2);
	

	render = new Render(fb, ppc, tms, tmsN, proj_map);
	render->RenderProjector();
	fb->redraw();
	Fl::check();
	for (int i = 0; i < 10; i++)
	{
		PPC proj_pos = proj_map->GetPosAsPPC();
		proj_pos.C = proj_pos.C + (proj_pos.GetVD()) * 15;
		proj_map->SetPosFromPPC(&proj_pos);
		render->Clear(V3(0.5, 0.5, 0.5));
		render->RenderProjector();
		fb->redraw();
		Fl::check();
	}

	

}

void Scene::scene_3()
{
	//init camera
	float hfov = 55.0f;
	ppc = new PPC(hfov, fb->w, fb->h);

	//init triangle mesh
	tmsN = 1;
	tms = new TM[tmsN];
	//tms[0].LoadBin("geometry/auditorium.bin");
	tms[0].LoadBin("geometry/auditorium.bin");


	//put the camera in an appropriate position
	/*ppc->PositionAndOrient(tms[0].GetAABB().GetCenter() - V3(0, 0, -tms[0].GetAABB().GetDiagonal()) , tms[0].GetCenter(), V3(0.0f, 1.0f, 0.0f));*/
	tms[0].PositionAndSize(V3(0, 0, 200), 150.f);

	ppc->PositionAndOrient(V3(0, -30, 0), V3(0, 0, 200), V3(0, 1, 0));

	//proj_map = new ProjectiveMap(V3(0, 0, 0), V3(0, -80, 200), V3(0, 1, 0), 40.f, "proj_1_952_640.png", tms, tmsN);
	proj_map = new ProjectiveMap(V3(0, 30, 0), tms[0].GetCenter(), V3(0, 1, 0), 20.f, "proj_2_800_800.jpg", tms, tmsN, 4, 4);

	render = new Render(fb, ppc, tms, tmsN, proj_map);

	render->RenderProjector();

}

//void Scene::Render() {
//
//
//	if (fb) {
//		Render(ppc, fb);
//	}
//
//}

//void Scene::Render(PPC *currppc, FrameBuffer *currfb) {
//
//	currfb->SetBGR(0xFFFFFFFF);
//	currfb->ClearZB(0.0f);
//	for (int tmi = 0; tmi < tmsN; tmi++) {
//		//tms[tmi].RenderFilled(currppc, currfb);
//		tms[tmi].RenderWireframe(currppc, currfb);
//		//tms[tmi].RenderPoints(currppc, currfb);
//	}
//	currfb->redraw();
//
//}

void Scene::DBG() {


	{
		V3 color(1.f, 0.f, 0.f), color_g(0.f,1.f,0.f);
		V3 p0(10, 30, 0), p1(20, 60, 0), p2(150, 40, 0);
		AABB aabb(p0);
		aabb.AddPoint(p1);
		aabb.AddPoint(p2);
		

		fb->DrawRectangle(aabb.c0[0], aabb.c0[1], aabb.c1[0], aabb.c1[1], color_g.GetColor());
		fb->Set(p0[0], p0[1], color.GetColor());
		fb->Set(p1[0], p1[1], color.GetColor());
		fb->Set(p2[0], p2[1], color.GetColor());
		//fb->DrawCircle(200, 200, 50, 0xFFFF0000);
		//fb->DrawSegment(p0, color_g, p1, color_g);


		fb->redraw();
		Fl::check();

		return;
	}

	{
		V3 p(420, 240, 0), o(320, 240, 0), direction(0,0.5,1);
		V3 circle(p), normal(0,0,1);

		
		for (int step = 0; step <360*10; step++)
		{
			fb->SetBGR(0xFFFFFFFF);
			for (int stepc = 0; stepc < 360; stepc++)
			{
				fb->pix[(int)circle[1] * (fb->w) + (int)circle[0] + 1] = 0xFF0000FF;
				circle = circle.RotateThisPointAboutArbitraryAxis(o, normal, 1);
			}

			fb->pix[(int)p[1] * (fb->w) + (int)p[0] + 1] = 0xFFFF0000;
			fb->pix[(int)p[1] * (fb->w) + (int)p[0] + 2] = 0xFFFF0000;
			fb->pix[(int)p[1] * (fb->w) + (int)p[0] + 3] = 0xFFFF0000;
			fb->pix[(int)p[1] * (fb->w) + (int)p[0] + 4] = 0xFFFF0000;
			fb->pix[(int)p[1] * (fb->w) + (int)p[0] + 5] = 0xFFFF0000;
			p = p.RotateThisPointAboutArbitraryAxis(o, direction, 0.5);
			fb->redraw();
			Fl::check();

			
		}

		return;
		
	}

	cerr << "INFO: pressed DBG" << endl;

}