#pragma once

#include <iostream>

using namespace std;


class V3 {
public:
	//construction
	V3() {};
	V3(float x, float y, float z);

	//as vector
	float Length();
	V3 UnitVector();
	V3 RotateThisVectorAboutDirection(V3 direction, float angled);

	//as point
	V3 RotateThisPointAboutArbitraryAxis(V3 origin, V3 direction, float angle_d);

	//as color
	// store RGB value in V3 as range 0~1
	void SetFromColor(unsigned int color);
	//convert the RGB value stored in V3 to a unsigned int, set alpha value default as 1 (255)
	//the machine is Little endian, unsigned int has stored by "RGBA" (the actual value is "0xAGBR")
	unsigned int GetColor(float alpha = 1.f);

	//redefine the operator for V3
	float& operator[](int idx);
	float operator*(V3 v1);
	V3 operator*(float scf);
	V3 operator/(float scf);
	V3 operator+(V3 v1);
	V3 operator-(V3 v1);
	V3 operator^(V3 v1);
	friend ostream& operator<<(ostream& ostr, V3 v);


private:
	float xyz[3];

};