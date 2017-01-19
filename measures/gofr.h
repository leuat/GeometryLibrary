#ifndef GOFR_H
#define GOFR_H

#include "measure.h"

class GOfR : public Measure
{
public:
    GOfR();

    // Measure interface
public:
    virtual void compute(Model &model) override;
    virtual QVector<QPointF> histogram(int bins) override;
};

#endif // GOFR_H
