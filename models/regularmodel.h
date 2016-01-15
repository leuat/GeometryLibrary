#ifndef REGULARMODEL_H
#define REGULARMODEL_H
#include "geometrymodel.h"

class RegularModel : public GeometryModel
{
private:
    double m_scale = 1;
    double m_threshold = 0;
    bool m_inverted = false;
    bool m_absolute = false;

public:
    RegularModel();
    void Initialize(Noise::NoiseType noiseType, Parameters* np) override;

    bool IsInVoid(const QVector3D& pos) override;


};

#endif // REGULARMODEL_H
