#include "parameters.h"
QVector<Parameter*> Parameters::parameters() const
{
    return m_parameters;
}

Parameter* Parameters::getParam(QString name)
{
    for(int i=0;i<m_parameters.size();i++ )
        if (m_parameters[i]->getName().toLower()==name.toLower())
            return m_parameters[i];

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
    return m_name;
}

void Parameter::setName(const QString &value)
{
    m_name = value;
}

Parameter::Parameter(QObject *parent) : QObject(parent)
{

}

Parameter::Parameter(const Parameter &param)
{
    m_name = param.name();
    m_value = param.value();
    m_max = param.max();
    m_min = param.min();

}

double Parameter::value() const
{
    return m_value;
}

void Parameter::setValue(double value)
{
    m_value = value;
}
