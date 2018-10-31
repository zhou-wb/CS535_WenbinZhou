#pragma once
#include "v3.h"

class M33 {
public:
	//construction
	M33() {};
	M33(V3 row0, V3 row1, V3 row2);
	
	//redefine operator
	V3& operator[](int idx);
	V3 operator*(V3 v);
	M33 operator*(M33 m1);
	friend ostream& operator<<(ostream& ostr, M33 m);

	//access for column
	void SetColumn(int idx, V3 column);
	V3 GetColumn(int idx);

	//calculation
	M33 Inverted();
	M33 Transposed();

	//return basic rotation matrix about X/Y/Z axis
	static M33 SetRotationAboutXYZ(char axis, float angled);


private:
	V3 rows[3];
};