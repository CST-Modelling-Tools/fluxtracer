#ifndef FLUXPLANE_H
#define FLUXPLANE_H

#include "functors/RayFunctor.h"
#include "math/2D/FluxSurface.h"

namespace cyi {



class FluxPlane: public RayFunctor
{
public:
    // constructors
    /**
     * @brief FluxPlane finds vector flux on a horizontal plane
     * @param xGrid is the grid in x-direction
     * @param yGrid is the grid in y-direction
     * @param z is the height of the plane
     */
    FluxPlane(const Grid& xGrid, const Grid& yGrid, real z);

    FluxPlane* copy() const {return new FluxPlane(*this);}

    // calculations
    void process(const Ray& ray);
    void combine(const RayFunctor& rf);

    // export
    bool save(const QString& fileName) const;

private:
    Grid m_xGrid;
    Grid m_yGrid;
    FluxSurface m_fluxSurface;
    double m_z;
};



} // namespace cyi

#endif // FLUXPLANE_H
