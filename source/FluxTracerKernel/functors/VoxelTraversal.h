#ifndef VOXELTRAVERSAL_H
#define VOXELTRAVERSAL_H

#include "functors/RayFunctor.h"
#include "math/3D/VoxelGrid.h"

namespace cyi {



class VoxelTraversal: public RayFunctor
{
public:
    // constructors
    /**
     * @brief VoxelTraversal fills a voxel grid with rays
     * @param box is the bounding box of the grid
     * @param dims are the dimensions of the grid
     */
    VoxelTraversal(const BoundingBox& box, const vec3i& dims);
    VoxelTraversal* copy() const {return new VoxelTraversal(*this);}

    // calculations
    void process(const Ray& ray);
    void combine(const RayFunctor& rf);

    // export
    bool save(const QString& fileName) const;

private:
    VoxelGrid m_voxels;
};



} // namespace cyi

#endif // VOXELTRAVERSAL_H
