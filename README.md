# Code Examples: Pathfinding
This is my small pathfinding project within a console application using Visual Studio C++.
If you've found your way here on accident perhaps try looking at my portfolio: https://ScottGarryFoster.com

## Where's the code? What should I be looking at?
The best way to understand the code is to look at this link: https://ScottGarryFoster.com/pathfinding.html
However if you've come from there to see more of the code follow below.
The CPP and header files are two folders deep in [CodeExamples-PathFinding](https://github.com/ScottGarryFoster/Portfolio-CodeExamples-PathfindingAStar/tree/master/CodeExamples-PathFinding/CodeExamples-PathFinding) then:
* CodeExamples-PathFinding.cpp holds int main() and is where interaction occurs. Owns the other classes and draws to the screen through cout.
* GridStorage is the class for the maze and is passed around to the pathfinding and then finally read by the main method for drawing
* PathfindingAStar is the class for the actual path finding, it needs Vector2Int and the costs class to work.
* PathfindingCosts more of a storage class for the costs but used for the f, g and h costs and parent nodes.
* Vector2Int holds two whole numbers for x and y