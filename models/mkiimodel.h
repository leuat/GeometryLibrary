#ifndef MKIIMODEL_H
#define MKIIMODEL_H

#include "model.h"
class MKIIModel : public Model
{
    Q_OBJECT
protected:
    class Noise *m_noise = nullptr;
    double m_f0 = 1;
    double m_f1 = 2;
    double m_f2 = 4;
    double m_A0 = 1;
    double m_A1 = 1;
    double m_A2 = 1;
    double m_scale = 1;
    double m_threshold = 1;
    double m_inverted = 1;
    double m_seed = 1;
    double m_absolute = 1;
    double m_steepness = 1.0;
    QString m_noiseType;
public:
    MKIIModel();

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

#endif // MKIIMODEL_H
