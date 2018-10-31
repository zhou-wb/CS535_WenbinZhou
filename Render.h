#pragma once
#include "framebuffer.h"
#include "PPC.h"
#include "TM.h"
#include "v3.h"
#include "m33.h"
#include "ProjectiveMap.h"
class Light;
class ProjectiveMap;

class Render
{
public:
	Render(FrameBuffer *_fb, PPC *_ppc, TM *_tms, int _tmsN,  Light *_light) :fb(_fb), ppc(_ppc), tms(_tms), 
		tmsN(_tmsN), light(_light) {
		fb->ClearZB(0.f);
	};
	Render(FrameBuffer *_fb, PPC *_ppc, TM *_tms, int _tmsN) :fb(_fb), ppc(_ppc), tms(_tms),
		tmsN(_tmsN) {
		fb->ClearZB(0.f);
	};
	Render(FrameBuffer *_fb, PPC *_ppc, TM *_tms, int _tmsN, ProjectiveMap *_proj_map) :fb(_fb), ppc(_ppc), tms(_tms),
		tmsN(_tmsN), proj_map(_proj_map) {
		fb->ClearZB(0.f);
	};
	void Clear(V3 color);
	void RenderOriginalColor();
	void RenderZBuffer();
	void RenderProjector();


	~Render();

private:
	inline void SetVertsIndex(int currTriangle);
	//put current triangle's 3 verts in to verts[3]
	inline void AssignCurrVerts(V3 p0, V3 p1, V3 p2);
	//project verts[3] into screen plane. stored in pverts[3]
	inline void VertsProjection();
	//return false if p0, p1, p1 are behind the camera
	inline bool ClipFront();
	inline void SetModelSpaceInterpolationMatrixQ();
	//return false if the projected triangle is out of the screen, otherwise return true and set screen_cliped_AABB which has integer bound indicate pixel index
	inline bool ClipScreen();
	inline void SetEdgeEquation();
	inline void SetColorABC();
	inline void SetNormalABC();
	inline void SetVertsABC();
	void SetTextCoordABC();
	inline bool IsInsideTriangle(V3 p);
	//return the depth(1/w) of current pixel
	inline void SetInterpolateVectorAndDepth(float u, float v);
	inline V3 Interpolate(M33 &property_ABC);



	FrameBuffer *fb;
	PPC *ppc;
	TM *tms;
	TM *tm;
	int tmsN;
	Light *light; // (V3(200, 0, 100), 0.1, V3(1, 0, 0));
	ProjectiveMap *proj_map;

	//parameters per triangle 
	unsigned int verts_index[3];
	V3 verts[3];
	V3 pverts[3];
	AABB screen_cliped_AABB;
	M33 edge_equation;
	M33 color_ABC, normal_ABC, text_coord_ABC, verts_ABC;
	M33 model_space_interpolation_matrix_q;
	
	
	//parameters per pixel per triangle
	V3 interpolate_vector;
	float depth;
};

