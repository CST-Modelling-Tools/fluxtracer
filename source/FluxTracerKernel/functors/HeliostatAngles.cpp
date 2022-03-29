#include "HeliostatAngles.h"

using namespace cyi;



HeliostatAngles::HeliostatAngles(const vec3r& axis, const Grid& aGrid):
    m_axis(axis),
    m_bins(aGrid)
{
    m_axis.normalize();
}

void HeliostatAngles::process(const Ray& ray)
{
    real cosAlpha = dot(m_axis, ray.vector().normalized());
    real alpha = 90. - acos(cosAlpha) / degree;

    m_bins(alpha) += ray.energy();

    const double& h = ray.surface;
    if (!m_hBins.contains(h))
        m_hBins[h].set(m_bins.grid());
    m_hBins[h](alpha) += ray.energy();
}

void HeliostatAngles::combine(const RayFunctor& rf)
{
    const HeliostatAngles& other = dynamic_cast<const HeliostatAngles&>(rf);
    m_bins += other.m_bins;

    for (double h : other.m_hBins.keys()) {
        if (!m_hBins.contains(h))
            m_hBins[h].set(m_bins.grid());
        m_hBins[h] += other.m_hBins[h];
    }
}

bool HeliostatAngles::save(const QString& fileName) const
{
    BinGrid bins = m_bins;
    bins.integrate();
    if (!bins.save(fileName)) return false;

    for (double h : m_hBins.keys()) {
        BinGrid bins = m_hBins[h];
        bins.integrate();
        QString name = fileName + QString::number(h);
        if (!bins.save(name)) return false;
    }

    return true;
}

