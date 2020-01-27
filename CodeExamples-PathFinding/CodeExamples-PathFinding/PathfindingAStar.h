#pragma once

#include "Vector2Int.h"
#include <string>
#include <list>
#include <iostream>
#include "PathfindingCosts.h"

using namespace std;

class PathfindingAStar
{
	bool m_debug;
	char** m_grid;
	int m_width, m_height;
	bool m_haveSetupGrid;

	list<PathfindingCosts> m_openList;
	list<Vector2Int> m_closedList;
public:
	PathfindingAStar(char** grid, int width, int height);
	PathfindingAStar(bool debuging);
	PathfindingAStar();

	~PathfindingAStar();

	list<Vector2Int> FindPathBetweenTwoPoints(Vector2Int origin, Vector2Int target, char** grid, int width, int height);
	list<Vector2Int> FindPathBetweenTwoPoints(Vector2Int origin, Vector2Int target);

	void AddToOpenList(Vector2Int self, int g = -1, int h = -1, int f = -1, Vector2Int parent = Vector2Int(-1,-1));
	void AddToOpenList(PathfindingCosts value);

	void AddToClosedList(Vector2Int location);

	void RemoveFromOpenList(Vector2Int location);

	PathfindingCosts FindFromOpenList(Vector2Int location);

	void VaildateNewDirection(Vector2Int positionToTest, PathfindingCosts currentPoint, Vector2Int targetPosition);
	bool PositionInGridIsCollision(char** grid, int width, int height, int x, int y);
	bool CheckClosedList(Vector2Int positionToTest);
};