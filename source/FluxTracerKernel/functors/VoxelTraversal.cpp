#include "VoxelTraversal.h"

using namespace cyi;



VoxelTraversal::VoxelTraversal(const BoundingBox& box, const vec3i& dims):
    m_voxels(box, dims)
{

}

void VoxelTraversal::process(const Ray& ray)
{
    BoundingBox& box = m_voxels.box();
    Tensor3<float>& tensor = m_voxels.tensor();
    real tMin;
    real tMax;
    if (!box.isIntersected(ray, tMin, tMax)) return;

    // parameters
    vec3r t0 = (box.cornerMin() - ray.start())*ray.rvector();
    vec3r dt = m_voxels.step()*ray.rvector();
    vec3r nd = (ray.point(tMin) - box.cornerMin())/m_voxels.step();
    nd = clamp(nd.floor(), vec3r(0.), tensor.dimensions() - vec3i(1));
    vec3i n = nd;
    vec3r t = t0 + nd*dt;

    real tx = t.mx;
    real ty = t.my;
    real tz = t.mz;
    dt = dt.abs();
    vec3i dn(1);
    ray.vector().x() >= 0. ? tx += dt.mx : dn.mx = -1;
    ray.vector().y() >= 0. ? ty += dt.my : dn.my = -1;
    ray.vector().z() >= 0. ? tz += dt.mz : dn.mz = -1;

    // traversal
    while (tx <= tMax || ty <= tMax || tz <= tMax) {
        if (n >= vec3i(0) && n < m_voxels.tensor().dimensions())
            tensor(n) += ray.energy();

        if (tx < ty) {
            if (tx < tz) {
                n.mx += dn.mx;
                tx += dt.mx;
            } else {
                n.mz += dn.mz;
                tz += dt.mz;
            }
        } else {
            if (ty < tz) {
                n.my += dn.my;
                ty += dt.my;
            } else {
                n.mz += dn.mz;
                tz += dt.mz;
            }
        }
    }
}

void VoxelTraversal::combine(const RayFunctor& rf)
{
    const VoxelTraversal& vr = dynamic_cast<const VoxelTraversal&>(rf);
    m_voxels += vr.m_voxels;
}

bool VoxelTraversal::save(const QString& fileName) const
{
    return m_voxels.save(fileName);
}
