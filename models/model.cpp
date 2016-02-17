#include "model.h"

Model::Model()
{

}

Parameters *Model::parameters() const
{
    return m_parameters;
}

bool Model::isInVoid(const QVector3D &point)
{
    return isInVoid(point.x(), point.y(), point.z());
}

void Model::setParameters(Parameters *parameters)
{
    if (m_parameters == parameters)
        return;

    m_parameters = parameters;
    emit parametersChanged(parameters);
    parametersUpdated();
}
