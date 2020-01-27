#include <iostream>
#include "GridStorage.h"
#include <string>
#include <iomanip>      // std::setw
#include "Vector2Int.h"
#include "PathfindingAStar.h"

using namespace std;

int main()
{
	GridStorage *grid = new GridStorage(20,80);
	PathfindingAStar* pathingAStar = nullptr;
	list<Vector2Int> pathBetweenPoints;
	Vector2Int playerLocation, newPlayerLocation, pathfindingLocation = Vector2Int(-1,-1);
	bool showGrid = false;
	while (true)
	{
		system("CLS");
		if (showGrid)
		{
			if (pathfindingLocation.x > -1)//Pathfinding
			{
				if (pathingAStar != nullptr)
					delete pathingAStar;
				pathingAStar = new PathfindingAStar(false);
				pathBetweenPoints.empty();
				cout << "Path between: (" << playerLocation.x << ", " << playerLocation.y << ") and (" << pathfindingLocation.x << ", " << pathfindingLocation.y << ")" << endl;
				pathBetweenPoints = pathingAStar->FindPathBetweenTwoPoints(pathfindingLocation, playerLocation, grid->GetGrid(), grid->GetGridWidth(), grid->GetGridHeight());
				cout << "pathBetweenPoints: " << pathBetweenPoints.size() << endl;
			}

			//Show the Y Ten numbers
			cout << "      ";
			int eachTen = 0;
			for (int c = 0; c < grid->GetGridHeight(); c++)
			{
				if (eachTen % 10 == 0)
					cout << eachTen;
				else if (eachTen % 11 != 0)
					cout << " ";
				eachTen++;
			}
			cout << endl;

			//Show the actual Single Diget numbers
			cout << "      ";
			int zeroToNine = 0;
			for (int c = 0; c < grid->GetGridHeight(); c++)
			{
				cout << zeroToNine++;
				if (zeroToNine == 10) zeroToNine = 0;
			}
			cout << endl;

			//Print the actual grid
			for (int r = 0; r < grid->GetGridWidth(); r++)
			{
				cout << std::setw(4) << r;
				string currentLine = grid->GetGridRow(r);
				cout << "| ";
				for (int c = 0; c < currentLine.length(); c++)
				{
					if (r == playerLocation.y && c == playerLocation.x)
					{
						cout << "@";
						continue;
					}

					bool isPath = false;
					list<Vector2Int>::iterator it;
					for (it = pathBetweenPoints.begin(); it != pathBetweenPoints.end(); ++it)
						if (it->x == r && it->y == c)
						{
							cout << "%";
							isPath = true;
							break;
						}
					if (isPath) continue;

					cout << currentLine[c];
				}
					
				cout << endl;
			}
		}

		cout << "Options: 1. Load Maze | 2. Move Player" << endl;

		int option = 0, reuseInt1 = 0, reuseInt2 = 0;
		cin >> option;

		switch (option)
		{
		case 1:
			grid->ClearGrid();
			grid->MazeGrid();
			showGrid = true;
			break;
		case 2:
			cout << "Enter new position X: " << endl;
			cin >> newPlayerLocation.x;
			if (newPlayerLocation.x < 0) continue;
			if (newPlayerLocation.x >= grid->GetGridHeight()) continue;

			cout << "Enter new position Y: " << endl;
			cin >> newPlayerLocation.y;
			if (newPlayerLocation.y < 0) continue;
			if (newPlayerLocation.y >= grid->GetGridWidth()) continue;
			pathfindingLocation = playerLocation;
			playerLocation = newPlayerLocation;
			continue;
			break;
		}
	}

	return 0;
}