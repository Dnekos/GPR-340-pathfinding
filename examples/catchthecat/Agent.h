#ifndef AGENT_H
#define AGENT_H
#include "Point2D.h"

class World;

class Agent {
 public:
  explicit Agent() = default;
  
  virtual Point2D Move(World*) = 0;
  void DoPathfinding(World * world, Point2D startingPos);
};


struct Node
{
	Point2D cameFrom;
	bool visited = false;
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
