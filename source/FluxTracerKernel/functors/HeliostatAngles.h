#ifndef HELIOSTATANGLES_H
#define HELIOSTATANGLES_H

#include "functors/RayFunctor.h"
#include "math/1D/BinGrid.h"

namespace cyi {



class HeliostatAngles: public RayFunctor
{
public:
    // constructors
    /**
     * @brief HeliostatAngles counts the energy of the rays intercepted by a sphere as a function of angle
     * @param axis is the aiming axis
     * @param aGrid is the grid for angles
     */
    HeliostatAngles(const vec3r& axis, const Grid& aGrid);
    HeliostatAngles* copy() const {return new HeliostatAngles(*this);}

    // calculations
    void process(const Ray& ray);
    void combine(const RayFunctor& rf);

    // export
    bool save(const QString& fileName) const;

private:
    vec3r m_axis;
    BinGrid m_bins; // common bins
    QMap<double, BinGrid> m_hBins; // heliostat bins
};



} // namespace cyi

#endif // HELIOSTATANGLES_H
