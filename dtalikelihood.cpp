#include "dtalikelihood.h"
#include <QDebug>
#include "GeometryLibrary/models/models.h"

int DTALikelihood::voxelsPerDimension() const
{
    return m_numberOfRandomVectors;
}

void DTALikelihood::setVoxelsPerDimension(int voxelsPerDimension)
{
    m_numberOfRandomVectors = voxelsPerDimension;
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
    return m_cutoff;
}

void DTALikelihood::setCutoff(double cutoff)
{
    m_cutoff = cutoff;
}

DTALikelihood::DTALikelihood()
{

}

LGraph DTALikelihood::calculateStatisticsDirect(Particles &particles)
{
    QVector<QVector3D> qp;
    particles.getVector3DList(qp);
    LGraph graph;
    calculateStatistics(qp, graph);
    return graph;

}

void DTALikelihood::calculateStatistics(QVector<QVector3D> &points, LGraph& graph)
{
    DistanceToAtom da(m_numberOfRandomVectors); // voxes_per_dimension
    if (points.size()==0)
        return;
    da.compute(points, m_cutoff); // cutoff
    QVector<QPointF> hist = da.histogram(m_histogramBins); // bins
    graph.fromQVector(hist);
    graph.normalizeArea();

}

void DTALikelihood::calculateModel(Model *model)
{
    m_modelParticles.clear();
    m_originalParticles->calculateBoundingBox();
    model->start();
    for (Particle* pos : m_originalParticles->getParticles()) {
        if (!model->isInVoid(pos->getPos())) {
            m_modelParticles.append(pos->getPos());
        }
    }
    model->stop();
    calculateStatistics(m_modelParticles,m_modelData);
}
