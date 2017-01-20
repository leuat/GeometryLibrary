#ifndef MEASURE_H
#define MEASURE_H
#include <QObject>
#include <QVector>
#include <QVector3D>
#include <QPointF>
#include "../models/model.h"

class Measure : public QObject
{
    Q_OBJECT
public:
    explicit Measure(QObject *parent = nullptr);
    virtual void compute(Model &model);
    virtual void compute(const QVector<QVector3D> &points);
    virtual QVector<QPointF> histogram(int bins);
};

#endif // MEASURE_H
