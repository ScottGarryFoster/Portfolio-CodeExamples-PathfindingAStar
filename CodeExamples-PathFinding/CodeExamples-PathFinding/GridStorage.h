#pragma once
#include <iostream>
#include <string>
#include "Vector2Int.h"

using namespace std;

class GridStorage
{
	char **m_grid;
	int m_gridWidth = 100;
	int m_gridHeight = 100;

	enum class direction {none = 0, up, down, left, right};
public:
	GridStorage(int width = 100, int height = 100);
	~GridStorage();

	const int GetGridWidth() { return m_gridWidth; }
	const int GetGridHeight() { return m_gridHeight; }
	char** GetGrid() { return m_grid; }

	bool CanUseGrid();

	string GetGridRow(int row);

	void ClearGrid();
	void MazeGrid();

	void MakePathBetweenTwoPoints(Vector2Int& currentPoint, Vector2Int target, GridStorage::direction &lastDirection, int randomElement = 2);
private:
	int FindDirectionOfVector(Vector2Int me, Vector2Int target);
};

