
#include <array>
#include <algorithm>

#include "../include/pooler.h"
#include "../include/game_object.h"
#include "../include/constants.h"

Pooler* Pooler::Instance;

using namespace std;

Pooler::Pooler() 
{
	this->isActive = false;
	this->size = 0;
	this->limit = 0;
}

Pooler::~Pooler()
{
	for (size_t i = 0; i < poolVector.size(); i++) 
	{
		GameObject* currentGameObjectPointer = poolVector[i];
		delete currentGameObjectPointer;
	}
	poolVector.clear();
	delete Pooler::Instance;
}

void Pooler::Init(const int size, const int limit)
{
	this->size = size;
	this->limit = limit;
	this->isActive = true;

	if (this->limit < this->size) 
	{
		this->size = this->limit;
	}
	poolVector.reserve(this->limit);
	
	for (size_t i = 0; i < this->size; i++) 
	{
		GameObject * gameObjectPointer = new GameObject(CreateGameObject());
		gameObjectPointer->SetActive(false);

		poolVector.push_back(gameObjectPointer);
	}
}

Pooler* Pooler::GetInstance()
{
	if (Pooler::Instance == nullptr) 
	{
		Pooler::Instance = new Pooler();
	}
	return Instance;
}

bool Pooler::HasNext() 
{
	bool hasNext = false;

	if (poolVector.size() < limit) 
	{
		hasNext = true;
	}
	else 
	{
		for (size_t i = 0; i < poolVector.size(); i++)
		{
			GameObject* currentGameObject = poolVector[i];

			if (!currentGameObject->IsActive())
			{
				hasNext = true;
				break;
			}
		}
	}
	return hasNext;
}

int Pooler::GetSize() 
{
	return this->size;
}

GameObject* Pooler::GetNext(GameObject::Color color)
{
	for (size_t i = 0; i < poolVector.size(); i++) 
	{
		GameObject* currentGameObjectPointer = poolVector[i];

		if (!currentGameObjectPointer->IsActive())
		{
			currentGameObjectPointer->SetActive(true);
			currentGameObjectPointer->SetColor(color);
			return currentGameObjectPointer;
		}
	}

	if (poolVector.size() < limit)
	{
		GameObject* newGameObjectPointer = new GameObject(CreateGameObject());

		poolVector.push_back(newGameObjectPointer);
		newGameObjectPointer->SetActive(true);
		newGameObjectPointer->SetColor(color);
	
		return newGameObjectPointer;
	}
	return nullptr;
}

void Pooler::Return(GameObject * gameObject) 
{
	bool hasReturned = false;
	size_t i = 0;

	for (; i < poolVector.size(); i++)
	{
		GameObject* currentGameObject = poolVector[i];

		if (currentGameObject == gameObject)
		{
			currentGameObject->SetActive(false);
			hasReturned = true;
			break;
		}
	}

	if (hasReturned && poolVector.size() > size)
	{
		poolVector.erase(
			remove_if(
				poolVector.begin(), 
				poolVector.end(), 
				[gameObject](GameObject * comparingGameObject) { return comparingGameObject == gameObject; }), 
			poolVector.end());
		delete gameObject;
	}
}

GameObject Pooler::CreateGameObject() 
{
	Vector2 gameObjectPosition(0, 0);
	GameObject newGameObject = GameObject(gameObjectPosition, GameObject::Color::RED);
	return newGameObject;
}

void Pooler::Freezed(bool value, vector<GameObject *> &gameObjectsVector)
{
	for(GameObject * &listedGameObjectPointer : gameObjectsVector)
	{
		if(find(poolVector.begin(), poolVector.end(), listedGameObjectPointer) != poolVector.end())
		{
			listedGameObjectPointer->SetActive(!value);
		}
	}
}


