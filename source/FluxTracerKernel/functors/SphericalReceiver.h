#ifndef SPHERICALRECEIVER_H
#define SPHERICALRECEIVER_H

#include "functors/RayFunctor.h"
#include "math/1D/BinGrid.h"

namespace cyi {



class SphericalReceiver: public RayFunctor
{
public:
    // constructors
    /**
     * @brief SphericalReceiver counts the energy of the rays intercepted by a sphere as a function of radius
     * @param center is the center of sphere
     * @param rGrid is the grid for radiuses
     */
    SphericalReceiver(const vec3r& center, const Grid& rGrid);
    SphericalReceiver* copy() const {return new SphericalReceiver(*this);}

    // calculations
    void process(const Ray& ray);
    void combine(const RayFunctor& rf);

    // export
    bool save(const QString& fileName) const;

private:
    vec3r m_center;
    BinGrid m_bins;
};



} // namespace cyi

#endif // SPHERICALRECEIVER_H
