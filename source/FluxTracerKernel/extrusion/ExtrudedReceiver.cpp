#include "ExtrudedReceiver.h"

#include "math/1D/Grid.h"
#include "common/BinaryStream.h"

using namespace cyi;



ExtrudedReceiver::ExtrudedReceiver()
{

}

void ExtrudedReceiver::initMesh(int nMax, int mMax)
{
    m_points.setDimensions(nMax, mMax);
    m_trianglesIndexed.clear();
    for (int n = 0; n < nMax; ++n) { // phi
        for (int m = 0; m < mMax - 1; ++m) { // rho
            int p = (n + m)%2;
            int n1 = (n + 1)%nMax;
            int m1 = m + 1;
            m_trianglesIndexed << TriangleIndexed(
                m_points.index(n, m),
                m_points.index(n, m1),
                m_points.index(n1, m1 - p)
            );
           m_trianglesIndexed << TriangleIndexed(
               m_points.index(n1, m1),
               m_points.index(n1, m),
               m_points.index(n, m + p)
           );
        }
    }
}

void ExtrudedReceiver::initPoints(const QVector<double>& parameters)
{
    // the first nMax*mMax parameters are for vertical extrusion
    // the next 2*nMax parameters are for radial scaling

    // update points
    const int nMax = m_points.rows();
    const int mMax = m_points.cols();

    Grid phis(Interval(0., 2.*pi), nMax);
    QVector<Grid> rhos;
    for (int p = m_points.data().size(); p < parameters.size(); p += 2)
        rhos << Grid(Interval(parameters[p], parameters[p + 1]), mMax - 1);

    for (int n = 0; n < nMax; ++n) { // phi
        double phi = phis.point(n);
        double x = cos(phi);
        double y = sin(phi);
        for (int m = 0; m < mMax; ++m) { // rho
            double rho = rhos[n].point(m);
            double z = parameters[m_points.index(n, m)];
            m_points(n, m) = vec3r(rho*x, rho*y, z);
        }
    }

    // update triangles
    m_triangles.clear();
    const QVector<vec3r>& data = m_points.data();
    for (const TriangleIndexed& tri : m_trianglesIndexed)
        m_triangles << Triangle(data[tri.a], data[tri.b], data[tri.c]);
}

double ExtrudedReceiver::area() const
{
    double area = 0.;
    for (const Triangle& tri: m_triangles)
        area += tri.area();
    return area;
}

bool ExtrudedReceiver::isIntersected(const Ray& ray) const
{
    for (int n = 0; n < m_triangles.size(); ++n)
        if (m_triangles[n].isIntersected(ray)) return true;
    return false;
}

double ExtrudedReceiver::intercept(const QVector<Ray>& rays)
{
    double intercepted = 0.;
    double total = 0.;
    for (const Ray& ray : rays) {
        if (isIntersected(ray)) intercepted += ray.energy();
        total += ray.energy();
    }
    return intercepted/total;
}

bool ExtrudedReceiver::save(const QString& fileName) const
{
    try {
        // open file
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
            throw "File cannot be opened";
        BinaryStream out(&file);
//        out.setByteOrder(QDataStream::BigEndian); // by default QDataStream::BigEndian
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);

        // header
        out.writeLine("# vtk DataFile Version 2.0");
        out.writeLine("FluxTracer ExtrudedCylinder");
        out.writeLine("BINARY");

        // dataset
        out.writeLine("DATASET POLYDATA");
        out.writeLine(QString("POINTS %1 float").arg(m_points.data().size()));
        for (const vec3f& p : m_points.data())
            out << p.mx << p.my << p.mz;

        // points
        int mTris = m_trianglesIndexed.size();
        out.writeLine(QString("POLYGONS %1 %2").arg(mTris).arg(4*mTris));
        for (const TriangleIndexed& tri : m_trianglesIndexed)
            out << 3 << tri.a << tri.b << tri.c;
    }
    catch (const char* msg) {
        cout << QString("Error in writing\n%1\n%2").arg(fileName).arg(msg);
        return false;
    }
    return true;
}





