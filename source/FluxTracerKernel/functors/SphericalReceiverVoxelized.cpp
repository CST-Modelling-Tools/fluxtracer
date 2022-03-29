#include "SphericalReceiverVoxelized.h"

using namespace cyi;



SphericalReceiverVoxelized::SphericalReceiverVoxelized(const vec3r& center, const BoundingBox& box, const vec3i& dims):
    m_center(center),
    m_voxels(box, dims)
{

}

void SphericalReceiverVoxelized::process(const Ray& ray)
{
    // point
    real t = dot(m_center - ray.start(), ray.vector());
    t /= ray.vector().norm2();
    if (t < 0. || t > 1.) return;
    vec3r p = ray.point(t);

    // voxel
    vec3i n = m_voxels.index(p);
    if (n >= vec3i(0) && n < m_voxels.tensor().dimensions())
        m_voxels.tensor()(n) += ray.energy();
}

void SphericalReceiverVoxelized::combine(const RayFunctor& rf)
{
    const SphericalReceiverVoxelized& srv = dynamic_cast<const SphericalReceiverVoxelized&>(rf);
    m_voxels += srv.m_voxels;
}

bool SphericalReceiverVoxelized::save(const QString& fileName) const
{
    return m_voxels.save(fileName);
}
