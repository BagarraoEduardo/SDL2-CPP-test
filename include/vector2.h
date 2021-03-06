#ifndef Vector2_HEADER
#define Vector2_HEADER

#include <iostream>

using namespace std;

class Vector2
{
public:
	
	Vector2();
	Vector2(const Vector2 &other);
	Vector2(float x, float y);

	float GetX();
	float GetY();

	void SetX(float x);
	void SetY(float y);

	Vector2 operator + (Vector2 const& other);

	Vector2 operator - (Vector2 const& other);

	Vector2 operator / (Vector2 const& other);

	Vector2 operator * (Vector2 const& other);
	
	Vector2 operator *= (Vector2 const& other);

	friend Vector2 operator * (const float multiplier, Vector2 const& vector);

	friend Vector2 operator *= (const float multiplier, Vector2 const& vector);

	friend Vector2 operator * (Vector2 const& vector, const float multiplier);
	
	friend Vector2 operator *= (Vector2 const& vector, const float multiplier);

	friend ostream& operator << (ostream& outStream, Vector2 const& vector);

private:

	float x;
	float y;
};

#endif