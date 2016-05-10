#pragma once
#include <vector>
#include <QVector>
#include <random>
#include <QVector3D>
using std::vector;

typedef vector<vector<vector<vector<QVector3D> > > > CellList;
template<class T>
bool checkRange(T value, T min, T max) {
    return (value >= min) && (value <= max);
}
float floatRandom(const float & min, const float & max);

class DistanceToAtom
{
private:
    // int m_numberOfRandomVectors = 8192;
    int m_numberOfRandomVectors = 65536;
    QVector<float> m_values;
    QVector<float> m_randomNumbers;
    bool m_isValid = false;
    inline int periodic(int idx, int count) { return (idx + count) % count; }
    CellList buildCellList(const QVector<QVector3D> &points, float size, float cutoff, float &cellSize);
public:
    DistanceToAtom(int numberOfRandomVectors = 8192);
    void compute(const QVector<QVector3D> &points, float cutoff);
    bool isValid();
    QVector<QPointF> histogram(int bins);
};
