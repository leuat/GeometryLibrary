#ifndef SURFACEAREA_H
#define SURFACEAREA_H

#include "measure.h"



class SurfaceArea : public Measure
{
    Q_OBJECT
    Q_PROPERTY(int nx READ nx WRITE setNx NOTIFY nxChanged)
    Q_PROPERTY(int ny READ ny WRITE setNy NOTIFY nyChanged)
    Q_PROPERTY(int nz READ nz WRITE setNz NOTIFY nzChanged)
    Q_PROPERTY(float threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)
    Q_PROPERTY(float cutoff READ cutoff WRITE setCutoff NOTIFY cutoffChanged)

public:
    SurfaceArea();

    // Measure interface
public:
    virtual void compute(const QVector<QVector3D> &points, bool printResult = false) override;
    int nx() const;
    int ny() const;
    int nz() const;
    float threshold() const;
    float cutoff() const;

public slots:
    void setNx(int nx);
    void setNy(int ny);
    void setNz(int nz);
    void setThreshold(float threshold);
    void setCutoff(float cutoff);

signals:
    void nxChanged(int nx);
    void nyChanged(int ny);
    void nzChanged(int nz);
    void thresholdChanged(float threshold);
    void cutoffChanged(float cutoff);

private:
    int m_nx;
    int m_ny;
    int m_nz;
    float m_threshold;
    float m_cutoff;
};

#endif // SURFACEAREA_H
