#ifndef TUBULARRECEIVER_H
#define TUBULARRECEIVER_H

#include "functors/RayFunctor.h"
#include "math/1D/BinGrid.h"

namespace cyi {



class TubularReceiver: public RayFunctor
{
public:
    // constructors
    /**
     * @brief TubularReceiver counts the energy of the rays intercepted by a tube as a function of radius
     * @param center is the center of tube
     * @param direction is the direction of tube
     * @param rGrid is the grid for radiuses
     */
    TubularReceiver(const vec3r& center, const vec3r& direction, const Grid& rGrid);
    TubularReceiver* copy() const {return new TubularReceiver(*this);}

    // calculations
    void process(const Ray& ray);
    void combine(const RayFunctor& rf);

    // export
    bool save(const QString& fileName) const;

private:
    vec3r m_center;
    vec3r m_direction;
    BinGrid m_bins;
};



} // namespace cyi

#endif // TUBULARRECEIVER_H
