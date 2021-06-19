#include "fabamaq-application.h"

using namespace std;


int main()
{
	cout << "\n\n\n\n\n";

	Pooler* pooler = Pooler::GetInstance();

	pooler->Init(5, 10);

	vector<GameObject *> gameObjectPointerVector;

	GameObject::Color color2 = GameObject::Color::GREEN;

	float speed2 = 10000;

	Vector2 position2(99, 99);

	Vector2 movement2(100, 100);

	GameObject* obj1 = nullptr;
	GameObject* obj2 = nullptr;
	GameObject* obj3 = nullptr;
	GameObject* obj4 = nullptr;
	GameObject* obj5 = nullptr;

	//Testing GameObjects
	if (pooler->HasNext()) 
	{
		obj1 = pooler->GetNext(GameObject::Color::BLUE);
	}

	pooler->GetNext(GameObject::Color::BLUE); 
	pooler->GetNext(GameObject::Color::BLUE);
	pooler->GetNext(GameObject::Color::BLUE);
	pooler->GetNext(GameObject::Color::BLUE);
	pooler->GetNext(GameObject::Color::BLUE);
	pooler->GetNext(GameObject::Color::BLUE);
	pooler->GetNext(GameObject::Color::BLUE);
	pooler->GetNext(GameObject::Color::BLUE);
	
	if (pooler->HasNext())
	{
		obj2 = pooler->GetNext(GameObject::Color::BLUE);
	}

	obj3 = pooler->GetNext(GameObject::Color::BLUE);

	if (pooler->HasNext())
	{
		obj4 = pooler->GetNext(GameObject::Color::BLUE);
	}

	gameObjectPointerVector.push_back(obj1);
	gameObjectPointerVector.push_back(obj2);
	gameObjectPointerVector.push_back(obj3);
	gameObjectPointerVector.push_back(obj4);

	cout << "\n\n\n\n############################################################################" << endl;

	size_t i = 0;
	for (; i < gameObjectPointerVector.size(); i++)
	{
		GameObject * currentGameObjectPointer = gameObjectPointerVector[i];

		if (currentGameObjectPointer != nullptr) 
		{
			GameObject currentGameObjectBeforeChanges = *currentGameObjectPointer;

			cout << currentGameObjectBeforeChanges << " | before the changes" << endl;
			currentGameObjectPointer->SetPosition(position2);
			currentGameObjectPointer->SetMovement(movement2);
			currentGameObjectPointer->SetSpeed(speed2);
			currentGameObjectPointer->SetColor(color2);

			GameObject currentGameObjectAfterChanges = *currentGameObjectPointer;

			cout << currentGameObjectAfterChanges << " | after the changes inside the block\n\n\n" << endl;
		}
	}

	for (i = 0; i < gameObjectPointerVector.size(); i++)
	{
		if (gameObjectPointerVector[i] != nullptr)
		{
			cout << *gameObjectPointerVector[i] << " | after the changes, but outsite of the initial block\n" << endl;
		}
	}

	cout << "\n\n\n\n############################################################################" << endl;


	for (i = 0; i < gameObjectPointerVector.size(); i++)
	{
		if (gameObjectPointerVector[i] != nullptr)
		{
			GameObject * pointerToReturn = gameObjectPointerVector[i];

			pooler->Return(pointerToReturn);
		}
	}

	gameObjectPointerVector.erase(
		std::remove_if(
			gameObjectPointerVector.begin(),
			gameObjectPointerVector.end(),
			[obj1, obj2, obj3, obj4](GameObject* comparingGameObject) 
			{ 
				return (comparingGameObject == obj1) || (comparingGameObject == obj2) || (comparingGameObject == obj3) || (comparingGameObject == obj4);
			}),
		gameObjectPointerVector.end());

	if (pooler->HasNext())
	{
		obj5 = pooler->GetNext(GameObject::Color::BLUE);
	}

	gameObjectPointerVector.push_back(obj5);
	for (int i = 0; i < gameObjectPointerVector.size(); i++)
	{
		GameObject* currentGameObjectPointer = gameObjectPointerVector[i];

		if (currentGameObjectPointer != nullptr)
		{
			GameObject currentGameObject = *currentGameObjectPointer;

			cout << currentGameObject << " | final test" << endl;
		}
	}

	return 0;
}
