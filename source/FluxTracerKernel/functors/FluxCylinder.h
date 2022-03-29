#ifndef FLUXCYLINDER_H
#define FLUXCYLINDER_H

#include "functors/RayFunctor.h"
#include "math/2D/FluxSurface.h"

namespace cyi {



class FluxCylinder: public RayFunctor
{
public:
    // constructors
    /**
     * @brief FluxCylinder finds vector flux on cylindrical surface
     * @param rho is the radius of cylinder
     * @param phiDivs is the number of divisions for azimuth
     * @param zGrid is the grid in z-direction
     */
    FluxCylinder(real rho, int phiDivs, const Grid& zGrid);

    FluxCylinder* copy() const {return new FluxCylinder(*this);}

    // calculations
    void process(const Ray& ray);
    void combine(const RayFunctor& rf);

    // export
    bool save(const QString& fileName) const;

private:
    double m_rho;
    Grid m_phiGrid;
    Grid m_zGrid;
    FluxSurface m_fluxSurface;
};



} // namespace cyi

#endif // FLUXCYLINDER_H
