#ifndef GOFR_H
#define GOFR_H

#include "measure.h"

class GOfR
{
public:
    GOfR();
    void compute(const QVector<QVector3D> &points, float cutoff, int numBins);
    QVector<QPointF> histogram(bool cap = false, float min = 0, float max = 0);
    QVector<QPointF> m_histogram;
    float m_dr;
    int m_numBins;
};

#endif // GOFR_H
