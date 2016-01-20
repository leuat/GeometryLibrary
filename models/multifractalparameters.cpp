#include "multifractalparameters.h"

MultiFractalParameters::MultiFractalParameters(double octaves, double scale, double persistence, double threshold, double inverted, double seed, double absolute, double lacunarity, double gain, double offset)
{
    createParameter("Octaves", octaves, 1, 7, 1);
    createParameter("Scale", scale, 0.01, 5, 0.1);
    createParameter("Persistence", persistence, 0.1, 3, 0.1);
    createParameter("Threshold", threshold, -1, 1, 0.1);
    createParameter("Inverted", inverted, 1, 1);
    createParameter("Seed", seed, 1, 100, 1);
    createParameter("Absolute", absolute, 0, 1, 1);
    createParameter("Lacunarity", lacunarity, 0.1, 3, 0.1);
    createParameter("Gain", gain, 0, 2, 0.1);
    createParameter("Offset", offset, 0, 2, 0.1);
}

MultiFractalParameters::MultiFractalParameters() :
    MultiFractalParameters::MultiFractalParameters(1.0, 1.0, 1.0, 0.1, 1.0, 13.0, 0.0, 2.2, 1.5, 0.6)
{

}
