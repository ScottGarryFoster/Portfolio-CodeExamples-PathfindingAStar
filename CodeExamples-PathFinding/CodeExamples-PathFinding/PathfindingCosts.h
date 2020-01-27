#pragma once
#include "Vector2Int.h"
#include <list>

using namespace std;

class PathfindingCosts
{
public:
	int g = 99999, h = 99999, f = 99999;
	Vector2Int self;
	Vector2Int parent;
	bool explored = false;

	enum class testDirection { none = 0, up, down, left, right };
	PathfindingCosts ReturnSelfInDirection(testDirection direction);

	void LoadFromIterator(list<PathfindingCosts>::iterator it);

};
