#include "neighborlist.h"
#include <QDebug>
#include "GeometryLibrary/misc/points.h"
using std::vector;

NeighborList::NeighborList()
{

}

void NeighborList::build(const QVector<QVector3D> &points, float cutoff)
{
    Points p;
    p.setPoints(points);
    p.calculateBoundingbox();
    QVector3D systemSize = p.systemSize();
    QVector3D cellSize;
    QVector3D numCells;
    CellList cellList = buildCellList(points, systemSize, cutoff, cellSize, numCells);
    QVector3D oneOverCellSize;
    oneOverCellSize[0] = 1.0 / cellSize[0];
    oneOverCellSize[1] = 1.0 / cellSize[1];
    oneOverCellSize[2] = 1.0 / cellSize[2];

    if(m_neighborVectors.size() != points.size()) {
        m_neighborVectors.resize(points.size());
    }
    if(m_neighborIndices.size() != points.size()) {
        m_neighborIndices.resize(points.size());
    }

    float cutsq = cutoff*cutoff;

    for(int i=0; i<points.size(); i++) {
        m_neighborVectors[i].clear();

        const QVector3D &point1 = points.at(i);

        const int cx = point1[0] * oneOverCellSize[0];
        const int cy = point1[1] * oneOverCellSize[1];
        const int cz = point1[2] * oneOverCellSize[2];

        // Loop through all 27 cells with size=cutoff
        for(int dx=-1; dx<=1; dx++) {
            for(int dy=-1; dy<=1; dy++) {
                for(int dz=-1; dz<=1; dz++) {
                    const vector<QVector3D> &pointsInCell = cellList[periodic(cx+dx, numCells[0])][periodic(cy+dy, numCells[1])][periodic(cz+dz, numCells[2])];
                    const int numberOfPointsInCell = pointsInCell.size();

                    for(int j=0; j<numberOfPointsInCell; j++) {
                        // const QVector3D &point = points[pointIndex];
                        const QVector3D &point2 = pointsInCell[j];

                        float dx = point1.x() - point2.x();
                        float dy = point1.y() - point2.y();
                        float dz = point1.z() - point2.z();
                        if(dx < -0.5*systemSize[0]) dx += systemSize[0];
                        else if(dx > 0.5*systemSize[0]) dx -= systemSize[0];

                        if(dy < -0.5*systemSize[1]) dy += systemSize[1];
                        else if(dy > 0.5*systemSize[1]) dy -= systemSize[1];

                        if(dz < -0.5*systemSize[2]) dz += systemSize[2];
                        else if(dz > 0.5*systemSize[2]) dz -= systemSize[2];
                        const float dr2 = dx*dx + dy*dy + dz*dz;
                        if(dr2<cutsq) {
                            // Should be in neighbor list
                            m_neighborVectors[i].push_back(point2);
                        }
                    }
                }
            }
        }
    }
}

const QVector<QVector<QVector3D> > &NeighborList::getNeighborVectors() const
{
    return m_neighborVectors;
}

const QVector<QVector<int> > &NeighborList::getNeighborIndices() const
{
    return m_neighborIndices;
}

CellList NeighborList::buildCellList(const QVector<QVector3D> &points, QVector3D size, float cutoff, QVector3D &cellSize, QVector3D &numCells)
{
    CellList cellList;
    for(int a=0; a<3; a++) {
        numCells[a] = int(size[a] / cutoff);
        cellSize[a] = size[a] / numCells[a];
    }

    cellList.resize(numCells[0], vector<vector<vector<QVector3D> > >(numCells[1], vector<vector<QVector3D> >(numCells[2])));
    for(const QVector3D &p : points) {
        int ci = p[0] / cellSize[0];
        int cj = p[1] / cellSize[1];
        int ck = p[2] / cellSize[2];

        if(ci==numCells[0]) ci = numCells[0]-1;
        if(cj==numCells[1]) cj = numCells[1]-1;
        if(ck==numCells[2]) ck = numCells[2]-1;

        if(!checkRange<int>(ci, 0, numCells[0]-1) || !checkRange<int>(cj, 0, numCells[1]-1) || !checkRange<int>(ck, 0, numCells[2]-1)) {
            qDebug() << "DistanceToAtomMap::buildCellList() error: particle " << p << " is out of cell list bounds.";
            exit(1);
        }
        cellList[ci][cj][ck].push_back(p);
    }
    return cellList;
}
