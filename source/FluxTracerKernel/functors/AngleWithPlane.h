#ifndef ANGLEWITHPLANE_H
#define ANGLEWITHPLANE_H

#include "functors/RayFunctor.h"
#include "math/1D/BinGrid.h"

namespace cyi {

class AngleWithPlane: public RayFunctor
{
public:
    // constructors
    /**
     * @brief AngleWithPlane does something with rays
     * @param box is the bounding box of the grid
     * @param dims are the dimensions of the grid
     */
    AngleWithPlane(const vec3r& planeNormal, const Grid& angleGrid);
    AngleWithPlane* copy() const {return new AngleWithPlane(*this);}

    // calculations
    void process(const Ray& ray);
    void combine(const RayFunctor& rf);

    // export
    bool save(const QString& fileName) const;

    private:
    vec3r m_planeNormal;
    BinGrid m_bins;

};

} // namespace cyi

#endif // ANGLEWITHPLANE_H
