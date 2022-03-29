#ifndef TUBULARRECEIVERVOXELIZED_H
#define TUBULARRECEIVERVOXELIZED_H

#include "functors/RayFunctor.h"
#include "math/3D/VoxelGrid.h"

namespace cyi {



class TubularReceiverVoxelized: public RayFunctor
{
public:
    // constructors
    /**
     * @brief TubularReceiverVoxelized finds a point on a ray
     * which has the shortest distance to the line and
     * stores it in a voxel grid
     * @param center is the center of tube
     * @param direction is the direction of tube
     * @param box is the bounding box of the grid
     * @param dims are the dimensions of the grid
     */
    TubularReceiverVoxelized(const vec3r& center, const vec3r& direction, const BoundingBox& box, const vec3i& dims);
    TubularReceiverVoxelized* copy() const {return new TubularReceiverVoxelized(*this);}

    // calculations
    void process(const Ray& ray);
    void combine(const RayFunctor& rf);

    // export
    bool save(const QString& fileName) const;

private:
    vec3r m_center;
    vec3r m_direction;
    VoxelGrid m_voxels;
};



} // namespace cyi

#endif // TUBULARRECEIVERVOXELIZED_H
