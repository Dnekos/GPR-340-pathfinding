#ifndef RECURSIVEDIVISION_H
#define RECURSIVEDIVISION_H

#include "../MazeGeneratorBase.h"
#include "Point2D.h"
#include <map>
#include <vector>

#define HORIZONTAL true;
#define VERTICAL false;

struct DivideParameters;

class RecursiveDivision : public MazeGeneratorBase 
{
private:
	std::vector<DivideParameters> stack;
	bool ChooseOrientation(int width, int height);
	bool FirstRun = true;
public:
	RecursiveDivision() = default;
	std::string GetName() override {
		return "Recursive Division";
	};
	bool Step(World* world) override;
	void Clear(World* world) override;
};

struct DivideParameters
{
	int x, y, w, h;
	bool orientation;
};

#endif  // RECURSIVEDIVISION_H
