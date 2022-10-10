#ifndef AGENT_H
#define AGENT_H
#include "Point2D.h"

class World;

class Agent {
public:
	explicit Agent() = default;

	virtual Point2D Move(World*) = 0;
	Point2D DoPathfinding(World* world, Point2D startingPos, Point2D endPos, bool GiveEnd = false);
	Point2D GetClosestEdge(World* world, Point2D start);

};


struct Node
{
	Point2D cameFrom;
	bool visited = false;
	float cost;
};
struct QueueEntry {
	QueueEntry(Point2D pos, float w)
	{
		position = pos;
		weight = -w; // we make it negative so that the quere is reversed, giving the smallest
	}
	Point2D position;
	float weight;
	bool operator<(const QueueEntry & rhs) const
	{
		return weight < rhs.weight;
	}
};

#endif  // AGENT_H
