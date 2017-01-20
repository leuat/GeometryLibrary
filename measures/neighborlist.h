#ifndef NEIGHBORLIST_H
#define NEIGHBORLIST_H

#include <QVector>
#include <QVector3D>
#include <vector>

typedef std::vector<std::vector<std::vector<std::vector<QVector3D> > > > CellList;

template<class T>
bool checkRange(T value, T min, T max) {
    return (value >= min) && (value <= max);
}

class NeighborList
{
public:
    NeighborList();
    void build(const QVector<QVector3D> &points, float cutoff);
    CellList buildCellList(const QVector<QVector3D> &points, QVector3D size, float cutoff, QVector3D &cellSize, QVector3D &numCells);
    const QVector<QVector<QVector3D>> &getNeighborVectors() const;
    inline int periodic(int idx, int count) { return (idx + count) % count; }

private:
    QVector<QVector<QVector3D>> m_neighborVectors;

};

#endif // NEIGHBORLIST_H
