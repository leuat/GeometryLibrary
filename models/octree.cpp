#include "octree.h"

int Octree::maxDepth() const
{
    return m_maxDepth;
}

QVector<SimVis::TriangleCollectionVBOData>* Octree::build2DTriangleList()
{
    if (m_root==nullptr)
        return nullptr;

    QVector<SimVis::TriangleCollectionVBOData>* data = new QVector<SimVis::TriangleCollectionVBOData>();
    data->clear();
    m_root->build2DTriangleList(*data);

}

void OctNode::build2DTriangleList(QVector<SimVis::TriangleCollectionVBOData> &data)
{
    if (hasChildren()) {
        for (int i=0;i<m_children.size()/2;i++)
            m_children[i]->build2DTriangleList(data);
    }
    else {
        SimVis::TriangleCollectionVBOData tri;
        QVector3D color(0.8,0.2,0);
        if (m_value!=0)
            color = QVector3D(0.1, 0.2, 0.8);

        data.append(tri);
        data.append(tri);
        data.append(tri);
        int i = data.count()-3;
        data[i].vertex = QVector3D(m_corner1.x(), m_corner1.y(), 0);
        data[i+1].vertex = QVector3D(m_corner1.x(), m_corner2.y(), 0);
        data[i+2].vertex = QVector3D(m_corner2.x(), m_corner1.y(), 0);

        QVector3D N = QVector3D::crossProduct((data[i].vertex-data[i+2].vertex), (data[i].vertex-data[i+1].vertex)).normalized();

        data[i].color = color;
        data[i+1].color = color;
        data[i+2].color = color;
        data[i].normal = N;
        data[i+1].normal = N;
        data[i+2].normal = N;

        data.append(tri);
        data.append(tri);
        data.append(tri);
        i = data.count()-3;

        data[i].vertex = QVector3D(m_corner2.x(), m_corner2.y(), 0);
        data[i+1].vertex = QVector3D(m_corner1.x(), m_corner2.y(), 0);
        data[i+2].vertex = QVector3D(m_corner2.x(), m_corner1.y(), 0);

        data[i].color = color;
        data[i+1].color = color;
        data[i+2].color = color;
        data[i].normal = N;
        data[i+1].normal = N;
        data[i+2].normal = N;

    }
}


void Octree::setMaxDepth(int maxDepth)
{
    m_maxDepth = maxDepth;
}

bool Octree::isInVoid(float x, float y, float z)
{

}

void Octree::parametersUpdated()
{
//    XYZModel
    m_maxDepth = m_parameters->getValue("maxdepth");
    m_threshold = m_parameters->getValue("threshold");

}

void Octree::createParameters()
{
    m_parameters->createParameter("maxdepth", 4, 1, 12, 1);
    m_parameters->createParameter("threshold", 2,1 ,5, 0.1);

}

Octree::Octree()
{

}

void Octree::buildTree()
{
    double inf = 1E30;


    QVector3D minVal(inf, inf, inf);
    QVector3D maxVal(-inf, -inf, -inf);
    // Get min / Max values
    m_maxDepth = m_parameters->getValue("maxdepth");
    for(int i=0;i<m_points.count();i++) {
        minVal.setX(min(minVal.x(), m_points[i].x()));
        minVal.setY(min(minVal.y(), m_points[i].y()));
        minVal.setZ(min(minVal.z(), m_points[i].z()));
        maxVal.setX(max(maxVal.x(), m_points[i].x()));
        maxVal.setY(max(maxVal.y(), m_points[i].y()));
        maxVal.setZ(max(maxVal.z(), m_points[i].z()));
    }

    m_root = new OctNode(minVal, maxVal, 0, 0);

    for (QVector3D p : m_points)
        insertValueAtPoint(p, 1);

}

void Octree::loadOctree(QString filename)
{

}

void Octree::saveOctree(QString filename)
{

}

void Octree::insertValueAtPoint(const QVector3D &p, float value)
{
    if (m_root == nullptr)
        return;
    m_root->insertValueAtDeepestPoint(p,value,m_maxDepth);
}

void Octree::melt()
{
    if (m_root==nullptr)
        return;
    m_root->melt();
}

void Octree::initialize(Noise::NoiseType noiseType, Parameters *np)
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


int OctNode::getDepth() const
{
    return m_depth;
}

void OctNode::melt()
{
    if (!hasChildren())
        return;

    for (OctNode* on : m_children)
        on->melt();

    // check if all children has same value
    float value = m_children[0]->getValue();
    bool allSame = true;
    for (OctNode* on : m_children) {
        if (on->getValue()!=value)
            allSame = false; // all values must be equal
         if (on->hasChildren())
             allSame = false; // and none of the children must have children
       }

    if (allSame) { // MELT!
        m_value = value;
        m_children.resize(0);
    }
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
        return m_children[index];

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
    m_children[1] = new OctNode(QVector3D(c.x(), c1.y(), c1.z()), QVector3D(c2.x(), c.y(), c.z()), m_depth+1, m_value);
    m_children[2] = new OctNode(QVector3D(c.x(), c.y(), c1.z()), QVector3D(c2.x(), c2.y(), c.z()), m_depth+1, m_value);
    m_children[3] = new OctNode(QVector3D(c1.x(), c.y(), c1.z()), QVector3D(c.x(), c2.y(), c.z()), m_depth+1, m_value);

    m_children[4] = new OctNode(QVector3D(c1.x(), c1.y(), c.z()), QVector3D(c.x(), c.y(), c2.z()), m_depth+1, m_value);
    m_children[5] = new OctNode(QVector3D(c.x(), c1.y(), c.z()), QVector3D(c2.x(), c.y(), c2.z()), m_depth+1, m_value);
    m_children[6] = new OctNode(c, c2, m_depth+1, m_value);
    m_children[7] = new OctNode(QVector3D(c1.x(), c.y(), c.z()), QVector3D(c.x(), c2.y(), c2.z()), m_depth+1, m_value);


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

OctNode* OctNode::findNodePoint(const QVector3D &p)
{
    if (!pointIsWithin(p))
        return nullptr;

    if (hasChildren()) {
        for (OctNode* on: m_children) {
            OctNode* ret = on->findNodePoint(p);
            if (ret != nullptr)
                return ret;
        }
    }
    else
        return this;

}


void OctNode::insertValueAtDeepestPoint(const QVector3D &p, float value, int maxDepth) {
    if (!pointIsWithin(p))
        return;

    if (value == m_value)
        return;

    if (m_depth<maxDepth) {
        subdivide();
        OctNode* n = findNodePoint(p); // should always be found
        n->insertValueAtDeepestPoint(p,value, maxDepth);
    }
    // depth == maxdepth

    m_value = value;
}

// NOT USE YET
void OctNode::insertValueAtPoint(const QVector3D &p, float value, int maxDepth)
{
    if (!pointIsWithin(p))
        return;
    // point is within, so check value

    if (m_value!= value) { // subdivide and insert
        subdivide();
        OctNode* n = findNodePoint(p); // should always be found
        if (n == nullptr) qDebug("SOMETHING SERIOUS WROING IN OCTNODE::INSERTVALUEATPOINT");
     //   n->
    }
}

