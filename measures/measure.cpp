#include "measure.h"

Measure::Measure(QObject *parent) : QObject(parent)
{

}

void Measure::compute(Model &model, bool printResult)
{
    qDebug() << "Error, Model::compute() called with no overloaded implementation";
}

void Measure::compute(const QVector<QVector3D> &points, bool printResult)
{
    Q_UNUSED(points)
    qDebug() << "Error, Model::compute() called with no overloaded implementation";
}

QVector<QPointF> Measure::histogram(int bins)
{
    Q_UNUSED(bins)
    qDebug() << "Error, Model::histogram() called with no overloaded implementation";
}
