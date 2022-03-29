#ifndef EXTRUSIONREADER_H
#define EXTRUSIONREADER_H

#include "common/InputReader.h"
#include "ExtrusionController.h"

namespace cyi {



struct ExtrudedReceiverXML
{
    double version;
    QString inputPath;
    QString outputPath;
};



class ExtrusionReader: public InputReader
{
public:
    ExtrusionReader(const QString& fileName);

protected:
    void readExtrudedReceiver();
    void readRays();
    void readDisk();
    void readInterception();

    ExtrudedReceiverXML m_xml;
    ExtrusionController m_extrusion;
};



} // namespace cyi

#endif // EXTRUSIONREADER_H
