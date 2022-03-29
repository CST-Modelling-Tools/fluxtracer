#ifndef SPHERICALRECEIVERANGULAR_H
#define SPHERICALRECEIVERANGULAR_H

#include "functors/RayFunctor.h"
#include "math/1D/BinGrid.h"
#include "math/2D/Matrix.h"

namespace cyi {



class SphericalReceiverAngular: public RayFunctor
{
public:
    // constructors
    /**
     * @brief SphericalReceiverAngular finds the receiver shape
     * in spherical coordinates by using the most distant radial points
     * @param center is the focal point
     * @param phiDivs is the number of division for azimuth
     * @param thetaDivs is the number of division for elevation
     * @param rGrid is the radial grid
     * @param eta is the threshold
     */
    SphericalReceiverAngular(const vec3r& center, int phiDivs, int thetaDivs, const Grid& rGrid, double eta);
    SphericalReceiverAngular* copy() const {return new SphericalReceiverAngular(*this);}

    // calculations
    void process(const Ray& ray);
    void combine(const RayFunctor& rf);

    // export
    bool save(const QString& fileName) const;

private:
    vec3r m_center;
    Grid m_phiGrid;
    Grid m_thetaGrid;
    Matrix<BinGrid> m_matrix;
    double m_eta;
};



} // namespace cyi

#endif // SPHERICALRECEIVERANGULAR_H
