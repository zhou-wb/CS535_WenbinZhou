#include "Render.h"
#include "Light.h"



//Render::Render(FrameBuffer *fb, PPC *ppc, TM *tm)
//{
//	Clear(V3(1,1,1));
//}

void Render::Clear(V3 color)
{
	fb->SetBGR(color.GetColor());
	fb->ClearZB(0.f);
}

void Render::SetVertsIndex(int currTriangle)
{
	for (int i=0; i<3; i++)
	{
		verts_index[i] = tm->tris[3 * currTriangle + i];
	}
}

void Render::AssignCurrVerts(V3 p0, V3 p1, V3 p2)
{
	verts[0] = p0;
	verts[1] = p1;
	verts[2] = p2;
}

void Render::VertsProjection()
{
	for (int i=0; i<3; i++)
	{
		ppc->Project(verts[i], pverts[i]);
	}
}

bool Render::ClipFront()
{
	for (int i=0; i<3; i++)
	{
		if (pverts[i][0] == FLT_MAX)
		{
			return false;
		}
	}
	return true;
}

bool Render::ClipScreen()
{
	AABB origin_aabb;
	for (int i=0; i<3; i++)
	{
		origin_aabb.AddPoint(pverts[i]);
	}

	if (!origin_aabb.Clip2D(0.0f, (float)fb->w, 0.0f, (float)fb->h))
		return false;
	
	int lefti = (int)(origin_aabb.c0[0] + 0.5f);
	int righti = (int)(origin_aabb.c1[0] - 0.5f);
	int topi = (int)(origin_aabb.c0[1] + 0.5f);
	int bottomi = (int)(origin_aabb.c1[1] - 0.5f);

	screen_cliped_AABB.AddPoint(V3(lefti, topi, 0));
	screen_cliped_AABB.AddPoint(V3(righti, bottomi, 0));
	
	return true;
}

void Render::SetEdgeEquation()
{
	for (int ei = 0; ei < 3; ei++) 
	{
		V3 v0 = pverts[ei];
		V3 v1 = pverts[(ei + 1) % 3];
		V3 v2 = pverts[(ei + 2) % 3]; 
		v2[2] = 1.0f;
		edge_equation[ei][0] = v0[1] - v1[1];
		edge_equation[ei][1] = v1[0] - v0[0];
		edge_equation[ei][2] = -v0[0] * edge_equation[ei][0] - v0[1] * edge_equation[ei][1];
		if (edge_equation[ei] * v2 < 0.0f)
			edge_equation[ei] = edge_equation[ei] * -1.0f;
	}
}

bool Render::IsInsideTriangle(V3 p)
{
	V3 ss = edge_equation * p;
	if (ss[0] < 0.0f || ss[1] < 0.0f || ss[2] < 0.0f)
		return false;
	return true;
}

void Render::SetInterpolateVectorAndDepth(float u, float v)
{
	V3 p(u, v, 1);
	V3 temp_lkw = model_space_interpolation_matrix_q * p;
	//depth = 1/w
	depth = temp_lkw[0] + temp_lkw[1] + temp_lkw[2];
	interpolate_vector = temp_lkw / depth;
}

V3 Render::Interpolate(M33 &property_ABC)
{
	return property_ABC * interpolate_vector;
}



void Render::RenderOriginalColor()
{
	light->Visualize(fb, ppc);
	for (int i = 0; i < tmsN; i++)
	{
		tm = &tms[i];
		for (int tri = 0; tri < tm->trisN; tri++)
		{
			screen_cliped_AABB.Initialize();
			SetVertsIndex(tri);
			AssignCurrVerts(tm->verts[verts_index[0]], tm->verts[verts_index[1]], tm->verts[verts_index[2]]);
			VertsProjection();

			if (!ClipFront() || !ClipScreen())
			{
				continue;
			}


			SetEdgeEquation();
			SetColorABC();
			SetNormalABC();
			SetModelSpaceInterpolationMatrixQ();


			for (int v = screen_cliped_AABB.c0[1]; v <= screen_cliped_AABB.c1[1]; v++)
			{
				for (int u = screen_cliped_AABB.c0[0]; u <= screen_cliped_AABB.c1[0]; u++)
				{
					V3 pc(.5f + (float)u, .5f + (float)v, 1.0f);
					if (!IsInsideTriangle(pc))
					{
						continue;
					}
					SetInterpolateVectorAndDepth(pc[0], pc[1]);
					if (!fb->Visible(u, v, depth))
					{
						continue;
					}
					SetVertsABC();
					V3 lights_scf = light->ColorScf(Interpolate(verts_ABC), Interpolate(normal_ABC), ppc->C, 300);

					fb->Set(u, v, V3(1 * lights_scf[0], 0 * lights_scf[2], 0 * lights_scf[2]).GetColor());
				}
			}

		}
	}
}

