#ifndef ANGULARMATRIX_H
#define ANGULARMATRIX_H

#include "functors/RayFunctor.h"
#include "math/1D/BinGrid.h"
#include "math/2D/Matrix.h"

namespace cyi {

class AngularMatrix : public RayFunctor
{

public:
    // constructors
    /**
     * @brief AngleWithPlane does something with rays
     * @param box is the bounding box of the grid
     * @param dims are the dimensions of the grid
     */
    AngularMatrix(const vec3r& mainOpticalDirection, const Grid& gridAzmuth, const Grid& gridZenith);
    AngularMatrix* copy() const {return new AngularMatrix(*this);}

    // calculations
    void process(const Ray& ray);
    void combine(const RayFunctor& rf);

    // export
    bool save(const QString& fileName) const;

    private:
    vec3r m_ip;
    vec3r m_jp;
    vec3r m_kp;
    Grid m_gridAzimuth;
    Grid m_gridZenith;
    QMap<QString, Matrix<double> > m_results;
    Matrix<double> m_matrix;

};

} // namespace cyi

#endif // ANGULARMATRIX_H
