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
    if (FirstRun)
    {
        FirstRun = false;

        // starting parameters
        auto sideOver2 = w->GetSize() / 2;
        stack.push_back({ -sideOver2, -sideOver2, w->GetSize(), w->GetSize(), ChooseOrientation(w->GetSize(), w->GetSize()) });

        // reset map to no walls
        Node blank = Node(false, false, false, false);
        for (int y = -sideOver2; y <= sideOver2; y++)
        {
            for (int x = -sideOver2; x <= sideOver2; x++)
            {
                w->SetNodeColor({ x,y }, Color::Black);
                w->SetNode({ x, y }, blank);
            }

            // fix outline on horizontal edge
            w->SetEast({ sideOver2, y }, true);
            w->SetWest({ -sideOver2, y }, true);

        }
        // fix the top and bottom rows
        for (int x = -sideOver2; x <= sideOver2; x++)
        {
            w->SetSouth({ x, sideOver2 }, true);
            w->SetNorth({ x, -sideOver2 }, true);
        }

    }
    else if (stack.empty())
    {
        return false;
    }
    
    // get next recursion
    DivideParameters p = stack.back();
    stack.pop_back();

    if (p.w < 2 || p.h < 2)
        return true;

    // where will the wall be drawn from
    int wx = p.x + (p.orientation ? 0 : Random::Range(1, p.w - 2));


    int rando = Random::Range(1, p.h - 2);
    int wy = p.y + (p.orientation ? rando : 0);
    std::cout << wy << " = " << p.y << " + (" << p.orientation << " ? " << rando << " : 0)\n";

    // where will the passage through the wall exist ?
    int px = wx + (p.orientation ? Random::Range(1, p.w) : 0);
    int py = wy + (p.orientation ? 0 : Random::Range(1, p.h));

        // what direction will the wall be drawn ?
    int dx = p.orientation ? 1 : 0;
    int dy = p.orientation ? 0 : 1;

    std::cout << p.x << " " << p.y << " " << p.orientation << " " << wx << " " << wy << " " << px << " " << py << std::endl;

        // how long will the wall be ?
    int length = p.orientation ? p.w : p.h;

        // what direction is perpendicular to the wall ?

    for (int i = 0; i < length; i++)
    {
        if (wx != px || wy != py)
        {
            if (p.orientation)
                w->SetSouth({ wx,wy }, true);
            else
                w->SetEast({ wx,wy }, true);
        }
        wx += dx;
        wy += dy;
    }

    if (p.orientation)
    {
        stack.push_back({ p.x, p.y, p.w, wy - p.y + 1, ChooseOrientation(p.w, wy - p.y + 1) });

        stack.push_back({ p.x, wy + 1, p.w, p.y + p.h - wy - 1, ChooseOrientation(p.w, p.y + p.h - wy - 1) });
    }
    else
    {
        stack.push_back({ p.x,p.y, wx-p.x+1, p.h, ChooseOrientation(wx - p.x + 1, p.h) });

        stack.push_back({ wx+1, p.y, p.x+p.w-wx-1, p.h, ChooseOrientation(p.w,p.y + p.h - wy - 1) });

    }

    return true;
}
void RecursiveDivision::Clear(World* world) {
  visited.clear();
  stack.clear();
  auto sideOver2 = world->GetSize()/2;
  FirstRun = true;

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
