#include "parameters.h"
#include <qdebug.h>
#include <QUrl>
#include <QFile>

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
        qDebug() << "Could not find parameter " << name;
        return nullptr;
    }
    return *iterator;
}

void Parameters::createParameter(QString name, float value, float min, float max, float stepSize)
{
    auto parameterIterator = m_parametersMap.find(name);
    if(parameterIterator == m_parametersMap.end()) {
        // Create new
        Parameter *parameter = new Parameter(name, value, min, max, stepSize);
        m_parameters.append(QVariant::fromValue(parameter));
        m_parametersMap.insert(name.toLower(), parameter);
        connect(parameter, &Parameter::valueChanged, this, &Parameters::parameterUpdated);
    } else {
        // We already have it. Update instead.
        Parameter *parameter = parameterIterator.value();
        parameter->setMin(min);
        parameter->setMax(max);
        parameter->setStepSize(stepSize);
        parameter->setValue(value);
    }
}

bool Parameters::removeParameter(QString name)
{
    // Check if it exists
    auto parameterIterator = m_parametersMap.find(name);

    if(parameterIterator != m_parametersMap.end()) {
        // Find the QVariant version and remove it
        for(const QVariant &variant : m_parameters) {
            Parameter *parameter = variant.value<Parameter*>();
            if(parameter->name().compare(name) == 0) {
                m_parameters.removeOne(variant);
                m_parametersMap.remove(name);
                delete parameter;
                return true;
            }
        }
    }

    return false;
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

double Parameters::getValue(QString name)
{
    Parameter *parameter = getParameter(name);
    if(parameter) return parameter->value();
    else return 0;
}

void Parameters::save(QString filename)
{
    QFile file(QUrl(filename).toLocalFile());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qWarning() << "Could not open file "+filename;
        return;
    }

    QTextStream out(&file);

    for(Parameter *parameter : parameterList()) {
        out << parameter->name() << " " << parameter->min() << " " << parameter->max() << " " << parameter->stepSize() << " " << parameter->value() << "\n";
    }

    file.close();
}

void Parameters::load(QString filename)
{
    QFile file(QUrl(filename).toLocalFile());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Could not open file "+filename;
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList words = line.split(" ");
        if(words.length() != 5) continue;
        QString name = words[0];

        bool castOk;
        float min = QString(words[1]).toFloat(&castOk);
        if(!castOk) continue;
        float max = QString(words[2]).toFloat(&castOk);
        if(!castOk) continue;
        float stepSize = QString(words[3]).toFloat(&castOk);
        if(!castOk) continue;
        float value = QString(words[4]).toFloat(&castOk);
        if(!castOk) continue;
        if(m_parametersMap.find(name.toLower()) != m_parametersMap.end()) {
            Parameter *parameter = m_parametersMap[name.toLower()];
            parameter->setValue(value);
            parameter->setMin(min);
            parameter->setMax(max);
            parameter->setStepSize(stepSize);
        }
    }
    file.close();
}

double Parameter::stepSize() const
{
    return m_stepSize;
}

void Parameter::setStepSize(double stepSize)
{
    if (m_stepSize == stepSize)
        return;

    m_stepSize = stepSize;
    emit stepSizeChanged(stepSize);
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
    if (m_value == value)
        return;

    m_value = value;
    emit valueChanged(value);
}
