#include "multifractalmodel.h"
#include "../noise.h"
//#include <QDebug>

bool MultiFractalModel::isInVoid(float x, float y, float z)
{

    double val = m_noise->getRidgedMf(QVector3D(x,y,z)*m_scale,1,(int)m_octaves,m_lacunarity,0,m_offset-2, m_gain-2);

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


MultiFractalModel::MultiFractalModel()
{
    createParameters();
}

/*bool MultiFractalModel::isInVoid(float x, float y, float z)
{
    if(!m_noise) {
        qDebug() << "Warning, tried to call MultiFractalModel::isInVoid(float x, float y, float z), but no noise type is chosen. Maybe you didn't call start()?";
        return false;
    }

    double add = m_seed;
    double sx = 0.1245;
    double sy = 1.2312;
    double sz = 0.9534;
    double p1 = (1 + m_skewAmplitude*m_noise->get(m_skewScale*x + sx,m_skewScale*y+sy,m_skewScale*z+sz));

    double val = m_noise->get(x*m_scale*p1 + add, y*m_scale*p1,z*m_scale*p1);

    if (m_absolute) {
        val = fabs(val);
    }

    if (m_inverted) {
        if (val>m_threshold) return true;
    } else {
        if (val<m_threshold)  return true;
    }
    return false;
}
*/
void MultiFractalModel::parametersUpdated()
{

    RegularNoiseModel::parametersUpdated();
    m_lacunarity = m_parameters->getValue("lacunarity");
    m_gain = m_parameters->getValue("gain");
    m_offset = m_parameters->getValue("offset");
}


void MultiFractalModel::createParameters()
{
    m_parameters->createParameter("lacunarity", 1.6, 0, 4, 0.05);
    m_parameters->createParameter("gain", 0.75, 0, 4, 0.05);
    m_parameters->createParameter("offset", 0.75, 0, 4, 0.05);
    RegularNoiseModel::createParameters();


//    parametersUpdated();
}

void MultiFractalModel::loadParameters(CIniFile *iniFile)
{
    m_parameters->setParameter("lacunarity", iniFile->getdouble("lacunarity"), 0, 4, 0.05);
    m_parameters->setParameter("offset", iniFile->getdouble("offset"), 0, 4, 0.05);
    m_parameters->setParameter("gain", iniFile->getdouble("gain"), 0, 4, 0.05);
    RegularNoiseModel::loadParameters(iniFile);

//    parametersUpdated();
}
