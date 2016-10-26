#include "points.h"
#include <QDebug>

Points::Points()
{

}

QVector3D Points::systemSize()
{
    return m_systemSize;
}

QVector<QVector3D> Points::points()
{
    return m_points;
}

void Points::calculateBoundingbox() {
    double maxX = -1e9;
    double maxY = -1e9;
    double maxZ = -1e9;
    double minX = 1e9;
    double minY = 1e9;
    double minZ = 1e9;
    // Loop over all points and update min and max values
    for (const QVector3D &p : m_points) {
        maxX = std::max(maxX, (double)p.x());
        maxY = std::max(maxY, (double)p.y());
        maxZ = std::max(maxZ, (double)p.z());

        minX = std::min(minX, (double)p.x());
        minY = std::min(minY, (double)p.y());
        minZ = std::min(minZ, (double)p.z());
    }

    double deltaX = maxX - minX;
    double deltaY = maxY - minY;
    double deltaZ = maxZ - minZ;

    // Now move all points to origin
    for(QVector3D &p : m_points) {
        p[0] -= minX;
        p[1] -= minY;
        p[2] -= minZ;
    }
    m_systemSize = QVector3D(deltaX, deltaY, deltaZ);
    m_systemSize[0] += 0.001*deltaX;
    m_systemSize[1] += 0.001*deltaY;
    m_systemSize[2] += 0.001*deltaZ;
}

void Points::readXYZ(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file "+filename;
        return;
    }

    int numberOfAtoms = 0;
    int lineNumber = 0;
    bool foundNumberOfAtoms = false;
    int positionCount = 0;
    // Set lengths to zero and update based on the maximum measured coordinate in the atoms. Add a small epsilon after we're done
    // so x / m_lx always is smaller than the number of voxels in that dimension.
    while (!file.atEnd()) {
        QString line = file.readLine();
        if(++lineNumber == 2) continue; // second line which is a comment, ignore.

        QStringList splitted = line.split(QRegExp("\\s+"));
        if(splitted.count() == 2) {
            bool ok;
            numberOfAtoms = splitted[0].toDouble(&ok);

            if(!ok) {
                qDebug() << QString("Error, tried to read number of atoms, but line '%1' didn't cast well.").arg(line);
                return;
            }
            foundNumberOfAtoms = true;
            m_points.resize(numberOfAtoms);
        } else if(splitted.count() == 5) {
            // AtomType x y z \n
            QString atomType = splitted[0];
            Q_UNUSED(atomType);
            bool x_ok, y_ok, z_ok;
            float x = splitted[1].toDouble(&x_ok);
            float y = splitted[2].toDouble(&y_ok);
            float z = splitted[3].toDouble(&z_ok);
            if(!x_ok || !y_ok || !z_ok) {
                qDebug() << QString("Error, tried to read atom line, but '%1' didn't cast well.").arg(line);
                return;
            }
            if(positionCount<m_points.size()) {
                m_points[positionCount++] = QVector3D(x,y,z);
            } else break; // If this is a multi timestep xyz-file, just ignore the rest
        }
    }
    calculateBoundingbox();
}
