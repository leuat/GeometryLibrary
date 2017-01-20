#include "dtalikelihood.h"
#include <QDebug>
#include "GeometryLibrary/models/models.h"

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

int DTALikelihood::numberOfRandomVectors() const
{
    return m_numberOfRandomVectors;
}

void DTALikelihood::setNumberOfRandomVectors(int numberOfRandomVectors)
{
    m_numberOfRandomVectors = numberOfRandomVectors;
}

DTALikelihood::DTALikelihood()
{
    
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
    DistanceToAtom da; // voxes_per_dimension
    da.setNumberOfRandomVectors(m_numberOfRandomVectors);
    da.setCutoff(m_cutoff);
    if (points.size()==0)
        return;

    da.compute(points); // cutoff
    QVector<QPointF> hist = da.histogram(m_histogramBins); // bins
//    qDebug() << m_histogramBins;
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
