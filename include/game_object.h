#ifndef GameObject_HEADER
#define GameObject_HEADER

#include "vector2.h"
#include "SDL2/SDL.h"

class GameObject
{

public:
	enum Color { LILAC, YELLOW, BLUE, TURQUOISE, RED, ORANGE, GREEN };

	~GameObject();

	GameObject();
	GameObject(const GameObject& other);
	GameObject(Vector2 position, Color color);
	GameObject(Vector2 position, Color color, Vector2 movement);
	GameObject(Vector2 position, Color color, Vector2 movement, float speed);

	float GetSpeed();
	void SetSpeed(float speed);

	SDL_Rect* GetPosition();
	void SetPosition(Vector2 position);

	Vector2 GetMovement();
	void SetMovement(Vector2 position);

	Color GetColor();
	void SetColor(Color Color);

	SDL_Surface * GetSurface();
	
	bool IsActive();
	void SetActive(bool isActive);

	friend ostream& operator << (ostream& outStream, GameObject const& vector);

	friend ostream& operator << (ostream& outStream, Color const& vector);


private:
	float speed;

	SDL_Rect *position;
	Vector2 movement;

	GameObject::Color color;

	bool isActive;

	SDL_Surface *surface;

	void LoadImageSurface();
};

#endif