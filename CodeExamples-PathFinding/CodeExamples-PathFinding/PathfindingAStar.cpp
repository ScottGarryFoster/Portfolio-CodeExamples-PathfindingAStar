#include "PathfindingAStar.h"
#include <list>
using namespace std;

PathfindingAStar::PathfindingAStar(char** grid, int width, int height)
{
	m_grid = grid;
	m_width = width;
	m_height = height;
	m_haveSetupGrid = true;
	m_debug = true;
}

PathfindingAStar::PathfindingAStar(bool debuging)
{
	m_grid = nullptr;
	m_width = -1;
	m_height = -1;
	m_haveSetupGrid = false;
	m_debug = debuging;
}
PathfindingAStar::PathfindingAStar()
{
	m_grid = nullptr;
	m_width = -1;
	m_height = -1;
	m_haveSetupGrid = false;
	m_debug = true;
}

PathfindingAStar::~PathfindingAStar()
{
	//Not really required but I'd like an error to occur here if at all:
	m_openList.empty();
	m_closedList.empty();
}
list<Vector2Int> PathfindingAStar::FindPathBetweenTwoPoints(Vector2Int origin, Vector2Int target, char** grid, int width, int height)
{
	m_grid = grid;
	m_width = width;
	m_height = height;
	m_haveSetupGrid = true;
	return FindPathBetweenTwoPoints(origin, target);
}

list<Vector2Int> PathfindingAStar::FindPathBetweenTwoPoints(Vector2Int origin, Vector2Int target)
{
	list<Vector2Int> returnList;
	if (!m_haveSetupGrid)
	{
		if(m_debug)
			cout << "PathfindingAStar::FindPathBetweenTwoPoints:: Have not setup grid yet." << endl;
		return returnList;
	}

	m_openList.empty();
	m_closedList.empty();
	PathfindingCosts currentPoint;
	currentPoint.self = origin;
	currentPoint.f = 0;
	currentPoint.g = 0;
	currentPoint.h = 0;

	AddToOpenList(currentPoint);

	bool foundTarget = false;
	while (!foundTarget)
	{
		for(int i = 1; i < 5; i++)//Vaildate all directions adding to open list if we can
			VaildateNewDirection(currentPoint.ReturnSelfInDirection((PathfindingCosts::testDirection)i).self, currentPoint, target);

		RemoveFromOpenList(currentPoint.self);//No longer look for a path in the current position
		AddToClosedList(currentPoint.self);//Already have the shortest route to this node

		int LowestFCostFound = 999999999, LowestHCostFound = 999999999;
		PathfindingCosts foundLowest;
		foundLowest.self = Vector2Int(-1, -1);
		if (m_openList.size() <= 0)
		{
			if(m_debug)
				cout << "PathfindingAStar::FindPathBetweenTwoPoints:: Could not find route, open list was empty." << endl;
			return returnList;//There is no path
		}
		list<PathfindingCosts>::iterator it;
		for (it = m_openList.begin(); it != m_openList.end(); ++it)
		{	//Find the next lowest cost to search
			if (it->explored) continue;
			if (LowestFCostFound > it->f || (LowestFCostFound == it->f && LowestHCostFound > it->h))
			{
				LowestFCostFound = it->f;
				LowestHCostFound = it->h;
				foundLowest.LoadFromIterator(it);
			}
		}
		if (foundLowest.self.x == -1 || foundLowest.self.y == -1)
		{
			if(m_debug)
				cout << "PathfindingAStar::FindPathBetweenTwoPoints:: Could not find route." << endl;
			return returnList;//There is no path
		}

		currentPoint = foundLowest;
		if (currentPoint.self.x == target.x && currentPoint.self.y == target.y)
			foundTarget = true;
	}

	returnList.push_back(currentPoint.self);
	while (true)
	{
		currentPoint = FindFromOpenList(currentPoint.parent);
		
		if (currentPoint.self.x == -1)
			return returnList;
		
		returnList.push_back(currentPoint.self);
		if(currentPoint.self.x == origin.x && currentPoint.self.y == origin.y)
			return returnList;
	}
	return returnList;
}

