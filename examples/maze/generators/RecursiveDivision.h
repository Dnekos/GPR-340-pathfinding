#ifndef RECURSIVEBACKTRACKER_H
#define RECURSIVEBACKTRACKER_H

#include "../MazeGeneratorBase.h"
#include "Point2D.h"
#include <map>
#include <vector>

#define HORIZONTAL true;
#define VERTICAL false;

class RecursiveDivision: public MazeGeneratorBase {
 private:
  std::vector<DivideParameters> stack;
  std::map<int, std::map<int, bool>> visited; // naive. not optimal

  bool ChooseOrientation(int width, int height);
  std::vector<Point2D> getVisitables(World* w, const Point2D& p);
 public:
  RecursiveBacktracker() = default;
  std::string GetName() override {return "Recursive Back-Tracker";};
  bool Step(World * world) override;
  void Clear(World * world) override;
};

struct DivideParameters
{
	int x, y, w, h;
	bool orientation;
};

#endif  // RECURSIVEBACKTRACKER_H
