#include "multifractalparameters.h"

MultiFractalParameters::MultiFractalParameters(double octaves, double scale, double persistence, double threshold, double inverted, double seed, double absolute, double lacunarity, double gain, double offset)
{
    m_parameters.append(Parameter(octaves, 1, "Octaves"));
    m_parameters.append(Parameter(scale, 0.5, "Scale"));
    m_parameters.append(Parameter(persistence, 0.2, "Persistence"));
    m_parameters.append(Parameter(threshold, 0.1, "Threshold"));
    m_parameters.append(Parameter(inverted, 0, "Inverted"));
    m_parameters.append(Parameter(seed, 0, "Seed"));
    m_parameters.append(Parameter(absolute, 0, "Absolute"));
    m_parameters.append(Parameter(lacunarity, 0.2, "Lacunarity"));
    m_parameters.append(Parameter(gain, 0.2, "Gain"));
    m_parameters.append(Parameter(offset, 0.2, "Offset"));
}

MultiFractalParameters::MultiFractalParameters()
{
    m_parameters.append(Parameter(1, 1, "Octaves"));
    m_parameters.append(Parameter(1, 0.5, "Scale"));
    m_parameters.append(Parameter(1, 0.2, "Persistence"));
    m_parameters.append(Parameter(0.1, 0.1, "Threshold"));
    m_parameters.append(Parameter(1, 0, "Inverted"));
    m_parameters.append(Parameter(13, 0, "Seed"));
    m_parameters.append(Parameter(0, 0, "Absolute"));
    m_parameters.append(Parameter(2.2, 0.2, "Lacunarity"));
    m_parameters.append(Parameter(1.5, 0.2, "Gain"));
    m_parameters.append(Parameter(0.6, 0.2, "Offset"));
}
