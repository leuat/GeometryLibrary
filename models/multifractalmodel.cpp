#include "multifractalmodel.h"
#include <QDebug>

bool MultiFractalModel::isInVoid(const QVector3D &p)
{
    if (!m_initialized)
        return false;

    double val = m_noise->getRidgedMf(p*m_scale,1,(int)m_octaves,m_lacunarity,0,m_offset, m_gain);

    if (m_absolute)
        val = fabs(val);

    if (m_inverted) {
        if (val>m_threshold)
            return true;
    }
    else {
        if (val<m_threshold)
            return true;
    }
    return false;
}

void MultiFractalModel::initialize(Noise::NoiseType noiseType, Parameters *np)
{
    RegularModel::initialize(noiseType, np);
    m_lacunarity = np->getParameter("lacunarity")->value();
    m_offset = np->getParameter("offset")->value();
    m_gain = np->getParameter("gain")->value();
    m_octaves = np->getParameter("octaves")->value();
}

MultiFractalParameters::MultiFractalParameters(double octaves, double scale, double persistence, double threshold, double inverted, double seed, double absolute, double lacunarity, double gain, double offset)
{
    createParameter("Octaves", octaves, 1, 7, 1);
    createParameter("Scale", scale, 0.01, 5, 0.1);
    createParameter("Persistence", persistence, 0.1, 3, 0.1);
    createParameter("Threshold", threshold, -1, 1, 0.1);
    createParameter("Inverted", inverted, 1, 1);
    createParameter("Seed", seed, 1, 100, 1);
    createParameter("Absolute", absolute, 0, 1, 1);
    createParameter("Lacunarity", lacunarity, 0.1, 3, 0.1);
    createParameter("Gain", gain, 0, 2, 0.1);
    createParameter("Offset", offset, 0, 2, 0.1);
}

MultiFractalParameters::MultiFractalParameters() :
    MultiFractalParameters::MultiFractalParameters(1.0, 1.0, 1.0, 0.1, 1.0, 13.0, 0.0, 2.2, 1.5, 0.6)
{

}
