#include "model.h"
#include <QDebug>
Model::Model()
{
    m_parameters = new Parameters();
    connect(m_parameters, &Parameters::parameterUpdated, this, &Model::parametersUpdated);
}

Parameters *Model::parameters() const
{
    return m_parameters;
}

bool Model::isInVoid(QVector3D point)
{
    return isInVoid(point.x(), point.y(), point.z());
}

void Model::start()
{

}

void Model::stop()
{

}

void Model::loadParameters(CIniFile *iniFile)
{
    Q_UNUSED(iniFile);
}

void Model::randomWalk()
{

}

void Model::setParameters(Parameters *parameters)
{
    if (m_parameters == parameters)
        return;

    m_parameters = parameters;
    emit parametersChanged(parameters);
    parametersUpdated();
}
