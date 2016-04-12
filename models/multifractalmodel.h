#ifndef MULTIFRACTALMODEL_H
#define MULTIFRACTALMODEL_H
#include "regularnoisemodel.h"

class MultiFractalModel : public RegularNoiseModel
{
protected:
    double m_lacunarity, m_gain, m_offset;

    MultiFractalModel();

    // Model interface
public:
    virtual bool isInVoid(float x, float y, float z) override;
    virtual void parametersUpdated() override;
    virtual void createParameters() override;
    void loadParameters(CIniFile *iniFile) override;

};
#endif // REGULARNOISEMODEL_H
