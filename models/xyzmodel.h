#ifndef XYZMODEL_H
#define XYZMODEL_H
#include "model.h"

class XYZModel : public Model
{
    Q_OBJECT
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(int voxelsPerDimension READ voxelsPerDimension WRITE setVoxelsPerDimension NOTIFY voxelsPerDimensionChanged)
    Q_PROPERTY(float threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)
private:
    QString m_file;
    float m_lx=0, m_ly=0, m_lz=0;
    float m_oneOverLx=0, m_oneOverLy=0, m_oneOverLz=0;
    int m_nx=0, m_ny=0, m_nz=0;
    int m_voxelsPerDimension = 0;
    float m_threshold = 0;
    vector<int> m_voxels;
    inline int index(const int &i, const int &j, const int &k) { return i*m_ny*m_nz + j*m_nz + k; }
    void getIndexVectorFromIndex(const int &index, int &i, int &j, int &k) {
        i = index/(m_ny*m_nz);
        j = (index / m_nz) % m_ny;
        k = index % m_nz;
    }
public:
    XYZModel();
    QString file() const;

    // Model interface
public:
    bool isInVoid(float x, float y, float z) override;
    void parametersUpdated() override;
    void createParameters() override;
    void loadParameters(CIniFile *iniFile) override;
    int voxelsPerDimension() const;
    float threshold() const;

public slots:
    void setFile(QString file);
    void setVoxelsPerDimension(int voxelsPerDimension);
    void setThreshold(float threshold);

signals:
    void fileChanged(QString file);
    void voxelsPerDimensionChanged(int voxelsPerDimension);
    void thresholdChanged(float threshold);
};

#endif // XYZMODEL_H
