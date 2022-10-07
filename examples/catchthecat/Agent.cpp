#include "Agent.h"
#include <map>
#include <queue>
#include "World.h"

void Agent::DoPathfinding(World * world, Point2D startingPos) {
    std::map<int, std::map<int, Node>> visitedMap;  // inputting a coordinates [x][y] gives the point2D of where it  came from
    //std::map<int, std::map<int, bool>> visited;
    std::priority_queue<QueueEntry> frontier;


    //visited[startingPos.x][startingPos.y] = true;
    //AddNeighbors(world, startingPos, 0, frontier, visited);
    frontier.push({ startingPos ,0 });

    int weight = 0;
    while (!frontier.empty())
    {
        QueueEntry current = frontier.top();
        frontier.pop();
        visitedMap[startingPos.x][startingPos.y].visited = true;

        AddNeighbors(world, current.position, weight, frontier, visitedMap);
    }
}

void AddNeighbors(World* world, Point2D p, float weight, std::priority_queue<QueueEntry>& frontier, std::map<int, std::map<int, Node>> visited)
{
    std::vector<Point2D> neighbor = world->GetNeighbors(p);
    for (int i = 0; i < neighbor.size(); i++)
    {
        if (visited.find(neighbor[i].x) != visited.end() && visited[neighbor[i].x].find(neighbor[i].y) != visited[neighbor[i].x].end()
            && !visited[neighbor[i].x][neighbor[i].y].visited && !world->getContent(neighbor[i]))
        {
            frontier.push(QueueEntry(p, weight));//+ DiagonalManhattanDist(p, p)));
            visited[neighbor[i].x][neighbor[i].y].cameFrom = p;
        }
    }
}
float DiagonalManhattanDist(Point2D start, Point2D end)
{
    // equation taken from https://theory.stanford.edu/~amitp/GameProgramming/Heuristics.html
    float dx = abs(start.x - end.x);
    float dy = abs(start.y - end.y);
    return D * (dx + dy) + (D2 - 2 * D) * std::min(dx, dy);
}
