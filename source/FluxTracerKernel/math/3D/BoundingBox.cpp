#include "BoundingBox.h"

using namespace cyi;



BoundingBox::BoundingBox(const vec3r& cornerMin, const vec3r& cornerMax)
{
    setCorners(cornerMin, cornerMax);
}

void BoundingBox::setCorners(const vec3r& cornerMin, const vec3r& cornerMax)
{
    m_cornerMin = cornerMin;
    m_cornerMax = cornerMax;
    m_extent = m_cornerMax - m_cornerMin;
}

bool BoundingBox::isInside(const vec3r &v) const
{
    return v.x() >= m_cornerMin.x() && v.x() <= m_cornerMax.x() &&
           v.y() >= m_cornerMin.y() && v.y() <= m_cornerMax.y() &&
           v.z() >= m_cornerMin.z() && v.z() <= m_cornerMax.z();
}

bool BoundingBox::isEqual(const BoundingBox& other, real eps) const
{
    return m_cornerMin.equal(other.m_cornerMin, eps) && m_cornerMax.equal(other.m_cornerMax, eps);
}

/**
 * Find intersection of box with ray
 * t = 0 corresponds to ray.start
 * t = 1 corresponds to ray.end
 */
bool BoundingBox::isIntersected(const Ray& ray, real& tNear, real& tFar) const
{
    // intersect with box sides
    vec3r vMin = (m_cornerMin - ray.start())*ray.rvector();
    vec3r vMax = (m_cornerMax - ray.start())*ray.rvector();

    // order in ray direction
    vec3r vNear = min(vMin, vMax);
    vec3r vFar = max(vMin, vMax);

    // select common part
    tNear = vNear.maxComponent();
    tFar = vFar.minComponent();

    // check validity
    if (tNear >= tFar) return false;

    // limit to segment
    if (tNear >= 1.) return false;
    if (tFar <= 0.) return false;

    if (tNear < 0.) tNear = 0.;
//    if (tFar > 1.) tFar = 1.; // for virtual wall

    return true;
}

BoundingBox::operator QString() const
{
    return QString("BoundingBox(%1, %2)").arg(m_cornerMin).arg(m_cornerMax);
}

QTextStream& cyi::operator<<(QTextStream& out, const BoundingBox& box)
{
    out << QString(box);
    return out;
}
