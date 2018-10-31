
#include <iostream>

using namespace std;

#include <fstream>

#include "TM.h"
#include "aabb.h"
#include "m33.h"

void TM::RotateAboutArbitraryAxis(V3 O, V3 a, float angled) {

	for (int vi = 0; vi < vertsN; vi++) {
		verts[vi] = verts[vi].RotateThisPointAboutArbitraryAxis(O, a, angled);
		normals[vi] = normals[vi].RotateThisPointAboutArbitraryAxis(O, a, angled);
	}

}


void TM::RenderPoints(PPC *ppc, FrameBuffer *fb) {

	for (int vi = 0; vi < vertsN; vi++) {
		fb->Draw3DPoint(verts[vi], colors[vi], ppc, 7);
	}

}

void TM::RenderWireframe(PPC *ppc, FrameBuffer *fb) {

	for (int tri = 0; tri < trisN; tri++) {
		for (int ei = 0; ei < 3; ei++) {
			int vi0 = tris[3 * tri + ei];
			int vi1 = tris[3 * tri + ((ei + 1) % 3)];
			fb->Draw3DSegment(verts[vi0], colors[vi0], verts[vi1], colors[vi1], ppc);
			//fb->Draw3DSegment(verts[vi0], V3(1,0,0), verts[vi1], V3(1, 0, 0), ppc);
		}
	}

}



float BilinearInterpolation(float q11, float q12, float q21, float q22, float x1, float x2, float y1, float y2, float x, float y)
{
	float x2x1, y2y1, x2x, y2y, yy1, xx1;
	x2x1 = x2 - x1;
	y2y1 = y2 - y1;
	x2x = x2 - x;
	y2y = y2 - y;
	yy1 = y - y1;
	xx1 = x - x1;
	return 1.0 / (x2x1 * y2y1) * (
		q11 * x2x * y2y +
		q21 * xx1 * y2y +
		q12 * x2x * yy1 +
		q22 * xx1 * yy1
		);
}



void TM::SetRectangle(V3 O, float rw, float rh) {

	vertsN = 4;
	trisN = 2;
	Allocate();

	verts[0] = O + V3(-rw / 2.0f, +rh / 2.0f, 0.0f);
	verts[1] = O + V3(-rw / 2.0f, -rh / 2.0f, 0.0f);
	verts[2] = O + V3(+rw / 2.0f, -rh / 2.0f, 0.0f);
	verts[3] = O + V3(+rw / 2.0f, +rh / 2.0f, 0.0f);

	texture_coordinates[0] = V3(0, 0, 0);
	texture_coordinates[1] = V3(0, rh - 1, 0);
	texture_coordinates[2] = V3(rw - 1, rh - 1, 0);
	texture_coordinates[3] = V3(rw - 1, 0, 0);

	int tri = 0;
	tris[3 * tri + 0] = 0;
	tris[3 * tri + 1] = 1;
	tris[3 * tri + 2] = 2;
	tri++;

	tris[3 * tri + 0] = 2;
	tris[3 * tri + 1] = 3;
	tris[3 * tri + 2] = 0;
	tri++;

	for (int vi = 0; vi < vertsN; vi++) {
		colors[vi] = V3(1.0f, 0.0, 0.0f);
		normals[vi] = V3(0.f, 0.f, -1.f);
	}

}

void TM::Allocate() {

	verts = new V3[vertsN];
	colors = new V3[vertsN];
	normals = new V3[vertsN];
	texture_coordinates = new V3[vertsN];
	tris = new unsigned int[3 * trisN];

}


void TM::LoadBin(char *fname) {

	ifstream ifs(fname, ios::binary);
	if (ifs.fail()) {
		cerr << "INFO: cannot open file: " << fname << endl;
		return;
	}

	ifs.read((char*)&vertsN, sizeof(int));
	char yn;
	ifs.read(&yn, 1); // always xyz
	if (yn != 'y') {
		cerr << "INTERNAL ERROR: there should always be vertex xyz data" << endl;
		return;
	}
	if (verts)
		delete verts;
	verts = new V3[vertsN];

	ifs.read(&yn, 1); // cols 3 floats
	if (colors)
		delete colors;
	colors = 0;
	if (yn == 'y') {
		colors = new V3[vertsN];
	}

	ifs.read(&yn, 1); // normals 3 floats
	if (normals)
		delete normals;
	normals = 0;
	if (yn == 'y') {
		normals = new V3[vertsN];
	}

	ifs.read(&yn, 1); // texture coordinates 2 floats
	float *tcs = 0; // don't have texture coordinates for now
	if (tcs)
		delete tcs;
	tcs = 0;
	if (yn == 'y') {
		tcs = new float[vertsN * 2];
	}

	ifs.read((char*)verts, vertsN * 3 * sizeof(float)); // load verts

	if (colors) {
		ifs.read((char*)colors, vertsN * 3 * sizeof(float)); // load cols
	}

	if (normals)
		ifs.read((char*)normals, vertsN * 3 * sizeof(float)); // load normals

	if (tcs)
		ifs.read((char*)tcs, vertsN * 2 * sizeof(float)); // load texture coordinates

	ifs.read((char*)&trisN, sizeof(int));
	if (tris)
		delete tris;
	tris = new unsigned int[trisN * 3];
	ifs.read((char*)tris, trisN * 3 * sizeof(unsigned int)); // read tiangles

	ifs.close();

	cerr << "INFO: loaded " << vertsN << " verts, " << trisN << " tris from " << endl << "      " << fname << endl;
	cerr << "      xyz " << ((colors) ? "rgb " : "") << ((normals) ? "nxnynz " : "") << ((tcs) ? "tcstct " : "") << endl;

	delete[]tcs;

}

AABB TM::GetAABB() {

	if (!verts)
		return AABB(V3(0.0f, 0.0f, 0.0f));

	AABB aabb(verts[0]);
	for (int i = 1; i < vertsN; i++)
		aabb.AddPoint(verts[i]);

	return aabb;

}

V3 TM::GetCenter() {

	V3 tmc(0.0f, 0.0f, 0.0f);
	for (int vi = 0; vi < vertsN; vi++) {

		tmc = tmc + verts[vi];

	}

	tmc = tmc * (1.0f / (float)vertsN);

	return tmc;

}

void TM::PositionAndSize(V3 tmC, float tmSize) {

	AABB aabb = GetAABB();
	V3 oldC = aabb.GetCenter();
	float oldSize = aabb.GetDiagonal();

	Translate(V3(0.0f, 0.0f, 0.0f) - oldC);
	Scale(tmSize / oldSize);
	Translate(tmC);

}

void TM::Translate(V3 tv) {

	for (int vi = 0; vi < vertsN; vi++)
		verts[vi] = verts[vi] + tv;

}

void TM::Scale(float scf) {

	for (int vi = 0; vi < vertsN; vi++)
		verts[vi] = verts[vi] * scf;

}
