#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <QVector>
#include <QString>
#include <QMap>
#include <ostream>
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


    friend std::ostream& operator<<(std::ostream& out, const Parameters& params)
    {
        out <<"[ ";
        for(Parameter& p: params.parameters()) {
            out << "(" << p.getName().toStdString() <<", "  << p.value() << ") ";
        }
        return out;
        out <<"] ";

    }
};

#endif // PARAMETERS_H
