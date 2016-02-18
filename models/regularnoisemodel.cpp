#include "regularnoisemodel.h"
#include "../../cinifile.h"
#include "../simplex.h"
#include "../perlin.h"
RegularNoiseModel::RegularNoiseModel() : Model()
{
    // m_noise = new Simplex();
    // NoiseParameters *parameters = new NoiseParameters(iniFile->getdouble("octaves"), iniFile->getdouble("scale"), iniFile->getdouble("persistence"), iniFile->getdouble("threshold"), iniFile->getdouble("inverted"), iniFile->getdouble("seed"), iniFile->getdouble("absolute"));

}



bool RegularNoiseModel::isInVoid(float x, float y, float z)
{
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
        if (val>m_threshold) {
            return true;
        }
    } else {
        if (val<m_threshold)
            return true;
    }
    return false;
}

void RegularNoiseModel::parametersUpdated()
{
    if(m_noise) delete m_noise;
    m_noise = nullptr;

    m_octaves = m_parameters->getValue("octaves");
    m_scale = m_parameters->getValue("scale");
    m_persistence = m_parameters->getValue("persistence");
    m_threshold = m_parameters->getValue("threshold");
    m_inverted = m_parameters->getValue("inverted");
    m_seed = m_parameters->getValue("seed");
    m_absolute = m_parameters->getValue("absolute");
    m_skewScale = m_parameters->getValue("skewscale");
    m_skewAmplitude = m_parameters->getValue("skewamplitude");

}

void RegularNoiseModel::createParameters()
{
    m_parameters->createParameter("octaves", 1, 1, 7, 1);
    m_parameters->createParameter("scale", 0.05, 0.01, 2.0, 0.025);
    m_parameters->createParameter("persistence", 1.0, 0.1, 3, 0.1);
    m_parameters->createParameter("threshold", 0.1, -1, 1, 0.1);
    m_parameters->createParameter("inverted", 0.0, 0, 1);
    m_parameters->createParameter("seed", 13.0, 1, 100, 1);
    m_parameters->createParameter("absolute", 0.0, 0, 1, 1);
    m_parameters->createParameter("skewscale", 1.0, 0,1,0.1);
    m_parameters->createParameter("skewamplitude", 0.0, 0, 1, 0.1);
    parametersUpdated();
}

void RegularNoiseModel::loadParameters(CIniFile *iniFile)
{
    m_parameters->setParameter("octaves", iniFile->getdouble("octaves"), 1, 7, 1);
    m_parameters->setParameter("scale", iniFile->getdouble("scale"), 0.01, 2.0, 0.025);
    m_parameters->setParameter("persistence", iniFile->getdouble("persistence"), 0.1, 3, 0.1);
    m_parameters->setParameter("threshold", iniFile->getdouble("threshold"), -1, 1, 0.1);
    m_parameters->setParameter("inverted", iniFile->getdouble("inverted"), 0, 1, 1);
    m_parameters->setParameter("seed", iniFile->getdouble("seed"), 1, 100, 1);
    m_parameters->setParameter("absolute", iniFile->getdouble("absolute"), 0, 1, 1);
    m_parameters->setParameter("skewscale", iniFile->getdouble("skewscale"), 0,1, 0.1);
    m_parameters->setParameter("skewamplitude", iniFile->getdouble("skewamplitude"), 0, 1, 0.1);
    parametersUpdated();
}
