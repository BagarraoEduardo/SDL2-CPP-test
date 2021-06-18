#include <iostream>

using namespace std;

class Vector2
{

private:
	
	float x;
	float y;

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

	friend ostream& operator << (ostream& outStream, Vector2 const& vector);
};