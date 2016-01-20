#ifndef GEOMETRYMODEL_H
#define GEOMETRYMODEL_H
#include "../noise.h"
#include "parameters.h"
#include <QVector3D>

class Model
{
protected:
    Noise* m_noise;
    Parameters* m_parameters;
    bool m_initialized = false;

public:
    Model();

    virtual void initialize(Noise::NoiseType noiseType, Parameters* np) = 0;

    virtual bool isInVoid(const QVector3D& pos) = 0;


};

#endif // GEOMETRYMODEL_H
