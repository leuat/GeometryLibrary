#include "mkiimodel.h"
#include <QDebug>
#include "../simplex.h"
#include "../perlin.h"
#include "../misc/random.h"
#include <cmath>

MKIIModel::MKIIModel() : Model()
{
    createParameters();
}

template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

bool MKIIModel::isInVoid(float x, float y, float z)
{
    float value = getValue(x,y,z);
    if (m_inverted) {
        if (value>m_threshold) return true;
    } else {
        if (value<m_threshold)  return true;
    }
    return false;
}

float MKIIModel::getValue(float x, float y, float z)
{
    if(!m_noise) {
        qDebug() << "Warning, tried to call MKIIModel::isInVoid(float x, float y, float z), but no noise type is chosen. Maybe you didn't call start()?";
        return 0;
    }

    double add = m_seed;
    double sx = 0.1245;
    double sy = 1.2312;
    double sz = 0.9534;
    // double p1 = (1 + m_skewAmplitude*m_noise->get(m_skewScale*x + sx,m_skewScale*y+sy,m_skewScale*z+sz));
    double f = 1.0;
    double value = 0;//

    float scale = m_scale;

    f = scale * m_f0;
    value += m_A0*m_noise->get_raw(x*f + add, y*f,z*f);
    f = scale * m_f1;
    value += m_A1*m_noise->get_raw(x*f + add, y*f,z*f);
    f = scale * m_f2;
    value += m_A2*m_noise->get_raw(x*f + add, y*f,z*f);
    value = sign<float>(value)*pow(fabs(value), m_steepness);

    if (m_absolute) {
        value = fabs(value);
    }

    return value;
}

void MKIIModel::parametersUpdated()
{

    m_scale = m_parameters->getValue("scale");
    m_A0 = m_parameters->getValue("A0");
    m_A1 = m_parameters->getValue("A1");
    m_A2 = m_parameters->getValue("A2");


    m_scale = m_parameters->getValue("scale");
    m_threshold = m_parameters->getValue("threshold");
    m_inverted = m_parameters->getValue("inverted");
    m_seed = m_parameters->getValue("seed");
    m_absolute = m_parameters->getValue("absolute");
    m_steepness = m_parameters->getValue("steepness");

    Logger::write(QString("MKIIModel::parametersUpdated: updating scale=%1").arg(m_scale));
    Logger::write(QString("MKIIModel::parametersUpdated: updating threshold=%1").arg(m_threshold));
    Logger::write(QString("MKIIModel::parametersUpdated: updating inverted=%1").arg(m_inverted));
    Logger::write(QString("MKIIModel::parametersUpdated: updating seed=%1").arg(m_seed));
    Logger::write(QString("MKIIModel::parametersUpdated: updating absolute=%1").arg(m_absolute));
    Logger::write(QString("MKIIModel::parametersUpdated: updating steepness=%1").arg(m_steepness));

    QString newNoiseType = m_parameters->getString("noisetype");
    if(newNoiseType.compare(m_noiseType) != 0) {
        // The string has changed
        if(newNoiseType.compare("simplex") == 0 || newNoiseType.compare("perlin") == 0) {
            m_noiseType = newNoiseType;
            Logger::write(QString("MKIIModel::parametersUpdated: updating noisetype=%1").arg(m_noiseType));
        }
    }
}


void MKIIModel::createParameters()
{
    m_parameters->createParameter("A0", 0.5, 0, 1, 0.00001);
    m_parameters->createParameter("A1", 0.25, 0, 1, 0.00001);
    m_parameters->createParameter("A2", 0.12, 0, 1, 0.00001);
    m_parameters->createParameter("scale", 0.01, 0.001, 0.1, 0.00001);
    m_parameters->createParameter("threshold", 0.1, -0.4, 0.4, 0.00001);
    m_parameters->createParameter("inverted", 0, 0, 1, 1.0);
    m_parameters->createParameter("seed", 13.0, 1, 100, 1);
    m_parameters->createParameter("absolute", 0.0, 0, 1, 1);
    m_parameters->createParameter("skewscale", 1.0, 0, 1, 0.1);
    m_parameters->createParameter("skewamplitude", 0.0, 0, 1, 0.1);
    m_parameters->createParameter("steepness", 1.0, 0.5, 1.5, 0.000001);
    m_parameters->createParameter("noisetype", "simplex");
    Logger::write(QString("MKIIModel::createParameters: creating octaves=%1").arg(m_parameters->getValue("octaves")));
    Logger::write(QString("MKIIModel::createParameters: creating scale=%1").arg(m_parameters->getValue("scale")));
    Logger::write(QString("MKIIModel::createParameters: creating persistence=%1").arg(m_parameters->getValue("persistence")));
    Logger::write(QString("MKIIModel::createParameters: creating threshold=%1").arg(m_parameters->getValue("threshold")));
    Logger::write(QString("MKIIModel::createParameters: creating inverted=%1").arg(m_parameters->getValue("inverted")));
    Logger::write(QString("MKIIModel::createParameters: creating seed=%1").arg(m_parameters->getValue("seed")));
    Logger::write(QString("MKIIModel::createParameters: creating absolute=%1").arg(m_parameters->getValue("absolute")));
    Logger::write(QString("MKIIModel::createParameters: creating skewscale=%1").arg(m_parameters->getValue("skewscale")));
    Logger::write(QString("MKIIModel::createParameters: creating skewaplitude=%1").arg(m_parameters->getValue("skewamplitude")));
    Logger::write(QString("MKIIModel::createParameters: creating noisetype=%1").arg(m_parameters->getValue("noisetype")));
    Logger::write(QString("MKIIModel::createParameters: creating steepness=%1").arg(m_parameters->getValue("steepness")));
    parametersUpdated();
}

