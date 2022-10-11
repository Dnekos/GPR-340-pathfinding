#include "Catcher.h"
#include "World.h"

Point2D Catcher::Move(World* world) {
	// find an edge
	Point2D edge = GetClosestEdge(world, world->getCat());

	// if there is an empty edge, do Astar
	if (!world->getContent(edge))
		return DoPathfinding(world, edge, world->getCat(), true);
	else // if there isnt an empty edge, chase the cat by picking a neighbor
	{
		// get neighbor of cat
		Point2D chosenPos;
		std::vector<Point2D> neighbors = world->GetNeighbors(world->getCat());
		int redundancy = 0; // redundancy breaks loop after 100 times, to prevent getting stuck

		// loop for cat neighbor that is free
		do
			chosenPos = neighbors[Random::Range(0, 5)];
		while ((!world->isValidPosition(chosenPos) || world->getContent(chosenPos)) && redundancy++ < 100);
		
		// if we did suceed in finding a neighbor, return, else get random point
		if (world->isValidPosition(chosenPos) && !world->getContent(chosenPos))
			return chosenPos;
		
		// if all else fails, get random point
		auto side = world->getWorldSideSize() / 2;
		for (;;) {
			Point2D p = { Random::Range(-side, side), Random::Range(-side, side) };
			auto cat = world->getCat();
			if (cat.x != p.x && cat.y != p.y && !world->getContent(p))
				return p;
		}
		
	}
}
