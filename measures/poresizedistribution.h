#ifndef PORESIZEDISTRIBUTION_H
#define PORESIZEDISTRIBUTION_H

#include "measure.h"

class PoreSizeDistribution : public Measure
{
public:
    struct Voxel {
        int i,j,k;
        float value;
    };

    PoreSizeDistribution();
    virtual void compute(Model &model) override;
    virtual QVector<QPointF> histogram(int bins) override;
};

#endif // PORESIZEDISTRIBUTION_H
