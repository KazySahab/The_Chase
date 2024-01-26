#pragma once
#include"iostream"
#include "cmath"

class Vec2
{
public:
	float x = 0;
	float y = 0;

	Vec2() {};
	Vec2(const float Xin, const float Yin);

	bool operator==(Vec2& rhs);
	bool operator!=(Vec2& rhs);

	Vec2 operator+(Vec2& rhs);
	Vec2 operator-(Vec2& rhs);
	Vec2 operator*(Vec2& rhs);
	Vec2 operator/(Vec2& rhs);


	void operator+=(Vec2& rhs);
	void operator-=(Vec2& rhs);
	void operator*=(Vec2& rhs);
	//void operator/=(Vec2& rhs);
	void operator*(float& rhs);
	void operator*=(float& rhs);

	float dist(Vec2& rhs);



};

