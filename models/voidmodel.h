#ifndef VOIDMODEL_H
#define VOIDMODEL_H
#include "model.h"

class VoidModel : public Model
{
public:
    VoidModel();

    // Model interface
public:
    virtual bool isInVoid(float x, float y, float z) override;
    virtual void parametersUpdated() override;
    virtual void createParameters() override;
};

#endif // VOIDMODEL_H
