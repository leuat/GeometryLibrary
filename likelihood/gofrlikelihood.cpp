//#include "gofrlikelihood.h"
//#include "measures/gofr.h"

//GOfRLikelihood::GOfRLikelihood() : Likelihood(),
//    m_cutoff(15), m_numBins(100)
//{

//}

//void GOfRLikelihood::calculateStatistics(QVector<QVector3D> &points, LGraph& graph)
//{
//    if (points.size()==0)
//        return;

//    GOfR gr;
//    gr.compute(points, m_cutoff, m_numBins);
//    QVector<QPointF> hist = gr.histogram(); // bins
//    graph.fromQVector(hist);
//    graph.normalizeArea();
//}

//void GOfRLikelihood::calculateModel(Model *modelData)
//{
//    m_modelParticles.clear();
//    m_originalParticles->calculateBoundingBox();

//    model->start();
//      for (int i=0;i< m_originalParticles->getParticles().size();i++) {

//         Particle* pos = m_originalParticles->getParticles()[i];
//         if (!model->isInVoid(pos->getPos())) {
//            m_modelParticles.append(pos->getPos());
//        }
//    }
//    model->stop();
//    calculateStatistics(m_modelParticles,m_modelData);
//}

//float GOfRLikelihood::cutoff() const
//{
//    return m_cutoff;
//}

//void GOfRLikelihood::setCutoff(float cutoff)
//{
//    m_cutoff = cutoff;
//}

//int GOfRLikelihood::numBins() const
//{
//    return m_numBins;
//}

//void GOfRLikelihood::setNumBins(int numBins)
//{
//    m_numBins = numBins;
//}
