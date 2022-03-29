#ifndef INPUTREADER_H
#define INPUTREADER_H

#include <QXmlStreamReader>

#include "math/vec3r.h"

namespace cyi {



class InputReader: protected QXmlStreamReader
{
public:
    InputReader(const QString& fileName);

    QString readRoot();
    void readElement(const QString& element);
    void skipElement();

    void readParameter(const QString& name, QString& s);
    void readParameter(const QString& name, float& x);
    void readParameter(const QString& name, double& x);
    void readParameter(const QString& name, int& n);
    void readParameter(const QString& name, vec3i& v);
    void readParameter(const QString& name, vec3f& v);
    void readParameter(const QString& name, vec3d& v);

private:
    QFile m_file;
};



} // namespace cyi

#endif // INPUTREADER_H
