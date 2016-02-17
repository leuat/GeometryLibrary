#ifndef REGULARNOISEMODEL_H
#define REGULARNOISEMODEL_H

#include "model.h"
#include "../noise.h"
class RegularNoiseModel : public Model
{
protected:
    Noise *m_noise = nullptr;
    double m_octaves = 1;
    double m_scale = 1;
    double m_persistence = 1;
    double m_threshold = 1;
    double m_inverted = 1;
    double m_seed = 1;
    double m_absolute = 1;
    double m_skewScale = 1;
    double m_skewAmplitude = 1;
public:
    RegularNoiseModel();

    // Model interface
public:
    virtual bool isInVoid(float x, float y, float z) override;
    virtual void parametersUpdated() override;
};

class NoiseParameters : public Parameters
{
    Q_OBJECT
public:
    NoiseParameters();
    NoiseParameters(double octaves, double scale, double persistence, double threshold, double inverted, double seed, double absolute, double skewScale, double skewAmplitude);
    NoiseParameters(double octaves, double scale, double persistence, double threshold, double inverted, double seed, double absolute);
};
#endif // REGULARNOISEMODEL_H
