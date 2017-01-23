#include "gofrlikelihood.h"
#include "../measures/gofr.h"

GOfRLikelihood::GOfRLikelihood() : ParticleLikelihood(),
    m_cutoff(15)
{
    setNormalize(false);
}

void GOfRLikelihood::calculateStatistics(QVector<QVector3D> &points, LGraph& graph)
{
    if (points.size()==0) return;
    //qDebug() << "Bins: " << m_numberOfHistogramBins << " " <<
    m_gOfr.setNumBins(m_numberOfHistogramBins);
    m_gOfr.setCutoff(m_cutoff);
    m_gOfr.setCutoff(15);
    m_gOfr.setNumBins(250);

    m_gOfr.compute(points);
    m_gOfr.setXRange(3,1000);
    QVector<QPointF> hist = m_gOfr.histogram(m_numberOfHistogramBins); // bins
    graph.fromQVector(hist);
}

float GOfRLikelihood::cutoff() const
{
    return m_cutoff;
}

void GOfRLikelihood::setCutoff(float cutoff)
{
    m_cutoff = cutoff;
}
