#ifndef NOISEPARAMETERS_H
#define NOISEPARAMETERS_H
#include "parameters.h"

class NoiseParameters : public Parameters
{
    Q_OBJECT
public:
    NoiseParameters();
    NoiseParameters(double octaves, double scale, double persistence, double threshold, double inverted, double seed, double absolute, double skewScale, double skewAmplitude);
    NoiseParameters(double octaves, double scale, double persistence, double threshold, double inverted, double seed, double absolute);
};

#endif // NOISEPARAMETERS_H
