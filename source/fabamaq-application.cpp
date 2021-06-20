#include "../include/fabamaq-application.h"

using namespace std;

int main()
{
	srand(time(NULL));

	Game game;

	game.Start();
	
	game.Quit();
	
	return 0;
}
