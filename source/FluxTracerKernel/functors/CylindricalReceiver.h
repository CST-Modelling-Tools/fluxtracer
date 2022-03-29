#ifndef CYLINDRICALRECEIVER_H
#define CYLINDRICALRECEIVER_H

#include "functors/RayFunctor.h"
#include "math/1D/BinGrid.h"

namespace cyi {



class CylindricalReceiver: public RayFunctor
{
public:
    // constructors
    /**
     * @brief CylindricalReceiver counts the energy intercepted by cylinder as a function of diameter and height
     * @param center is the center of cylinder
     * @param dGrid is the grid for diameters
     * @param hGrid is the grid for heights
     */
    CylindricalReceiver(const vec3r& center, const Grid& dGrid, const Grid& hGrid);

    CylindricalReceiver* copy() const {return new CylindricalReceiver(*this);}

    // calculations
    void process(const Ray& ray);
    void combine(const RayFunctor& rf);

    // export
    bool save(const QString& fileName) const;

private:
    vec3r m_center;
    QVector<double> m_ds;
    QVector<BinGrid> m_bins;
};



} // namespace cyi

#endif // CYLINDRICALRECEIVER_H
