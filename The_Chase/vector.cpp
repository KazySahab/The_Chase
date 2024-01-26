#include "vector.h"

Vec2::Vec2(const float Xin, const float Yin)
{
	x = Xin;
	y = Yin;
}


bool Vec2::operator==(Vec2& rhs)
{
	return ((x == rhs.x) && (y == rhs.y));
}

bool Vec2::operator!=(Vec2& rhs)
{
	return ((x != rhs.x) || (y != rhs.y));
}




Vec2 Vec2::operator+(Vec2& rhs)
{
	return Vec2(this->x + rhs.x, this->y + rhs.y);
}

Vec2 Vec2::operator-(Vec2& rhs)
{
	return Vec2(this->x - rhs.x, this->y - rhs.y);
}

Vec2 Vec2::operator*(Vec2& rhs)
{
	return Vec2(this->x * rhs.x, this->y * rhs.y);
}

Vec2 Vec2::operator/(Vec2& rhs)
{
	return Vec2(this->x / rhs.x, this->y / rhs.y);
}




void Vec2::operator += (Vec2& rhs)
{
	this->x += rhs.x;
	this->y += rhs.y;
}

void Vec2::operator -= (Vec2& rhs)
{
	this->x -= rhs.x;
	this->y -= rhs.y;
}

void Vec2::operator *= (Vec2& rhs)
{
	this->x *= rhs.x;
	this->y *= rhs.y;
}

//void Vec2::operator /= (Vec2& rhs)
//{
//	this->x /= rhs.x;
//	this->y /= rhs.y;
//}

void Vec2 :: operator*=(float& rhs)
{
	this->x *= rhs;
	this->y *= rhs;
}

void Vec2 :: operator*(float& rhs)
{
	this->x* rhs;
	this->y* rhs;
}




float Vec2::dist(Vec2& rhs)
{
	return sqrt(pow(this->x - rhs.x, 2) + pow(this->y - rhs.y, 2));

}


