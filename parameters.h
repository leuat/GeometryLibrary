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


    Parameter(double value, double stepSize, QString n) : m_value(value), m_stepSize(stepSize), m_name(n){

    }


//    QVariantMap createProperty(QString name, QString type, QString guiType, float min, float max) {
    QVariantMap createProperty() {
        QVariantMap property;
        property["name"] = m_name;
        property["type"] = "float";
        property["guiType"] = "slider";
        property["min"] = m_min;
        property["max"] = m_max;
        return property;
    }


    double value() const;
    void setValue(double value);
    double stepSize() const;
    void setStepSize(double stepSize);
    QString getName() const;
    void setName(const QString &value);
    double min() const
    {
        return m_min;
    }

    double max() const
    {
        return m_max;
    }

    QString name() const
    {
        return m_name;
    }

public slots:
    void setMin(double min)
    {
        if (m_min == min)
            return;

        m_min = min;
        emit minChanged(min);
    }

    void setMax(double max)
    {
        if (m_max == max)
            return;

        m_max = max;
        emit maxChanged(max);
    }

signals:
    void valueChanged(double value);
    void minChanged(double min);
    void maxChanged(double max);
    void nameChanged(QString name);
};

class Parameters : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList properties READ properties WRITE setProperties NOTIFY propertiesChanged)
    QVariantList m_properties;

protected:
//    QVariantMap m_parameters;
    QVector<Parameter*> m_parameters;
public:
    Parameters();
    QVector<Parameter*> parameters() const;

    Parameter* getParam(QString name);



    friend std::ostream& operator<<(std::ostream& out, const Parameters& params)
    {
        out <<"[ ";
        for(Parameter* p: params.parameters()) {
            out << "(" << p->getName().toStdString() <<", "  << p->value() << ") ";
        }
        return out;
        out <<"] ";

    }

/*    void buildParameterList() {
        m_properties.clear();
        foreach (Parameter* p: m_parameters) {
            m_properties.append();
        }
    }
*/
    QVariantList properties() const
    {
        return m_properties;
    }
public slots:
    void setProperties(QVariantList properties)
    {
        if (m_properties == properties)
            return;

        m_properties = properties;
        emit propertiesChanged(properties);
    }
signals:
    void propertiesChanged(QVariantList properties);
};

#endif // PARAMETERS_H

