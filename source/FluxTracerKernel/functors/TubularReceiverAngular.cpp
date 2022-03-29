#include "TubularReceiverAngular.h"

#include "math/2D/MatrixGrid.h"

using namespace cyi;



TubularReceiverAngular::TubularReceiverAngular(const vec3r& center, const vec3r& direction, const vec3r& normal,
                                   int phiDivs, const Grid& hGrid, const Grid& rGrid, double eta):
    m_center(center),
    m_direction(direction),
    m_normal(normal),

    m_phiGrid(Interval(-pi, pi), phiDivs),
    m_hGrid(hGrid),
    m_matrix(phiDivs, hGrid.divisions()),
    m_eta(eta)
{
    m_normal2 = cross(m_direction, m_normal);
    m_normal2.normalize();

    for (BinGrid& bin : m_matrix.data())
        bin.set(rGrid);
}

void TubularReceiverAngular::process(const Ray& ray)
{
    const vec3r& dL = m_direction;
    const vec3r& rL = m_center;
    const vec3r& d = ray.vector();
    const vec3r& r0 = ray.start();

    // point
    real t;
    vec3r n = cross(d, dL);
    real nNorm2 = n.norm2();
    if (nNorm2 > 0.) { // skew lines
        vec3r q = rL - r0;
        t = dot(q, d)*dot(dL, dL) - dot(q, dL)*dot(d, dL);
        t /= nNorm2;
    } else { // parallel lines
        t = 0.5;
    }
//    if (t < 0. || t > 1.) return;
    vec3r p = ray.point(t);

    // cylindrical coordinates
    p -= m_center;
    double z = dot(p, dL);
    double r = (p - z*dL).norm();
    double phi = atan2(dot(p, m_normal2), dot(p, m_normal));

    // matrix indices
    int nPhi = m_phiGrid.index(phi);
    int nZ = m_hGrid.index(z);
    m_matrix(nPhi, nZ).addKahan(r, ray.energy());
}

void TubularReceiverAngular::combine(const RayFunctor& rf)
{
    const TubularReceiverAngular& other = dynamic_cast<const TubularReceiverAngular&>(rf);
    m_matrix += other.m_matrix;
}

bool TubularReceiverAngular::save(const QString& fileName) const
{
    MatrixGrid matrixGrid(m_phiGrid.centers(), m_hGrid.centers());
    for (int n = 0; n < m_matrix.rows(); ++n)
        for (int m = 0; m < m_matrix.cols(); ++m)
            matrixGrid(n, m) = m_matrix(n, m).fraction(m_eta);
    return matrixGrid.save(fileName);
}
