#include "cylindermodel.h"

CylinderModel::CylinderModel()
{
    createParameters();
}


bool CylinderModel::isInVoid(float x, float y, float z)
{
    float r2 = x*x + y*y;
    if(m_inverted) return (r2>m_radius*m_radius);
    else return (r2<m_radius*m_radius);
}

void CylinderModel::parametersUpdated()
{
    m_radius = m_parameters->getValue("radius");
    m_inverted = m_parameters->getValue("inverted");
}

void CylinderModel::createParameters()
{
    m_parameters->createParameter("radius", 10.0, 0.0, 100.0, 1.0);
    m_parameters->createParameter("inverted", 0.0, 0.0, 1.0, 1.0);
}

float CylinderModel::radius() const
{
    return m_radius;
}

bool CylinderModel::inverted() const
{
    return m_inverted;
}

void CylinderModel::setRadius(float radius)
{
    if (m_radius == radius)
        return;

    m_radius = radius;
    emit radiusChanged(radius);
}

void CylinderModel::setInverted(bool inverted)
{
    if (m_inverted == inverted)
        return;

    m_inverted = inverted;
    emit invertedChanged(inverted);
}


void CylinderModel::loadParameters(CIniFile *iniFile)
{
    setRadius(iniFile->getdouble("cylinder_radius"));
    setInverted(iniFile->getbool("cylinder_inverted"));
}
