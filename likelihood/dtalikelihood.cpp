#include "dtalikelihood.h"
#include <QDebug>
#include "GeometryLibrary/models/models.h"

DTALikelihood::DTALikelihood() : ParticleLikelihood()
{

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
    if (points.size()==0) return;

    m_da.compute(points); // cutoff
    QVector<QPointF> hist = m_da.histogram(m_numberOfHistogramBins); // bins
    graph.fromQVector(hist);
    graph.normalizeArea();
}
