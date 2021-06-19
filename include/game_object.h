#ifndef GameObject_HEADER
#define GameObject_HEADER

#include "vector2.h"

class GameObject
{

public:
	enum Color { LILAC, YELLOW, BLUE, TURQUOISE, RED, ORANGE, GREEN };

	GameObject();
	GameObject(const GameObject& other);
	GameObject(Vector2 position, Color color);
	GameObject(Vector2 position, Color color, Vector2 movement);
	GameObject(Vector2 position, Color color, Vector2 movement, float speed);

	float GetSpeed();
	void SetSpeed(float speed);

	Vector2 GetPosition();
	void SetPosition(Vector2 position);

	Vector2 GetMovement();
	void SetMovement(Vector2 position);

	Color GetColor();
	void SetColor(Color Color);

	bool IsActive();
	void SetActive(bool isActive);

	friend ostream& operator << (ostream& outStream, GameObject const& vector);

	friend ostream& operator << (ostream& outStream, Color const& vector);


private:

	float speed;

	Vector2 position;
	Vector2 movement;

	GameObject::Color color;

	bool isActive;
};

#endif