void MKIIModel::loadParameters(CIniFile *iniFile)
{
    Logger::write(QString("MKIIModel::loadParameters: loading parameters from file %1").arg(QString::fromStdString(iniFile->filename)));

    m_parameters->setParameter("A0", iniFile->getdouble("A0"), 0.5, 0.000, 1);
    m_parameters->setParameter("A1", iniFile->getdouble("A1"), 0.25, 0.000, 1);
    m_parameters->setParameter("A2", iniFile->getdouble("A2"), 0.12, 0.000, 1);
    m_parameters->setParameter("scale", iniFile->getdouble("noise_scale"), 0.001, 0.1, 0.001);
    m_parameters->setParameter("threshold", iniFile->getdouble("noise_threshold"), -1, 1, 0.1);
    m_parameters->setParameter("inverted", iniFile->getdouble("noise_inverted"), 0, 1, 1);
    m_parameters->setParameter("seed", iniFile->getdouble("noise_seed"), 1, 100, 1);
    m_parameters->setParameter("absolute", iniFile->getdouble("noise_absolute"), 0, 1, 1);
    m_parameters->setParameter("skewscale", iniFile->getdouble("noise_skewscale"), 0, 1, 0.1);
    m_parameters->setParameter("skewamplitude", iniFile->getdouble("noise_skewamplitude"), 0, 1, 0.1);
    m_parameters->setParameter("noisetype", QString::fromStdString(iniFile->getstring("noise_noisetype")));
    m_parameters->setParameter("steepness", QString::fromStdString(iniFile->getstring("noise_steepness")));
    parametersUpdated();
}

void MKIIModel::start()
{
    if(m_noiseType.compare("perlin") == 0) {
        m_noise = new Perlin(3, 1, 1, m_seed);
    } else if(m_noiseType.compare("simplex") == 0) {
        m_noise = new Simplex(3, 1, 1, m_seed);
    }
}

void MKIIModel::stop()
{
    if(m_noise) {
        delete m_noise;
    }
}

void MKIIModel::randomWalk()
{
    Logger::write(QString("MKIIModel::randomWalk: starting random walk"));
    double delta;

    delta = Random::nextGaussian(0, 0.5*m_parameters->getStepSize("scale"));
    m_parameters->setValue("scale", m_parameters->getValue("scale") + delta);
    m_parameters->fitBounds("scale");
    Logger::write(QString("RegularNoiseModel::randomWalk: changing scale with %1 to %2").arg(delta, m_parameters->getValue("scale")));

    delta = Random::nextGaussian(0, 0.5*m_parameters->getStepSize("A0"));
    m_parameters->setValue("A0", m_parameters->getValue("A0") + delta);
    m_parameters->fitBounds("A0");

    Logger::write(QString("MKIIModel::randomWalk: starting random walk"));
    delta = Random::nextGaussian(0, 0.5*m_parameters->getStepSize("A1"));
    m_parameters->setValue("A1", m_parameters->getValue("A1") + delta);
    m_parameters->fitBounds("A1");

    Logger::write(QString("MKIIModel::randomWalk: starting random walk"));
    delta = Random::nextGaussian(0, 0.5*m_parameters->getStepSize("A2"));
    m_parameters->setValue("A2", m_parameters->getValue("A2") + delta);
    m_parameters->fitBounds("A2");


    delta = Random::nextGaussian(0, m_parameters->getStepSize("threshold"));
    m_parameters->setValue("threshold", m_parameters->getValue("threshold") + delta);
    m_parameters->fitBounds("threshold");
    Logger::write(QString("MKIIModel::randomWalk: changing threshold with %1 to %2").arg(delta, m_parameters->getValue("threshold")));

    delta = Random::nextGaussian(0, 0.5*m_parameters->getStepSize("steepness"));
    m_parameters->setValue("steepness", m_parameters->getValue("steepness") + delta);
    m_parameters->fitBounds("steepness");
    Logger::write(QString("MKIIModel::randomWalk: changing steepness with %1 to %2").arg(delta, m_parameters->getValue("steepness")));

//    delta = Random::nextGaussian(0, 0.5*m_parameters->getStepSize("skewScale"));
//    m_parameters->setValue("skewScale", m_parameters->getValue("skewScale") + delta);
//    m_parameters->fitBounds("skewScale");
//    Logger::write(QString("MKIIModel::randomWalk: changing skewScale with %1 to %2").arg(delta, m_parameters->getValue("skewScale")));

//    delta = Random::nextGaussian(0, 0.5*m_parameters->getStepSize("skewAmplitude"));
//    m_parameters->setValue("skewAmplitude", m_parameters->getValue("skewAmplitude") + delta);
//    m_parameters->fitBounds("skewAmplitude");
//    Logger::write(QString("MKIIModel::randomWalk: changing skewAmplitude with %1 to %2").arg(delta, m_parameters->getValue("skewAmplitude")));

//    qDebug() << "Setting scale: " << m_parameters->getValue("scale") << " persistence: " << m_parameters->getValue("persistence") << " threshold: " << m_parameters->getValue("threshold") << " steepness: " << m_parameters->getValue("steepness");
    parametersUpdated();
}
