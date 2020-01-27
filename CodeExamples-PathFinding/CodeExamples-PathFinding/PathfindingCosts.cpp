#include "PathfindingCosts.h"

PathfindingCosts PathfindingCosts::ReturnSelfInDirection(testDirection direction)
{
	PathfindingCosts returnValue = *this;
	switch (direction)
	{
	case testDirection::up:
		returnValue.self.y -= 1;
		return returnValue;
	case testDirection::down:
		returnValue.self.y += 1;
		return returnValue;
	case testDirection::left:
		returnValue.self.x -= 1;
		return returnValue;
	case testDirection::right:
		returnValue.self.x += 1;
		return returnValue;
	default:
		return *this;
	}
}

void PathfindingCosts::LoadFromIterator(list<PathfindingCosts>::iterator it)
{
	f = it->f;
	g = it->g;
	h = it->h;
	self.x = it->self.x;
	self.y = it->self.y;
	parent.x = it->parent.x;
	parent.y = it->parent.y;
}