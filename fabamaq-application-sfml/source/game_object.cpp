#include "../include/game_object.h"

#include <string>

GameObject::GameObject(const GameObject& other) 
{
	this->speed = other.speed;

	this->position = other.position;
	this->movement = other.movement;

	this->color = other.color;
}

GameObject::GameObject(Vector2 position, Color color)
{
	this->position = position;
	this->color = color;

	speed = 0;
	movement = Vector2(0, 0);
}

GameObject::GameObject(Vector2 position, Color color, Vector2 movement)
{
	this->position = position;
	this->color = color;
	this->movement = movement;

	this->speed = 0;
}

GameObject::GameObject(Vector2 position, Color color, Vector2 movement, float speed)
{
	this->position = position;
	this->color = color;
	this->movement = movement;
	this->speed = speed;
}

float GameObject::GetSpeed() 
{
	return this->speed;
}
void GameObject::SetSpeed(float speed) 
{
	this->speed = speed;
}

Vector2 GameObject::GetPosition() 
{
	return this->position;
}
void GameObject::SetPosition(Vector2 position) 
{
	this->position = position;
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
}

ostream& operator << (ostream& outStream, GameObject::Color const& color)
{
	string colorTextArray[7] = { "LILAC", "YELLOW", "BLUE", "TURQUOISE", "RED", "ORANGE", "GREEN"};

	outStream << colorTextArray[color];

	return outStream;
}

ostream& operator << (ostream& outStream, GameObject const& gameObject)
{
	outStream << "**********************************\nGameObject \nPosition: " << gameObject.position << "\nMovement: " << gameObject.movement << "\nColor: " << gameObject.color << "\nSpeed: " << gameObject.speed << "\n\n***************************************";

	return outStream;
}


