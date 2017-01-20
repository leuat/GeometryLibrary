#ifndef GOFR_H
#define GOFR_H

#include "measure.h"

class GOfR : public Measure
{
    Q_OBJECT
public:
    explicit GOfR(QObject *parent = nullptr);
    void compute(const QVector<QVector3D> &points, float cutoff, int numBins);
    QVector<QPointF> histogram(bool cap = false, float min = 0, float max = 0);
    QVector<QPointF> m_histogram;
    float m_dr;
    int m_numBins;

    // Measure interface
public:
    virtual void compute(const QVector<QVector3D> &points) override;
    virtual QVector<QPointF> histogram(int bins) override;
};

#endif // GOFR_H
