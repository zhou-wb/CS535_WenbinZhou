#include "framebuffer.h"
#include "math.h"
#include <iostream>
#include "scene.h"
#include "v3.h"
#include <iostream>

#include "tiffio.h"

using namespace std;

FrameBuffer::FrameBuffer(int u0, int v0,int _w, int _h) 
	: Fl_Gl_Window(u0, v0, _w, _h, 0) {

	w = _w;
	h = _h;
	pix = new unsigned int[w*h];
	zb = new float[w*h];
}


void FrameBuffer::draw() {

	glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, pix);

}

int FrameBuffer::handle(int event) {

	switch (event)
	{
	case FL_KEYBOARD: {
		KeyboardHandle();
		return 0;
	}
	default:
		break;
	}
	return 0;
}

void FrameBuffer::KeyboardHandle() {

	int key = Fl::event_key();
	switch (key) {
	case ',': {
		cerr << "INFO: pressed ," << endl;
		break;
	}
	default:
		cerr << "INFO: do not understand key press" << endl;
	}
}


void FrameBuffer::SetBGR(unsigned int bgr) {

	for (int uv = 0; uv < w*h; uv++)
		pix[uv] = bgr;

}


// load a tiff image to pixel buffer
void FrameBuffer::LoadTiff(char* fname) {
	TIFF* in = TIFFOpen(fname, "r");
	if (in == NULL) {
		cerr << fname << " could not be opened" << endl;
		return;
	}

	int width, height;
	TIFFGetField(in, TIFFTAG_IMAGEWIDTH, &width);
	TIFFGetField(in, TIFFTAG_IMAGELENGTH, &height);
	if (w != width || h != height) {
		w = width;
		h = height;
		delete[] pix;
		pix = new unsigned int[w*h];
		size(w, h);
		glFlush();
		glFlush();
	}

	if (TIFFReadRGBAImage(in, w, h, pix, 0) == 0) {
		cerr << "failed to load " << fname << endl;
	}

	TIFFClose(in);
}

