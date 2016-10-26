#ifndef MEASURE_H
#define MEASURE_H

#include <QVector>
#include <QVector3D>
#include <QPointF>
#include "../models/model.h"

class Measure
{
public:
    Measure();
    virtual void compute(Model &model) = 0;
    virtual QVector<QPointF> histogram(int bins) = 0;
};

#endif // MEASURE_H
