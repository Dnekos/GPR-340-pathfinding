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
    else if (width > height)
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

    int wx, wy, px, py, dx, dy;
    if (p.orientation)
    {
        // whats the position of the new wall
        wx = p.x;
        wy = p.y + Random::Range(0, p.h - 2);

        // where is the wall's hole
        px = wx + Random::Range(1, p.w - 1);
        py = wy;

        // what direction will the wall be drawn ?
        dx = 1;
        dy = 0;
    }
    else
    {
        // whats the position of the new wall
        wx = p.x + Random::Range(0, p.w - 2);
        wy = p.y;

        // where is the wall's hole
        px = wx;
        py = wy + Random::Range(1, p.h - 1);

        // what direction will the wall be drawn ?
        dx = 0;
        dy = 1;
    }

    // iterate through the wall
    int length = p.orientation ? p.w : p.h;
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
void RecursiveDivision::Clear(World* world) 
{
  stack.clear();
  FirstRun = true;
}
