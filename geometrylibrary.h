#pragma once
#include "geometrylibrary_global.h"
#include <Noise.h>
#include "noiseparameters.h"

class GEOMETRYLIBRARYSHARED_EXPORT GeometryLibrary
{

private:
    Noise* m_noise;
    NoiseParameters* m_noiseParameters;
    bool m_initialized = false;
    double m_scale = 1;
    double m_threshold = 0;
    bool m_inverted = false;
    bool m_absolute = false;

public:
    enum NoiseType { SimplexNoise, PerlinNoise };
    GeometryLibrary();

    void Initialize(NoiseType noiseType, NoiseParameters* np);

    bool IsInVoid(const QVector3D& pos);
};

