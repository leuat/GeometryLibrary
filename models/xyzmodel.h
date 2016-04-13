#ifndef XYZMODEL_H
#define XYZMODEL_H
#include "model.h"
#include <SimVis/TriangleCollection>
#include <QVector>
#include <QVector3D>

typedef vector<vector<vector<vector<QVector3D> > > > CellList;
class XYZModel : public Model
{
    Q_OBJECT
    Q_PROPERTY(QString file READ file WRITE setFile NOTIFY fileChanged)
    Q_PROPERTY(int voxelsPerDimension READ voxelsPerDimension WRITE setVoxelsPerDimension NOTIFY voxelsPerDimensionChanged)
    Q_PROPERTY(float threshold READ threshold WRITE setThreshold NOTIFY thresholdChanged)
    Q_PROPERTY(float maxDistance READ maxDistance WRITE setMaxDistance NOTIFY maxDistanceChanged)
    Q_PROPERTY(int fillAndErodeDepth READ fillAndErodeDepth WRITE setFillAndErodeDepth NOTIFY fillAndErodeDepthChanged)

protected:
    QString m_file;
    QVector<QVector3D> m_points;
    vector<int> m_voxels;
    int m_fillAndErodeDepth = 1;
    float m_lx=0, m_ly=0, m_lz=0;
    float m_oneOverLx=0, m_oneOverLy=0, m_oneOverLz=0;
    int m_voxelsPerDimension = 0;
    float m_threshold = 0;
    float m_maxDistance = 100;
    bool m_isValid = false;

public:
    void calculateBoundingbox();
    Q_INVOKABLE void erode(int depth = 1);
    Q_INVOKABLE void fill(int depth = 1);

    inline int index(const int &i, const int &j, const int &k) { return i*m_voxelsPerDimension*m_voxelsPerDimension + j*m_voxelsPerDimension + k; }
    inline int indexPeriodic(const int i, const int j, const int k) { return ((i+m_voxelsPerDimension) % m_voxelsPerDimension)*m_voxelsPerDimension*m_voxelsPerDimension + ((j+m_voxelsPerDimension) % m_voxelsPerDimension)*m_voxelsPerDimension + ((k + m_voxelsPerDimension) % m_voxelsPerDimension); }
    inline void getIndexVectorFromIndex(const int &index, int &i, int &j, int &k) {
        i = index/(m_voxelsPerDimension*m_voxelsPerDimension);
        j = (index / m_voxelsPerDimension) % m_voxelsPerDimension;
        k = index % m_voxelsPerDimension;
    }

    XYZModel();
    QString file() const;
    int voxelsPerDimension() const;
    float threshold() const;
    float maxDistance() const;
    Q_INVOKABLE void readFile();
    Q_INVOKABLE void removeCylinder(float r);
    void updateDistanceToAtomField();
    float getLx() const;
    float getLy() const;
    float getLz() const;
    QVector<QVector3D> getPoints() const;
    CellList buildCellList(QVector3D cellSize, int numCellsX, int numCellsY, int numCellsZ);

    static void addQuad(QVector<SimVis::TriangleCollectionVBOData> &data, QVector3D c1,QVector3D c2,QVector3D c3, QVector3D c4, QVector3D color);

    using Model::isInVoid;

    virtual bool isInVoid(float x, float y, float z) override;
    virtual void parametersUpdated() override;
    virtual void createParameters() override;
    void loadParameters(CIniFile *iniFile) override;
    void removeFromModel(Model *model);
    int fillAndErodeDepth() const;

public slots:
    void setFile(QString file);
    void setVoxelsPerDimension(int voxelsPerDimension);
    void setThreshold(float threshold);
    void setMaxDistance(float maxDistance);
    void setFillAndErodeDepth(int fillAndErodeDepth);

signals:
    void fileChanged(QString file);
    void voxelsPerDimensionChanged(int voxelsPerDimension);
    void thresholdChanged(float threshold);
    void maxDistanceChanged(float maxDistance);
    void fillChanged(int fill);
    void fillAndErodeDepthChanged(int fillAndErodeDepth);
};

#endif // XYZMODEL_H
