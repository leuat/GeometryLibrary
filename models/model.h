#ifndef GEOMETRYMODEL_H
#define GEOMETRYMODEL_H
#include "../parameters.h"
#include "../misc/cinifile.h"

#include <QObject>
#include <QVector3D>

class Model : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Parameters* parameters READ parameters WRITE setParameters NOTIFY parametersChanged)
protected:
    Parameters* m_parameters = nullptr;

public:
    Model();
    Parameters* parameters() const;
    bool isInVoid(const QVector3D &point);
    virtual void start();
    virtual void stop();
    virtual bool isInVoid(float x, float y, float z) = 0;
    virtual void parametersUpdated() = 0;
    virtual void createParameters() = 0;
    virtual void loadParameters(class CIniFile *iniFile);
    virtual void randomWalk();

public slots:
    void setParameters(Parameters* parameters);

signals:
    void parametersChanged(Parameters* parameters);
};

#endif // GEOMETRYMODEL_H
