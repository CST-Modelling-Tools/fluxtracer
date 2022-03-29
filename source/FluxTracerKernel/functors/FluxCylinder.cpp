#include "FluxCylinder.h"

#include "math/equations.h"

using namespace cyi;



FluxCylinder::FluxCylinder(real rho, int phiDivs, const Grid& zGrid)
{
   m_rho = rho;
   m_phiGrid = Grid(Interval(-pi, pi), phiDivs);
   m_zGrid = zGrid;

   m_fluxSurface.initCylinder(rho, m_phiGrid, zGrid);
}

void FluxCylinder::process(const Ray& ray)
{
    const vec3r& r0 = ray.start();
    const vec3r& d = ray.vector();

    // point
    QuadraticEquation eq;
    eq.a = d.mx*d.mx + d.mz*d.mz; // cylinder axis is y
    eq.b = 2.*(r0.mx*d.mx + r0.mz*d.mz);
    eq.c = r0.mx*r0.mx + r0.mz*r0.mz - m_rho*m_rho;

    if (eq.solve() != 2) return;
    double t1 = eq.x1;
    if (t1 < 0. || t1 > 1.) return;
    vec3r p = ray.point(t1);

    // cell
    double z = p.my;
    if (!m_zGrid.interval().isInside(z)) return;
    int ny = m_zGrid.index(z);

    double phi = atan2(-p.mz, p.mx);
    int nx = m_phiGrid.index(phi);

    // vector
    vec3f dn = d.normalized();
    m_fluxSurface.vectors(nx, ny) += ray.energy()*vec3r(dn.mx, -dn.mz, dn.my);
}

void FluxCylinder::combine(const RayFunctor& rf)
{
    const FluxCylinder& fc = dynamic_cast<const FluxCylinder&>(rf);
    m_fluxSurface += fc.m_fluxSurface;
}

bool FluxCylinder::save(const QString& fileName) const
{
    return m_fluxSurface.save(fileName);
}
