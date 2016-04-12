#include "octree.h"
#include <fstream>

int Octree::maxDepth() const
{
    return m_maxDepth;
}

void Octree::build2DTriangleList()
{
    // return;

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
            color = QVector3D(0.99, 0.2, 0.1);
            return;
        }
        color.setY(1.0*(rand()%100 / 100.0));
        if (m_melted)
            color*=0.3;

//        float z = m_corner1.z();

        XYZModel::addQuad(data, getCorner(0), getCorner(1), getCorner(3), getCorner(2),color);
        XYZModel::addQuad(data, getCorner(4), getCorner(5), getCorner(7), getCorner(6),color);
        XYZModel::addQuad(data, getCorner(1), getCorner(5), getCorner(2), getCorner(6),color);
        XYZModel::addQuad(data, getCorner(0), getCorner(4), getCorner(3), getCorner(7),color);
        XYZModel::addQuad(data, getCorner(3), getCorner(2), getCorner(7), getCorner(6),color);
        XYZModel::addQuad(data, getCorner(0), getCorner(1), getCorner(4), getCorner(5),color);


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

void OctNode::DepthHistogram(QVector<QPointF>& points)
{
    if (!hasChildren())
        points[m_depth].setY(points[m_depth].y()+1);
    else
        for (OctNode* octNode : m_children)
            octNode->DepthHistogram(points);
}


void Octree::setMaxDepth(int maxDepth)
{
    m_maxDepth = maxDepth;
}

void Octree::setPoints(QVector<QVector3D> &points)
{
    m_points = points;
}

void Octree::calculateOctreeMeasure(QVector<QPointF> &points)
{
    points.resize(m_maxDepth);
    for (int i=0;i<points.size();i++)
        points[i] = QPoint(i,0);

    m_root->DepthHistogram(points);

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
    m_maxDistance = m_threshold;

}

void Octree::createParameters()
{
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

void Octree::buildTree(bool fromCellList, bool createTriangleList)
{

    m_root = new OctNode(QVector3D(0,0,0), QVector3D(m_lx, m_ly, m_lz), 0, 0);
    qDebug() << m_lx;

    if (!fromCellList) {
        qDebug() << "Building octree with max depth : " << m_maxDepth << " and " << m_points.size() << " particles";
        for (const QVector3D& p : m_points)
            insertValueAtPoint(p, 1);
    }
    else {
        // Based on cell list
        int N = pow(2, m_maxDepth);
//        qDebug() << "build octree from voxellist (test): cellsize = " << size;
        qDebug() << "number of cells: " << N;

        setVoxelsPerDimension(N);

        qDebug() << "updateDistance";
        updateDistanceToAtomField();
        qDebug() << "end";

        //CellList cellList = buildCellList(size, N,N,N);
        for (int i=0;i<N;i++) {
            for (int j=0;j<N;j++) {
 //               qDebug() << j;

                for (int k=0;k<N;k++) {

                    if (m_voxels[index(i,j,k)]!=0)
                    {

                        QVector3D p( (i+0.5)*m_lx/(float)N,
                                     (j+0.5)*m_ly/(float)N,
                                     (k+0.5)*m_lz/(float)N );

                        insertValueAtPoint(p, m_voxels[index(i,j,k)]);
                    }
                }
            }
        }
//*/
    }
    melt();
    if (createTriangleList)
        build2DTriangleList();

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

QVector3D OctNode::getCorner(const int id) const
{
    if (id==0)
        return m_corner1;
    if (id==1)
        return QVector3D(m_corner2.x(), m_corner1.y(), m_corner1.z());
    if (id==2)
        return QVector3D(m_corner2.x(), m_corner2.y(), m_corner1.z());
    if (id==3)
        return QVector3D(m_corner1.x(), m_corner2.y(), m_corner1.z());
    if (id==4)
        return QVector3D(m_corner1.x(), m_corner1.y(), m_corner2.z());
    if (id==5)
        return QVector3D(m_corner2.x(), m_corner1.y(), m_corner2.z());
    if (id==6)
        return m_corner2;
    if (id==7)
        return QVector3D(m_corner1.x(), m_corner2.y(), m_corner2.z());
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



void Octree::loadParameters(CIniFile *iniFile)
{
    m_file = QString::fromStdString(iniFile->getstring("xyzfile_file"));

    m_maxDepth = log2(iniFile->getdouble("xyzfile_resolution"));
    m_threshold = iniFile->getdouble("xyzfile_threshold");
    readFile();
    if(iniFile->find(QString("cut_cylinder"), true)) {
        double radius = iniFile->getdouble("cylinder_radius");
        double cutRadius = radius / (m_lx*0.5); // this radius is between 0 and 1
        removeCylinder(cutRadius);
    }
    this->buildTree(true, true);
}
