#ifndef MULTIFRACTALMODEL_H
#define MULTIFRACTALMODEL_H
#include "regularmodel.h"

class MultiFractalModel : public RegularModel
{
protected:
    double m_lacunarity, m_octaves, m_gain, m_offset;

public:
    MultiFractalModel();

    bool isInVoid(const QVector3D& pos) override;
    void initialize(Noise::NoiseType noiseType, Parameters* np) override;
};

class MultiFractalParameters : public Parameters
{
    Q_OBJECT
public:
    MultiFractalParameters();
    MultiFractalParameters(double octaves, double scale, double persistence, double threshold, double inverted, double seed, double absolute,
                           double lacunarity, double gain, double offset);

};

#endif // MULTIFRACTALMODEL_H
