#pragma once
#include "v3.h"
#include "ppc.h"
#include "framebuffer.h"
#include "aabb.h"


class TM {
public:
	V3 *verts, *colors, *normals, *texture_coordinates;
	int vertsN;
	unsigned int *tris;
	int trisN;
	TM() : verts(0), vertsN(0), colors(0), normals(0), tris(0), trisN(0) {};
	void SetRectangle(V3 O, float rw, float rh);
	void Allocate();
	
	void RenderPoints(PPC *ppc, FrameBuffer *fb);
	void RenderWireframe(PPC *ppc, FrameBuffer *fb);
	
	void RotateAboutArbitraryAxis(V3 O, V3 a, float angled);
	void LoadBin(char *fname);
	AABB GetAABB();
	void PositionAndSize(V3 tmC, float tmSize);
	void Translate(V3 tv);
	void Scale(float scf);
	V3 GetCenter();
};

