#include "FluxPlane.h"

using namespace cyi;



FluxPlane::FluxPlane(const Grid& xGrid, const Grid& yGrid, real z)
{
    m_xGrid = xGrid;
    m_yGrid = yGrid;
    m_z = z;

    m_fluxSurface.initPlane(xGrid, yGrid, z);
}

void FluxPlane::process(const Ray& ray)
{
    // Confluence: x(East), y(North), z(Zenith)
    // Tonatiuh: x(East), y(Zenith), z(South)

    // point
    real t = (m_z - ray.start().my)*ray.rvector().my;
    vec3f p = ray.point(t);

    if (!m_xGrid.interval().isInside(p.mx)) return;
    if (!m_yGrid.interval().isInside(-p.mz)) return;
    int nx = m_xGrid.index(p.mx);
    int ny = m_yGrid.index(-p.mz);

    // vector
    vec3f d = ray.vector().normalized();
    m_fluxSurface.vectors()(nx, ny) += ray.energy()*vec3r(d.mx, -d.mz, d.my);
}

void FluxPlane::combine(const RayFunctor& rf)
{
    const FluxPlane& fp = dynamic_cast<const FluxPlane&>(rf);
    m_fluxSurface += fp.m_fluxSurface;
}

bool FluxPlane::save(const QString& fileName) const
{
    return m_fluxSurface.save(fileName);
}
