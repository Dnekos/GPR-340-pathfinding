#include "RecursiveDivision.h"
#include "../World.h"
#include "Random.h"
#include <climits>

// returns true if it should be Horizontal
bool RecursiveDivision::ChooseOrientation(int width, int height)
{
    if (width < height)
    {
        return HORIZONTAL;
    }
    else if (height > width)
    {
        return VERTICAL;
    }
    else
        return Random::Range(0, 1) == 0;
}

bool RecursiveDivision::Step(World* w) 
{
    DivideParameters p;
    if (stack.empty())
    {
       // p = 
    }
     
    return true;
}
void RecursiveDivision::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize()/2;

  for(int i=-sideOver2;i<=sideOver2;i++){
    for(int j=-sideOver2;j<=sideOver2;j++){
      visited[i][j] = false;
    }
  }
}
Point2D RecursiveDivision::randomStartPoint(World* world) {
  auto sideOver2 = world->GetSize()/2;

  for(int y=-sideOver2; y<=sideOver2; y++)
    for(int x=-sideOver2; x<=sideOver2; x++)
    //  if(!visited[y][x])
        return {x,y};
  return {INT_MAX, INT_MAX};
}
