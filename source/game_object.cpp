#include "../include/game_object.h"
#include "../include/error.h"

#include <string>

string colorTextArray[7] = { "LILAC", "YELLOW", "BLUE", "TURQUOISE", "RED", "ORANGE", "GREEN"};

GameObject::~GameObject()
{
	if(surface != NULL)
	{
		SDL_FreeSurface(surface);
	}
	delete position;
}

GameObject::GameObject()
{
	this->speed = 0;

	this->position = new SDL_Rect;

	this->position->x = 0;
	this->position->y = 0;

	this->movement = Vector2(0,0);

	this->color = Color::RED;

	this->isActive = true;

	this->surface = NULL;

	LoadImageSurface();
}

GameObject::GameObject(const GameObject& other) 
{
	this->speed = other.speed;

	this->position = new SDL_Rect;

	this->position = other.position;
	this->movement = other.movement;

	this->color = other.color;

	this->isActive = true;

	this->surface = NULL;

	LoadImageSurface();
}

GameObject::GameObject(Vector2 position, Color color)
{
	this->position = new SDL_Rect;

	this->position->x = position.GetX();
	this->position->y = position.GetY();
	
	this->color = color;

	speed = 0;
	movement = Vector2(0, 0);

	this->isActive = true;
	
	this->surface = NULL;

	LoadImageSurface();
}

GameObject::GameObject(Vector2 position, Color color, Vector2 movement)
{
	this->position = new SDL_Rect;

	this->position->x = position.GetX();
	this->position->y = position.GetY();

	this->color = color;
	this->movement = movement;
	this->speed = 0;
	
	this->isActive = true;
	
	this->surface = NULL;

	LoadImageSurface();
}

GameObject::GameObject(Vector2 position, Color color, Vector2 movement, float speed)
{
	this->position = new SDL_Rect;

	this->position->x = position.GetX();
	this->position->y = position.GetY();

	this->color = color;
	this->movement = movement;
	this->speed = speed;

	this->isActive = true;
	
	this->surface = NULL;

	LoadImageSurface();
}

float GameObject::GetSpeed() 
{
	return this->speed;
}
void GameObject::SetSpeed(float speed) 
{
	this->speed = speed;
}

SDL_Rect* GameObject::GetPosition() 
{
	return this->position;
}
void GameObject::SetPosition(Vector2 position) 
{
	this->position->x = position.GetX();
	this->position->y = position.GetY();
}

Vector2 GameObject::GetMovement()
{
	return this->movement;
}
void GameObject::SetMovement(Vector2 movement) 
{
	this->movement = movement;
}

GameObject::Color GameObject::GetColor()
{
	return this->color;
}
void GameObject::SetColor(Color color)
{
	this->color = color;
	
	LoadImageSurface();
}

SDL_Surface* GameObject::GetSurface()
{
	return this->surface;
}

bool GameObject::IsActive()
{
	return this->isActive;
}
void GameObject::SetActive(bool isActive)
{
	this->isActive = isActive;
}

ostream& operator << (ostream& outStream, GameObject::Color const& color)
{
	outStream << colorTextArray[color];

	return outStream;
}

ostream& operator << (ostream& outStream, GameObject const& gameObject)
{
	outStream << "**********************************\nGameObject \nPosition: [" << gameObject.position->x << "," << gameObject.position->y << "]\nMovement: " << gameObject.movement << "\nColor: " << gameObject.color << "\nSpeed: " << gameObject.speed << "\n\n***************************************";

	return outStream;
}

void GameObject::LoadImageSurface()
{
	
	string folder = "./../resources/sprites/";
	string filename = colorTextArray[color];
	string extension = ".bmp";
	
	string path = folder + filename + extension;
	
	if(surface != NULL)
	{
		SDL_FreeSurface(surface);
	}

	surface = SDL_LoadBMP(path.c_str());
	if(surface == NULL)
	{
		throw Error::SDL_LOAD_BMP_ERROR;
	}

	position->h = surface->h;
	position->w = surface->w;
}



