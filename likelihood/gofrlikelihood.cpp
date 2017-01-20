#include "gofrlikelihood.h"
#include "../measures/gofr.h"

GOfRLikelihood::GOfRLikelihood() : ParticleLikelihood(),
    m_cutoff(15)
{

}

void GOfRLikelihood::calculateStatistics(QVector<QVector3D> &points, LGraph& graph)
{
    if (points.size()==0) return;

    m_gOfr.setNumBins(m_numberOfHistogramBins);
    m_gOfr.setCutoff(m_cutoff);
    m_gOfr.compute(points);
    QVector<QPointF> hist = m_gOfr.histogram(m_numberOfHistogramBins); // bins
    graph.fromQVector(hist);
    if(m_normalize) graph.normalizeArea();
}

float GOfRLikelihood::cutoff() const
{
    return m_cutoff;
}

void GOfRLikelihood::setCutoff(float cutoff)
{
    m_cutoff = cutoff;
}
