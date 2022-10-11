#include "Agent.h"
#include <map>
#include <queue>
#include "World.h"
#include "Point2D.h"
#include <iostream>

float DiagonalManhattanDist(Point2D start, Point2D end);
Point2D GetNextStep(Point2D startingPos, Point2D endPos, std::map<int, std::map<int, Node>> visitedMap, bool StepAtStart);

Point2D Agent::GetClosestEdge(World* world, Point2D start)
{
	// we seperate the closest free edge and closest edge so that the agent can still have a path if all edges are blocked
	Point2D ClosestEdge;
	float ClosestEdgeDist = INT_MAX;

	Point2D ClosestFreeEdge;
	float ClosestFreeEdgeDist = INT_MAX;

	for (int x = -world->getWorldSideSize() * 0.5f; x < world->getWorldSideSize() * 0.5f; x++)
	{
		for (int y = -world->getWorldSideSize() * 0.5f; y <= world->getWorldSideSize() * 0.5f; y += world->getWorldSideSize() - 1)
		{
			Point2D currentPoint = { x,y };
			float currentDist = DiagonalManhattanDist(start, currentPoint);
			//std::cout << "distance = " << currentDist;
			if (!world->getContent(currentPoint) && currentDist < ClosestFreeEdgeDist)
			{
				ClosestFreeEdgeDist = currentDist;
				ClosestFreeEdge = currentPoint;
			}
			else if (world->getContent(currentPoint) && currentDist < ClosestEdgeDist)
			{
				ClosestEdgeDist = currentDist;
				ClosestEdge = currentPoint;
			}

			// now we do the same inverted
			currentPoint = { y,x };
			currentDist = DiagonalManhattanDist(start, currentPoint);
			if (!world->getContent(currentPoint) && currentDist < ClosestFreeEdgeDist)
			{
				ClosestFreeEdgeDist = currentDist;
				ClosestFreeEdge = currentPoint;
			}
			else if (world->getContent(currentPoint) && currentDist < ClosestEdgeDist)
			{
				ClosestEdgeDist = currentDist;
				ClosestEdge = currentPoint;
			}
		}
	}


	// check to see if we did actually find a free edge, and if so, return it. Else, return the closest regardless
	if (ClosestFreeEdgeDist != INT_MAX)
		return ClosestFreeEdge;
	else
		return ClosestEdge;
}





Point2D Agent::DoPathfinding(World* world, Point2D startingPos, Point2D endPos, bool StepAtStart)
{
	// astar pathfinding

	// inputting a coordinates [x][y] gives the point2D of where it  came from
	std::map<int, std::map<int, Node>> visitedMap;  
	std::priority_queue<QueueEntry> frontier;

	// add starting position to queue
	frontier.push(QueueEntry(startingPos, 0));
	visitedMap[startingPos.x][startingPos.y] = { startingPos, false, 0 };

	QueueEntry current = { Point2D(), 0 };
	while (!frontier.empty()) {
		current = frontier.top();
		frontier.pop();
		visitedMap[current.position.x][current.position.y].visited = true;

		if (current.position == endPos)
			return GetNextStep(startingPos, endPos, visitedMap, StepAtStart);
		
		std::vector<Point2D> neighbors = world->GetNeighbors(current.position);
		for (int i = 0; i < neighbors.size(); i++)
		{
			// get the cost *from* the starting position
			float cost = visitedMap[current.position.x][current.position.y].cost + 1;
			
				// if it hasn't been visited, or if the current cost is more expensive than it we just calculated, AND its empty
			if (!(visitedMap.find(neighbors[i].x) != visitedMap.end() &&
				visitedMap[neighbors[i].x].find(neighbors[i].y) != visitedMap[neighbors[i].x].end()) ||
				cost < visitedMap[neighbors[i].x][neighbors[i].y].cost)
			{
				// double check the position is good
				if (!world->getContent(neighbors[i]) && world->isValidPosition(neighbors[i]))
				{
					frontier.push(QueueEntry(neighbors[i], cost + DiagonalManhattanDist(neighbors[i], endPos)));
						visitedMap[neighbors[i].x][neighbors[i].y] = { current.position, false, cost };
				}
			}

		}
	}

	// if we didnt find our destination, still make a step towards the last point calculated
	return GetNextStep(startingPos, current.position, visitedMap, StepAtStart);

}

Point2D GetNextStep(Point2D startingPos, Point2D endPos, std::map<int, std::map<int, Node>> visitedMap, bool StepAtStart)
{
	// for catcher, the starting position is valid, so return it immediatly
	if (StepAtStart)
		return startingPos;

	Point2D current = endPos; // start at the end
	while (visitedMap[current.x][current.y].cameFrom != startingPos)// iterate backwards until we get to the step that came from StartingPosition
	{
		current = visitedMap[current.x][current.y].cameFrom;
	} 

	// return the next step
	return current;
}

float DiagonalManhattanDist(Point2D start, Point2D end)
{
	// really basic and probably inefficient distance function
	return sqrt(((start.x - end.x) * (start.x - end.x)) + ((start.y - end.y) * (start.y - end.y)));
}
