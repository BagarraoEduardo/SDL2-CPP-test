#include "fabamaq-application-sfml.h"

using namespace std;

int main()
{
	cout << "\n\n\n\n\n";

	vector<GameObject> gameObjectVector;

	GameObject::Color color1 = GameObject::Color::BLUE;
	GameObject::Color color2 = GameObject::Color::GREEN;

	float speed1 = 1;
	float speed2 = 10000;

	Vector2 position1(100, 100);
	Vector2 position2(99, 99);

	Vector2 movement1(99, 99);
	Vector2 movement2(100, 100);

	//Testing GameObjects
	GameObject obj1(position1, color1, movement1, speed1);
	GameObject obj2(position1, color1, movement1, speed1);
	GameObject obj3(position1, color1, movement1, speed1);
	GameObject obj4(position1, color1, movement1, speed1);

	gameObjectVector.push_back(obj1);
	//gameObjectVector.push_back(obj2);
	//gameObjectVector.push_back(obj3);
	//gameObjectVector.push_back(obj4);

	size_t i = 0;

	cout << "\n\n\n\n############################################################################" << endl;

	for (i=0 ; i < gameObjectVector.size(); i++)
	{
		cout << gameObjectVector[i] << " | before the changes" << endl;
		gameObjectVector[i].SetPosition(position2);
		gameObjectVector[i].SetMovement(movement2);
		gameObjectVector[i].SetSpeed(speed2);
		gameObjectVector[i].SetColor(color2);

		cout << gameObjectVector[i] << " | after the changes inside the block\n\n\n" << endl;
	}

	for (i = 0; i < gameObjectVector.size(); i++)
	{
		cout << gameObjectVector[i] << " | after the changes, but outsite of the initial block\n" << endl;
	}

	cout << "\n\n\n\n############################################################################" << endl;

	return 0;
}
