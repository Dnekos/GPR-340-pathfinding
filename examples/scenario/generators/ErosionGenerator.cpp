#include "ErosionGenerator.h"
#include "../FastNoiseLite.h"
#include "../PerlinNoise.hpp"
#include <iostream>
#include <algorithm>

// do not use this one to your assignment. this is my sample generator
std::vector<Color32> ErosionGenerator::Generate(int sideSize, float displacement, float other_param)
{
    std::vector<Color32> colors;
    std::vector<Color32> Thermalcolors;


    //   create your own function for noise generation
    FastNoiseLite base;
    base.SetFractalOctaves(3);
    base.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    base.SetFractalType(FastNoiseLite::FractalType_FBm);
    FastNoiseLite cellular;
    cellular.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    cellular.SetFractalOctaves(3);
    for (int l = 0; l < sideSize; l++) {
        for (int c = 0; c < sideSize; c++) {
            float c1 = ((base.GetNoise((float)c, (float)l, displacement * 50) + 1) / 2) * 255;
            float c2 = ((cellular.GetNoise((float)c, (float)l, displacement * 50) + 1) / 2) * 255;
            auto avg = (c1 + c2) / 2;
            colors.emplace_back(avg, avg, avg);
            Thermalcolors.emplace_back(avg, avg, avg);
        }
    }

    // should be a parameter
    float threshold = other_param;
    std::cout << "thresh " << threshold << std::endl;

    // create our neighbors
    std::vector<Vector3> neighbors;
    for (float nLine = -5; nLine < 5; nLine++)
        for (float nCol = -5; nCol < 5; nCol++)
            neighbors.push_back({ nLine, nCol, 0 });

    int changed = 0;
    for (int l = 5; l < sideSize - 5; l++)
    {
        for (int c = 5; c < sideSize - 5; c++)
        {
            float height = colors[l * sideSize + c].r;
            float limit = height - threshold;

            for (int i = 0; i < neighbors.size(); i++)
            {
                float nx = l + neighbors[i].x;
                float ny = c + neighbors[i].y;
                float nHeight = colors[nx * sideSize + ny].r;

                // is the neighbor below the threshold?
                if (nHeight < limit)
                {
                    changed++;
                    //std::cout << "height " << height <<" "<< nHeight << std::endl;

                    // some of the height moves, from 0 to 1/# of neighbor of the threshold, depending on height difference
                    float delta = (limit - nHeight) / threshold;
                    if (delta > 2)
                        delta = 2;
                    float change = delta * threshold / ((neighbors.size() - 1) * 2);

                    // write to the copy
					float neg_change = std::max(0.0f, Thermalcolors[l * sideSize + c].r - change);
					float pos_change = std::max(0.0f, Thermalcolors[nx * sideSize + ny].r + change);

                    Thermalcolors[l * sideSize + c] = Color32(neg_change, neg_change, neg_change);
                    Thermalcolors[nx * sideSize + ny] = Color32(pos_change, pos_change, pos_change);
                }
            }
        }
    }

    std::cout << "changed " << changed << std::endl;
    return Thermalcolors;
}
std::string ErosionGenerator::GetName() {
    return "Thermal Erosion";
}
