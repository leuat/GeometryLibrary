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
    Q_PROPERTY(double stepSize READ stepSize WRITE setStepSize NOTIFY stepSizeChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString string READ string WRITE setString NOTIFY stringChanged)

private:
    double m_value;
    double m_min;
    double m_max;
    double m_stepSize;
    QString m_name;
    QString m_string;

public:

    explicit Parameter(QObject* parent = nullptr);
    Parameter(const Parameter& param);
    Parameter(QString name, float value = 0, float min = 0, float max = 0, float stepSize = 0);
    Parameter(QString name, QString string);

    double value() const;
    double stepSize() const;
    QString getName() const;
    double min() const;
    double max() const;
    QString name() const;
    QString string() const;

public slots:
    void setMin(double min);
    void setMax(double max);
    void setString(QString string);
    void setValue(double value);
    void setStepSize(double stepSize);
    void setName(const QString &value);

signals:
    void valueChanged(double value);
    void minChanged(double min);
    void maxChanged(double max);
    void nameChanged(QString name);
    void stepSizeChanged(double stepSize);
    void stringChanged(QString string);
};

class Parameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList parameters READ parameters WRITE setParameters NOTIFY parametersChanged)
private:
    QMap<QString, Parameter*> m_parametersMap;
    QVariantList m_parameters;

public:
    Parameters() { }
    QVariantList parameters() const;
    QVector<Parameter*> parameterList() const;
    Parameter* getParameter(QString name);
    void setParameter(QString name, float value, float min, float max, float stepSize);
    void setParameter(QString name, QString string);
    void createParameter(QString name, float value = 0, float min = 0, float max = 0, float stepSize = 0);
    void createParameter(QString name, QString string);
    bool removeParameter(QString name);
    QMap<QString, Parameter *> parametersMap() const;
    void setParametersMap(const QMap<QString, Parameter *> &parametersMap);
    double getValue(QString name);
    double getMax(QString name);
    double getMin(QString name);
    double getStepSize(QString name);
    void setValue(QString name, double value);
    QString getString(QString name);
    Q_INVOKABLE void save(QString filename);
    Q_INVOKABLE void load(QString filename);

public slots:
    void setParameters(QVariantList parameters);

signals:
    void parametersChanged(QVariantList parameters);
    void parameterUpdated();

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

