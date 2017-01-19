#ifndef POINTS_H
#define POINTS_H
#include <QFile>
#include <QVector3D>
#include <QVector>

class Points
{
public:
    Points();
    void clear();
    size_t size();
    QVector3D systemSize();
    const QVector<QVector3D> &points();
    void readXYZ(QString filename);
    void setPoints(const QVector<QVector3D> &points);
    void calculateBoundingbox();
    const QVector3D &operator()(int index) { return m_points.at(index); }
    QVector3D &operator[](int index) { return m_points[index]; }

private:
    QVector<QVector3D> m_points;
    QVector3D m_systemSize;
};

#endif // POINTS_H
