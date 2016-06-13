#include "grid.h"

#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDebug>

Grid::Grid(int nx, int ny, int nz)
{
    resize(nx, ny, nz);
}

Grid::Grid()
{

}

void Grid::setNx(int nx) {
    m_nx = nx;
    reallocate();
}

void Grid::setNy(int ny) {
    m_ny = ny;
    reallocate();
}

void Grid::setNz(int nz) {
    m_nz = nz;
    reallocate();
}

QVector3D Grid::voxelSize(QVector3D systemSize) {
    return QVector3D(systemSize[0]/m_nx, systemSize[1]/m_ny, systemSize[2]/m_nz);
}

void Grid::setDimensions(QVector3D dimensions)
{
    resize(dimensions);
}

void Grid::setDimensions(int nx, int ny, int nz)
{
    resize(QVector3D(nx, ny, nz));
}

void Grid::resize(int nx, int ny, int nz) {
    resize(QVector3D(nx, ny, nz));
}

void Grid::resize(QVector3D dimensions)
{
    m_nx = dimensions[0];
    m_ny = dimensions[1];
    m_nz = dimensions[2];
    reallocate();
}

void Grid::toVTKFile(QString filename, QVector3D systemSize)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Could not open file " << filename;
        exit(1);
    }
    QTextStream stream(&file);
    int N = m_nx*m_ny*m_nz;
    stream << "# vtk DataFile Version 2.0" << endl;
    stream << "structured point" << endl;
    stream << "ASCII" << endl;
    stream << endl;
    stream << "DATASET STRUCTURED_POINTS" << endl;
    stream << "DIMENSIONS " << m_nx << " " << m_ny << " " << m_nz << endl;
    stream << "ORIGIN 0.0 0.0 0.0" << endl;
    // ofile << "SPACING 1 1 1" << endl;
    stream << "SPACING " << systemSize[0]/m_nx << " " << systemSize[1]/m_ny << " " << systemSize[2]/m_nz << endl;
    stream << "POINT_DATA " << N << endl;
    stream << "SCALARS atomdist double" << endl;
    stream << "LOOKUP_TABLE default" << endl;
    stream << endl;

    // column-major ordering...
    for (int k = 0; k < m_nz; k++) {
        for (int j = 0; j < m_ny; j++) {
            for (int i = 0; i < m_nx; i++) {
                stream << m_voxels[index(i, j, k)] << endl;
            }
        }
    }

    file.close();
}

void Grid::setValue(int i, int j, int k, float value)
{
    m_voxels[index(i,j,k)] = value;
}

void Grid::clear()
{
    memset(&m_voxels[0], 0, m_voxels.size()*sizeof(float));
}

void Grid::iterate(QVector3D systemSize, std::function<void (QVector3D p, float &v)> action)
{
    QVector3D voxelSize = this->voxelSize(systemSize);
    for(int i=0; i<m_nx; i++) {
        for(int j=0; j<m_ny; j++) {
            for(int k=0; k<m_nz; k++) {
                const QVector3D voxelCenter((i+0.5)*voxelSize[0], (j+0.5)*voxelSize[1], (k+0.5)*voxelSize[2]);
                action(voxelCenter, m_voxels[index(i,j,k)]);
            }
        }

    }
}

QVector<float> &Grid::voxels()
{
    return m_voxels;
}

void Grid::reallocate()
{
    m_voxels.resize(m_nx*m_ny*m_nz);
    clear();
}












