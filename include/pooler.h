#ifndef Pooler_HEADER
#define Pooler_HEADER

#include <vector>

#include "game_object.h"

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

private:
	static Pooler* Instance;

	Pooler();

	GameObject CreateGameObject();

	bool isActive;
	int size;
	int limit;

	vector<GameObject *> poolVector;
};

#endif