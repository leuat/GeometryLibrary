#include "dtalikelihood.h"
#include <QDebug>
#include "GeometryLibrary/models/models.h"

DTALikelihood::DTALikelihood()
{

}

int DTALikelihood::histogramBins() const
{
    return m_histogramBins;
}

void DTALikelihood::setHistogramBins(int histogramBins)
{
    m_histogramBins = histogramBins;
}

double DTALikelihood::cutoff() const
{
    return m_da.cutoff();
}

void DTALikelihood::setCutoff(double cutoff)
{
    m_da.setCutoff(cutoff);
}

int DTALikelihood::numberOfRandomVectors() const
{
    return m_da.numberOfRandomVectors();
}

void DTALikelihood::setNumberOfRandomVectors(int numberOfRandomVectors)
{
    m_da.setNumberOfRandomVectors(numberOfRandomVectors);
}


void DTALikelihood::setDataInput(Particles *dataParticles) {
    dataParticles->appendToQVector3DList(m_dataParticles);
    calculateStatistics(m_dataParticles, m_data);
    if (debugGraphs) m_data.SaveText("data.txt");
}

void DTALikelihood::setOriginalInput(Particles *originalParticles) {
    m_originalParticles = originalParticles;
}

LGraph DTALikelihood::calculateStatisticsDirect(Particles &particles)
{
    QVector<QVector3D> qp;
    particles.appendToQVector3DList(qp);
    LGraph graph;
    calculateStatistics(qp, graph);
    return graph;

}

void DTALikelihood::calculateStatistics(QVector<QVector3D> &points, LGraph& graph)
{
    if (points.size()==0)
        return;

    m_da.compute(points); // cutoff
    QVector<QPointF> hist = m_da.histogram(m_histogramBins); // bins
    graph.fromQVector(hist);
    graph.normalizeArea();
}

void DTALikelihood::calculateModel(Model *model)
{
    m_modelParticles.clear();
    m_originalParticles->calculateBoundingBox();

    model->start();
    for (int i=0;i< m_originalParticles->getParticles().size();i++) {

        Particle* pos = m_originalParticles->getParticles()[i];
        if (!model->isInVoid(pos->getPos())) {
            m_modelParticles.append(pos->getPos());
        }
    }
    model->stop();
    calculateStatistics(m_modelParticles,m_modelData);
}
