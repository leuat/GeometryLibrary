#include "distancetoatom.h"
#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_histogram.h>
#include <gsl/gsl_errno.h>
#include <cmath>
#include <QDebug>
#include <QElapsedTimer>

float floatRandom(const float & min, const float & max) {
    static std::mt19937 generator;
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

DistanceToAtom::DistanceToAtom(int numberOfRandomVectors)
{
    m_numberOfRandomVectors = numberOfRandomVectors;
}

CellList DistanceToAtom::buildCellList(const QVector<QVector3D> &points, float size, float cutoff, float &cellSize)
{
    CellList cellList;
    cellSize = cutoff;
    int numCells = size / cutoff;
    cellSize = size / numCells;
    cellList.resize(numCells, vector<vector<vector<QVector3D> > >(numCells, vector<vector<QVector3D> >(numCells)));
    for(int i=0; i<points.size(); i++) {
        const QVector3D &p = points[i];
        int ci = p[0] / cellSize;
        int cj = p[1] / cellSize;
        int ck = p[2] / cellSize;
        if(!checkRange<int>(ci, 0, numCells-1) || !checkRange<int>(cj, 0, numCells-1) || !checkRange<int>(ck, 0, numCells-1)) {
            qFatal("DistanceToAtom::buildCellList() error: particle %d is out of cell list bounds.", i);
            exit(1);
        }
        cellList[ci][cj][ck].push_back(p);
    }
    return cellList;
}

void DistanceToAtom::compute(const QVector<QVector3D> &pointsOriginal, float cutoff)
{
    if(pointsOriginal.size() == 0) {
        qDebug() << "DistanceToAtom::compute WARNING: input vector is empty.";
        return;
    }

    QElapsedTimer timer;
    timer.start();

    float min = 1e90;
    float max = -1e90;
    for(const QVector3D &point : pointsOriginal) {
        min = std::min(min, point[0]);
        min = std::min(min, point[1]);
        min = std::min(min, point[2]);

        max = std::max(max, point[0]);
        max = std::max(max, point[1]);
        max = std::max(max, point[2]);
    }
    max += 1e-5;
    const float systemSize = max - min;

    // Now translate all points
    QVector<QVector3D> points = pointsOriginal;
    for(QVector3D &point : points) {
        point[0] -= min;
        point[1] -= min;
        point[2] -= min;
    }
    float cellSize;
    CellList cellList = buildCellList(points, systemSize, cutoff, cellSize);
    const int numCells = cellList.size(); // Each index should be identical


    m_values.clear();
    m_values.resize(m_numberOfRandomVectors);
    m_randomNumbers.resize(3*m_numberOfRandomVectors);


    for(int i=0; i<3*m_numberOfRandomVectors; i++) {
        m_randomNumbers[i] = floatRandom(0, systemSize);
    }

    const float oneOverCellSize = 1.0/cellSize;

#pragma omp parallel for num_threads(8)
    for(int i=0; i<m_numberOfRandomVectors; i++) {
        const float x = m_randomNumbers[3*i+0];
        const float y = m_randomNumbers[3*i+1];
        const float z = m_randomNumbers[3*i+2];

        const int cx = x * oneOverCellSize;
        const int cy = y * oneOverCellSize;
        const int cz = z * oneOverCellSize;
        float minimumDistanceSquared = 1e10;
        const float minimumDistanceSquared0 = minimumDistanceSquared;

        // Loop through all 27 cells with size=cutoff
        for(int dx=-1; dx<=1; dx++) {
            for(int dy=-1; dy<=1; dy++) {
                for(int dz=-1; dz<=1; dz++) {
                    const vector<QVector3D> &pointsInCell = cellList[periodic(cx+dx, numCells)][periodic(cy+dy, numCells)][periodic(cz+dz, numCells)];
                    const int numberOfPointsInCell = pointsInCell.size();

                    for(int j=0; j<numberOfPointsInCell; j++) {
                        // const QVector3D &point = points[pointIndex];
                        const QVector3D &point = pointsInCell[j];

                        float dx = x - point[0];
                        float dy = y - point[1];
                        float dz = z - point[2];
                        if(dx < -0.5*systemSize) dx += systemSize;
                        else if(dx > 0.5*systemSize) dx -= systemSize;

                        if(dy < -0.5*systemSize) dy += systemSize;
                        else if(dy > 0.5*systemSize) dy -= systemSize;

                        if(dz < -0.5*systemSize) dz += systemSize;
                        else if(dz > 0.5*systemSize) dz -= systemSize;
                        const float distanceSquared = dx*dx + dy*dy + dz*dz;
                        if(distanceSquared < minimumDistanceSquared) minimumDistanceSquared = distanceSquared;
                    }
                }
            }
        }

        if(minimumDistanceSquared == minimumDistanceSquared0) {
            minimumDistanceSquared = -1;
        }

        m_values[i] = float(minimumDistanceSquared);
    }

    m_randomNumbers.clear();
    points.clear();

    // qDebug() << "DTO finished after " << timer.elapsed() << " ms.";
    m_isValid = true;

    //    if(pointsOriginal.size() == 0) {
    //        qDebug() << "DistanceToAtom::compute WARNING: input vector is empty.";
    //        return;
    //    }

    //    float min = 1e90;
    //    float max = -1e90;
    //    for(const QVector3D &point : pointsOriginal) {
    //        min = std::min(min, point[0]);
    //        min = std::min(min, point[1]);
    //        min = std::min(min, point[2]);

    //        max = std::max(max, point[0]);
    //        max = std::max(max, point[1]);
    //        max = std::max(max, point[2]);
    //    }
    //    max += 1e-5;
    //    const float systemSize = max - min;

    //    // Now translate all points
    //    QVector<QVector3D> points = pointsOriginal;
    //    for(QVector3D &point : points) {
    //        point[0] -= min;
    //        point[1] -= min;
    //        point[2] -= min;
    //    }

    //    float cellSize;
    //    CellList cellList = buildCellList(points, systemSize, cutoff, cellSize);
    //    const int numCells = cellList.size(); // Each index should be identical

    //    const float voxelSize = systemSize / m_size;
    //    for(int i=0; i<m_size; i++) {
    //        for(int j=0; j<m_size; j++) {
    //            for(int k=0; k<m_size; k++) {
    //                const QVector3D voxelCenter((i+0.5)*voxelSize, (j+0.5)*voxelSize, (k+0.5)*voxelSize);
    //                float minimumDistanceSquared0 = 1e10;
    //                float minimumDistanceSquared = 1e10;
    //                // Find the cell list where this position belongs and loop through all cells around
    //                const int cx = voxelCenter[0] / cellSize;
    //                const int cy = voxelCenter[1] / cellSize;
    //                const int cz = voxelCenter[2] / cellSize;

    //                // Loop through all 27 cells with size=cutoff
    //                for(int dx=-1; dx<=1; dx++) {
    //                    for(int dy=-1; dy<=1; dy++) {
    //                        for(int dz=-1; dz<=1; dz++) {
    //                            const vector<int> &pointsInCell = cellList[periodic(cx+dx, numCells)][periodic(cy+dy, numCells)][periodic(cz+dz, numCells)];
    //                            for(const int &pointIndex : pointsInCell) {
    //                                const QVector3D &point = points[pointIndex];

    //                                const float distanceSquared = periodicDistanceSquared(point, voxelCenter, systemSize);
    //                                minimumDistanceSquared = std::min(minimumDistanceSquared, distanceSquared);
    //                            }
    //                        }
    //                    }
    //                }
    //                if(minimumDistanceSquared == minimumDistanceSquared0) {
    //                    minimumDistanceSquared = -1;
    //                }
    //                setValue(i,j,k,float(minimumDistanceSquared));
    //            }
    //        }
    //    }

    //    m_isValid = true;
}

bool DistanceToAtom::isValid()
{
    return m_isValid;
}

QVector<QPointF> DistanceToAtom::histogram(int bins) {
    QVector<QPointF> histogramVector;

    if(!m_isValid) {
        qFatal("DistanceToAtom is not valid. Run compute() first.");
        exit(1);
    }
    float minValue = 1e90;
    float maxValue = 0;

    for(const float &val : m_values) {
        if(val >= 0) {
            minValue = std::min(minValue, (float)sqrt(val));
            maxValue = std::max(maxValue, (float)sqrt(val));
        }
    }
    gsl_histogram *hist = gsl_histogram_alloc (bins);
    gsl_histogram_set_ranges_uniform (hist, minValue, maxValue);
    for(const float &value : m_values) {
        if(value >= 0) {
            gsl_histogram_increment (hist, sqrt(value));
        }
    }

    histogramVector.resize(bins);
    for(int i=0; i<bins; i++) {
        double upper, lower;
        gsl_histogram_get_range(hist, i, &lower, &upper);
        float middle = 0.5*(upper+lower);
        histogramVector[i].setX(middle);
        histogramVector[i].setY(gsl_histogram_get(hist,i));
    }

    gsl_histogram_free (hist);

    return histogramVector;
}
