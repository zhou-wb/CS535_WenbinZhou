#pragma once

#include "v3.h"
#include "framebuffer.h"
#include "m33.h"

class FrameBuffer;

class PPC
{
public:
	PPC();
	//set a,b,C by default and calculate c by hfov
	PPC(float hfov, int _w, int _h);
	//convert the x,y,z coordinates to u,v,1/w (z-buffer) according to camera, return 0 (and pp[0] == FLT_MAX) if w<0 (p behind the camera)
	int Project(V3 p, V3 &pp);
	//return visual direction, unit vector
	V3 GetVD();
	//return the distance between C and the image plane
	float GetFocalLength();
	//change c to change the distance between C and image plane
	void ChangeFocalLength(float scf);
	//put the camera at point C1 and look at point V1. The vpv is the up vector
	void PositionAndOrient(V3 C1, V3 L1, V3 vpv);
	//rotate horizontally (against -b)
	void Pan(float angled);
	//rotate vertically (against a)
	void Tilt(float angled);
	//rotate against VD
	void Roll(float angled);
	//unproject a pixel, currf (= w) must be specified
	//V3 UnprojectPixel(float uf, float vf, float currf);
	//unproject a projected point, pP[2] (= 1/w) has the currf
	V3 Unproject(V3 pP);
	void SetPosFromAnotherPPC(PPC * ppc);

	void Visualize(PPC *ppc3, FrameBuffer *fb3, float vf);

	M33 GetABCMatrix();


	~PPC();

	int w, h;
	V3 C;

private:
	V3 a, b, c;
	M33 CalMInvert();
	M33 m_invert;

};

