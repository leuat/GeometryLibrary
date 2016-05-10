#ifndef REGULARNOISEMODEL_H
#define REGULARNOISEMODEL_H

#include "model.h"
class RegularNoiseModel : public Model
{
    Q_OBJECT
protected:
    class Noise *m_noise = nullptr;
    double m_octaves = 1;
    double m_scale = 1;
    double m_persistence = 1;
    double m_threshold = 1;
    double m_inverted = 1;
    double m_seed = 1;
    double m_absolute = 1;
    double m_skewScale = 1;
    double m_skewAmplitude = 1;
    double m_steepness = 1.0;
    QString m_noiseType;
public:
    RegularNoiseModel();

    // Model interface
public:
    virtual bool isInVoid(float x, float y, float z) override;
    virtual float getValue(float x, float y, float z) override;
    virtual void parametersUpdated() override;
    virtual void createParameters() override;
    virtual void loadParameters(class CIniFile *iniFile) override;
    virtual void start() override;
    virtual void stop() override;
    virtual void randomWalk() override;
};
#endif // REGULARNOISEMODEL_H
