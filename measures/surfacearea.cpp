#include "surfacearea.h"
#include "../misc/distancetoatommap.h"
#include "../misc/marchingcubesgenerator.h"
#include "../misc/marchingcubes.h"

SurfaceArea::SurfaceArea() : Measure(),
    m_nx(32), m_ny(32), m_nz(32), m_threshold(3), m_cutoff(15)
{

}


void SurfaceArea::compute(const QVector<QVector3D> &points, bool printResult)
{
    Points p;
    p.setPoints(points);
    p.calculateBoundingbox();

    DistanceToAtomMap dtaMap(m_nx, m_ny, m_nz);
    dtaMap.build(points, m_cutoff);
    Grid &grid = dtaMap.grid();
    MarchingCubes marchingCubes;
    MarchingCubesGenerator generator;
    marchingCubes.setMinValues(QVector3D());
    marchingCubes.setMaxValues(p.systemSize());
    marchingCubes.setResolution(QVector3D(m_nx, m_ny, m_nz));
    marchingCubes.setThreshold(m_threshold);


    generator.synchronize(&marchingCubes);
    generator.setScalarFieldEvaluator([&](const QVector3D point) {
        int i = (point.x()) / p.systemSize().x() * m_nx;
        int j = (point.y()) / p.systemSize().y() * m_ny;
        int k = (point.z()) / p.systemSize().z() * m_nz;
        return grid(i,j,k);
    });
    generator.generateSurface();
    QVector<Triangle> &triangles = generator.trianglesFront();
    QVector<TriangleCollectionVBOData> &data = generator.data();
    double surfaceArea = 0.0;

    for(Triangle &triangle : triangles) {
        QVector3D A = data[triangle.vertexIndices[0]].vertex;
        QVector3D B = data[triangle.vertexIndices[1]].vertex;
        QVector3D C = data[triangle.vertexIndices[2]].vertex;
        // http://math.stackexchange.com/questions/128991/how-to-calculate-area-of-3d-triangle
        QVector3D AB = B-A;
        QVector3D AC = C-A;
        double cosTheta = QVector3D::dotProduct(AB, AC) / (AB.length()*AC.length());
        double sinTheta = sqrt(1.0 - cosTheta*cosTheta);
        double triangleArea = 0.5*AB.length()*AC.length()*sinTheta;
        surfaceArea += triangleArea;
    }

    if(printResult) {
        qDebug() << "Surface area: " << surfaceArea;
    }
}

int SurfaceArea::nx() const
{
    return m_nx;
}

int SurfaceArea::ny() const
{
    return m_ny;
}

int SurfaceArea::nz() const
{
    return m_nz;
}

float SurfaceArea::threshold() const
{
    return m_threshold;
}

float SurfaceArea::cutoff() const
{
    return m_cutoff;
}

void SurfaceArea::setNx(int nx)
{
    if (m_nx == nx)
        return;

    m_nx = nx;
    emit nxChanged(nx);
}

void SurfaceArea::setNy(int ny)
{
    if (m_ny == ny)
        return;

    m_ny = ny;
    emit nyChanged(ny);
}

void SurfaceArea::setNz(int nz)
{
    if (m_nz == nz)
        return;

    m_nz = nz;
    emit nzChanged(nz);
}

void SurfaceArea::setThreshold(float threshold)
{
    if (m_threshold == threshold)
        return;

    m_threshold = threshold;
    emit thresholdChanged(threshold);
}

void SurfaceArea::setCutoff(float cutoff)
{
    if (m_cutoff == cutoff)
        return;

    m_cutoff = cutoff;
    emit cutoffChanged(cutoff);
}
