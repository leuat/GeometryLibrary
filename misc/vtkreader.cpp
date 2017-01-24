#include "vtkreader.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

VTKReader::VTKReader()
{

}

VTKReader::~VTKReader()
{
    m_points.clear();
}

void VTKReader::read(QString fileName)
{
    qDebug() << "Reading VTK file " << fileName;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file " << fileName;
        return;
    }

    int lineNumber = 0;
    QTextStream in(&file);
    while (!in.atEnd()) {
        lineNumber++;
        QString line = in.readLine();
        // qDebug() << lineNumber << ": " << line;
        if(lineNumber<3) continue;
        if(lineNumber==3) {
            if(line.trimmed().compare(QString("ASCII")) != 0) { qFatal("Error, could not read .vtk file not in ASCII mode"); }
        }
        if(lineNumber==6) {
            QStringList words = line.split(" ");
            if(words.count() != 4) qFatal("Could not read dimensions");

            bool ok;
            m_nx = words[1].toInt(&ok);
            if(!ok) qFatal("Could not read dimensions");
            m_ny = words[2].toInt(&ok);
            if(!ok) qFatal("Could not read dimensions");
            m_nz = words[3].toInt(&ok);
            if(!ok) qFatal("Could not read dimensions");
            int N = m_nx*m_ny*m_nz;
            m_points.resize(N);
        }

        if(lineNumber==7) {
            QStringList words = line.split(" ");
            if(words.count() != 4) qFatal("Could not read origin");

            bool ok;
            m_origin[0] = words[1].toFloat(&ok);
            if(!ok) qFatal("Could not read origin");
            m_origin[1] = words[2].toFloat(&ok);
            if(!ok) qFatal("Could not read origin");
            m_origin[2] = words[3].toFloat(&ok);
            if(!ok) qFatal("Could not read origin");
        }

        if(lineNumber==8) {
            QStringList words = line.split(" ");
            if(words.count() != 4) qFatal("Could not read spacing");

            bool ok;
            m_spacing[0] = words[1].toFloat(&ok);
            if(!ok) qFatal("Could not read spacing");
            m_spacing[1] = words[2].toFloat(&ok);
            if(!ok) qFatal("Could not read spacing");
            m_spacing[2] = words[3].toFloat(&ok);
            if(!ok) qFatal("Could not read spacing");
        }
        if(lineNumber >= 13) {
            int valueIndex = lineNumber - 13;
            bool ok;
            float value = line.toFloat(&ok);
            if(!value) qFatal("Could not parse value");
            m_points[valueIndex] = value;
        }
    }
}

int VTKReader::nz() const
{
    return m_nz;
}

QVector3D VTKReader::origin() const
{
    return m_origin;
}

QVector3D VTKReader::spacing() const
{
    return m_spacing;
}

const QVector<float> &VTKReader::points() const
{
    return m_points;
}

int VTKReader::ny() const
{
    return m_ny;
}

int VTKReader::nx() const
{
    return m_nx;
}
