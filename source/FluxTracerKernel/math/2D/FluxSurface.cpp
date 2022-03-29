#include "FluxSurface.h"

#include "common/BinaryStream.h"

using namespace cyi;



void FluxSurface::initPlane(const Grid& xGrid, const Grid& yGrid, real z)
{
    m_points.setDimensions(xGrid.divisions(), yGrid.divisions());
    QVector<double> xs = xGrid.centers();
    QVector<double> ys = yGrid.centers();
    vec3f point(0.f, 0.f, float(z));
    for (int nx = 0; nx < xs.size(); nx++) {
        point.mx = xs[nx];
        for (int ny = 0; ny < ys.size(); ny++) {
            point.my = ys[ny];
            m_points(nx, ny) = point;
        }
    }

    m_vectors.setDimensions(xGrid.divisions(), yGrid.divisions());
}

void FluxSurface::initCylinder(real rho, const Grid& phiGrid, const Grid& zGrid)
{
    m_points.setDimensions(phiGrid.divisions(), zGrid.divisions());
    QVector<double> phis = phiGrid.centers();
    QVector<double> zs = zGrid.centers();
    vec3f point;
    for (int nx = 0; nx < phis.size(); nx++) {
        real phi = phis[nx];
        point.mx = rho*cos(phi);
        point.my = rho*sin(phi);
        for (int ny = 0; ny < zs.size(); ny++) {
            point.mz = zs[ny];
            m_points(nx, ny) = point;
        }
    }

    m_vectors.setDimensions(phiGrid.divisions(), zGrid.divisions());
}

FluxSurface& FluxSurface::operator+=(const FluxSurface& other)
{
    m_vectors += other.m_vectors;
    return *this;
}

bool FluxSurface::save(const QString& fileName) const
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
        out.writeLine("FluxTracer FluxSurface");
        out.writeLine("BINARY");

        // dataset
        out.writeLine("DATASET STRUCTURED_GRID");
        int mx = m_points.rows();
        int my = m_points.cols();
        out.writeLine(QString("DIMENSIONS %1 %2 1").arg(mx).arg(my));
        out.writeLine(QString("POINTS %1 float").arg(mx*my));
        for (int ny = 0; ny < my; ++ny)
        for (int nx = 0; nx < mx; ++nx)
        {
            const vec3f& p = m_points(nx, ny);
            out << p.mx << p.my << p.mz;
        }

        // points
        out.writeLine(QString("POINT_DATA %1").arg(mx*my));
        out.writeLine("VECTORS Flux float");
        for (int ny = 0; ny < my; ++ny)
        for (int nx = 0; nx < mx; ++nx)
        {
            const vec3f& p = m_vectors(nx, ny);
            out << p.mx << p.my << p.mz;
        }
    }
    catch (const char* msg) {
        cout << QString("Error in writing\n%1\n%2").arg(fileName).arg(msg);
        return false;
    }
    return true;
}

bool FluxSurface::open(const QString& fileName)
{
    try {
        // open file
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
            throw "File cannot be opened";
        BinaryStream in(&file);
//        in.setByteOrder(QDataStream::BigEndian);
        in.setFloatingPointPrecision(QDataStream::SinglePrecision);

        // header
        in.read("# vtk DataFile Version 2.0");
        in.read("FluxTracer FluxSurface");
        in.read("BINARY");

        // dataset
        in.read("DATASET STRUCTURED_GRID");

        vec3i dimensions;
        in.read("DIMENSIONS", dimensions);
        int mx = dimensions.mx;
        int my = dimensions.my;
        if (dimensions.mz != 1) throw "Check DIMENSIONS";
        m_points.setDimensions(mx, my);
        m_vectors.setDimensions(mx, my);

        QString line;
        in.readLine(line);
        QStringList lineList = line.split(" ");
        if (lineList.size() != 3) throw line;
        if (lineList[0] != "POINTS") throw line;
        if (lineList[1].toInt() != mx*my) throw line;
        if (lineList[2] != "float") throw line;

        for (int ny = 0; ny < my; ++ny)
        for (int nx = 0; nx < mx; ++nx)
        {
            vec3f& p = m_points(nx, ny);
            in >> p.mx >> p.my >> p.mz;
        }

        // points
        int size;
        in.read("POINT_DATA", size);
        if (size != mx*my) throw "Check POINT_DATA";
        in.read("VECTORS Flux float");

        for (int ny = 0; ny < my; ++ny)
        for (int nx = 0; nx < mx; ++nx)
        {
            vec3f& p = m_vectors(nx, ny);
            in >> p.mx >> p.my >> p.mz;
        }
    }
    catch (const char* msg) {
        cout << QString("Error in reading\n%1\n%2").arg(fileName).arg(msg);
        return false;
    }
    catch (const QString& msg) {
        cout << QString("Error in reading\n%1\n%2").arg(fileName).arg(msg);
        return false;
    }
    return true;
}

