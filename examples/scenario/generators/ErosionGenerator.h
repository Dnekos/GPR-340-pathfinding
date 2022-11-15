#ifndef MOBAGEN_EROSIONSCENARIOGENERATOR_H
#define MOBAGEN_EROSIONSCENARIOGENERATOR_H

#include "../GeneratorBase.h"

class ErosionGenerator : public ScenarioGeneratorBase {
public:
	std::vector<Color32> Generate(int sideSize, float displacement = 0) override;
	std::string GetName() override;
};

#endif  // MOBAGEN_EROSIONSCENARIOGENERATOR_H
