#include "model.h"

Model::Model()
{
    m_parameters = new Parameters();
    createParameters();
}

Parameters *Model::parameters() const
{
    return m_parameters;
}

bool Model::isInVoid(const QVector3D &point)
{
    return isInVoid(point.x(), point.y(), point.z());
}

void Model::createParameters()
{

}

void Model::loadParameters(CIniFile *iniFile)
{
    Q_UNUSED(iniFile);
}

void Model::setParameters(Parameters *parameters)
{
    if (m_parameters == parameters)
        return;

    m_parameters = parameters;
    emit parametersChanged(parameters);
    parametersUpdated();
}
