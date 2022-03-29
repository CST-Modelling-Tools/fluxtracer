#include "CylindricalReceiver.h"

#include "math/equations.h"
#include "math/2D/MatrixGrid.h"

using namespace cyi;



CylindricalReceiver::CylindricalReceiver(const vec3r& center, const Grid& dGrid, const Grid& hGrid)
{
    m_center = center;
    for (double d : dGrid.points()) {
        m_ds << d;
        m_bins << BinGrid(hGrid);
    }
}

void CylindricalReceiver::process(const Ray& ray)
{
    const vec3r& r0 = ray.start();
    const vec3r& d = ray.vector();

    QuadraticEquation eq;
    eq.a = d.mx*d.mx + d.mz*d.mz; // cylinder axis is y
    eq.b = 2.*(r0.mx*d.mx + r0.mz*d.mz);
    double c = r0.mx*r0.mx + r0.mz*r0.mz;

    for (int n = 0; n < m_bins.size(); ++n) {
        double rho = 0.5*m_ds[n];
        eq.c = c - rho*rho;

        if (eq.solve() != 2) continue;
        double t1 = eq.x1;
        if (t1 < 0. || t1 > 1.) continue;

        double y = r0.my + t1*d.my;
        double h = 2.*std::abs(y - m_center.my);
        m_bins[n].addKahan(h, ray.energy());
    }
}

void CylindricalReceiver::combine(const RayFunctor& rf)
{
    const CylindricalReceiver& cr = dynamic_cast<const CylindricalReceiver&>(rf);
    for (int n = 0; n < m_bins.size(); ++n)
        m_bins[n] += cr.m_bins[n];
}

bool CylindricalReceiver::save(const QString& fileName) const
{
    QVector<double> hs = m_bins.first().grid().points();
    hs.removeFirst();

    MatrixGrid matrixGrid(m_ds, hs);

    for (int n = 0; n < m_bins.size(); ++n) {
        BinGrid bins = m_bins[n];
        bins.integrateKahan();
        for (int m = 0; m < bins.size(); ++m)
            matrixGrid(n, m) = bins.bin(m);
    }

    return matrixGrid.save(fileName);
}
