#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <QVector>
#include <QString>
#include <QMap>

class Parameter {
private:
    double m_value;
    double m_stepSize;
    QString name;

public:

    Parameter(double value, double stepSize, QString n) : m_value(value), m_stepSize(stepSize), name(n){

    }

    Parameter() {

    }

    double value() const;
    void setValue(double value);
    double stepSize() const;
    void setStepSize(double stepSize);
    QString getName() const;
    void setName(const QString &value);
};

class Parameters
{
protected:
    QVector<Parameter> m_parameters;
public:
    Parameters();
    QVector<Parameter> parameters() const;

    Parameter* getParam(QString name);

    void CopyTo(Parameters& dst) {

    }

    void Step() {

    }

    void Likelihood() {

    }


};

#endif // PARAMETERS_H
