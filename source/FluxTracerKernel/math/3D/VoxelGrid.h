#ifndef TENSORGRID3_H
#define TENSORGRID3_H

#include "BoundingBox.h"
#include "Tensor3.h"

namespace cyi {


/**
 * @brief VoxelGrid considers 3d grid as a set of bins (voxels)
 *
 * This is a 3d version of BinGrid
 */
class VoxelGrid
{
public:
    // constructors
    VoxelGrid() {}
    VoxelGrid(const BoundingBox& box, const vec3i& dims);

    // components
    const BoundingBox& box() const {return m_box;}
    BoundingBox& box() {return m_box;}

    vec3i index(const vec3r& p) const;

    const Tensor3<float>& tensor() const {return m_tensor;}
    Tensor3<float>& tensor(){return m_tensor;}

    const vec3r& step() const {return m_vStep;}

    // functions
    VoxelGrid& operator+=(const VoxelGrid& other);
    bool operator==(const VoxelGrid& other) const;

    // export
    bool save(const QString& fileName) const;
    bool open(const QString& fileName);

protected:
    BoundingBox m_box;
    vec3r m_vStep;
    Tensor3<float> m_tensor;
};



} // namespace cyi

#endif // TENSORGRID3_H
