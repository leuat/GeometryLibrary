#include "xyzmodel.h"
#include <QFile>
#include <QString>
#include <QRegExp>
#include <QDebug>
#include <QUrl>

float XYZModel::getLx() const
{
    return m_lx;
}

float XYZModel::getLy() const
{
    return m_ly;
}

float XYZModel::getLz() const
{
    return m_lz;
}

QVector<QVector3D> XYZModel::getPoints() const
{
    return m_points;
}

XYZModel::XYZModel() : Model()
{
    createParameters();
}

bool XYZModel::isInVoid(float x, float y, float z)
{
    if(m_file.isEmpty()) return true;
    int i = x * m_oneOverLx * m_nx;
    int j = y * m_oneOverLy * m_ny;
    int k = z * m_oneOverLz * m_nz;
    return m_voxels[index(i,j,k)] > m_threshold;
}

void XYZModel::parametersUpdated()
{
    m_voxelsPerDimension = m_parameters->getValue(QString("voxelsperdimension"));
    m_threshold = m_parameters->getValue(QString("threshold"));
    m_maxDistance = m_parameters->getValue(QString("maxdistance"));
}

void XYZModel::createParameters()
{
    m_parameters->createParameter(QString("voxelsperdimension"), 128.0, 16.0, 512.0, 1.0);
    m_parameters->createParameter(QString("threshold"), 3.0, 1.0, 5.0, 0.1);
    m_parameters->createParameter(QString("maxdistance"), 20.0, 10.0, 100.0, 2.0);
}

void XYZModel::loadParameters(CIniFile *iniFile)
{
    m_file = QString::fromStdString(iniFile->getstring("filename_xyz"));
    m_voxelsPerDimension = iniFile->getint("voxels_per_dimension");
    m_threshold = iniFile->getdouble("threshold");
    m_maxDistance = iniFile->getdouble("max_distance");
}

int XYZModel::voxelsPerDimension() const
{
    return m_voxelsPerDimension;
}

float XYZModel::threshold() const
{
    return m_threshold;
}

void XYZModel::readFile()
{
    QFile file(m_file);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        file.setFileName(QUrl(m_file).toLocalFile());
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Could not open file "+m_file;
            return;
        }
    }

    int numberOfAtoms = 0;
    int lineNumber = 0;
    bool foundNumberOfAtoms = false;
    int positionCount = 0;
    // Set lengths to zero and update based on the maximum measured coordinate in the atoms. Add a small epsilon after we're done
    // so x / m_lx always is smaller than the number of voxels in that dimension.
    m_lx = 0;
    m_ly = 0;
    m_lz = 0;
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
                m_lx = std::max(m_lx, x);
                m_ly = std::max(m_ly, y);
                m_lz = std::max(m_lz, z);
            } else break; // If this is a multi timestep xyz-file, just ignore the rest
        }
    }

    m_lx += 0.001*m_lx;
    m_ly += 0.001*m_ly;
    m_lz += 0.001*m_lz;
    m_oneOverLx = 1.0 / m_lx;
    m_oneOverLy = 1.0 / m_ly;
    m_oneOverLz = 1.0 / m_lz;
}

void XYZModel::removeCylinder(float r)
{
    QVector<QVector3D> points;

    double inf = 1E30;

    QVector3D minVal(inf, inf, inf);
    QVector3D maxVal(-inf, -inf, -inf);
    // Get min / Max values
    for(int i=0;i<m_points.count();i++) {
        minVal.setX(min(minVal.x(), m_points[i].x()));
        minVal.setY(min(minVal.y(), m_points[i].y()));
        minVal.setZ(min(minVal.z(), m_points[i].z()));
        maxVal.setX(max(maxVal.x(), m_points[i].x()));
        maxVal.setY(max(maxVal.y(), m_points[i].y()));
        maxVal.setZ(max(maxVal.z(), m_points[i].z()));
    }
    QVector3D center = (minVal + maxVal)/2;
    float size = (maxVal - minVal).length();
    float r2 = r*r;
    for (QVector3D p : m_points) {
        QVector3D d = (p-center)/size;
        d.setZ(0);
        if (d.lengthSquared()>r2)
            points.append(p);

    }
    m_points = points;

}

float XYZModel::maxDistance() const
{
    return m_maxDistance;
}

CellList XYZModel::buildCellList(QVector3D cellSize, int numCellsX, int numCellsY, int numCellsZ)
{
    CellList cellList;

    QVector3D oneOverCellSize;
    oneOverCellSize[0] = 1.0 / cellSize[0];
    oneOverCellSize[1] = 1.0 / cellSize[1];
    oneOverCellSize[2] = 1.0 / cellSize[2];
    cellList.resize(numCellsX, vector<vector<vector<QVector3D> > >(numCellsY, vector<vector<QVector3D> >(numCellsZ)));

    for(int i=0; i<m_points.size(); i++) {
        const QVector3D &p = m_points[i];
        int ci = p[0] * oneOverCellSize[0];
        int cj = p[1] * oneOverCellSize[1];
        int ck = p[2] * oneOverCellSize[2];
        if(ci < 0 || ci >= numCellsX || cj < 0 || cj >= numCellsY || ck < 0 || ck >= numCellsZ) {
            qDebug() << "Trouble with " << p;
            qDebug() << "with indices: " << ci << ", " << cj << ", " << ck;
            qDebug() << "and number of cells:: " << numCellsX << ", " << numCellsY << ", " << numCellsZ;
            qDebug() << "Cell size: " << cellSize;
            qDebug() << "Which gives systemSize: " << QVector3D(cellSize[0]*numCellsX, cellSize[1]*numCellsY, cellSize[2]*numCellsZ);
            //qFatal("XYZModel::buildCellList() error: point %d is out of cell list bounds.", i);
            //exit(1);
        }
        else
        cellList[ci][cj][ck].push_back(p);
    }

    return cellList;
}