// save as tiff image
void FrameBuffer::SaveAsTiff(char *fname) {

	TIFF* out = TIFFOpen(fname, "w");

	if (out == NULL) {
		cerr << fname << " could not be opened" << endl;
		return;
	}

	TIFFSetField(out, TIFFTAG_IMAGEWIDTH, w);
	TIFFSetField(out, TIFFTAG_IMAGELENGTH, h);
	TIFFSetField(out, TIFFTAG_SAMPLESPERPIXEL, 4);
	TIFFSetField(out, TIFFTAG_BITSPERSAMPLE, 8);
	TIFFSetField(out, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(out, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(out, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);

	for (uint32 row = 0; row < (unsigned int)h; row++) {
		TIFFWriteScanline(out, &pix[(h - row - 1) * w], row);
	}

	TIFFClose(out);
}


int FrameBuffer::IndexConvert(int u, int v)
{
	return (h - 1 - v)*w + u;
}

void FrameBuffer::Set(int u, int v, unsigned int color)
{
	pix[IndexConvert(u, v)] = color;
}

void FrameBuffer::Set_s(int u, int v, unsigned int color)
{
	if (u<0 || u>w-1 || v<0 || v>h-1)
	{
		return;
	}

	Set(u, v, color);
}

unsigned int FrameBuffer::Get(int u, int v)
{
	return pix[IndexConvert(u, v)];
}

void FrameBuffer::DrawRectangle(int u0, int v0, int u1, int v1, unsigned int color)
{
	if (!ClipToScreen(u0,v0,u1,v1))
	{
		return;
	}

	for (int u=u0;u<=u1;u++)
	{
		for (int v=v0;v<=v1;v++)
		{
			Set(u, v, color);
		}
	}
}

void FrameBuffer::DrawRectangle(int u0, int v0, int u1, int v1, unsigned int color, float depth)
{
	if (!ClipToScreen(u0, v0, u1, v1))
	{
		return;
	}

	for (int u = u0; u <= u1; u++)
	{
		for (int v = v0; v <= v1; v++)
		{
			if (!Visible(u,v, depth))
			{
				continue;
			}
			Set(u, v, color);
		}
	}
}

void FrameBuffer::DrawCircle(int u0, int v0, int r, unsigned int color)
{
	int left = u0 - r, right = u0 + r, top = v0 + r, bottom = v0 - r;

	if (!ClipToScreen(left, bottom, right, top))
	{
		return;
	}

	V3 point_c(u0 + 0.5f, v0 + 0.5f, 0);
	for (int u = left; u<=right; u++)
	{
		for (int v=bottom; v<=top; v++)
		{
			V3 p(u + 0.5f, v + 0.5f, 0);
			if ((p-point_c).Length()>r)
			{
				continue;
			}
			Set(u, v, color);
		}
	}
}

void FrameBuffer::DrawSegment(V3 point0, V3 color0, V3 point1, V3 color1)
{
	V3 p1p0 = point1 - point0, c1c0 = color1 - color0;
	int pixN = fabsf(p1p0[0]) > fabsf(p1p0[1]) ? (int)fabsf(p1p0[0]) + 1 : (int)fabsf(p1p0[1]) + 1;

	for (int stepsi = 0; stepsi < pixN + 1; stepsi++)
	{
		float fracf = (float)stepsi / (float)(pixN);
		V3 p = point0 + p1p0 * fracf;
		V3 c = color0 + c1c0 * fracf;
		int u = (int)p[0];
		int v = (int)p[1];
		if (u < 0 || u > w - 1 || v < 0 || v > h - 1)
			continue;
		// this z-buffer (1/w) is interpolated in screen space
		if (!Visible(u, v, p[2])) 
		{
			continue;
		}
		Set(u, v, c.GetColor());
	}
}

void FrameBuffer::ClearZB(float z0)
{
	for (int i = 0; i < w*h; i++)
		zb[i] = z0;
}
int FrameBuffer::Visible(int u, int v, float currz)
{
	if (u < 0 || u >= w || v < 0 || v >= h)
	{
		return 0;
	}
	int uv = (h - 1 - v)*w + u;
	if (zb[uv] > currz)
		return 0;
	zb[uv] = currz;
	return 1;
}

int FrameBuffer::ClipToScreen(int &u0, int &v0, int &u1, int &v1)
{
	if (u1<0 || v1<0 || u0>w-1 || v0>h-1)
	{
		return 0;
	}

	if (u0<0)
	{
		u0 = 0;
	}
	if (v0<0)
	{
		v0 = 0;
	}
	if (u1 > w - 1)
	{
		u1 = w - 1;
	}
	if (v1 > h-1)
	{
		v1 = h - 1;
	}
	return 1;

}

void FrameBuffer::Draw3DSegment(V3 p0, V3 c0, V3 p1, V3 c1, PPC *ppc) 
{
	V3 pp0, pp1;
	if (!ppc->Project(p0, pp0))
		return;
	if (!ppc->Project(p1, pp1))
		return;
	DrawSegment(pp0, c0, pp1, c1);
}

void FrameBuffer::Draw3DPoint(V3 p, V3 c, PPC *ppc, int psize) 
{
	V3 pp;
	//check if p is in front of the camera
	if (!ppc->Project(p, pp))
		return;
	//check if p is within the image plane
	if (pp[0] < 0.0f || pp[0] >= (float)w || pp[1] < 0.0f || pp[1] >= (float)h)
		return;
	//check if p is near enough the camera
	/*if (!Visible(pp[0], pp[1], pp[2]))
		return;*/
	//find integer index of pp
	int u = (int)pp[0];
	int v = (int)pp[1];
	//use a small rectangle to represent a point
	DrawRectangle(u - psize / 2, v - psize / 2, u + psize / 2, v + psize / 2, c.GetColor(), pp[2]);
}

void FrameBuffer::Visualize3D(PPC *ppc0, PPC *ppc1, FrameBuffer *fb1) 
{
	for (int v = 0; v < h; v++) 
	{
		for (int u = 0; u < w; u++) 
		{
			if (zb[(h - 1 - v)*w + u] == 0.0f)
				continue;
			V3 pP(.5f + (float)u, .5f + (float)v, zb[(h - 1 - v)*w + u]);
			V3 pixP = ppc0->Unproject(pP);
			V3 cv; cv.SetFromColor(Get(u, v));
			fb1->Draw3DPoint(pixP, cv, ppc1, 1);
		}
	}
}