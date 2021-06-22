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

/// <summary>
/// Method that initializes the Pooler stats
/// </summary>
/// <param name="size">size</param>
/// <param name="limit">limit</param>
void Pooler::Init(const int size, const int limit)
{
	this->size = size; //normal pool size
	this->limit = limit; //size that pool can reach when used on full capacity
	this->isActive = true;

	if (this->limit < this->size) 
	{
		this->size = this->limit;
	}
	poolVector.reserve(this->limit); //save space for the maximum pool limit
	
	for (size_t i = 0; i < this->size; i++) //creates all the objects and adds them to the vector
	{
		GameObject * gameObjectPointer = new GameObject;
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

/// <summary>
/// Method that checks if the pool has a gameobject free to return
/// </summary>
/// <returns>hasNext</returns>
bool Pooler::HasNext() 
{
	bool hasNext = false;

	if (isActive) 
	{
		if (poolVector.size() < limit) //in this case will always return true because the current size is below the limit
		{
			hasNext = true;
		}
		else //otherwise it will need to search for any gameobject htat it's currently unactive
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
	}
	return hasNext;
}

int Pooler::GetSize() 
{
	return this->size;
}

/// <summary>
/// Method that returns the next available GameObject.
/// It's advised to use the HasNext before this one, but
/// this one will return a nullptr in case of the pool reached
/// its maximum value
/// </summary>
/// <param name="color">color</param>
/// <returns></returns>
GameObject* Pooler::GetNext(GameObject::Color color)
{
	if (isActive)
	{
		for (size_t i = 0; i < poolVector.size(); i++) //first it will search for a gameobject that's created and it's not active
		{
			GameObject* currentGameObjectPointer = poolVector[i];

			if (!currentGameObjectPointer->IsActive())
			{
				currentGameObjectPointer->SetActive(true);
				currentGameObjectPointer->SetColor(color);
				return currentGameObjectPointer;
			}
		}

		if (poolVector.size() < limit) //only then, if the pool hasn't reached its limit, it will create a new gameobject
		{
			GameObject* newGameObjectPointer = new GameObject; 

			poolVector.push_back(newGameObjectPointer);
			newGameObjectPointer->SetActive(true);
			newGameObjectPointer->SetColor(color);

			return newGameObjectPointer;
		}
	}
	return nullptr; //nullptr if not active
}

/// <summary>
/// Method that releases a gameobject to be available to be used in the future
/// </summary>
/// <param name="gameObject">gameobject</param>
void Pooler::Return(GameObject * gameObject) 
{
	if (isActive)
	{
		bool hasReturned = false;
		size_t i = 0;

		for (; i < poolVector.size(); i++) //it will deactivate the gameobject
		{
			GameObject* currentGameObject = poolVector[i];

			if (currentGameObject == gameObject)
			{
				currentGameObject->SetActive(false);
				hasReturned = true;
				break;
			}
		}

		//if the pool size it's above its normal size it will 
		//adapt the size of the pool and delete the gameobject 
		if (hasReturned && poolVector.size() > size) 
		{
			poolVector.erase(
				remove_if(
					poolVector.begin(),
					poolVector.end(),
					[gameObject](GameObject* comparingGameObject) { return comparingGameObject == gameObject; }),
				poolVector.end());
			delete gameObject;
		}
	}
}

/// <summary>
/// It will freeze/unfreeze all the gameobjects
/// </summary>
/// <param name="value">freezeValue</param>
/// <param name="gameObjectsVector">gameObjects</param>
void Pooler::Freezed(bool value, vector<GameObject*> &gameObjectsVector)
{
	if (isActive)
	{
		for (GameObject*& listedGameObjectPointer : gameObjectsVector)
		{
			if (find(poolVector.begin(), poolVector.end(), listedGameObjectPointer) != poolVector.end())
			{
				listedGameObjectPointer->SetActive(!value);
			}
		}
	}
}


