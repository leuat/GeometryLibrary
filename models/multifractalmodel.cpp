#include "multifractalmodel.h"
#include <QDebug>

MultiFractalModel::MultiFractalModel() {

}


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
