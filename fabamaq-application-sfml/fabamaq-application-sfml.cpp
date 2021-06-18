#include "fabamaq-application-sfml.h"


using namespace std;

int main()
{
	cout << "\n\n\n\n\n";

	vector<Vector2> vector;

	//Testing Vector2
	Vector2 vec1(2, 2);
	Vector2 vec2(3, 3);
	Vector2 vec3(5, 5);

	vector.push_back(vec1);
	vector.push_back(vec2);
	vector.push_back(vec3);

	cout << "\n\n\n\n\n";

	for (size_t i = 0; i < vector.size(); i++) 
	{
		cout << vector[i] << " | before the changes\n";
		vector[i].SetY(999);
	}

	for (size_t i = 0; i < vector.size(); i++)
	{
		cout << vector[i] << " | after the changes inside the block\n";
	}

	cout << "\n\n\n\n\n";

	return 0;
}
