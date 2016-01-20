#include "parameters.h"

QVariantList Parameters::parameters() const
{
    return m_parameters;
}

QVector<Parameter *> Parameters::parameterList() const
{
    QVector<Parameter *> list;
    for(QVariant variant : m_parameters) {
        Parameter* parameter = variant.value<Parameter*>();
        list.push_back(parameter);
    }
    return list;
}

Parameter* Parameters::getParameter(QString name)
{
    name = name.toLower();
    auto iterator = m_parametersMap.find(name);
    if(iterator == m_parametersMap.end()) {
        return nullptr;
    }
    return *iterator;
}

void Parameters::createParameter(QString name, float value, float min, float max, float stepSize)
{
    Parameter *parameter = new Parameter(name, value, min, max, stepSize);
    m_parameters.append(QVariant::fromValue(parameter));
    m_parametersMap.insert(name.toLower(), parameter);
}

void Parameters::setParameters(QVariantList parameters)
{
    if (m_parameters == parameters)
        return;

    m_parameters = parameters;
    emit parametersChanged(parameters);
}

QMap<QString, Parameter *> Parameters::parametersMap() const
{
    return m_parametersMap;
}

void Parameters::setParametersMap(const QMap<QString, Parameter *> &parametersMap)
{
    m_parametersMap = parametersMap;
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

double Parameter::min() const
{
    return m_min;
}

double Parameter::max() const
{
    return m_max;
}

QString Parameter::name() const
{
    return m_name;
}

void Parameter::setMin(double min)
{
    if (m_min == min)
        return;

    m_min = min;
    emit minChanged(min);
}

void Parameter::setMax(double max)
{
    if (m_max == max)
        return;

    m_max = max;
    emit maxChanged(max);
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
    m_stepSize = param.stepSize();
}

Parameter::Parameter(QString name, float value, float min, float max, float stepSize) :
    m_name(name),
    m_value(value),
    m_min(min),
    m_max(max),
    m_stepSize(stepSize)
{

}

double Parameter::value() const
{
    return m_value;
}

void Parameter::setValue(double value)
{
    m_value = value;
}
