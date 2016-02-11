#ifndef REGULARMODEL_H
#define REGULARMODEL_H
#include "model.h"

class RegularModel : public Model
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
    RegularModel();
    virtual void initialize(Noise::NoiseType noiseType, Parameters* np) override;

    virtual bool isInVoid(const QVector3D& pos) override;


};

#endif // REGULARMODEL_H
