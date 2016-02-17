#include "regularnoisemodel.h"

RegularNoiseModel::RegularNoiseModel()
{

}

bool RegularNoiseModel::isInVoid(float x, float y, float z)
{
    double add = m_seed;
    double sx = 0.1245;
    double sy = 1.2312;
    double sz = 0.9534;
    double p1 = (1 + m_skewAmplitude*m_noise->get(m_skewScale*x + sx,m_skewScale*y+sy,m_skewScale*z+sz));

    double val = m_noise->get(x*m_scale*p1 + add, y*m_scale*p1,z*m_scale*p1);

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

void RegularNoiseModel::parametersUpdated()
{
    m_noise = nullptr;
    Parameter *parameter = m_parameters->getParameter("octaves");
    if(parameter) m_octaves = parameter->value();

    parameter = m_parameters->getParameter("scale");
    if(parameter) m_scale = parameter->value();

    parameter = m_parameters->getParameter("persistence");
    if(parameter) m_persistence = parameter->value();

    parameter = m_parameters->getParameter("threshold");
    if(parameter) m_threshold = parameter->value();

    parameter = m_parameters->getParameter("inverted");
    if(parameter) m_inverted = parameter->value();

    parameter = m_parameters->getParameter("seed");
    if(parameter) m_seed = parameter->value();

    parameter = m_parameters->getParameter("absolute");
    if(parameter) m_absolute = parameter->value();

    parameter = m_parameters->getParameter("skewScale");
    if(parameter) m_skewScale = parameter->value();

    parameter = m_parameters->getParameter("skewAmplitude");
    if(parameter) m_skewAmplitude = parameter->value();

}

NoiseParameters::NoiseParameters(double octaves, double scale, double persistence, double threshold, double inverted, double seed, double absolute, double skewScale, double skewAmplitude)
{
    createParameter("Octaves", octaves, 1, 7, 1);
    createParameter("Scale", scale, 0.01, 2.0, 0.025);
    createParameter("Persistence", persistence, 0.1, 3, 0.1);
    createParameter("Threshold", threshold, -1, 1, 0.1);
    createParameter("Inverted", inverted, 0, 1);
    createParameter("Seed", seed, 1, 100, 1);
    createParameter("Absolute", absolute, 0, 1, 1);
    createParameter("SkewScale", skewScale, 0,1,0.1);
    createParameter("SkewAmplitude", skewAmplitude, 0, 1, 0.1);
}

NoiseParameters::NoiseParameters(double octaves, double scale, double persistence, double threshold, double inverted, double seed, double absolute)
{
    createParameter("Octaves", octaves, 1, 7, 1);
    createParameter("Scale", scale, 0.01, 2, 0.025);
    createParameter("Persistence", persistence, 0.1, 3, 0.1);
    createParameter("Threshold", threshold, -1, 1, 0.1);
    createParameter("Inverted", inverted, 0, 1);
    createParameter("Seed", seed, 1, 100, 1);
    createParameter("Absolute", absolute, 0, 1, 1);
    createParameter("SkewScale", 1.0, 0,1,0.1);
    createParameter("SkewAmplitude", 0.0, 0, 1, 0.1);
}

NoiseParameters::NoiseParameters() :
    NoiseParameters::NoiseParameters(1.0, 0.05, 1.0, 0.1, 0.0, 13.0, 0.0)
{

}
