#ifndef GEOMETRYMODEL_H
#define GEOMETRYMODEL_H
#include "../parameters.h"
#include <QObject>
#include <QVector3D>

class Model : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Parameters* parameters READ parameters WRITE setParameters NOTIFY parametersChanged)
protected:
    Parameters* m_parameters = nullptr;
    bool m_initialized = false;

public:
    Model();
    Parameters* parameters() const;
    bool isInVoid(const QVector3D &point);
    virtual bool isInVoid(float x, float y, float z) = 0;
    virtual void parametersUpdated() = 0;

public slots:
    void setParameters(Parameters* parameters);

signals:
    void parametersChanged(Parameters* parameters);
};

#endif // GEOMETRYMODEL_H
