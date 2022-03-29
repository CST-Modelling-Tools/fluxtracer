#ifndef SPHERICALRECEIVERVOXELIZED_H
#define SPHERICALRECEIVERVOXELIZED_H

#include "functors/RayFunctor.h"
#include "math/3D/VoxelGrid.h"

namespace cyi {



class SphericalReceiverVoxelized: public RayFunctor
{
public:
    // constructors
    /**
     * @brief SphericalReceiverVoxelized finds a point on a ray
     * which has the shortest distance to the center and
     * stores it in a voxel grid
     * @param center is the center of sphere
     * @param box is the bounding box of the grid
     * @param dims are the dimensions of the grid
     */
    SphericalReceiverVoxelized(const vec3r& center, const BoundingBox& box, const vec3i& dims);
    SphericalReceiverVoxelized* copy() const {return new SphericalReceiverVoxelized(*this);}

    // calculations
    void process(const Ray& ray);
    void combine(const RayFunctor& rf);

    // export
    bool save(const QString& fileName) const;

private:
    vec3r m_center;
    VoxelGrid m_voxels;
};



} // namespace cyi

#endif // SPHERICALRECEIVERVOXELIZED_H
