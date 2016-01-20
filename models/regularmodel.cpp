#include "regularmodel.h"
#include "../simplex.h"
#include "../perlin.h"

RegularModel::RegularModel()
{

}

void RegularModel::initialize(Noise::NoiseType noiseType, Parameters* np)
{
    m_parameters = np;
    if (noiseType == Noise::NoiseType::Simplex)
        m_noise =  new Simplex((int)(np->getParameter("octaves")->value()), 1, np->getParameter("persistence")->value(), np->getParameter("seed")->value());
    if (noiseType == Noise::NoiseType::Perlin)
        m_noise =  new Perlin((int)np->getParameter("octaves")->value(), 1, np->getParameter("persistence")->value(), np->getParameter("seed")->value());

    m_scale = np->getParameter("scale")->value();
    m_threshold = np->getParameter("threshold")->value();
    m_skewScale = np->getParameter("SkewScale")->value();
    m_skewAmplitude = np->getParameter("SkewAmplitude")->value();

    if (np->getParameter("inverted")->value()>0.5)
        m_inverted = true;
    if (np->getParameter("absolute")->value()>0.5)
        m_absolute = true;

    m_initialized = true;
}

bool RegularModel::isInVoid(const QVector3D &p)
{
    if (!m_initialized)
        return false;

    double sx = 0.1245;
    double sy = 1.2312;
    double sz = 0.9534;
    double p1 = (1 + m_skewAmplitude*m_noise->Get(m_skewScale*p.x() + sx,m_skewScale*p.y()+sy,m_skewScale*p.z()+sz));

    double val = m_noise->Get(p.x()*m_scale*p1, p.y()*m_scale*p1,p.z()*m_scale*p1);

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
