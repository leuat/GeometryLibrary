#include "xyzmodel.h"

XYZModel::XYZModel()
{

}

bool XYZModel::isInVoid(float x, float y, float z)
{
    if(m_file.isEmpty()) return true;
    int i = x * m_oneOverLx;
    int j = y * m_oneOverLy;
    int k = z * m_oneOverLz;
    return m_voxels[index(i,j,k)];
}

void XYZModel::parametersUpdated()
{
    m_voxelsPerDimension = m_parameters->getValue(QString("voxelsperdimension"));
    m_file = m_parameters->getString(QString("file"));
    m_threshold = m_parameters->getValue(QString("threshold"));
}

void XYZModel::createParameters()
{
    m_parameters->createParameter(QString("voxelsperdimension"), 128.0, 16.0, 512.0, 1.0);
    m_parameters->createParameter(QString("file"), QString(""));
    m_parameters->createParameter(QString("threshold"), 3.0, 1.0, 5.0, 0.1);
}

void XYZModel::loadParameters(CIniFile *iniFile)
{

}

int XYZModel::voxelsPerDimension() const
{
    return m_voxelsPerDimension;
}

float XYZModel::threshold() const
{
    return m_threshold;
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
