#include "m33.h"

M33::M33(V3 row0, V3 row1, V3 row2)
{
	rows[0] = row0;
	rows[1] = row1;
	rows[2] = row2;
}

V3& M33::operator[](int idx)
{
	return rows[idx];
}

V3 M33::operator*(V3 v)
{
	M33 &m = *this;
	return V3(m[0] * v, m[1] * v, m[2] * v);
}

M33 M33::operator*(M33 m1)
{
	M33 &m0 = *this;
	V3 v00 = m0[0], v01 = m0[1], v02 = m0[2];
	V3 v10 = m1.GetColumn(0), v11 = m1.GetColumn(1), v12 = m1.GetColumn(2);
	return M33(V3(v00*v10, v00*v11, v00*v12), V3(v01*v10, v01*v11, v01*v12), V3(v02*v10, v02*v11, v02*v12));
}

ostream& operator<<(ostream& ostr, M33 m)
{
	return ostr << m[0] << endl << m[1] << endl << m[2];
}

void M33::SetColumn(int idx, V3 column)
{
	M33 &m = *this;
	m[0][idx] = column[0];
	m[1][idx] = column[1];
	m[2][idx] = column[2];
}

V3 M33::GetColumn(int idx)
{
	M33 &m = *this;
	return V3(m[0][idx], m[1][idx], m[2][idx]);
}

M33 M33::Inverted()
{
	V3 col0 = this->GetColumn(0);
	V3 col1 = this->GetColumn(1);
	V3 col2 = this->GetColumn(2);
	float det = col0*(col1^col2);
	M33 ret;
	return M33(col1^col2 / det, col2^col0 / det, col0^col1 / det);
}

M33 M33::Transposed()
{
	V3 col0 = this->GetColumn(0);
	V3 col1 = this->GetColumn(1);
	V3 col2 = this->GetColumn(2);
	return M33(col0, col1, col2);
}

M33 M33::SetRotationAboutXYZ(char axis, float angled)
{
	float angler = angled / 180.f * 3.1415927f;
	float cos_th = cos(angler), sin_th = sin(angler);
	if (axis == 'x' || axis == 'X')
	{
		return M33(V3(1, 0, 0), V3(0, cos_th, -sin_th), V3(0, sin_th, cos_th));
	}
	else if (axis == 'y' || axis == 'Y')
	{
		return M33(V3(cos_th,0,sin_th), V3(0, 1, 0), V3(-sin_th, 0, cos_th));
	}
	else if (axis == 'z' || axis == 'Z')
	{
		return M33(V3(cos_th, -sin_th, 0), V3(sin_th, cos_th, 0), V3(0, 0, 1));
	}
	else
	{
		cerr << "SetRotationAboutXYZ: invalid input" << endl;
		return M33(V3(0, 0, 0), V3(0, 0, 0), V3(0, 0, 0));
	}
}