#ifndef GOFR_H
#define GOFR_H

#include "measure.h"

class GOfR : public Measure
{
    Q_OBJECT
public:
    explicit GOfR(QObject *parent = nullptr);
    // QVector<QPointF> histogram(bool cap = false, float min = 0, float max = 0);
    QVector<QPointF> m_histogram;
    virtual void compute(const QVector<QVector3D> &points) override;
    virtual QVector<QPointF> histogram(int bins) override;

    int numBins() const;
    void setNumBins(int numBins);

    float cutoff() const;
    void setCutoff(float cutoff);

private:
    int m_numBins;
    float m_cutoff;
    float m_dr;
};

#endif // GOFR_H
