#include "AngleWithPlane.h"

using namespace cyi;

AngleWithPlane::AngleWithPlane(const vec3r& planeNormal, const Grid& angleGrid)
    : m_planeNormal{planeNormal}, m_bins{angleGrid}
{
}

void AngleWithPlane::process(const Ray& ray)
{
    vec3r unitVectorDirection = -ray.vector().normalized();
    float angle = asin(dot(m_planeNormal, unitVectorDirection));
    m_bins(angle) += ray.energy();
}

void AngleWithPlane::combine(const RayFunctor& rf)
{
    const AngleWithPlane& sr = dynamic_cast<const AngleWithPlane&>(rf);
    m_bins += sr.m_bins;
}

// export
bool AngleWithPlane::save(const QString& fileName) const
{
    return m_bins.save(fileName);
}
