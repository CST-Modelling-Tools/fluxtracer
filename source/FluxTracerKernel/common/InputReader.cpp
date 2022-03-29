#include "InputReader.h"

using namespace cyi;



InputReader::InputReader(const QString& fileName):
    m_file(fileName)
{
    if (!m_file.open(QIODevice::ReadOnly))
        throw QString("File cannot be opened\n%1").arg(fileName);
    setDevice(&m_file);
}

QString InputReader::readRoot()
{
    readNextStartElement();
    return name().toString();
}

void InputReader::readElement(const QString& element)
{
    readNextStartElement();
    if (name() != element) raiseError(element);
}

void InputReader::skipElement()
{
    skipCurrentElement();
}

void InputReader::readParameter(const QString& name, QString& s)
{
    s = attributes().value(name).toString();
}

void InputReader::readParameter(const QString& name, float& x)
{
    x = attributes().value(name).toFloat();
}

void InputReader::readParameter(const QString& name, double& x)
{
    x = attributes().value(name).toDouble();
}

void InputReader::readParameter(const QString& name, int& n)
{
    n = attributes().value(name).toInt();
}

void InputReader::readParameter(const QString& name, vec3i& v)
{
    QStringRef ref = attributes().value(name);
    QVector<QStringRef> vref = ref.split(',');
    if (vref.size() != 3) raiseError("vec3i");
    bool ok;
    for (int n = 0; n < 3; ++n) {
        v(n) = vref[n].toInt(&ok);
        if(!ok) raiseError("toInt");
    }
}

void InputReader::readParameter(const QString& name, vec3f& v)
{
    QStringRef ref = attributes().value(name);
    QVector<QStringRef> vref = ref.split(',');
    if (vref.size() != 3) raiseError("vec3d");
    bool ok;
    for (int n = 0; n < 3; ++n) {
        v(n) = vref[n].toFloat(&ok);
        if(!ok) raiseError("toDouble");
    }
}

void InputReader::readParameter(const QString& name, vec3d& v)
{
    QStringRef ref = attributes().value(name);
    QVector<QStringRef> vref = ref.split(',');
    if (vref.size() != 3) raiseError("vec3d");
    bool ok;
    for (int n = 0; n < 3; ++n) {
        v(n) = vref[n].toDouble(&ok);
        if(!ok) raiseError("toDouble");
    }
}
