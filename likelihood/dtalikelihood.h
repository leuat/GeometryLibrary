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

public:
    DTALikelihood();

    void setDataInput(Particles* dataParticles);
    void setOriginalInput(Particles* originalParticles);

    LGraph calculateStatisticsDirect(Particles& particles);
    void calculateStatistics(QVector<QVector3D>& particleList, LGraph& graph);
    QVector<QPointF> currentModel() const;
    QVector<QPointF> currentData() const;
    int histogramBins() const;
    void setHistogramBins(int histogramBins);
    double cutoff() const;
    void setCutoff(double cutoff);

    // Likelihood interface
    void calculateModel(class Model *model) override;
    int numberOfRandomVectors() const;
    void setNumberOfRandomVectors(int numberOfRandomVectors);
};

#endif // DTALIKELIHOOD_H
