#ifndef FLUXSURFACE_H
#define FLUXSURFACE_H

#include "Matrix.h"
#include "math/1D/Grid.h"
#include "math/vec3r.h"

namespace cyi {



class FluxSurface
{
public:
    FluxSurface() {}

    void initPlane(const Grid& xGrid, const Grid& yGrid, real z);
    void initCylinder(real rho, const Grid& phiGrid, const Grid& zGrid);

    // components
    const Matrix<vec3f>& points() const {return m_points;}
    Matrix<vec3f>& points(){return m_points;}
    const vec3f& points(int n, int m) const {return m_points(n, m);}
    vec3f& points(int n, int m) {return m_points(n, m);}

    const Matrix<vec3f>& vectors() const {return m_vectors;}
    Matrix<vec3f>& vectors(){return m_vectors;}

    const vec3f& vectors(int n, int m) const {return m_vectors(n, m);}
    vec3f& vectors(int n, int m) {return m_vectors(n, m);}

    // functions
    FluxSurface& operator+=(const FluxSurface& other);

    // export
    bool save(const QString& fileName) const;
    bool open(const QString& fileName);

protected:
    Matrix<vec3f> m_points;
    Matrix<vec3f> m_vectors;
};



} // namespace cyi

#endif // FLUXSURFACE_H
