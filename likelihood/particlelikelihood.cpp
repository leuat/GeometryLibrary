#include "particlelikelihood.h"
#include "../models/model.h"
ParticleLikelihood::ParticleLikelihood() : Likelihood(),
    m_numberOfHistogramBins(100), m_normalize(true)
{

}


void ParticleLikelihood::calculateModel(Model *model)
{
    m_modelParticles.clear();
    m_originalParticles->calculateBoundingBox();

    model->start();
    for (int i=0; i< m_originalParticles->getParticles().size(); i++) {
        Particle* pos = m_originalParticles->getParticles()[i];
        if (!model->isInVoid(pos->getPos())) {
            m_modelParticles.append(pos->getPos());
        }
    }
    model->stop();
    calculateStatistics(m_modelParticles,m_modelData);
}

int ParticleLikelihood::numberOfHistogramBins() const
{
    return m_numberOfHistogramBins;
}

void ParticleLikelihood::setNumberOfHistogramBins(int numberOfHistogramBins)
{
    m_numberOfHistogramBins = numberOfHistogramBins;
}

bool ParticleLikelihood::normalize() const
{
    return m_normalize;
}

void ParticleLikelihood::setNormalize(bool normalize)
{
    m_normalize = normalize;
}
