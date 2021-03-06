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
    int removed = 0;
    for (int i=0; i< m_originalParticles->getParticles().size(); i++) {
        Particle* pos = m_originalParticles->getParticles()[i];
        if (!model->isInVoid(pos->getPos())) {
            m_modelParticles.append(pos->getPos());
        } else {
            removed += 1;
        }
    }
    model->stop();
    calculateStatistics(m_modelParticles,m_modelData);
    qDebug() << "N_data: " << m_dataParticles.size() << ", N_model: " << m_modelParticles.size() << " (removed " << removed << " from " << m_originalParticles->getParticles().size() << ", porosity: " << removed / float(m_originalParticles->getParticles().size()) << ")";
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

void ParticleLikelihood::setDataInput(Particles *dataParticles) {
    dataParticles->appendToQVector3DList(m_dataParticles);
    calculateStatistics(m_dataParticles, m_data);
    if (debugGraphs) m_data.SaveText("data.txt");
}

QVector<QVector3D> &ParticleLikelihood::dataParticles()
{
    return m_dataParticles;
}

void ParticleLikelihood::setOriginalInput(Particles *originalParticles) {
    m_originalParticles = originalParticles;
}
