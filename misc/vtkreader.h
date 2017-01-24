#ifndef VTKREADER_H
#define VTKREADER_H

#include <QString>
#include <QVector>
#include <QVector3D>

class VTKReader
{
public:
    VTKReader();
    ~VTKReader();
    void read(QString fileName);
    QString fileName() const;
    void setFileName(const QString &fileName);

    int nx() const;
    int ny() const;
    int nz() const;
    QVector3D origin() const;
    QVector3D spacing() const;

    const QVector<float> &points() const;

private:
    int m_nx, m_ny, m_nz;
    QVector3D m_origin;
    QVector3D m_spacing;
    QVector<float> m_points;
};

#endif // VTKREADER_H
