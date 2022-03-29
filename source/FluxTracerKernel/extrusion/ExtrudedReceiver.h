#ifndef EXTRUDEDRECEIVER_H
#define EXTRUDEDRECEIVER_H

#include <QVector>
#include "math/2D/Matrix.h"
#include "math/vec3r.h"
#include "math/3D/Ray.h"
#include "math/3D/Triangle.h"

namespace cyi {



class ExtrudedReceiver
{
public:
    // constructors
    ExtrudedReceiver();

    void initMesh(int nMax, int mMax);
    void initPoints(const QVector<double>& parameters);

    // components
    QVector<Triangle> triangles() const {return m_triangles;}

    // calculations
    double area() const;
    bool isIntersected(const Ray& ray) const;
    double intercept(const QVector<Ray>& rays);

    // export
    bool save(const QString& fileName) const;

protected:
    Matrix<vec3r> m_points;
    QVector<TriangleIndexed> m_trianglesIndexed;
    QVector<Triangle> m_triangles;
};



} // namespace cyi

#endif // EXTRUDEDRECEIVER_H
