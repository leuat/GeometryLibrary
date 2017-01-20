#ifndef PARTICLELIKELIHOOD_H
#define PARTICLELIKELIHOOD_H

#include "likelihood.h"
#include "../particle.h"

class ParticleLikelihood : public Likelihood
{
public:
    ParticleLikelihood();
protected:
    QVector<QVector3D> m_modelParticles;
    Particles* m_originalParticles;
    QVector<QVector3D> m_dataParticles;
};

#endif // PARTICLELIKELIHOOD_H
