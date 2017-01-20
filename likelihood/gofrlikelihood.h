#ifndef GOFRLIKELIHOOD_H
#define GOFRLIKELIHOOD_H
#include "../measures/gofr.h"
#include "GeometryLibrary/likelihood/particlelikelihood.h"

class GOfRLikelihood : public ParticleLikelihood
{
public:
    GOfRLikelihood();
    float cutoff() const;
    void setCutoff(float cutoff);

private:
    float m_cutoff;
    GOfR m_gOfr;
    // ParticleLikelihood interface
protected:
    virtual void calculateStatistics(QVector<QVector3D> &points, LGraph &graph) override;
};

#endif // GOFRLIKELIHOOD_H
