#pragma once

#include "model.h"
#include <QVector>
#include <QVector3D>
#include "GeometryLibrary/noise.h"

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



class Octree : public Model
{
    Q_OBJECT
private:

    int m_maxDepth = 0;
    OctNode* m_root = nullptr;

public:
    Octree();

    // creates octtree structure from list of particles
    void create(QVector<QVector3D>& particleList, int maxDepth);
    // Load and save octtree states
    void load(QString filename);
    void save(QString filename);
    // inherited functions
    void initialize(Noise::NoiseType noiseType, Parameters* np);

    void insertValueAtPoint(const QVector3D& p, float value);
    void melt();

    int maxDepth() const;
    void setMaxDepth(int maxDepth);

    // Model interface
public:
    bool isInVoid(float x, float y, float z);

    void parametersUpdated();
};

