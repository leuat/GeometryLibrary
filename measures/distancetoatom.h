#pragma once
#include "measure.h"
#include <vector>
#include <QVector>
#include <random>
#include <QVector3D>
using std::vector;
float floatRandom(const float & min, const float & max);

class DistanceToAtom : public Measure
{
    Q_OBJECT
private:
    int m_numberOfRandomVectors;
    float m_cutoff;
    bool m_isValid = false;
    QVector<float> m_values;
    QVector<float> m_randomNumbers;
public:
    explicit DistanceToAtom(QObject *parent = nullptr);
    void compute(const QVector<QVector3D> &points, bool printResult = false) override;
    bool isValid();
    QVector<QPointF> histogram(int bins);
    int numberOfRandomVectors() const;
    void setNumberOfRandomVectors(int numberOfRandomVectors);
    float cutoff() const;
    void setCutoff(float cutoff);
};
