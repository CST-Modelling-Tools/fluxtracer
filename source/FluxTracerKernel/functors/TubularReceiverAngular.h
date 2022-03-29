#ifndef TUBULARRECEIVERANGULAR_H
#define TUBULARRECEIVERANGULAR_H

#include "functors/RayFunctor.h"
#include "math/1D/BinGrid.h"
#include "math/2D/Matrix.h"

namespace cyi {



class TubularReceiverAngular: public RayFunctor
{
public:
    // constructors
    /**
     * @brief RadialReceiver2D finds the receiver shape
     * in cylindrical coordinates by using the most distant radial points
     * @param center is the center of tube
     * @param direction is the direction of tube
     * @param normal is the normal of tube
     * @param phiDivs is the number of division for azimuth
     * @param hGrid is the grid for height
     * @param rGrid is the radial grid
     * @param eta is the threshold
     */
    TubularReceiverAngular(const vec3r& center, const vec3r& direction, const vec3r& normal,
                     int phiDivs, const Grid& hGrid, const Grid& rGrid, double eta);
    TubularReceiverAngular* copy() const {return new TubularReceiverAngular(*this);}

    // calculations
    void process(const Ray& ray);
    void combine(const RayFunctor& rf);

    // export
    bool save(const QString& fileName) const;

private:
    vec3r m_center;
    vec3r m_direction;
    vec3r m_normal;
    vec3r m_normal2;

    Grid m_phiGrid;
    Grid m_hGrid;
    Matrix<BinGrid> m_matrix;
    double m_eta;
};



} // namespace cyi

#endif // TUBULARRECEIVERANGULAR_H
