#include "octree.h"
#include <fstream>

int Octree::maxDepth() const
{
    return m_maxDepth;
}

void Octree::build2DTriangleList()
{
    if (m_root==nullptr)
        return;

    qDebug() << "Creating 2D Triangle list";
    m_vboData.clear();

    m_root->build2DTriangleList(m_vboData);
    m_vboDirty = true;
}

void OctNode::build2DTriangleList(QVector<SimVis::TriangleCollectionVBOData> &data)
{
    if (hasChildren()) {
        for (int i=0;i<m_children.size()/1;i++)
            m_children[i]->build2DTriangleList(data);
    }
     else {
    //    qDebug() << m_depth << " value " << m_value;
        SimVis::TriangleCollectionVBOData tri;
        QVector3D color(0.15, 0.2, 0.9);
        if (m_value==0) {
            color = QVector3D(0.8, 0.2, 0.1);
            return;
        }
        color.setY(rand()%100 / 100.0);
        if (m_melted)
            color*=0.3;

        data.append(tri);
        data.append(tri);
        data.append(tri);
        int i = data.count()-3;
        float z = m_corner1.z();
        data[i].vertex = QVector3D(m_corner1.x(), m_corner1.y(), z);
        data[i+1].vertex = QVector3D(m_corner1.x(), m_corner2.y(), z);
        data[i+2].vertex = QVector3D(m_corner2.x(), m_corner1.y(), z);

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

        data[i].vertex = QVector3D(m_corner2.x(), m_corner2.y(), z);
        data[i+1].vertex = QVector3D(m_corner1.x(), m_corner2.y(), z);
        data[i+2].vertex = QVector3D(m_corner2.x(), m_corner1.y(), z);

        data[i].color = color;
        data[i+1].color = color;
        data[i+2].color = color;
        data[i].normal = N;
        data[i+1].normal = N;
        data[i+2].normal = N;
    }
}

void OctNode::InsertNode(QStringList &list)
{
    if (list.length()==1)
        m_value = list[0].toFloat();
    else
    {
        int quadrant = list[0].toFloat();
        if (!hasChildren())
            subdivide();
        list.removeAt(0);
        m_children[quadrant]->InsertNode(list);
    }
}


void Octree::setMaxDepth(int maxDepth)
{
    m_maxDepth = maxDepth;
}

bool Octree::isInVoid(float x, float y, float z)
{
    if (m_root == nullptr)
        return true;

    OctNode* on = m_root->findNodePoint(QVector3D(x,y,z));
    if (on==nullptr) {
//        qDebug() << "Octree::isInVod outside bounds!";
        return true;
    }
    if (on->getValue()!=0)
        return false;

    return true;
}

void Octree::parametersUpdated()
{
//    XYZModel
    m_maxDepth = m_parameters->getValue("maxdepth");
    m_threshold = m_parameters->getValue("threshold");

}

void Octree::createParameters()
{
    qDebug() << "Creating in octree";
    m_parameters->createParameter("maxdepth", 6, 1, 12, 1);
    m_parameters->createParameter("threshold", 2,1 ,5, 0.1);
}

QVector<SimVis::TriangleCollectionVBOData> Octree::vboData() const
{
    return m_vboData;
}

Octree::Octree() : XYZModel()
{
    createParameters();
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
    // QVector3D center = (maxVal + minVal)/2.0;

//    for (int i=0;i<m_points.count();i++)
//        m_points[i]-=center;

//    maxVal-=center;
//    minVal-=center;

    m_root = new OctNode(minVal, maxVal, 0, 0);

    qDebug() << "Building octree with max depth : " << m_maxDepth << " and " << m_points.size() << " particles";

    for (QVector3D p : m_points)
        insertValueAtPoint(p, 1);

//    for (int i=0;i<6;i++)
    melt();

    build2DTriangleList();

//    saveOctree("octree.txt");
}

void Octree::loadOctree(QString filename)
{
    qDebug() << "Load octree from EHG" << filename;


    QVector3D minVal, maxVal;
    filename.remove(0,5);
    QFile inputFile(filename);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       int cnt = 0;

      while (!in.atEnd())
       {
          QStringList lst = in.readLine().split(",");
          if (cnt==0) {// corner 0
              minVal = QVector3D(lst[0].toFloat(),lst[1].toFloat(),lst[2].toFloat());
          }
          if (cnt==1) {// corner 1
              maxVal = QVector3D(lst[0].toFloat(),lst[1].toFloat(),lst[2].toFloat());
          }
          if (cnt==2) {
              m_root = new OctNode(minVal, maxVal,0,0);
          }
          if (cnt>=2) {
              m_root->InsertNode(lst);
          }
          cnt++;

       }
       inputFile.close();
    }
    build2DTriangleList();

}

void Octree::saveOctree(QString filename)
{
    qDebug() << "Saving octree to " << filename;
    if (m_root == nullptr)
        return;
    ofstream file;

    file.open(filename.toStdString().c_str(),std::ofstream::out);

    if(file.is_open()) {
        file << m_root->getCorner1().x() << ", " << m_root->getCorner1().y() << ", " <<  m_root->getCorner1().z() << endl;
        file << m_root->getCorner2().x() << ", " << m_root->getCorner2().y() << ", " <<  m_root->getCorner2().z() << endl;
        m_root->saveNode("", file);
        file.close();
    }

/*    corner 1
    corner 2

    # quadrant, quadrant, quadrant value
      1, 1
      2, 1, 0
*/
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
        m_melted = true;
        m_children.resize(0);
    }
}


QVector3D OctNode::getCorner1() const
{
    return m_corner1;
}

QVector3D OctNode::getCorner2() const
{
    return m_corner2;
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

void OctNode::saveNode(QString serialized, ofstream &file)
{
    if (hasChildren()) {
        for (int i=0;i<m_children.count();i++)
            m_children[i]->saveNode(serialized + QString::number(i) + ",", file);
    }
    else
    if (m_value!=0)
    {
        serialized += QString::number(m_value);
        file << serialized.toStdString() << std::endl;

    }


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


    if (m_depth<maxDepth) {
        if (!hasChildren())
            subdivide();
        OctNode* n = findNodePoint(p); // should always be found
        n->insertValueAtDeepestPoint(p,value, maxDepth);
    }
/*    if (value == m_value)
        return;
*/
    // depth == maxdepth
    if (m_depth == maxDepth) {
        m_value = value;
//        qDebug() << m_depth << "with value" << value << endl;
    }
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

