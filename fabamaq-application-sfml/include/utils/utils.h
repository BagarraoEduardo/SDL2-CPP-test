#include "../vector2.h"

class Utils 
{

public:
	const Vector2 Zero = Vector2(0, 0);

	const Vector2 Up = Vector2(0, 1);
	const Vector2 Down = Vector2(0, -1);
	const Vector2 Left = Vector2(-1, 0);
	const Vector2 Right = Vector2(1, 0);

	const Vector2 TopLeft = Vector2(-1, 1);
	const Vector2 BottomLeft = Vector2(-1, -1);
	const Vector2 TopRight = Vector2(1, 1);
	const Vector2 BottomRight = Vector2(1, -1);
};