#include "noiseparameters.h"

NoiseParameters::NoiseParameters(double octaves, double scale, double persistence, double threshold, double inverted, double seed, double absolute, double skewScale, double skewAmplitude)
{
    createParameter("Octaves", octaves, 1, 7, 1);
    createParameter("Scale", scale, 0.01, 2.0, 0.025);
    createParameter("Persistence", persistence, 0.1, 3, 0.1);
    createParameter("Threshold", threshold, -1, 1, 0.1);
    createParameter("Inverted", inverted, 0, 1);
    createParameter("Seed", seed, 1, 100, 1);
    createParameter("Absolute", absolute, 0, 1, 1);
    createParameter("SkewScale", skewScale, 0,1,0.1);
    createParameter("SkewAmplitude", skewAmplitude, 0, 1, 0.1);
}




NoiseParameters::NoiseParameters(double octaves, double scale, double persistence, double threshold, double inverted, double seed, double absolute)
{
    createParameter("Octaves", octaves, 1, 7, 1);
    createParameter("Scale", scale, 0.01, 2, 0.025);
    createParameter("Persistence", persistence, 0.1, 3, 0.1);
    createParameter("Threshold", threshold, -1, 1, 0.1);
    createParameter("Inverted", inverted, 0, 1);
    createParameter("Seed", seed, 1, 100, 1);
    createParameter("Absolute", absolute, 0, 1, 1);
    createParameter("SkewScale", 1.0, 0,1,0.1);
    createParameter("SkewAmplitude", 0.0, 0, 1, 0.1);
}

NoiseParameters::NoiseParameters() :
    NoiseParameters::NoiseParameters(1.0, 0.05, 1.0, 0.1, 0.0, 13.0, 0.0)
{

}
