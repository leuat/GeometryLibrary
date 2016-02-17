#ifndef REGULARNOISEMODEL_H
#define REGULARNOISEMODEL_H

#include "model.h"
#include "../noise.h"
class RegularNoiseModel : public Model
{
protected:
    Noise m_noise;
public:
    RegularNoiseModel();

    // Model interface
public:
    virtual void initialize(Noise::NoiseType noiseType, Parameters *np) override;
    virtual bool isInVoid(float x, float y, float z);
};

#endif // REGULARNOISEMODEL_H
