#include "Vector2Int.h"

Vector2Int::Vector2Int(int startX, int startY)
{
	x = startX;
	y = startY;
}

Vector2Int Vector2Int::flip()
{
	Vector2Int copyOfVector = *this;
	copyOfVector.x = y;
	copyOfVector.y = x;
	return copyOfVector;
};