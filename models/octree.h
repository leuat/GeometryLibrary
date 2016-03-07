#pragma once

#include "model.h"
#include "xyzmodel.h"
#include <QVector>
#include <QVector3D>
#include "GeometryLibrary/noise.h"
#include <SimVis/TriangleCollection>

class OctNode
{
private:
    QVector3D m_corner1, m_corner2, m_center;
    QVector<OctNode*> m_children;

    float m_value = 0;
    int m_depth = 0;


public:

    OctNode(QVector3D corner1, QVector3D corner2, int level, float value);

    OctNode* getQuadrant(const int& index);

    QVector<OctNode *> getChildren() const;
    void subdivide();
    bool hasChildren();

    void build2DTriangleList(QVector<SimVis::TriangleCollectionVBOData>& data);


    bool pointIsWithin(const QVector3D& p);
    OctNode* findNodePoint(const QVector3D& p);
    // Use this
    void insertValueAtDeepestPoint(const QVector3D& p, float value, int maxDepth);
    // Do not use
    void insertValueAtPoint(const QVector3D& p, float value, int maxDepth);

    int getValue() const;
    void setValue(int value);
    bool getHasChildren() const;
    int getDepth() const;
    void melt();
};

class Octree : public XYZModel
{
    Q_OBJECT
private:

    int m_maxDepth = 0;
    OctNode* m_root = nullptr;

public:
    Octree();

    // creates octtree structure from list of particles
    Q_INVOKABLE void buildTree();
    // Load and save octtree states
    Q_INVOKABLE void loadOctree(QString filename) ;
    Q_INVOKABLE void saveOctree(QString filename);



    // inherited functions
    void initialize(Noise::NoiseType noiseType, Parameters* np);

    void insertValueAtPoint(const QVector3D& p, float value);
    void melt();

    int maxDepth() const;
//    void build2DTriangleList(QVector<SimVis::TriangleCollectionVBOData>& data);
    QVector<SimVis::TriangleCollectionVBOData>* build2DTriangleList();

    void setMaxDepth(int maxDepth);

    // Model interface
public:
    bool isInVoid(float x, float y, float z) override;

    void parametersUpdated() override;

    // Model interface
public:
    void createParameters() override;
};

