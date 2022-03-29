#include "BinaryStream.h"

using namespace cyi;



void BinaryStream::writeLine(const QString& string)
{
    std::string s = string.toStdString() + '\n';
    writeRawData(s.c_str(), (int)s.size());
}

bool BinaryStream::readLine(QString& string)
{
    string.clear();
    char c;
    while (readRawData(&c, 1) == 1) {
        if (c == '\n') return true;
        string.append(c);
    }
    return false;
}

bool BinaryStream::read(const char* name)
{
    QString line;
    readLine(line);
    if (line != name) throw line;
    return true;
}

bool BinaryStream::read(const char* name, int& n)
{
    QString line;
    QStringList lineList;
    bool ok;

    readLine(line);
    lineList = line.split(" ");
    if (lineList.size() == 0 || lineList.size() > 2) throw line;
    if (lineList[0] != name) throw line;

    n = lineList[1].toInt(&ok);
    if (!ok) throw line;
    return true;
}

bool BinaryStream::read(const char* name, vec3i& v)
{
    QString line;
    QStringList lineList;
    bool ok;

    readLine(line);
    lineList = line.split(" ");
    if (lineList.size() == 0 || lineList.size() > 4) throw line;
    if (lineList[0] != name) throw line;

    double x = lineList[1].toInt(&ok);
    if (!ok) throw line;
    double y = lineList[2].toInt(&ok);
    if (!ok) throw line;
    double z = lineList[3].toInt(&ok);
    if (!ok) throw line;

    v = vec3i(x, y, z);
    return true;
}

bool BinaryStream::read(const char* name, vec3f& v)
{
    QString line;
    readLine(line);

    QStringList lineList = line.split(" ");
    if (lineList.size() == 0 || lineList.size() > 4) throw line;
    if (lineList[0] != name) throw line;

    bool ok;
    float x = lineList[1].toFloat(&ok);
    if (!ok) throw line;
    float y = lineList[2].toFloat(&ok);
    if (!ok) throw line;
    float z = lineList[3].toFloat(&ok);
    if (!ok) throw line;

    v = vec3f(x, y, z);
    return true;
}

bool BinaryStream::read(const char* name, vec3d& v)
{
    QString line;
    readLine(line);

    QStringList lineList = line.split(" ");
    if (lineList.size() == 0 || lineList.size() > 4) throw line;
    if (lineList[0] != name) throw line;

    bool ok;
    double x = lineList[1].toDouble(&ok);
    if (!ok) throw line;
    double y = lineList[2].toDouble(&ok);
    if (!ok) throw line;
    double z = lineList[3].toDouble(&ok);
    if (!ok) throw line;

    v = vec3d(x, y, z);
    return true;
}
