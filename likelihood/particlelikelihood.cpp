#include "particlelikelihood.h"
#include "../models/model.h"
ParticleLikelihood::ParticleLikelihood()
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
