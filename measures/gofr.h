#ifndef GOFR_H
#define GOFR_H

#include "measure.h"
int intRandom(const int & min, const int & max);

class GOfR : public Measure
{
    Q_OBJECT
public:
    explicit GOfR(QObject *parent = nullptr);
    void capY(float min, float max);
    void setXRange(float min, float max);
    QVector<QPointF> m_histogram;
    virtual void compute(const QVector<QVector3D> &points) override;
    virtual QVector<QPointF> histogram(int bins) override;

    int numBins() const;
    void setNumBins(int numBins);

    float cutoff() const;
    void setCutoff(float cutoff);

private:
    int m_numBins;
    int m_maximumNumberOfPoints;
    float m_cutoff;
    float m_dr;
};

#endif // GOFR_H
