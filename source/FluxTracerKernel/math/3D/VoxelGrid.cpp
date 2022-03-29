#include "VoxelGrid.h"

#include "common/BinaryStream.h"

using namespace cyi;



VoxelGrid::VoxelGrid(const BoundingBox& box, const vec3i& dims):
    m_box(box),
    m_tensor(dims)
{
    m_vStep = box.extent()/dims;
}

vec3i VoxelGrid::index(const vec3r& p) const
{
    return (p - m_box.cornerMin())/m_vStep;
}

VoxelGrid& VoxelGrid::operator+=(const VoxelGrid& other)
{
    m_tensor += other.m_tensor;
    return *this;
}

bool VoxelGrid::operator==(const VoxelGrid& other) const
{
    if (!(m_box.isEqual(other.m_box))) return false;
    if (!(m_tensor == other.m_tensor)) return false;
    return true;
}

bool VoxelGrid::save(const QString& fileName) const
{
    try {
        // open file
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
            throw "File cannot be opened";
        BinaryStream out(&file);

        // header
        out.writeLine("# vtk DataFile Version 2.0");
        out.writeLine("FluxTracer");
        out.writeLine("BINARY");

        // dataset
        out.writeLine("DATASET STRUCTURED_POINTS");
        vec3i dimensions = tensor().dimensions();
        vec3r spacings = m_box.extent()/dimensions;
        vec3r origin = m_box.cornerMin() + 0.5*spacings;
        out.writeLine(QString("DIMENSIONS %1 %2 %3").arg(dimensions.mx).arg(dimensions.my).arg(dimensions.mz));
        out.writeLine(QString("ORIGIN %1 %2 %3").arg(origin.mx).arg(origin.my).arg(origin.mz));
        out.writeLine(QString("SPACINGS %1 %2 %3").arg(spacings.mx).arg(spacings.my).arg(spacings.mz));

        // points
        out.writeLine(QString("POINT_DATA %1").arg(dimensions.mx*dimensions.my*dimensions.mz));
        out.writeLine("SCALARS Scalar float");
        out.writeLine("LOOKUP_TABLE default");
//        out.setByteOrder(QDataStream::BigEndian);
        out.setFloatingPointPrecision(QDataStream::SinglePrecision);
        for (float v : m_tensor.data()) out << v;
    }
    catch (const char* msg) {
        cout << QString("Error in writing\n%1\n%2").arg(fileName).arg(msg);
        return false;
    }
    return true;
}

bool VoxelGrid::open(const QString& fileName)
{
    try {
        // open file
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
            throw "File cannot be opened";
        BinaryStream in(&file);

        // header
        in.read("# vtk DataFile Version 2.0");
        in.read("FluxTracer");
        in.read("BINARY");

        // dataset
        in.read("DATASET STRUCTURED_POINTS");
        vec3i dimensions;
        in.read("DIMENSIONS", dimensions);
        vec3r origin, spacings;
        in.read("ORIGIN", origin);
        in.read("SPACINGS", spacings);
        vec3r bmin = origin - 0.5*spacings;
        vec3r bmax = bmin + dimensions*spacings;
        m_box.setCorners(bmin, bmax);
        m_tensor.setDimensions(dimensions);
        m_vStep = m_box.extent()/dimensions;

        // points
        int size;
        in.read("POINT_DATA", size);
        if (size != dimensions.mx*dimensions.my*dimensions.mz)
            throw "size mismatch";
        in.read("SCALARS Scalar float");
        in.read("LOOKUP_TABLE default");
//        in.setByteOrder(QDataStream::BigEndian);
        in.setFloatingPointPrecision(QDataStream::SinglePrecision);
          for (int n = 0; n < size; n++) {
            in >> m_tensor.data()[n];
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
