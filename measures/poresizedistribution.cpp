//#include "poresizedistribution.h"
//#include "../misc/distancetoatommap.h"
//#include "../models/models.h"

//PoreSizeDistribution::PoreSizeDistribution()
//{

//}


//void PoreSizeDistribution::compute(Model &model)
//{
//    DistanceToAtomMap dtam(128, 128, 128);

//    QVector<QVector3D> points;
//    XYZModel *xyz = qobject_cast<XYZModel*>(&model);

//    if(xyz) points = xyz->getPoints();
//    if(!points.size()) {
//        qDebug() << "Error in void PoreSizeDistribution::compute(Model &model), we have no points.";
//        return;
//    }

//    dtam.build(points, 15);
//    QVector<float> voxels = dtam.grid().voxels();
//    qSort(voxels);

//}

//QVector<QPointF> PoreSizeDistribution::histogram(int bins)
//{

//}
