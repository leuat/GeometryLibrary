#include "octree.h"

int Octree::maxDepth() const
{
    return m_maxDepth;
}

void Octree::setMaxDepth(int maxDepth)
{
    m_maxDepth = maxDepth;
}

Octree::Octree()
{

}

void Octree::create(QVector<QVector3D>& particleList, int maxDepth)
{
    double inf = 1E30;


    QVector3D minVal(inf, inf, inf);
    QVector3D maxVal(-inf, -inf, -inf);
    // Get min / Max values
    for(int i=0;i<particleList.count();i++) {
        minVal.setX(min(minVal.x(), particleList[i].x()));
        minVal.setY(min(minVal.y(), particleList[i].y()));
        minVal.setZ(min(minVal.z(), particleList[i].z()));
        maxVal.setX(max(maxVal.x(), particleList[i].x()));
        maxVal.setY(max(maxVal.y(), particleList[i].y()));
        maxVal.setZ(max(maxVal.z(), particleList[i].z()));
    }

    m_root = new OctNode(minVal, maxVal, 0, 0);


}

void Octree::initialize(Noise::NoiseType noiseType, Parameters *np)
{

}

bool Octree::isInVoid(const QVector3D &pos)
{

}

int OctNode::getValue() const
{
    return m_value;
}

void OctNode::setValue(int value)
{
    m_value = value;
}

bool OctNode::getHasChildren() const
{
    return m_hasChildren;
}

int OctNode::getDepth() const
{
    return m_depth;
}


OctNode::OctNode(QVector3D corner1, QVector3D corner2, int level, float value)
{
    m_corner1 = corner1;
    m_corner2 = corner2;
    m_center = (corner1 + corner2)*0.5;
    m_depth = level;
    m_value = value;
}



OctNode *OctNode::getQuadrant(const int &index)
{
    if (hasChildren())
        return children[i];

    return nullptr;
}

QVector<OctNode *> OctNode::getChildren() const
{
    return m_children;
}

void OctNode::subdivide()
{
    QVector3D c = m_center;
    QVector3D c1 = m_corner1;
    QVector3D c2 = m_corner2;

    m_children.resize(8);
    m_children[0] = new OctNode(c1, c, m_depth+1, m_value);
    m_children[1] = new OctNode(QVector3D(c.x, c1.y, c1.z), QVector3D(c2.x, c.y, c.z), m_depth+1, m_value);
    m_children[2] = new OctNode(QVector3D(c.x, c.y, c1.z), QVector3D(c2.x, c2.y, c.z), m_depth+1, m_value);
    m_children[3] = new OctNode(QVector3D(c1.x, c.y, c1.z), QVector3D(c.x, c2.y, c.z), m_depth+1, m_value);

    m_children[4] = new OctNode(QVector3D(c1.x, c1.y, c.z), QVector3D(c.x, c.y, c2.z), m_depth+1, m_value);
    m_children[5] = new OctNode(QVector3D(c.x, c1.y, c.z), QVector3D(c2.x, c.y, c2.z), m_depth+1, m_value);
    m_children[6] = new OctNode(c, c2, m_depth+1, m_value);
    m_children[7] = new OctNode(QVector3D(c1.x, c.y, c.z), QVector3D(c.x, c2.y, c2.z), m_depth+1, m_value);


}

bool OctNode::hasChildren()
{
    if (m_children.count()!=8)
        return false;

    return true;

}

bool OctNode::pointIsWithin(const QVector3D &p)
{
    for (int i=0;i<3;i++)
    if (p[i]>=m_corner2[i] || p[i]<m_corner1[i])
        return false;

    return true;
}

OctNode OctNode::findNodePoint(const QVector3D &p)
{
    if (!pointIsWithin(p))
        return nullptr;

    if (hasChildren()) {
        foreach (OctNode* on: m_children) {
            OctNode* ret = on->findNodePoint(p);
            if (ret != nullptr)
                return ret;
        }
    }
    else
        return this;

}