void Render::RenderProjector()
{
	int i;
	for (i = 0, tm = tms; i < tmsN; i++, tm++)
	{

		for (int tri = 0; tri < tm->trisN; tri++)
		{
			screen_cliped_AABB.Initialize();
			// cerr << (float)tri / (float)tm->trisN << "\r";
			SetVertsIndex(tri);
			AssignCurrVerts(tm->verts[verts_index[0]], tm->verts[verts_index[1]], tm->verts[verts_index[2]]);
			VertsProjection();
			if (!ClipFront() || !ClipScreen())
			{
				continue;
			}
			SetEdgeEquation();
			SetColorABC();
			SetNormalABC();
			SetModelSpaceInterpolationMatrixQ();

			for (int v = screen_cliped_AABB.c0[1]; v <= screen_cliped_AABB.c1[1]; v++)
			{
				for (int u = screen_cliped_AABB.c0[0]; u <= screen_cliped_AABB.c1[0]; u++)
				{
					V3 pc(.5f + (float)u, .5f + (float)v, 1.0f);
					if (!IsInsideTriangle(pc))
					{
						continue;
					}
					SetInterpolateVectorAndDepth(pc[0], pc[1]);
					if (!fb->Visible(u, v, depth))
					{
						continue;
					}
					SetVertsABC();

					V3 color = proj_map->GetColor(Interpolate(verts_ABC), Interpolate(normal_ABC));

					if (color[0] == -1)
					{
						continue;
					}
					else
					{
						fb->Set(u, v, color.GetColor());
					}
					//fb->Set(u, v, 0xFFFFF0000);
				}
			}
		}
	}
}

void Render::RenderZBuffer()
{
	int i;
	for (i = 0, tm = tms; i < tmsN; i++, tm++)
	{
		for (int tri = 0; tri < tm->trisN; tri++)
		{
			//cerr << (float)tri / (float)tm->trisN << "\r";
			SetVertsIndex(tri);
			AssignCurrVerts(tm->verts[verts_index[0]], tm->verts[verts_index[1]], tm->verts[verts_index[2]]);
			VertsProjection();
			if (!ClipFront() || !ClipScreen())
			{
				continue;
			}
			SetEdgeEquation();
			SetModelSpaceInterpolationMatrixQ();

			for (int v = screen_cliped_AABB.c0[1]; v <= screen_cliped_AABB.c1[1]; v++)
			{
				for (int u = screen_cliped_AABB.c0[0]; u <= screen_cliped_AABB.c1[0]; u++)
				{
					V3 pc(.5f + (float)u, .5f + (float)v, 1.0f);
					if (!IsInsideTriangle(pc))
					{
						continue;
					}
					SetInterpolateVectorAndDepth(pc[0], pc[1]);
					fb->Visible(u, v, depth);
				}
			}
		}
	}
}

void Render::SetColorABC()
{
	for (int i=0; i<3; i++)
	{
		color_ABC.SetColumn(i, tm->colors[verts_index[i]]);
	}
}

void Render::SetNormalABC()
{
	for (int i = 0; i < 3; i++)
	{
		normal_ABC.SetColumn(i, tm->normals[verts_index[i]]);
	}
}

void Render::SetVertsABC()
{
	for (int i = 0; i < 3; i++)
	{
		verts_ABC.SetColumn(i, verts[i]);
	}
}

void Render::SetModelSpaceInterpolationMatrixQ()
{
	M33 v_c_mat, abc_mat;
	//v-c matrix
	for (int i=0; i<3; i++)
	{
		v_c_mat.SetColumn(i, verts[i]-ppc->C);
	}

/*
	abc_mat.SetColumn(0, ppc->a);
	abc_mat.SetColumn(1, ppc->b);
	abc_mat.SetColumn(2, ppc->c);*/

	//M33 test = v_c_mat.Inverted();
	model_space_interpolation_matrix_q = v_c_mat.Inverted() * ppc->GetABCMatrix();
}

Render::~Render()
{
}
