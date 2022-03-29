#ifndef BINARYSTREAM_H
#define BINARYSTREAM_H

#include "qiostream.h"
#include "math/vec3r.h"

namespace cyi {



class BinaryStream: public QDataStream
{
public:
    BinaryStream(QFile* file): QDataStream(file) {}

    void writeLine(const QString& string);
    bool readLine(QString& string);

    bool read(const char* name);
    bool read(const char* name, int& n);
    bool read(const char* name, vec3i& v);
    bool read(const char* name, vec3f& v);
    bool read(const char* name, vec3d& v);
};



} // namespace cyi

#endif // BINARYSTREAM_H
