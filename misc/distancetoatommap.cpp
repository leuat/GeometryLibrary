#include "distancetoatommap.h"
#include <cmath>
#include <QDebug>
#include <QElapsedTimer>
#include "points.h"

DistanceToAtomMap::DistanceToAtomMap(int nx, int ny, int nz)
{
    m_grid.resize(nx, ny, nz);
}

QVector3D DistanceToAtomMap::findBoundingBox(QVector<QVector3D> &points) {
    double maxX = -1e9;
    double maxY = -1e9;
    double maxZ = -1e9;
    double minX = 1e9;
    double minY = 1e9;
    double minZ = 1e9;
    // Loop over all points and update min and max values
    for (const QVector3D &p : points) {
        maxX = std::max(maxX, (double)p.x());
        maxY = std::max(maxY, (double)p.y());
        maxZ = std::max(maxZ, (double)p.z());

        minX = std::min(minX, (double)p.x());
        minY = std::min(minY, (double)p.y());
        minZ = std::min(minZ, (double)p.z());
    }

    double deltaX = maxX - minX;
    double deltaY = maxY - minY;
    double deltaZ = maxZ - minZ;

    // Now move all points to origin
    for(QVector3D &p : points) {
        p[0] -= minX;
        p[1] -= minY;
        p[2] -= minZ;
    }
    QVector3D max(deltaX, deltaY, deltaZ);
    max[0] += 0.001*deltaX;
    max[1] += 0.001*deltaY;
    max[2] += 0.001*deltaZ;
    return max;
}

void DistanceToAtomMap::build(QString xyzFilename, float cutoff) {
    Points points;
    points.readFile(xyzFilename);
    build(points.points(), cutoff);
}

void DistanceToAtomMap::build(QVector<QVector3D> points, float cutoff)
{
    if(points.size() == 0) {
        qDebug() << "DistanceToAtom::compute WARNING: input vector is empty.";
        return;
    }

    QElapsedTimer timer;
    timer.start();
    QVector3D systemSize = findBoundingBox(points); // This will also move all particles so lowest coordinate is 0 in each dimension

    QVector3D cellSize;
    QVector3D numCells;
    CellList cellList = buildCellList(points, systemSize, cutoff, cellSize, numCells);

    QVector3D voxelSize = m_grid.voxelSize(systemSize);
    for(int i=0; i<m_grid.nx(); i++) {
        for(int j=0; j<m_grid.ny(); j++) {
            for(int k=0; k<m_grid.nz(); k++) {
                const QVector3D voxelCenter((i+0.5)*voxelSize[0], (j+0.5)*voxelSize[1], (k+0.5)*voxelSize[2]);
                float minimumDistanceSquared0 = 1e9;
                float minimumDistanceSquared = 1e9;
                // Find the cell list where this position belongs and loop through all cells around
                const int cx = voxelCenter[0] / cellSize[0];
                const int cy = voxelCenter[1] / cellSize[1];
                const int cz = voxelCenter[2] / cellSize[2];

                // Loop through all 27 cells with size=cutoff
                for(int dx=-1; dx<=1; dx++) {
                    for(int dy=-1; dy<=1; dy++) {
                        for(int dz=-1; dz<=1; dz++) {
                            const vector<QVector3D> &pointsInCell = cellList[periodic(cx+dx, numCells[0])][periodic(cy+dy, numCells[1])][periodic(cz+dz, numCells[2])];
                            for(const QVector3D &point : pointsInCell) {
                                QVector3D delta = voxelCenter - point;

                                // Apply periodic boundary conditions
                                for(int a=0; a<3; a++) {
                                    if(delta[a] < -0.5*systemSize[a]) delta[a] += systemSize[a];
                                    else if(delta[a] > 0.5*systemSize[a]) delta[a] -= systemSize[a];
                                }

                                minimumDistanceSquared = std::min(minimumDistanceSquared, delta.lengthSquared());
                            }
                        }
                    }
                }
                if(minimumDistanceSquared == minimumDistanceSquared0) {
                    minimumDistanceSquared = nanf("");
                }

                m_grid(i,j,k) = minimumDistanceSquared;
            }
        }
    }
}

Grid &DistanceToAtomMap::grid()
{
    return m_grid;
}

CellList DistanceToAtomMap::buildCellList(const QVector<QVector3D> &points, QVector3D size, float cutoff, QVector3D &cellSize, QVector3D &numCells)
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
