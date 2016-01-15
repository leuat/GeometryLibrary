#pragma once
#include "geometrylibrary_global.h"
#include "noise.h"
#include "parameters.h"
#include "models/regularmodel.h"
#include "geometrymodel.h"

class GEOMETRYLIBRARYSHARED_EXPORT GeometryLibrary
{

private:

    GeometryModel* m_geometryModel = 0;

public:
    enum Model { Regular, MultiFractal };
    GeometryLibrary();

    void Initialize(Model m, Noise::NoiseType noiseType, Parameters* p) {
        if (m == Regular)
            m_geometryModel = new RegularModel();

        if (m_geometryModel!=nullptr) {
            m_geometryModel->Initialize(noiseType, p);
        }

    }
    bool IsInVoid(const QVector3D& pos) {
        if (m_geometryModel == nullptr)
            return false;
        return m_geometryModel->IsInVoid(pos);
    }


    GeometryModel *geometryModel() const;
};

