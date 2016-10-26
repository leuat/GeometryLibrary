#ifndef GRID_H
#define GRID_H
#include <QVector>
#include <QVector3D>
#include <functional>
class Grid
{
public:
    Grid(int nx, int ny, int nz);
    Grid();
    int index(const int &i, const int &j, const int &k) { return i*m_ny*m_nz + j*m_nz + k; }
    int indexPeriodic(const int i, const int j, const int k) { return ((i+m_nx) % m_nx)*m_ny*m_nz + ((j+m_ny) % m_ny)*m_nz + ((k + m_nz) % m_nz); }
    void getIndexVectorFromIndex(const int &index, int &i, int &j, int &k) {
        i = index/(m_ny*m_nz);
        j = (index / m_nz) % m_ny;
        k = index % m_nz;
    }
    float &operator[](int i) { return m_voxels[i]; }
    float &operator()(int i, int j, int k) { return m_voxels[indexPeriodic(i, j, k)]; }
    int nx() const { return m_nx; }
    void setNx(int nx);
    int ny() const { return m_ny; }
    void setNy(int ny);
    int nz() const { return m_nz; }
    void setNz(int nz);
    QVector3D dimensions() { return QVector3D(m_nx, m_ny, m_nz); }
    QVector3D voxelSize(QVector3D systemSize);
    void setDimensions(QVector3D dimensions);
    void setDimensions(int nx, int ny, int nz);
    void resize(int nx, int ny, int nz);
    void resize(QVector3D dimensions);
    void toVTKFile(QString filename, QVector3D systemSize = QVector3D(1.0, 1.0, 1.0));
    void setValue(int i, int j, int k, float value);
    void clear();
    void iterate(QVector3D systemSize, std::function<void(QVector3D p, float &value)> action);
private:
    QVector<float> m_voxels;
    int m_nx = 0;
    int m_ny = 0;
    int m_nz = 0;
    void reallocate();
};

#endif // GRID_H
