#ifndef MULTIFRACTALPARAMETERS_H
#define MULTIFRACTALPARAMETERS_H
#include "parameters.h"

class MultiFractalParameters : public Parameters
{
public:
    MultiFractalParameters();
    MultiFractalParameters(double octaves, double scale, double persistence, double threshold, double inverted, double seed, double absolute,
                           double lacunarity, double gain, double offset);

};

#endif // MULTIFRACTALPARAMETERS_H
