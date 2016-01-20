#ifndef PARAMETERS_H
#define PARAMETERS_H
#include <QVector>
#include <QString>
#include <QMap>
#include <ostream>
#include <QVariantMap>
#include <QObject>

class Parameter : public QObject {
    Q_OBJECT
    Q_PROPERTY(double value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(double min READ min WRITE setMin NOTIFY minChanged)
    Q_PROPERTY(double max READ max WRITE setMax NOTIFY maxChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

private:
    double m_value;
    double m_min;
    double m_max;
    double m_stepSize;
    QString m_name;
public:

    explicit Parameter(QObject* parent = nullptr);
    Parameter(const Parameter& param);
    Parameter(QString name, float value = 0, float min = 0, float max = 0, float stepSize = 0);

    double value() const;
    void setValue(double value);
    double stepSize() const;
    void setStepSize(double stepSize);
    QString getName() const;
    void setName(const QString &value);
    double min() const;
    double max() const;
    QString name() const;

public slots:
    void setMin(double min);
    void setMax(double max);

signals:
    void valueChanged(double value);
    void minChanged(double min);
    void maxChanged(double max);
    void nameChanged(QString name);
};

class Parameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList parameters READ parameters WRITE setParameters NOTIFY parametersChanged)
private:
    QMap<QString, Parameter*> m_parametersMap;
    QVariantList m_parameters;

public:
    Parameters();
    QVariantList parameters() const;
    QVector<Parameter*> parameterList() const;
    Parameter* getParameter(QString name);
    void createParameter(QString name, float value = 0, float min = 0, float max = 0, float stepSize = 0);
    QMap<QString, Parameter *> parametersMap() const;
    void setParametersMap(const QMap<QString, Parameter *> &parametersMap);

public slots:
    void setParameters(QVariantList parameters);

signals:
    void parametersChanged(QVariantList parameters);

public:
    friend std::ostream& operator<<(std::ostream& out, const Parameters& params)
    {
        out <<"[ ";
        for(Parameter* p: params.parameterList()) {
            out << "(" << p->getName().toStdString() <<", "  << p->value() << ") ";
        }
        return out;
        out <<"] ";
    }
};

#endif // PARAMETERS_H

