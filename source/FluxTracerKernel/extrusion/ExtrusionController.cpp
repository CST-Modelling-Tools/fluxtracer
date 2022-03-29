#include "ExtrusionController.h"

#include "math/2D/FluxSurface.h"
#include "common/BinaryStream.h"

using namespace cyi;



ExtrusionController::ExtrusionController()
{

}

void ExtrusionController::initLinear()
{
    int nMax = m_gridPhi.divisions();
    int mMax = m_gridR.divisions() + 1;
    m_receiver.initMesh(nMax, mMax);

    QVector<double> parameters;
    for (int n = 0; n < nMax; ++n)
      for (int m = 0; m < mMax; ++m)
        parameters << m_gridZ.point(m);
    for (int n = 0; n < nMax; ++n)
        parameters << m_gridR.interval().min() << m_gridR.interval().max();
    m_receiver.initPoints(parameters);
}

void ExtrusionController::setRays(const QString& fileName)
{
    m_rays.clear();

    FluxSurface fs;
    fs.open(fileName);

    for (int n = 0; n < fs.points().data().size(); ++n) {
        const vec3f& origin = fs.points().data()[n];
        const vec3f& direction = fs.vectors().data()[n];
        real energy = direction.norm();
        if (energy > 0.)
            m_rays << Ray(origin, origin + direction, energy);
    }
}
