#include "SphericalReceiverAngular.h"

#include "math/2D/MatrixGrid.h"

using namespace cyi;



SphericalReceiverAngular::SphericalReceiverAngular(const vec3r& center, int phiDivs, int thetaDivs, const Grid& rGrid, double eta):
    m_center(center),
    m_phiGrid(Interval(-pi, pi), phiDivs),
    m_thetaGrid(Interval(0., pi), thetaDivs),
    m_matrix(phiDivs, thetaDivs),
    m_eta(eta)
{
    for (BinGrid& bin : m_matrix.data())
        bin.set(rGrid);
}

void SphericalReceiverAngular::process(const Ray& ray)
{
    // point
    real t = dot(m_center - ray.start(), ray.vector());
    t /= ray.vector().norm2();
    if (t < 0. || t > 1.) return;
    vec3r p = ray.point(t);

    // spherical coordinates
    p -= m_center;
    double r = p.norm();
    double phi = atan2(p.mz, p.mx);
    double theta = atan2(p.mx*p.mx + p.mz*p.mz, p.my);

    // matrix indices
    int nPhi = m_phiGrid.index(phi);
    int nTheta = m_thetaGrid.index(theta);
    m_matrix(nPhi, nTheta).addKahan(r, ray.energy());
}

void SphericalReceiverAngular::combine(const RayFunctor& rf)
{
    const SphericalReceiverAngular& other = dynamic_cast<const SphericalReceiverAngular&>(rf);
    m_matrix += other.m_matrix;
}

bool SphericalReceiverAngular::save(const QString& fileName) const
{
    MatrixGrid matrixGrid(m_phiGrid.centers(), m_thetaGrid.centers());
    for (int n = 0; n < m_matrix.rows(); ++n)
        for (int m = 0; m < m_matrix.cols(); ++m)
            matrixGrid(n, m) = m_matrix(n, m).fraction(m_eta);
    return matrixGrid.save(fileName);
}
