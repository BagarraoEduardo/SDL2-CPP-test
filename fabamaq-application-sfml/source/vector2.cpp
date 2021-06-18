#include "./../include/vector2.h"

Vector2::Vector2()
{
	this->x = 0;
	this->y = 0;
}

Vector2::Vector2(const Vector2& other)
{
	this->x = other.x;
	this->y = other.y;
}

Vector2::Vector2(float x, float y) 
{
	this->x = x;
	this->y = y;
}

float Vector2::GetX() 
{
	return this->x;
}

float Vector2::GetY() 
{
	return this->y;
}

void Vector2::SetX(float x) 
{
	this->x = x;
}

void Vector2::SetY(float y) 
{
	this->y = y;
}

Vector2 Vector2::operator + (Vector2 const& other)
{
	Vector2 sum;
	sum.x += other.x;
	sum.y += other.y;

	return sum;
}

Vector2 Vector2::operator - (Vector2 const& other)
{
	Vector2 subtract;
	subtract.x -= other.x;
	subtract.y -= other.y;

	return subtract;
}

Vector2 Vector2::operator / (Vector2 const& other)
{
	Vector2 divide;
	divide.x /= other.x;
	divide.y /= other.y;

	return divide;
}

Vector2 Vector2::operator * (Vector2 const& other)
{
	Vector2 multiply;
	multiply.x *= other.x;
	multiply.y *= other.y;

	return multiply;
}

ostream& operator << (ostream& outStream, Vector2 const& vector)
{
	outStream << "Vector2 [" << vector.x << "," << vector.y << "]";

	return outStream;
}