#include "parameters.h"

QVector<Parameter> Parameters::parameters() const
{
    return m_parameters;
}

Parameter* Parameters::getParam(QString name)
{
    for(int i=0;i<m_parameters.size();i++ )
        if (m_parameters[i].getName().toLower()==name.toLower())
            return &m_parameters[i];

    return 0;
}

Parameters::Parameters()
{

}

double Parameter::stepSize() const
{
    return m_stepSize;
}

void Parameter::setStepSize(double stepSize)
{
    m_stepSize = stepSize;
}

QString Parameter::getName() const
{
    return name;
}

void Parameter::setName(const QString &value)
{
    name = value;
}

double Parameter::value() const
{
    return m_value;
}

void Parameter::setValue(double value)
{
    m_value = value;
}
