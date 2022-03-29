#include "TubularReceiver.h"

using namespace cyi;



TubularReceiver::TubularReceiver(const vec3r& center, const vec3r& direction, const Grid& rGrid):
    m_center(center),
    m_direction(direction),
    m_bins(rGrid)
{

}

void TubularReceiver::process(const Ray& ray)
{
    real r;
    vec3r s = cross(ray.vector(), m_direction);
    real sNorm = s.norm();
    if (sNorm > 0.) { // skew lines
        r = dot(ray.start() - m_center, s);
        r = std::abs(r/sNorm);
    } else { // parallel lines
        real q = cross(ray.start() - m_center, m_direction).norm2();
        r = sqrt(q/m_direction.norm2());
    }
    m_bins(r) += ray.energy();
}

void TubularReceiver::combine(const RayFunctor& rf)
{
    const TubularReceiver& tr = dynamic_cast<const TubularReceiver&>(rf);
    m_bins += tr.m_bins;
}

bool TubularReceiver::save(const QString& fileName) const
{
    BinGrid bins = m_bins;
    bins.integrate();
    return bins.save(fileName);
}
