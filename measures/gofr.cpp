#include "gofr.h"
#include "neighborlist.h"
#include <omp.h>
#include "GeometryLibrary/misc/points.h"
#include <random>
int intRandom(const int & min, const int & max) {
    static std::mt19937 generator(time(0));
    std::uniform_int_distribution<> distribution(min, max);
    return distribution(generator);
}

GOfR::GOfR(QObject *parent) : Measure(parent),
    m_numBins(100), m_maximumNumberOfPoints(10000), m_cutoff(15)
{

}

void GOfR::capY(float min, float max)
{
    for(int binIndex=0; binIndex<m_numBins; binIndex++) {
        if(m_histogram[binIndex].y() < min) {
            m_histogram[binIndex].setY(min);
        }
        if(m_histogram[binIndex].y() > max) {
            m_histogram[binIndex].setY(max);
        }
    }
}

void GOfR::setXRange(float min, float max) {
    QVector<QPointF> newPoints;

    for(int binIndex=0; binIndex<m_numBins; binIndex++) {
        if(m_histogram[binIndex].x() >= min && m_histogram[binIndex].x() <= max) {
            newPoints.push_back(m_histogram[binIndex]);
        }
    }
    m_histogram = newPoints;
}

QVector<QPointF> GOfR::histogram(int numberOfBins)
{
    Q_UNUSED(numberOfBins)
//    if(cap) {
//        for(int binIndex=0; binIndex<m_numBins; binIndex++) {
//            if(m_histogram[binIndex].y() < min) {
//                m_histogram[binIndex].setY(min);
//            }
//            if(m_histogram[binIndex].y() > max) {
//                m_histogram[binIndex].setY(max);
//            }
//        }
//    }
    return m_histogram;
}


void GOfR::compute(const QVector<QVector3D> &points)
{
    NeighborList list;
    QVector3D cellSize;
    QVector3D numCells;
    Points p;
    p.setPoints(points);
    p.calculateBoundingbox();
    QVector3D systemSize = p.systemSize();

    CellList cellList = list.buildCellList(points, systemSize, m_cutoff, cellSize, numCells);
    QVector3D oneOverCellSize;
    oneOverCellSize[0] = 1.0 / cellSize[0];
    oneOverCellSize[1] = 1.0 / cellSize[1];
    oneOverCellSize[2] = 1.0 / cellSize[2];

    float cutsq = m_cutoff*m_cutoff;
    m_dr = m_cutoff / m_numBins;
    float oneOverDr = 1.0/m_dr;

    int numthreads = omp_get_max_threads();

    QVector<QVector<int>> allCounts;
    allCounts.resize(numthreads);
    for(int i=0; i<numthreads; i++) {
        allCounts[i].resize(m_numBins);
    }

    QVector<int> indices;
    if(p.size() > m_maximumNumberOfPoints) {
        indices.resize(m_maximumNumberOfPoints);
        for(int i=0; i<m_maximumNumberOfPoints; i++) {
            indices[i] = intRandom(0, p.size()-1);
        }
    } else {
        indices.resize(p.size());
        for(int i=0; i<p.size(); i++) {
            indices[i] = i;
        }
    }

#pragma omp parallel num_threads(numthreads)
    {
        QVector<int> &counts = allCounts[omp_get_thread_num()];
#pragma omp for
    for(int i=0; i<indices.size(); i++) {
        const QVector3D &point1 = points.at(indices.at(i));

        const int cx = point1[0] * oneOverCellSize[0];
        const int cy = point1[1] * oneOverCellSize[1];
        const int cz = point1[2] * oneOverCellSize[2];

        // Loop through all 27 cells with size=cutoff
        for(int dx=-1; dx<=1; dx++) {
            for(int dy=-1; dy<=1; dy++) {
                for(int dz=-1; dz<=1; dz++) {
                    const vector<QVector3D> &pointsInCell = cellList[list.periodic(cx+dx, numCells[0])][list.periodic(cy+dy, numCells[1])][list.periodic(cz+dz, numCells[2])];
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
                            float dr = sqrt(dr2);
                            int binIndex = dr * oneOverDr;
                            if (binIndex >= m_numBins) continue;
                            counts[binIndex]++;
                        }
                    }
                }
            }
        }
    }
    }

    QVector<int> counts;
    counts.resize(m_numBins);
    for(int i=0; i<numthreads; i++) {
        for(int binIndex=0; binIndex<m_numBins; binIndex++) {
            counts[binIndex] += allCounts[i][binIndex];
        }
    }

    m_histogram.resize(m_numBins);
    double constant = 4.0*M_PI / (3.0*systemSize[0]*systemSize[1]*systemSize[2]);
    for(int binIndex=0; binIndex<m_numBins; binIndex++) {
        float rlower = binIndex*m_dr;
        float rupper = (binIndex+1)*m_dr;
        float rmean = 0.5*(rlower + rupper);

        float vfrac = constant * (rupper*rupper*rupper - rlower*rlower*rlower);
        float gr = counts[binIndex] / (vfrac * points.size() * points.size());
        m_histogram[binIndex] = QPointF(rmean, gr);
    }
}

int GOfR::numBins() const
{
    return m_numBins;
}

void GOfR::setNumBins(int numBins)
{
    m_numBins = numBins;
}

float GOfR::cutoff() const
{
    return m_cutoff;
}

void GOfR::setCutoff(float cutoff)
{
    m_cutoff = cutoff;
}
