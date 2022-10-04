#include "Agent.h"
#include <map>
#include <queue>
#include "World.h"

void Agent::DoPathfinding(World * world, Point2D startingPos) {
    std::map<int, std::map<int, Point2D>> hereFrom;  // inputting a coordinates [x][y] gives the point2D of where it  came from
    std::map<int, std::map<int, bool>> visited;
    std::priority_queue<QueueEntry> frontier;


    visited[startingPos.x][startingPos.y] = true;
    AddNeighbors(world, startingPos, frontier, visited);

}

void AddNeighbors(World * world, Point2D p, std::priority_queue<QueueEntry> &frontier, std::map<int, std::map<int, bool>> visited)
{
    PushIfNotVisited(world, world->E(p), frontier, visited);
    PushIfNotVisited(world, world->NE(p), frontier, visited);
    PushIfNotVisited(world, world->NW(p), frontier, visited);
    PushIfNotVisited(world, world->SE(p), frontier, visited);
    PushIfNotVisited(world, world->SW(p), frontier, visited);
    PushIfNotVisited(world, world->W(p), frontier, visited);
}
void PushIfNotVisited(World * world, Point2D p, std::priority_queue<QueueEntry> & frontier, std::map<int, std::map<int, bool>> visited)
{
    if (visited.find(p.x) != visited.end() && visited[p.x].find(p.y) != visited[p.x].end() && !world->getContent(p))
        frontier.push(p);
}
float DiagonalManhattanDist(Point2D start, Point2D end)
{
    // equation taken from https://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html
    float dx = abs(start.x - end.x);
    float dy = abs(start.y - end.y);
    return D * (dx + dy) + (D2 - 2 * D) * min(dx, dy);
}
