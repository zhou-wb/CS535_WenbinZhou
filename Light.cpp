#include "Light.h"



Light::Light()
{

}

Light::Light(float _k_ambient, int maxmun, TM *tms, int tmsN) :k_ambient(_k_ambient), tms(tms), tmsN(tmsN)
{
	positions = new V3[maxmun]; 
	colors = new V3[maxmun]; 
	lightsN = 0; 
	lights_maxmum = maxmun;
	
	w = 1500; h = 1500;
	ppc = new PPC(90, w, h);
	fb = new FrameBuffer(0, 0, w, h);
	render_ZB = new Render(fb, ppc, tms, tmsN);
	Reset();
};

V3 Light::GetTMsCenter()
{
	V3 ret(0, 0, 0);
	for (int i=0; i<tmsN; i++)
	{
		ret = ret + tms[i].GetCenter();
	}

	return ret / tmsN;
}

void Light::ChangePosition(int i, V3 new_pos)
{
	positions[i] = new_pos;
	Reset();
}

void Light::Reset()
{
	ppc->PositionAndOrient(positions[0], GetTMsCenter(), V3(0, 1, 0));
	fb->ClearZB(0.f);
	render_ZB->RenderZBuffer();
}

void Light::AddPointLight(V3 _position, V3 _color)
{
	if (lightsN == lights_maxmum)
		return;
	positions[lightsN] = _position;
	colors[lightsN] = _color;
	lightsN++;
	
	Reset();
}

void Light::InitZB()
{
	
}

bool Light::IsVisiable(V3 object_pos, float cos_th)
{
	V3 pp, light = object_pos - positions[0];
	float bias = 0.3*tan(acos(cos_th));
	ppc->Project(object_pos, pp);
	bias = 0.1f;
	return fb->Visible(pp[0], pp[1], pp[2]+bias);
}

V3 Light::ColorScf(V3 object_pos, V3 normal, V3 eye_pos, int specuar_exp)
{
	V3 light, reflect, position, color, ret(0,0,0);

	for (int i=0; i<lightsN; i++)
	{
		position = positions[i];
		color = colors[i];
		light = (object_pos - position).UnitVector();
		reflect = light - (normal.UnitVector()*(light*normal.UnitVector())) * 2;
		float k_diffuse = (light.UnitVector() * normal.UnitVector())*(-1);
		if (!IsVisiable(object_pos, k_diffuse))
		{
			return V3(k_ambient, k_ambient, k_ambient);
		}
		float k_speculate;
		if (k_diffuse <= 0)
		{
			k_diffuse = 0;
			k_speculate = 0;
		}
		else
		{
			k_speculate = pow((float)(reflect.UnitVector()*(eye_pos - object_pos).UnitVector()), specuar_exp);
			//k_speculate = 0;
		}

		ret = ret + color*(k_ambient + (1.0f - k_ambient)*k_diffuse + k_speculate);
	}
	return ret;
}

void Light::Visualize(FrameBuffer *fb_visual, PPC *ppc_visual)
{
	for (int i=0; i<lightsN; i++)
	{
		fb_visual->Draw3DPoint(positions[i], colors[i], ppc_visual, 10);
	}
}

Light::~Light()
{
	delete ppc;
	delete fb;
	delete render_ZB;
}
