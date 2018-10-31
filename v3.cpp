#include "v3.h"

#include "m33.h"

V3::V3(float x, float y, float z) {

	xyz[0] = x;
	xyz[1] = y;
	xyz[2] = z;

}

float& V3::operator[](int idx)
{
	/*if (0 <= idx && idx <=2)
		return xyz[idx];
	else
	{
		cerr << "V3 index invalid!" << endl;
		return xyz[0];
	}*/
	return xyz[idx];
}

float V3::operator*(V3 v1)
{
	V3 &v0 = *this;
	return v0[0] * v1[0] + v0[1] * v1[1] + v0[2] * v1[2];
}

V3 V3::operator*(float scf)
{
	V3 &v0 = *this;
	return V3(scf*v0[0], scf*v0[1], scf*v0[2]);
}

V3 V3::operator/(float scf)
{
	V3 &v0 = *this;
	return V3(v0[0] / scf, v0[1] / scf, v0[2] / scf);
}

V3 V3::operator+(V3 v1)
{
	V3 &v0 = *this;
	return V3(v0[0] + v1[0], v0[1] + v1[1], v0[2] + v1[2]);
}

V3 V3::operator-(V3 v1)
{
	V3 &v0 = *this;
	return V3(v0[0] - v1[0], v0[1] - v1[1], v0[2] - v1[2]);
}

V3 V3::operator^(V3 v1)
{
	V3 &v0 = *this;
	return V3(v0[1] * v1[2] - v0[2] * v1[1], v0[2] * v1[0] - v0[0] * v1[2], v0[0] * v1[1] - v0[1] * v1[0]);
}

ostream& operator<<(ostream& ostr, V3 v)
{
	return ostr << v[0] << " " << v[1] << " " << v[2];
}

void V3::SetFromColor(unsigned int color)
{
	V3 &v = *this;
	
	//ptr represents every red/ green/ blue in unsigned in color
	unsigned char* ptr = (unsigned char*)&color;

	v[0] = ((float)ptr[1]) / 255.f;
	v[1] = ((float)ptr[2]) / 255.f;
	v[2] = ((float)ptr[3]) / 255.f;
}

unsigned int V3::GetColor(float alpha)
{
	V3 &v = *this;
	unsigned char color[4];
	for (int i=0; i<3; i++)
	{
		if (v[i]<0)
		{
			color[i] = 0;
		}
		else if (v[i]>1)
		{
			color[i] = 255;
		}
		else
		{
			color[i] = (unsigned char)(v[i] * 255.f);
		}
	}

	if (alpha < 0)
	{
		color[3] = 0;
	}
	else if (alpha > 1)
	{
		color[3] = 255;
	}
	else
	{
		color[3] = (unsigned char)(alpha * 255.f);
	}
		
	return *((unsigned int*)color);
}

float V3::Length()
{
	V3 &v = *this;
	return sqrtf(v*v);
}

V3 V3::UnitVector()
{
	return (*this) / this->Length();
}

/*
V3 V3::RotateThisVectorAboutDirection(V3 direction, float angle_d)
{
	//polar coordinates
	float theta_r = atan(direction[1] / direction[0]);
	float phi_r = acos(direction[2] / direction.Length());

	//convert degree to radius
	float angle_r = angle_d / 180.f * 3.1415927f;
	
	//set basic rotation matrix
	M33 rot_z_theta, rot_y_phi, rot_z_angle;
	rot_z_theta.SetRotationAboutXYZ('z', theta_r);
	rot_y_phi.SetRotationAboutXYZ('y', phi_r);
	rot_z_angle.SetRotationAboutXYZ('z', angle_r);

	//final rotation matrix
	M33 rot_mat = rot_z_theta*rot_y_phi*rot_z_angle*rot_y_phi.Transposed()*rot_z_theta.Transposed();

	return rot_mat*(*this);
}*/

V3 V3::RotateThisVectorAboutDirection(V3 a, float angled) {

	// find auxiliary axis
	V3 aux;
	if (fabsf(a[0]) > fabsf(a[1])) {
		aux = V3(0.0f, 1.0f, 0.0f);
	}
	else {
		aux = V3(1.0f, 0.0f, 0.0f);
	}

	V3 a0 = (aux ^ a).UnitVector();
	V3 a2 = (a0 ^ a).UnitVector();
	M33 lcs;
	lcs[0] = a0;
	lcs[1] = a;
	lcs[2] = a2;

	V3 &p = *this;
	// change to local coordinate system O, a0, a, a2
	V3 p1 = lcs * p;
	// rotate about "Second axis" in local coordinate system;
	M33 mr; mr.SetRotationAboutXYZ('y', angled);
	V3 p2 = mr * p1;
	V3 p3 = lcs.Inverted()*p2;
	return p3;

}

V3 V3::RotateThisPointAboutArbitraryAxis(V3 origin, V3 direction, float angle_d)
{
	//select auxiliary axis
	V3 aux;
	if (direction[0] > direction[1])
		aux = V3(0, 1, 0);
	else
		aux = V3(1, 0, 0);

	V3 v1 = direction^aux;
	V3 v2 = direction^v1;

	M33 trans_rot(direction, v1, v2);

	V3 &p0 = *this;
	V3 p1 = trans_rot*(p0 - origin);

	M33 rot_dir;
	rot_dir = M33::SetRotationAboutXYZ('x', angle_d);
	V3 p2 = rot_dir*p1;

	return trans_rot.Inverted()*p2 + origin;
}


