#pragma once
#include <vector>
#include <QVector>
#include <random>
#include <QVector3D>
using std::vector;
float floatRandom(const float & min, const float & max);

class DistanceToAtom
{
private:
    // int m_numberOfRandomVectors = 8192;
    int m_numberOfRandomVectors = 65536;
    QVector<float> m_values;
    QVector<float> m_randomNumbers;
    float m_cutoff;
    bool m_isValid = false;
public:
    DistanceToAtom(int numberOfRandomVectors = 8192);
    void compute(const QVector<QVector3D> &points, float cutoff);
    bool isValid();
    QVector<QPointF> histogram(int bins);
};
