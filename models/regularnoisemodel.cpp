#include "regularnoisemodel.h"

RegularNoiseModel::RegularNoiseModel()
{

}


void RegularNoiseModel::initialize(Noise::NoiseType noiseType, Parameters *np)
{
}

bool RegularNoiseModel::isInVoid(float x, float y, float z)
{
    if (!m_initialized)
        return false;

    double add = m_seed;

    double sx = 0.1245;
    double sy = 1.2312;
    double sz = 0.9534;
    double p1 = (1 + m_skewAmplitude*m_noise->Get(m_skewScale*p.x() + sx,m_skewScale*p.y()+sy,m_skewScale*p.z()+sz));

    double val = m_noise->Get(p.x()*m_scale*p1 + add, p.y()*m_scale*p1,p.z()*m_scale*p1);

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
