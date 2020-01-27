#include "GridStorage.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <stack>
#include <list>
#include <iostream>

using namespace std;

GridStorage::GridStorage(int width, int height)
{
	m_gridWidth = width;
	m_gridHeight = height;

	m_grid = new char*[m_gridWidth];
	for (int i = 0; i < m_gridWidth; i++)
		m_grid[i] = new char[m_gridHeight];
	ClearGrid();
	MazeGrid();
}

GridStorage::~GridStorage()
{
	if (CanUseGrid())
		for (int r = 0; r < m_gridWidth; r++)
			delete[] m_grid[r];
}

bool GridStorage::CanUseGrid()
{
	if (m_gridWidth == 0) return false;
	if (m_gridHeight == 0) return false;
	if (m_grid == nullptr)	return false;
	return true;
}

string GridStorage::GetGridRow(int row)
{
	if (!CanUseGrid()) return "";
	if (row < 0) return "";
	if (row >= m_gridWidth) return "";
	string returnString = "";
	for (int c = 0; c < m_gridHeight; c++)
		returnString += m_grid[row][c];
	return returnString;
}

void GridStorage::ClearGrid()
{
	if (!CanUseGrid()) return;
	for (int r = 0; r < m_gridWidth; r++)
		for (int c = 0; c < m_gridHeight; c++)
			m_grid[r][c] = '#';
}

void GridStorage::MazeGrid()
{
	srand(time(NULL));//Ensure this is random

	list<Vector2Int> masterPathList;
	//Default Top Left
	Vector2Int currentLocation; currentLocation.x = 0; currentLocation.y = 0;
	//Find Bottom Right
	Vector2Int masterLocationTarget; masterLocationTarget.x = m_gridWidth - 1; masterLocationTarget.y = m_gridHeight - 1;
	//Then Bottom Left
	Vector2Int masterLocationTarget2; masterLocationTarget2.y = m_gridHeight - 1;

	bool completedMasterPath = false, completedSecondMasterPath = false;//Used to make two paths between the two corners
	direction lastDirection = direction::up;
	int randomPaths = 0;//Number of random paths

	while (true)
	{
		if (!completedMasterPath)//First Path top left to bottom right
		{
			masterPathList.push_front(currentLocation);
			if (currentLocation.x == masterLocationTarget.x && currentLocation.y == masterLocationTarget.y)
			{
				//If we've reached the target setup the next path
				completedMasterPath = true;
				currentLocation.x = m_gridWidth - 1;
				currentLocation.y = 0;
				lastDirection = direction::none;
				continue;
			}
			MakePathBetweenTwoPoints(currentLocation, masterLocationTarget, lastDirection, masterPathList.size());
		}
		else if (!completedSecondMasterPath)
		{
			masterPathList.push_front(currentLocation);
			if (currentLocation.x == masterLocationTarget2.x && currentLocation.y == masterLocationTarget2.y)
			{
				//If we've reached the target setup the random paths
				completedSecondMasterPath = true;
				masterLocationTarget.x = rand() % m_gridWidth;
				masterLocationTarget.y = rand() % m_gridHeight;
				currentLocation.x = rand() % m_gridWidth;
				currentLocation.y = rand() % m_gridHeight;
				lastDirection = direction::none;
				continue;
			}
			MakePathBetweenTwoPoints(currentLocation, masterLocationTarget2, lastDirection, masterPathList.size());
		}
		else if(randomPaths < 7)//Find 7 random lines
		{
			masterPathList.push_front(currentLocation);
			if (currentLocation.x == masterLocationTarget.x && currentLocation.y == masterLocationTarget.y)
			{
				//Random paths just finds random places and draws lines between them
				randomPaths++;
				masterLocationTarget.x = rand() % m_gridWidth;
				masterLocationTarget.y = rand() % m_gridHeight;
				currentLocation.x = rand() % m_gridWidth;
				currentLocation.y = rand() % m_gridHeight;
				lastDirection = direction::none;
				continue;
			}
			MakePathBetweenTwoPoints(currentLocation, masterLocationTarget, lastDirection, masterPathList.size());
		}
		else
			break;
	}

	//Add the paths we just made above
	for (int r = 0; r < m_gridWidth; r++)
		for (int c = 0; c < m_gridHeight; c++)
			for (std::list<Vector2Int>::iterator it = masterPathList.begin(); it != masterPathList.end(); ++it)
				if (it->x == r && it->y == c)
					m_grid[r][c] = '.';
}

int GridStorage::FindDirectionOfVector(Vector2Int me, Vector2Int target)
{
	int returnNumber = 5;
	if (me.x < target.x)
		returnNumber = 6;
	else if (me.x > target.x)
		returnNumber = 4;

	if (me.y < target.y)
	{
		if (returnNumber == 6)
			returnNumber = 3;
		else if (returnNumber == 4)
			returnNumber = 1;
		else
			returnNumber = 2;
	}
	else if (me.y > target.y)
	{
		if (returnNumber == 6)
			returnNumber = 9;
		else if (returnNumber == 4)
			returnNumber = 7;
		else
			returnNumber = 8;
	}
	return returnNumber;
}

void GridStorage::MakePathBetweenTwoPoints(Vector2Int& currentPoint, Vector2Int target, GridStorage::direction& lastDirection, int randomElement)
{

	direction currentDirection = direction::none;

	while (currentDirection == direction::none)
	{
		int randomDirectionNumber = rand() % 4 + 1;

		int targetDirection = -1;
		if (randomElement % 2 == 0)
			targetDirection = FindDirectionOfVector(currentPoint, target);

		switch (randomDirectionNumber)
		{
		case 1://Up
			if (targetDirection > -1 && targetDirection < 7) continue;
			if (targetDirection == -1 && lastDirection == direction::down) continue;
			if (targetDirection == -1 && lastDirection == direction::up) continue;
			if (currentPoint.y == 0) continue;

			currentDirection = direction::up;
			currentPoint.y--;
			break;
		case 2://Down
			if (targetDirection > -1 && targetDirection > 3) continue;
			if (targetDirection == -1 && lastDirection == direction::up) continue;
			if (targetDirection == -1 && lastDirection == direction::down) continue;
			if (currentPoint.y == m_gridHeight - 1) continue;

			currentDirection = direction::down;
			currentPoint.y++;
			break;
		case 3://Left
			if (targetDirection > -1 && (targetDirection != 1 && targetDirection != 4 && targetDirection != 7)) continue;
			if (targetDirection == -1 && lastDirection == direction::right) continue;
			if (targetDirection == -1 && lastDirection == direction::left) continue;
			if (currentPoint.x == 0) continue;

			currentDirection = direction::left;
			currentPoint.x--;
			break;
		case 4://Right
			if (targetDirection > -1 && (targetDirection != 9 && targetDirection != 6 && targetDirection != 3)) continue;
			if (targetDirection == -1 && lastDirection == direction::left) continue;
			if (targetDirection == -1 && lastDirection == direction::right) continue;
			if (currentPoint.x == m_gridWidth - 1) continue;

			currentDirection = direction::right;
			currentPoint.x++;
			break;
		}
	}
	lastDirection = currentDirection;
}