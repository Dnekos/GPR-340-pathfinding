#include "ErosionGenerator.h"
#include "../Noise.h"
#include "../PerlinNoise.hpp"
#include <iostream>
// do not use this one to your assignment. this is my sample generator
std::vector<Color32> ErosionGenerator::Generate(int sideSize, float displacement)
{
    std::vector<Color32> colors;
    std::vector<Color32> Thermalcolors;


    //   create your own function for noise generation
    siv::BasicPerlinNoise<float> noise;
    noise.reseed(1337);
    //  Noise noise(1337, 1024,0,255);
    for (int l = 0; l < sideSize; l++)
    {
        for (int c = 0; c < sideSize; c++)
        {
            float rgb = abs(noise.octave3D(c / 50.0, l / 50.0, displacement, 4) * 255);
            colors.emplace_back(rgb, rgb, rgb);
            Thermalcolors.emplace_back(rgb, rgb, rgb);
        //      double color = noise.noise(c/50.0,l/50.0);
        //      colors.emplace_back(color,color,color);
        }
    }

    // should be a parameter
    float threshold = 20;
    Vector3 neighbors[4] = { {0,1,0},{1,0,0},{0,-1,0},{-1,0,0} };

    for (int l = 1; l < sideSize - 1; l++)
    {
        for (int c = 1; c < sideSize - 1; c++)
        {
            float height = colors[l * sideSize + c].r;
            float limit = height - threshold;

            for (int i = 0; i < 4; i++)
            {
                float nx = l + neighbors[i].x;
                float ny = c + neighbors[i].y;
                float nHeight = colors[nx * sideSize + ny].r;

                // is the neighbor below the threshold?
                if (nHeight < limit)
                {
                    //std::cout << "height " << height <<" "<< nHeight << std::endl;

                    // some of the height moves, from 0  to 1/4 of the threshold, depending on height difference
                    float delta = (limit - nHeight) / threshold;
                    if (delta > 2)
                        delta = 2;
                    float change = delta * threshold * 0.125f;

                    // write to the copy
                    Thermalcolors[l * sideSize + c] = Color32(Thermalcolors[l * sideSize + c].r - change, Thermalcolors[l * sideSize + c].r - change, Thermalcolors[l * sideSize + c].r - change);
                    Thermalcolors[nx * sideSize + ny] = Color32(Thermalcolors[nx * sideSize + ny].r + change, Thermalcolors[nx * sideSize + ny].r + change, Thermalcolors[nx * sideSize + ny].r + change);


                }
            }

        }
    }

    std::cout << colors.size() << std::endl;
    return Thermalcolors;
}
std::string ErosionGenerator::GetName() {
    return "Thermal Erosion";
}
