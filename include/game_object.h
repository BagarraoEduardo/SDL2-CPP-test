#ifndef GameObject_HEADER
#define GameObject_HEADER

#ifdef WIN32
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "vector2.h"

/// <summary>
/// Class that stores all the information about the GameObjects
/// </summary>
class GameObject
{

public:
	//possible color balls
	enum Color { LILAC, YELLOW, BLUE, TURQUOISE, RED, ORANGE, GREEN };

	~GameObject();

	GameObject(const GameObject& other);
	GameObject(Vector2 position = Vector2(0,0), Color color = Color::RED, Vector2 movement = Vector2(0,0), float speed = 0);

	void Update(float deltaTime);

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

	SDL_Rect *position; //GameObject rect
	Vector2 movement; //movement vector. it's used as a direction

	GameObject::Color color;
	
	//This will decide if the GameObject is currently 
	//active(if will be rendered and it if will move)
	bool isActive; 

	SDL_Surface *surface; //surface where the ball bitmap will be rendered

	void LoadImageSurface();
	void AdjustRect();
};

#endif