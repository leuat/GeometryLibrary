#include <QDebug>
#include "regularnoisemodel.h"
#include "../simplex.h"
#include "../perlin.h"
#include "../misc/random.h"
RegularNoiseModel::RegularNoiseModel() : Model()
{
    createParameters();
}

bool RegularNoiseModel::isInVoid(float x, float y, float z)
{
    if(!m_noise) {
        qDebug() << "Warning, tried to call RegularNoiseModel::isInVoid(float x, float y, float z), but no noise type is chosen. Maybe you didn't call start()?";
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

void RegularNoiseModel::parametersUpdated()
{
    m_octaves = m_parameters->getValue("octaves");
    m_scale = m_parameters->getValue("scale");
    m_persistence = m_parameters->getValue("persistence");
    m_threshold = m_parameters->getValue("threshold");
    m_inverted = m_parameters->getValue("inverted");
    m_seed = m_parameters->getValue("seed");
    m_absolute = m_parameters->getValue("absolute");
    m_skewScale = m_parameters->getValue("skewscale");
    m_skewAmplitude = m_parameters->getValue("skewamplitude");

    QString newNoiseType = m_parameters->getString("noisetype");
    if(newNoiseType.compare(m_noiseType) != 0) {
        // The string has changed
        if(newNoiseType.compare("simplex") == 0 || newNoiseType.compare("perlin") == 0) {
            m_noiseType = newNoiseType;
        }
    }
}


void RegularNoiseModel::createParameters()
{
    m_parameters->createParameter("octaves", 1, 1, 7, 1);
    m_parameters->createParameter("scale", 0.01, 0.001, 0.1, 0.001);
    m_parameters->createParameter("persistence", 1.0, 0.1, 3.0, 0.1);
    m_parameters->createParameter("threshold", 0.1, -1.0, 1.0, 0.1);
    m_parameters->createParameter("inverted", 0, 0, 1, 1.0);
    m_parameters->createParameter("seed", 13.0, 1, 100, 1);
    m_parameters->createParameter("absolute", 0.0, 0, 1, 1);
    m_parameters->createParameter("skewscale", 1.0, 0,1,0.1);
    m_parameters->createParameter("skewamplitude", 0.0, 0, 1, 0.1);
    m_parameters->createParameter("noisetype", "simplex");
    parametersUpdated();
}

void RegularNoiseModel::loadParameters(CIniFile *iniFile)
{
    m_parameters->setParameter("octaves", iniFile->getdouble("noise_octaves"), 1, 7, 1);
    m_parameters->setParameter("scale", iniFile->getdouble("noise_scale"), 0.001, 0.1, 0.001);
    m_parameters->setParameter("persistence", iniFile->getdouble("noise_persistence"), 0.1, 3, 0.1);
    m_parameters->setParameter("threshold", iniFile->getdouble("noise_threshold"), -1, 1, 0.1);
    m_parameters->setParameter("inverted", iniFile->getdouble("noise_inverted"), 0, 1, 1);
    m_parameters->setParameter("seed", iniFile->getdouble("noise_seed"), 1, 100, 1);
    m_parameters->setParameter("absolute", iniFile->getdouble("noise_absolute"), 0, 1, 1);
    m_parameters->setParameter("skewscale", iniFile->getdouble("noise_skewscale"), 0, 1, 0.1);
    m_parameters->setParameter("skewamplitude", iniFile->getdouble("noise_skewamplitude"), 0, 1, 0.1);
    m_parameters->setParameter("noisetype", QString::fromStdString(iniFile->getstring("noise_noisetype")));
    parametersUpdated();
}

void RegularNoiseModel::start()
{
    if(m_noiseType.compare("perlin") == 0) {
        m_noise = new Perlin(m_octaves, 1, m_persistence, m_seed);
    } else if(m_noiseType.compare("simplex") == 0) {
        m_noise = new Simplex(m_octaves, 1, m_persistence, m_seed);
    }
}

void RegularNoiseModel::stop()
{
    if(m_noise) {
        delete m_noise;
    }
}

void RegularNoiseModel::randomWalk()
{
    double delta = Random::nextGaussian(0, m_parameters->getStepSize("scale"));
    m_parameters->setValue("scale", m_parameters->getValue("scale") + delta);

    delta = Random::nextGaussian(0, 0.5*m_parameters->getStepSize("persistence"));
    m_parameters->setValue("persistence", m_parameters->getValue("persistence") + delta);

    delta = Random::nextGaussian(0, 0.5*m_parameters->getStepSize("threshold"));
    m_parameters->setValue("threshold", m_parameters->getValue("threshold") + delta);

//    delta = Random::nextGaussian(0, m_parameters->getStepSize("skewScale"));
//    m_parameters->setValue("skewScale", m_parameters->getValue("skewScale") + delta);

//    delta = Random::nextGaussian(0, m_parameters->getStepSize("skewAmplitude"));
//    m_parameters->setValue("skewAmplitude", m_parameters->getValue("skewAmplitude") + delta);

    qDebug() << "Setting scale: " << m_parameters->getValue("scale") << " persistence: " << m_parameters->getValue("persistence") << " threshold: " << m_parameters->getValue("threshold");
    parametersUpdated();
}
