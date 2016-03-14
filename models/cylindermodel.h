#ifndef CYLINDERMODEL_H
#define CYLINDERMODEL_H

#include "model.h"

class CylinderModel : public Model
{
    Q_OBJECT
    Q_PROPERTY(float radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(bool inverted READ inverted WRITE setInverted NOTIFY invertedChanged)
public:
    CylinderModel();
    // Model interface
public:
    virtual bool isInVoid(float x, float y, float z) override;
    virtual void parametersUpdated() override;
    virtual void createParameters() override;
    float radius() const;
    bool inverted() const;

public slots:
    void setRadius(float radius);
    void setInverted(bool inverted);

signals:
    void radiusChanged(float radius);
    void invertedChanged(bool inverted);

private:
    float m_radius = 10.0;
    bool m_inverted = false;
};

#endif // CYLINDERMODEL_H
