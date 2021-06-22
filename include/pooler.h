#ifndef Pooler_HEADER
#define Pooler_HEADER

#include <vector>

#include "game_object.h"

/// <summary>
/// Resource pool that manage every Gameobject that the user 
/// requests or releases in the game.
/// 
/// This class is a Singleton
/// </summary>
class Pooler 
{

public:
	~Pooler();
	
	static Pooler* GetInstance();

	bool HasNext();
	int GetSize();
	GameObject* GetNext(GameObject::Color color);
	void Return(GameObject* gameObject);
	void Init(const int size, const int limit);
	
	void Freezed(bool value,  vector<GameObject*> &gameObjectsVector);

private:
	static Pooler* Instance;

	Pooler();

	bool isActive; //it only can be used while is active(after call Init())

	//current size of the resource pool, although it 
	//can be extended when reaches the total size
	int size; 
	
	//this is the limit that the resource pool can reach. It cannot create 
	// more Gameobjects than this number
	int limit; 

	vector<GameObject*> poolVector; //vector that stores all the pool gameobjects
};

#endif