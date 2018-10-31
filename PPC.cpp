#include "PPC.h"
#include "m33.h"



PPC::PPC()
{

}
	
PPC::PPC(float hfov, int _w, int _h): a(1.0f, 0.0f, 0.0f), b(0.0f, -1.0f, 0.0f), C(0.0f, 0.0f, 0.0f), w(_w), h(_h)
{
	float hfovr = hfov * 3.14159f / 180.0f;
	//focal length is the distance between C and image plane
	float f = -(float)w / (2.0f *tanf(hfovr / 2.0f));
	c = V3(-(float)w / 2.0f, (float)h / 2.0f, f);
	m_invert = CalMInvert();
}

M33 PPC::CalMInvert()
{
	M33 M;
	M.SetColumn(0, a);
	M.SetColumn(1, b);
	M.SetColumn(2, c);
	return M.Inverted();
}

int PPC::Project(V3 p, V3 &pp)
{
	pp[0] = FLT_MAX;
	V3 q = m_invert*(p - C);
	if (q[2] < 0.0f)
		return 0;
	pp[2] = 1.0f / q[2];
	pp[0] = q[0] / q[2];
	pp[1] = q[1] / q[2];
	return 1;
}

V3 PPC::GetVD()
{
	return (a ^ b).UnitVector();
}

float PPC::GetFocalLength()
{
	return GetVD()*c;
}

void PPC::ChangeFocalLength(float scf)
{
	c = c + GetVD()*(GetFocalLength()*(scf - 1.0f));
	m_invert = CalMInvert();
}

void PPC::SetPosFromAnotherPPC(PPC *ppc0)
{
	this->a = ppc0->a;
	this->b = ppc0->b;
	this->c = ppc0->c;
	this->C = ppc0->C;
}

void PPC::PositionAndOrient(V3 C1, V3 L1, V3 vpv)
{
	V3 vd1 = (L1 - C1).UnitVector();
	V3 a1 = (vd1 ^ vpv).UnitVector() * a.Length();
	V3 b1 = (vd1 ^ a1).UnitVector() * b.Length();
	V3 c1 = vd1 * GetFocalLength() - b1 * ((float)h / 2.0f) - a1 * ((float)w / 2.0f);

	C = C1;
	a = a1;
	b = b1;
	c = c1;

	m_invert = CalMInvert();
}

void PPC::Pan(float angled)
{
	V3 dv = (b * -1.0f).UnitVector();
	a = a.RotateThisVectorAboutDirection(dv, angled);
	c = c.RotateThisVectorAboutDirection(dv, angled);
	m_invert = CalMInvert();
}

void PPC::Tilt(float angled)
{
	V3 dv = a.UnitVector();
	b = b.RotateThisVectorAboutDirection(dv, angled);
	c = c.RotateThisVectorAboutDirection(dv, angled);
	m_invert = CalMInvert();
}

void PPC::Roll(float angled)
{
	V3 dv = GetVD();
	a =  a.RotateThisVectorAboutDirection(dv, angled);
	b = b.RotateThisVectorAboutDirection(dv, angled);
	c = c.RotateThisVectorAboutDirection(dv, angled);
	m_invert = CalMInvert();
}

M33 PPC::GetABCMatrix()
{
	M33 ret;
	ret.SetColumn(0, a);
	ret.SetColumn(1, b);
	ret.SetColumn(2, c);
	return ret;
}

//V3 PPC::UnprojectPixel(float uf, float vf, float currf) 
//{
//	return C + (a*uf + b * vf + c)*currf * (1.0f / GetFocalLength());
//}


V3 PPC::Unproject(V3 pP) 
{
	return C + (a*pP[0] + b * pP[1] + c) * (1.0f / pP[2]);
}

void PPC::Visualize(PPC *ppc3, FrameBuffer *fb3, float vf) 
{
	V3 colv(0.0f, 0.0f, 0.0f);
	fb3->Draw3DPoint(C, colv, ppc3, 7);

	float scf = vf / GetFocalLength();
	fb3->Draw3DSegment(C, colv, C + c * scf, colv, ppc3);
	fb3->Draw3DSegment(C + c * scf, colv, C + (c + a * (float)w)*scf, colv, ppc3);
	fb3->Draw3DSegment(C + (c + a * (float)w)*scf, colv,
		C + (c + a * (float)w + b * (float)h)*scf, colv, ppc3);
	fb3->Draw3DSegment(
		C + (c + a * (float)w + b * (float)h)*scf, colv,
		C + (c + b * (float)h)*scf, colv,
		ppc3);
	fb3->Draw3DSegment(
		C + (c + b * (float)h)*scf, colv,
		C + c*scf, colv,
		ppc3);
}

PPC::~PPC()
{

}