void PathfindingAStar::AddToOpenList(Vector2Int self, int g, int h, int f, Vector2Int parent)
{
	PathfindingCosts newPathfinding;
	newPathfinding.self = self;
	if (g > -1) newPathfinding.g = g;
	if (h > -1) newPathfinding.h = h;
	if (f > -1) newPathfinding.f = f;
	if (parent.x > -1 && parent.y > -1) newPathfinding.parent = parent;
	AddToOpenList(newPathfinding);//Ensures no duplicates
}

void PathfindingAStar::AddToOpenList(PathfindingCosts value)
{
	while (true)
	{
		bool brokeEarly = false;
		list<PathfindingCosts>::iterator it;
		for (it = m_openList.begin(); it != m_openList.end(); ++it)
			if (it->self.x == value.self.x && it->self.y == value.self.y)
			{
				return;
				brokeEarly = true;
				m_openList.erase(it++);
				break;
			}
		if (!brokeEarly) break;
	}
	m_openList.push_back(value);
}

PathfindingCosts PathfindingAStar::FindFromOpenList(Vector2Int location)
{
	PathfindingCosts returnPathfinding;
	returnPathfinding.self.x = -1;
	returnPathfinding.self.y = -1;
	list<PathfindingCosts>::iterator it;
	for (it = m_openList.begin(); it != m_openList.end(); ++it)
		if (it->self.x == location.x && it->self.y == location.y)
		{
			returnPathfinding.LoadFromIterator(it);
			return returnPathfinding;
		}
	return returnPathfinding;
}

void PathfindingAStar::RemoveFromOpenList(Vector2Int location)
{
	list<PathfindingCosts>::iterator it;
	for (it = m_openList.begin(); it != m_openList.end(); ++it)
		if (it->self.x == location.x && it->self.y == location.y)
		{
			it->explored = true;
			return;
		}
}

void PathfindingAStar::AddToClosedList(Vector2Int location)
{
	list<Vector2Int>::iterator it;
	for (it = m_closedList.begin(); it != m_closedList.end(); ++it)
		if (it->x == location.x && it->y == location.y)
			return;//If it's already on the closed list don't add it
	m_closedList.push_back(location);
}

void PathfindingAStar::VaildateNewDirection(Vector2Int positionToTest, PathfindingCosts currentPoint, Vector2Int targetPosition)
{
	if (!m_haveSetupGrid) return;
	if (PositionInGridIsCollision(m_grid, m_width, m_height, positionToTest.x, positionToTest.y))//True means there is a collision
		AddToClosedList(positionToTest);//Ensures no duplicates
	else if(!CheckClosedList(positionToTest))//True means it's on the closed list
	{
		PathfindingCosts newCost;
		newCost.g += currentPoint.g + 1;//Cost to move to this from the current point
		newCost.h = abs(targetPosition.x - positionToTest.x) + abs(targetPosition.y - positionToTest.y);//Distance to target if it were a straight path
		newCost.f = newCost.g + newCost.h;//Grand Total
		newCost.self = positionToTest;
		newCost.parent = currentPoint.self;
		AddToOpenList(newCost);
	}
}

bool PathfindingAStar::PositionInGridIsCollision(char** grid, int width, int height, int x, int y)
{
	
	if (x < 0) return true;
	if (y < 0) return true;
	if (x >= width) return true;
	if (y >= height) return true;
	
	if (grid[x][y] == '#') return true;
	return false;
}

bool PathfindingAStar::CheckClosedList(Vector2Int positionToTest)
{
	list<Vector2Int>::iterator it;
	for (it = m_closedList.begin(); it != m_closedList.end(); ++it)
		if (it->x == positionToTest.x && it->y == positionToTest.y)
			return true;
	return false;
}