#pragma once
#include "grid.h"
#include "../measures/distancetoatom.h"
#include "points.h"
#include <vector>
#include <QVector>
#include <QVector3D>
#include "../measures/neighborlist.h"

using std::vector;

class DistanceToAtomMap
{
private:
    Grid m_grid;
    inline int periodic(int idx, int count) { return (idx + count) % count; }
    CellList buildCellList(const QVector<QVector3D> &points, QVector3D size, float cutoff, QVector3D &cellSize, QVector3D &numCells);
    QVector3D calculateBoundingBox(QVector<QVector3D> &points);
public:
    DistanceToAtomMap(int nx, int ny, int nz);
    void build(QVector<QVector3D> points, float cutoff);
    void build(Points &points, float cutoff);
    Grid &grid();
};
