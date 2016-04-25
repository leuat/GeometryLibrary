#include "model.h"
#include  "../misc/random.h"
#include "../likelihood/lgraph.h"
#include <QDebug>

Model::Model()
{
    m_parameters = new Parameters();
    connect(m_parameters, &Parameters::parameterUpdated, this, &Model::parametersUpdated);
}

Parameters *Model::parameters() const
{
    return m_parameters;
}

bool Model::isInVoid(const QVector3D& point)
{
    return isInVoid(point.x(), point.y(), point.z());
}

void Model::start()
{

}

void Model::stop()
{

}

void Model::loadParameters(CIniFile *iniFile)
{
    Q_UNUSED(iniFile);
}

void Model::randomWalk()
{

}

bool Model::fitSphere(const QVector3D &position, float radius, int count)
{
    // r uniform
    float dx = 1.0*radius;
    for (int i=0;i<count;i++) {
        QVector3D p = Random::nextQVector3D(0, dx);
        //p = p.normalized()*radius*Random::nextFloat();
        p = p + position;

        if (!isInVoid(p))
            return false;
    }
    return true;
}

float Model::calculateFractalDimension(float min, float max)
{
    float dM = (max-min);
    int N = 20;
    int fitCountScale = 10;
    LGraph results;
    results.initialize(N-1);
    for (int n=1;n<N;n++) {
        float r = (1- (float)n/N)*dM/20;
        int cnt = (int)(dM/r);
        int fits = 0;
        for (int i=0;i<cnt;i++)
            for (int j=0;j<cnt;j++)
                for (int k=0;k<cnt;k++)
                {
                    QVector3D p(i*r, j*r, k*r);
                    if (fitSphere(p, r, r*fitCountScale))
                        fits++;
                }
        if (fits==0) {
            qDebug() << "No fits for r = " << r;

        }
        else {
            float Df = log(fits)/log(1.0/(r/dM));
            qDebug() << Df << " for r = " << r;
            results.val[n-1] = Df;
            results.index[n-1] = r;
            results.IndexScaled[n-1] = r;
        }

        }
    results.SaveText("../../../../../fractal_dim.txt");

}

void Model::setParameters(Parameters *parameters)
{
    if (m_parameters == parameters)
        return;

    m_parameters = parameters;
    emit parametersChanged(parameters);
    parametersUpdated();
}
