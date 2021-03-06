#include "models.h"
#include  "../misc/random.h"
#include "../likelihood/lgraph.h"
#include  "../misc/cinifile.h"
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

float Model::getValue(const QVector3D &point)
{
    return getValue(point.x(), point.y(), point.z());
}

void Model::start()
{

}

void Model::stop()
{

}

float Model::getValue(float x, float y, float z)
{
    return isInVoid(x,y,z);
}

void Model::loadParameters(CIniFile *iniFile)
{
    Q_UNUSED(iniFile);
}

void Model::randomWalk()
{

}

bool Model::voxelFits(const QVector3D &position, float radius, int count)
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
                    if (voxelFits(p, r, r*fitCountScale))
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

Grid Model::grid(int nx, int ny, int nz)
{
    Grid grid(nx, ny, nz);
    grid.iterate(QVector3D(1.0, 1.0, 1.0), [&](QVector3D p, float &v) {
        v = this->isInVoid(p);
    });

    return grid;
}

void Model::setParameters(Parameters *parameters)
{
    if (m_parameters == parameters)
        return;

    m_parameters = parameters;
    emit parametersChanged(parameters);
    parametersUpdated();
}

void readNoiseParameters(CIniFile *iniFile, RegularNoiseModel *noiseModel) {
    if(QString::fromStdString((iniFile->getstring("noise_properties_filename"))).isEmpty()) {
        noiseModel->parameters()->setValue("octaves", iniFile->getdouble("noise_octaves"));
        noiseModel->parameters()->setValue("scale", iniFile->getdouble("noise_scale"));
        noiseModel->parameters()->setValue("persistence", iniFile->getdouble("noise_persistence"));
        noiseModel->parameters()->setValue("threshold", iniFile->getdouble("noise_threshold"));
        noiseModel->parameters()->setValue("inverted", iniFile->getbool("noise_inverted"));
        noiseModel->parameters()->setValue("seed", iniFile->getdouble("noise_seed"));
        noiseModel->parameters()->setValue("absolute", iniFile->getbool("noise_absolute"));
        noiseModel->parameters()->setValue("skewscale", iniFile->getdouble("noise_skewscale"));
        noiseModel->parameters()->setValue("skewamplitude", iniFile->getdouble("noise_skewamplitude"));
        noiseModel->parameters()->getParameter("noisetype")->setString(QString::fromStdString((iniFile->getstring("noise_noisetype"))));
    } else {
        noiseModel->parameters()->load(QString::fromStdString((iniFile->getstring("noise_properties_filename"))));
        noiseModel->parameters()->setValue("seed", iniFile->getdouble("noise_seed"));
    }
}
