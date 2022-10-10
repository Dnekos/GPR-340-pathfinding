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
	// arbitrary high number we dont expect to see
	const float DefaultDist = 9999;

	// we seperate the closest free edge and closest edge so that the agent can still have a path if all edges are blocked
	Point2D ClosestEdge;
	float ClosestEdgeDist = DefaultDist; 

	Point2D ClosestFreeEdge;
	float ClosestFreeEdgeDist = DefaultDist;

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
	if (ClosestFreeEdgeDist != DefaultDist)
		return ClosestFreeEdge;
	else
		return ClosestEdge;
}





Point2D Agent::DoPathfinding(World* world, Point2D startingPos, Point2D endPos, bool StepAtStart)
{
	std::map<int, std::map<int, Node>> visitedMap;  // inputting a coordinates [x][y] gives the point2D of where it  came from
	std::priority_queue<QueueEntry> frontier;


	frontier.push(QueueEntry(startingPos, 0));
	visitedMap[startingPos.x][startingPos.y] = { startingPos, false, 0 };

	QueueEntry current = { Point2D(),0 };
	while (!frontier.empty()) {
		current = frontier.top();
		frontier.pop();
		visitedMap[current.position.x][current.position.y].visited = true;

		if (current.position == endPos)
			return GetNextStep(startingPos, endPos, visitedMap, StepAtStart);
		
		std::vector<Point2D> neighbors = world->GetNeighbors(current.position);
		for (int i = 0; i < neighbors.size(); i++)
		{

			float cost = visitedMap[current.position.x][current.position.y].cost + 1;
			//std::cout << "looking at point (" << neighbors[i].x << ", " << neighbors[i].y << ") ";
			
				// if it hasn't been visited, or if the current cost is more expensive than it we just calculated, AND its empty
			if (!(visitedMap.find(neighbors[i].x) != visitedMap.end() &&
				visitedMap[neighbors[i].x].find(neighbors[i].y) != visitedMap[neighbors[i].x].end()) ||
				cost < visitedMap[neighbors[i].x][neighbors[i].y].cost)
			{

				if (!world->getContent(neighbors[i]) && world->isValidPosition(neighbors[i]))
				{
					//std::cout << "pushing point (" << neighbors[i].x << "," << neighbors[i].y << ") with weight " << DiagonalManhattanDist(neighbors[i], endPos) << std::endl;
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
	if (StepAtStart)
	{
		std::cout << " catcher going from " << startingPos.x << "," << startingPos.y << " to (" << endPos.x << "," << endPos.y << ") moving to (" << startingPos.x << ", " << startingPos.y << std::endl;

		return startingPos;
	}

	Point2D current = endPos; // start at the end
	while (visitedMap[current.x][current.y].cameFrom != startingPos)// iterate backwards until we get to the step that came from StartingPosition
	{
		current = visitedMap[current.x][current.y].cameFrom;
	} 

	std::cout << "cat going from "<< startingPos.x<<","<<startingPos.y << " to ("<< endPos.x<<","<<endPos.y<<") moving to (" << current.x << ", " << current.y << std::endl;
	// return the next step
	return current;
}

float DiagonalManhattanDist(Point2D start, Point2D end)
{

	return std::max(std::abs(start.x - end.x),
		std::abs((int)(start.x + std::floor(start.x / 2)) - (int)(end.y + std::floor(end.x / 2)))
	);
	/*return std::max(std::max(
		std::abs(end.y - start.y),
		std::abs(end.x - start.x)),
		std::abs((end.x - end.y) * -1 - (start.x - start.y) * -1));*/
    /*// equation taken from https://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html
    float dx = abs(start.x - end.x);
    float dy = abs(start.y - end.y);
    return (dx + dy) + (1 - 2) * std::min(dx, dy); // Chebyshev distance formula*/
}
