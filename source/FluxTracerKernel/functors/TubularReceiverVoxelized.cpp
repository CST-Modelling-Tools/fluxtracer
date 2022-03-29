#include "TubularReceiverVoxelized.h"

using namespace cyi;



TubularReceiverVoxelized::TubularReceiverVoxelized(const vec3r& center, const vec3r& direction, const BoundingBox& box, const vec3i& dims):
    m_center(center),
    m_direction(direction),
    m_voxels(box, dims)
{

}

void TubularReceiverVoxelized::process(const Ray& ray)
{
    const vec3r& dL = m_direction;
    const vec3r& rL = m_center;
    const vec3r& d = ray.vector();
    const vec3r& r0 = ray.start();

    // point
    real t;
    vec3r n = cross(d, dL);
    real nNorm2 = n.norm2();
    if (nNorm2 > 0.) { // skew lines
        vec3r q = rL - r0;
        t = dot(q, d)*dot(dL, dL) - dot(q, dL)*dot(d, dL);
        t /= nNorm2;
    } else { // parallel lines
        t = 0.5;
    }
    vec3r p = ray.point(t);

    // voxel
    vec3i m = m_voxels.index(p);
    if (m >= vec3i(0) && m < m_voxels.tensor().dimensions())
        m_voxels.tensor()(m) += ray.energy();
}

void TubularReceiverVoxelized::combine(const RayFunctor& rf)
{
    const TubularReceiverVoxelized& trv = dynamic_cast<const TubularReceiverVoxelized&>(rf);
    m_voxels += trv.m_voxels;
}

bool TubularReceiverVoxelized::save(const QString& fileName) const
{
    return m_voxels.save(fileName);
}
