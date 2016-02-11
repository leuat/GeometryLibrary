#ifndef GRAPHSTATISTICS_H
#define GRAPHSTATISTICS_H

#include <QVector>
#include "GeometryLibrary/likelihood/lgraph.h"


class GraphStatistics
{
private:
    QVector<LGraph> m_data;
    LGraph m_average;
    LGraph m_sigma;
    LGraph m_average_plus_sigma;
    int m_length = -1;

public:
    GraphStatistics();

    void add(LGraph& graph);

    void calculate();


    LGraph sigma() const;
    LGraph average() const;
    LGraph average_plus_sigma() const;
};

#endif // GRAPHSTATISTICS_H