void XYZModel::addQuad(QVector<SimVis::TriangleCollectionVBOData> &data, QVector3D c1,QVector3D c2,QVector3D c3, QVector3D c4, QVector3D color)
{
    SimVis::TriangleCollectionVBOData tri;

    data.append(tri);
    data.append(tri);
    data.append(tri);
    int i = data.count()-3;
    data[i].vertex = c1;
    data[i+1].vertex = c2;
    data[i+2].vertex = c3;

    QVector3D N = QVector3D::crossProduct((data[i].vertex-data[i+2].vertex), (data[i].vertex-data[i+1].vertex)).normalized();

    data[i].color = color;
    data[i+1].color = color;
    data[i+2].color = color;
    data[i].normal = N;
    data[i+1].normal = N;
    data[i+2].normal = N;

    data.append(tri);
    data.append(tri);
    data.append(tri);
    i = data.count()-3;

    data[i].vertex = c4;
    data[i+1].vertex = c2;
    data[i+2].vertex = c3;

    data[i].color = color;
    data[i+1].color = color;
    data[i+2].color = color;
    data[i].normal = N;
    data[i+1].normal = N;
    data[i+2].normal = N;

}

void XYZModel::updateDistanceToAtomField() {
    if(m_points.size() == 0) {
        qDebug() << "Error, tried to update distance to atom field with no atoms.";
        return;
    }

    int numCellsX = m_lx / m_maxDistance;
    int numCellsY = m_ly / m_maxDistance;
    int numCellsZ = m_lz / m_maxDistance;

    // If max distance is too large, have at least three cells
    if(numCellsX < 3) numCellsX = 3;
    if(numCellsY < 3) numCellsY = 3;
    if(numCellsZ < 3) numCellsZ = 3;

    QVector3D cellSize(m_lx / numCellsX, m_ly / numCellsY, m_lz / numCellsZ);
    QVector3D oneOverCellSize;
    oneOverCellSize[0] = 1.0 / cellSize[0]; oneOverCellSize[1] = 1.0 / cellSize[1]; oneOverCellSize[2] = 1.0 / cellSize[2];
    QVector3D voxelSize = QVector3D(m_lx, m_ly, m_lz) / QVector3D(m_nx, m_ny, m_nz);

    CellList cellList = buildCellList(cellSize, numCellsX, numCellsY, numCellsZ);
    for(int i=0; i<m_nx; i++) {
        for(int j=0; j<m_ny; j++) {
            for(int k=0; k<m_nz; k++) {
                // Now map this voxel
                QVector3D voxelCenter = (QVector3D(i,j,k) + QVector3D(0.5f, 0.5f, 0.5f)) * voxelSize;
                QVector3D cellListCoordinate = voxelCenter * oneOverCellSize;
                float minDistanceSquared = 1e10;
                float minDistanceSquaredStartValue = 1e10;

                for(int dx = -1; dx <= 1; dx++) {
                    for(int dy = -1; dy <= 1; dy++) {
                        for(int dz = -1; dz <= 1; dz++) {
                            int cx = (int(cellListCoordinate.x()) + numCellsX) % numCellsX;
                            int cy = (int(cellListCoordinate.y()) + numCellsY) % numCellsY;
                            int cz = (int(cellListCoordinate.z()) + numCellsZ) % numCellsZ;
                            vector<QVector3D> &points = cellList[cx][cy][cz];
                            for(const QVector3D &point : points) {
                                float distanceSquared = (point - voxelCenter).lengthSquared();
                                minDistanceSquared = std::min(minDistanceSquared, distanceSquared);
                            }
                        }
                    }
                }

                if(minDistanceSquared != minDistanceSquaredStartValue) {
                    m_voxels[index(i,j,k)] = sqrtf(minDistanceSquared);
                } else {
                    m_voxels[index(i,j,k)] = 0;
                }
            }
        }
    }
}

QString XYZModel::file() const
{
    return m_file;
}

void XYZModel::setFile(QString file)
{
    if (m_file == file)
        return;

    m_file = file;
    emit fileChanged(file);
//    readFile();
//    updateDistanceToAtomField();
}

void XYZModel::setVoxelsPerDimension(int voxelsPerDimension)
{
    if (m_voxelsPerDimension == voxelsPerDimension)
        return;

    m_voxelsPerDimension = voxelsPerDimension;
    emit voxelsPerDimensionChanged(voxelsPerDimension);
}

void XYZModel::setThreshold(float threshold)
{
    if (m_threshold == threshold)
        return;

    m_threshold = threshold;
    emit thresholdChanged(threshold);
}

void XYZModel::setMaxDistance(float maxDistance)
{
    if (m_maxDistance == maxDistance)
        return;

    m_maxDistance = maxDistance;
    emit maxDistanceChanged(maxDistance);
}
