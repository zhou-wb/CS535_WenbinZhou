#pragma once

#include <FL/Fl.H>
#include <FL/Fl_Gl_Window.H>
#include <GL/glut.h>

#include "v3.h"
#include "PPC.h"

class PPC;

class FrameBuffer : public Fl_Gl_Window {
public:
	unsigned int *pix;
	int w, h;
	float *zb;
	FrameBuffer(int u0, int v0, int _w, int _h);
	void draw();
	void KeyboardHandle();
	int handle(int guievent);
	void SetBGR(unsigned int bgr);
	void LoadTiff(char* fname);
	void SaveAsTiff(char* fname);

	//convert (x,y) to (u,v), basically the larger v, the lower point
	int IndexConvert(int u, int v);

	void Set(int u, int v, unsigned int color);
	//set color security, if the index is invalid, return
	void Set_s(int u, int v, unsigned int color);
	unsigned int Get(int u, int v);
	
	//image plane naive draw function
	//input index must be integer
	void DrawRectangle(int u0, int v0, int u1, int v1, unsigned int color);
	void DrawCircle(int u0, int v0, int r, unsigned int color);
	
	//has nothing to do with direction or index convert
	//(u0, v0) is the "lowest" point and (u1,v1) is the "largest" point
	int ClipToScreen(int &u0, int &v0, int &u1, int &v1);


	void DrawSegment(V3 point0, V3 color0, V3 point1, V3 color1);
	//project the p0 & p1 into screen space and interpolate z-buffer in screen space
	void Draw3DSegment(V3 p0, V3 c0, V3 p1, V3 c1, PPC *ppc);

	void Draw3DPoint(V3 p, V3 c, PPC *ppc, int psize);

	//set zb array to z0
	void ClearZB(float z0);
	//look at if currz are larger than the z-buffer associate with u,v, which means the curr-object are closer to camera
	int Visible(int u, int v, float currz);

	//draw every point associated with the pixel according to this frame buffer on another frame buffer by camera ppc1
	void Visualize3D(PPC *ppc0, PPC *ppc1, FrameBuffer *fb1);
};