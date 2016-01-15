#ifndef GEOMETRYMODEL_H
#define GEOMETRYMODEL_H
#include "noise.h"
#include "parameters.h"
#include <QVector3D>

class GeometryModel
{
protected:
    Noise* m_noise;
    Parameters* m_parameters;
    bool m_initialized = false;

public:
    GeometryModel();

    virtual void Initialize(Noise::NoiseType noiseType, Parameters* np) = 0;

    virtual bool IsInVoid(const QVector3D& pos) = 0;


};

#endif // GEOMETRYMODEL_H
