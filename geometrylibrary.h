#pragma once
#include "geometrylibrary_global.h"
#include "noise.h"
#include "models/parameters.h"
#include "models/regularmodel.h"
#include "models/model.h"
#include "models/multifractalmodel.h"

class GEOMETRYLIBRARYSHARED_EXPORT GeometryLibrary
{

private:
    Model* m_model = nullptr;

public:
    enum GeometryModel { Regular, MultiFractal };
    GeometryLibrary();

    void initialize(GeometryModel m, Noise::NoiseType noiseType, Parameters* p) {
        if (m == Regular)
            m_model = new RegularModel();
        if (m == MultiFractal)
            m_model = new MultiFractalModel();

        if (m_model!=nullptr) {
            m_model->initialize(noiseType, p);
        }

    }
    bool isInVoid(const QVector3D& pos) {
        if (m_model == nullptr)
            return false;
        return m_model->isInVoid(pos);
    }

    Model *model() const;
};

