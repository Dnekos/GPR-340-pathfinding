#include "Cat.h"
#include "World.h"
#include <stdexcept>
#include <queue>

Point2D Cat::Move(World* world)
{
	// simply return the next step towards an edge, GetClosestEdge does return a valid point even if its filled.
	return DoPathfinding(world, world->getCat(), GetClosestEdge(world, world->getCat()));
}
