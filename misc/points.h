#ifndef POINTS_H
#define POINTS_H
#include <QFile>
#include <QVector3D>
#include <QVector>

class Points
{
public:
    Points();
    QVector3D systemSize();
    QVector<QVector3D> points();
    void readXYZ(QString filename);
private:
    void calculateBoundingbox();
    QVector<QVector3D> m_points;
    QVector3D m_systemSize;
};

#endif // POINTS_H
