#ifndef PARTICLELIKELIHOOD_H
#define PARTICLELIKELIHOOD_H

#include "likelihood.h"
#include "../particle.h"

class ParticleLikelihood : public Likelihood
{
public:
    ParticleLikelihood();
    virtual void calculateModel(class Model *model) override;
    int numberOfHistogramBins() const;
    void setNumberOfHistogramBins(int numberOfHistogramBins);
    bool normalize() const;
    void setNormalize(bool normalize);

    void setOriginalInput(Particles *originalParticles);
    void setDataInput(Particles *dataParticles);
protected:
    int m_numberOfHistogramBins;
    bool m_normalize;
    virtual void calculateStatistics(QVector<QVector3D> &points, LGraph& graph) = 0;
    QVector<QVector3D> m_modelParticles;
    Particles* m_originalParticles;
    QVector<QVector3D> m_dataParticles;
};

#endif // PARTICLELIKELIHOOD_H
