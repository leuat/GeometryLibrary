#include "distancetoatom.h"
#include <stdio.h>
#include <stdlib.h>
#include <gsl/gsl_histogram.h>
#include <gsl/gsl_errno.h>
#include <cmath>
#include <QDebug>
#include <QElapsedTimer>
#include "GeometryLibrary/misc/points.h"
#include "neighborlist.h"

float floatRandom(const float & min, const float & max) {
    static std::mt19937 generator(time(0));


    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

DistanceToAtom::DistanceToAtom(QObject *parent) : Measure(parent),
    m_numberOfRandomVectors(65536), m_cutoff(15)
{

}

int DistanceToAtom::numberOfRandomVectors() const
{
    return m_numberOfRandomVectors;
}

void DistanceToAtom::setNumberOfRandomVectors(int numberOfRandomVectors)
{
    m_numberOfRandomVectors = numberOfRandomVectors;
}

float DistanceToAtom::cutoff() const
{
    return m_cutoff;
}

void DistanceToAtom::setCutoff(float cutoff)
{
    m_cutoff = cutoff;
}


void DistanceToAtom::compute(const QVector<QVector3D> &pointsOriginal, bool printResult)
{
    if(pointsOriginal.size() == 0) {
        qDebug() << "DistanceToAtom::compute WARNING: input vector is empty.";
        return;
    }
    QElapsedTimer timer;
    timer.start();

    Points points;
    points.setPoints(pointsOriginal);
    points.calculateBoundingbox();

    QVector3D systemSize = points.systemSize();
    QVector3D cellSize;
    QVector3D numCells;
    NeighborList list;
    CellList cellList = list.buildCellList(points.points(), systemSize, m_cutoff, cellSize, numCells);
    m_values.clear();
    m_values.resize(m_numberOfRandomVectors);
    m_randomNumbers.resize(3*m_numberOfRandomVectors);
    for(int i=0; i<m_numberOfRandomVectors; i++) {
        for(int a=0; a<3; a++) {
            m_randomNumbers[3*i+a] = floatRandom(0, systemSize[a]);
        }
    }

    QVector3D oneOverCellSize;
    oneOverCellSize[0] = 1.0/cellSize[0];
    oneOverCellSize[1] = 1.0/cellSize[1];
    oneOverCellSize[2] = 1.0/cellSize[2];
    float porosity = 0;
    float inside = 0;

#pragma omp parallel for
    for(int i=0; i<m_numberOfRandomVectors; i++) {
        const float x = m_randomNumbers[3*i+0];
        const float y = m_randomNumbers[3*i+1];
        const float z = m_randomNumbers[3*i+2];

        const int cx = x * oneOverCellSize[0];
        const int cy = y * oneOverCellSize[1];
        const int cz = z * oneOverCellSize[2];
        float minimumDistanceSquared = 1e10;
        const float minimumDistanceSquared0 = minimumDistanceSquared;

        // Loop through all 27 cells with size=cutoff
        for(int dx=-1; dx<=1; dx++) {
            for(int dy=-1; dy<=1; dy++) {
                for(int dz=-1; dz<=1; dz++) {
                    const vector<QVector3D> &pointsInCell = cellList[list.periodic(cx+dx, numCells[0])][list.periodic(cy+dy, numCells[1])][list.periodic(cz+dz, numCells[2])];
                    const int numberOfPointsInCell = pointsInCell.size();

                    for(int j=0; j<numberOfPointsInCell; j++) {
                        const QVector3D &point = pointsInCell[j];

                        float dx = x - point[0];
                        float dy = y - point[1];
                        float dz = z - point[2];
                        if(dx < -0.5*systemSize[0]) dx += systemSize[0];
                        else if(dx > 0.5*systemSize[0]) dx -= systemSize[0];

                        if(dy < -0.5*systemSize[1]) dy += systemSize[1];
                        else if(dy > 0.5*systemSize[1]) dy -= systemSize[1];

                        if(dz < -0.5*systemSize[2]) dz += systemSize[2];
                        else if(dz > 0.5*systemSize[2]) dz -= systemSize[2];
                        const float distanceSquared = dx*dx + dy*dy + dz*dz;
                        if(distanceSquared < minimumDistanceSquared) minimumDistanceSquared = distanceSquared;
                    }
                }
            }
        }

        if(minimumDistanceSquared == minimumDistanceSquared0) {
            minimumDistanceSquared = -1;
        }
        // 0.12 here corresponds to the correct porosity calculated from isInside
        if (sqrt(minimumDistanceSquared)<0.12*cellSize[0]) {
            inside++;
        }

        m_values[i] = float(minimumDistanceSquared);
    }

    qDebug() << "#6";
    qDebug() << "min dist: " << 0.12*cellSize;

    qDebug() << ((1-inside/(float)m_numberOfRandomVectors)*100) << "% porosity";
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
    float minValue = 0;
    float maxValue = m_cutoff;

/*    for(const float &val : m_values) {
        if(val >= 0) {
            minValue = std::min(minValue, (float)sqrt(val));

            maxValue = std::max(maxValue, (float)sqrt(val));
        }
    }*/

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
