#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "Ray.h"

namespace cyi {



/**
 * @brief BoundingBox represent a volume between two points (corners)
 *
 * This is a 3d version of Interval
 */
class BoundingBox
{
public:
    // constructors
    BoundingBox() {}
    BoundingBox(const vec3r& cornerMin, const vec3r& cornerMax);

    // components
    const vec3r& cornerMin() const {return m_cornerMin;}
    const vec3r& cornerMax() const {return m_cornerMax;}
    void setCorners(const vec3r& cornerMin, const vec3r& cornerMax);
    const vec3r& extent() const {return m_extent;}

    // functions
    bool isInside(const vec3r& v) const;
    bool isEqual(const BoundingBox& other, real eps = 1e-10) const;
    bool isIntersected(const Ray& ray, real& tNear, real& tFar) const;

    // conversions
    operator QString() const;

private:
    vec3r m_cornerMin;
    vec3r m_cornerMax;
    vec3r m_extent;
};



// stream
QTextStream& operator<<(QTextStream& out, const BoundingBox& box);



} // namespace cyi

#endif // BOUNDINGBOX_H
