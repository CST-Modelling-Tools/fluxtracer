#include "Ray.h"

using namespace cyi;



Ray::Ray(const vec3r& start, const vec3r& end, double energy):
    m_start(start),
    m_end(end),
    m_energy(energy)
{
    m_vector = m_end - m_start;
    m_rvector = 1./m_vector;
    // m_rvector = m_vector.reciprocal();

    // avoid nan;
    m_rvector = clamp(m_rvector, vec3r::NegInf, vec3r::PosInf);
}

void Ray::setPoints(const vec3r& start, const vec3r& end)
{
    m_start = start;
    m_end = end;
    m_vector = m_end - m_start;
    m_rvector = 1./m_vector;
    m_rvector = clamp(m_rvector, vec3r::NegInf, vec3r::PosInf);
}

vec3r Ray::point(double t) const
{
    return m_start + m_vector*t;
}
