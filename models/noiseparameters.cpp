#include "noiseparameters.h"

NoiseParameters::NoiseParameters(double octaves, double scale, double persistence, double threshold, double inverted, double seed, double absolute, double skewScale, double skewAmplitude)
{
    m_parameters.append(new Parameter(octaves, 1, "Octaves"));
    m_parameters.append(new Parameter(scale, 0.5, "Scale"));
    m_parameters.append(new Parameter(persistence, 0.2, "Persistence"));
    m_parameters.append(new Parameter(threshold, 0.1, "Threshold"));
    m_parameters.append(new Parameter(inverted, 0, "Inverted"));
    m_parameters.append(new Parameter(seed, 0, "Seed"));
    m_parameters.append(new Parameter(absolute, 0, "Absolute"));
    m_parameters.append(new Parameter(skewScale,0.1,"SkewScale"));
    m_parameters.append(new Parameter(skewAmplitude,0.1,"SkewAmplitude"));
}

NoiseParameters::NoiseParameters(double octaves, double scale, double persistence, double threshold, double inverted, double seed, double absolute)
{
    m_parameters.append(new Parameter(octaves, 1, "Octaves"));
    m_parameters.append(new Parameter(scale, 0.5, "Scale"));
    m_parameters.append(new Parameter(persistence, 0.2, "Persistence"));
    m_parameters.append(new Parameter(threshold, 0.1, "Threshold"));
    m_parameters.append(new Parameter(inverted, 0, "Inverted"));
    m_parameters.append(new Parameter(seed, 0, "Seed"));
    m_parameters.append(new Parameter(absolute, 0, "Absolute"));
    m_parameters.append(new Parameter(1,0.1,"SkewScale"));
    m_parameters.append(new Parameter(0,0.1,"SkewAmplitude"));
}

NoiseParameters::NoiseParameters()
{
    m_parameters.append(new Parameter(1, 1, "Octaves"));
    m_parameters.append(new Parameter(1, 0.5, "Scale"));
    m_parameters.append(new Parameter(1, 0.2, "Persistence"));
    m_parameters.append(new Parameter(0.1, 0.1, "Threshold"));
    m_parameters.append(new Parameter(1, 0, "Inverted"));
    m_parameters.append(new Parameter(13, 0, "Seed"));
    m_parameters.append(new Parameter(0, 0, "Absolute"));
    m_parameters.append(new Parameter(1,0.1,"SkewScale"));
    m_parameters.append(new Parameter(0,0.1,"SkewAmplitude"));
}
