#include "SphericalReceiver.h"

using namespace cyi;



SphericalReceiver::SphericalReceiver(const vec3r& center, const Grid& rGrid):
    m_center(center),
    m_bins(rGrid)
{

}

void SphericalReceiver::process(const Ray& ray)
{
    real r = ray.vector().norm2();
    if (r > 0.) {
        real q = cross(m_center - ray.start(), ray.vector()).norm2();
        r = sqrt(q/r);
    } else
        r = m_bins.grid().interval().max();

    m_bins(r) += ray.energy();
}

void SphericalReceiver::combine(const RayFunctor& rf)
{
    const SphericalReceiver& sr = dynamic_cast<const SphericalReceiver&>(rf);
    m_bins += sr.m_bins;
}

bool SphericalReceiver::save(const QString& fileName) const
{
    BinGrid bins = m_bins;
    bins.integrate();
    return bins.save(fileName);
}

