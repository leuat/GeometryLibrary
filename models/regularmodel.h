#ifndef REGULARMODEL_H
#define REGULARMODEL_H
#include "model.h"

class RegularNoiseModel : public Model
{
protected:
    double m_scale = 1;
    double m_seed = 0;
    double m_threshold = 0;
    double m_skewScale = 1;
    double m_skewAmplitude = 0;
    bool m_inverted = false;
    bool m_absolute = false;

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

#endif // REGULARMODEL_H
