#ifndef DTALIKELIHOOD_H
#define DTALIKELIHOOD_H

#include "GeometryLibrary/likelihood/particlelikelihood.h"
#include "../particle.h"
#include "../measures/distancetoatom.h"
#include <QVector>
#include <QVector3D>

class DTALikelihood: public ParticleLikelihood
{
private:
    int m_numberOfRandomVectors = 4096;
    int m_histogramBins = 10;
    double m_cutoff = 25;
    DistanceToAtom m_da;

public:
    DTALikelihood();

    LGraph calculateStatisticsDirect(Particles& particles);
    void calculateStatistics(QVector<QVector3D>& particleList, LGraph& graph);
    int histogramBins() const;
    void setHistogramBins(int histogramBins);

    // Setters and getters
    double cutoff() const;
    void setCutoff(double cutoff);
    int numberOfRandomVectors() const;
    void setNumberOfRandomVectors(int numberOfRandomVectors);
    void setDataInput(Particles* dataParticles);
    void setOriginalInput(Particles* originalParticles);
    QVector<QPointF> currentModel() const;
    QVector<QPointF> currentData() const;
};

#endif // DTALIKELIHOOD_H
