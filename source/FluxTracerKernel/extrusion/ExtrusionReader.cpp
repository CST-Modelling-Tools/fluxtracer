#include "ExtrusionReader.h"

#include "math/1D/Grid.h"

using namespace cyi;



ExtrusionReader::ExtrusionReader(const QString& fileName):
    InputReader(fileName)
{
    try {
        readExtrudedReceiver();
    } catch (const char* msg) {
        throw QString("Error in reading\n%1\n%2").arg(fileName).arg(msg);
    }
}

void ExtrusionReader::readExtrudedReceiver()
{
    ExtrudedReceiverXML& xml = m_xml;
    readElement("ExtrudedReceiver");

    readParameter("version", xml.version);
    if (xml.version != 0.3) throw "ExtrudedReceiver of version 0.3 required";

    readParameter("inputPath", xml.inputPath);
    readParameter("outputPath", xml.outputPath);
    if (xml.outputPath == "") xml.outputPath = xml.inputPath;

    cout << "Reading ExtrudedReceiver" << endl;
    cout << "inputPath = " << xml.inputPath << endl;
    cout << "outputPath = " << xml.outputPath << endl;

    while (readNextStartElement()) {
        if (name() == "Rays")
            readRays();
        else if (name() == "Disk")
            readDisk();
        else if (name() == "Interception")
            readInterception();
        else
            throw "Unknown xml-tag";
    }

    skipElement();
}

void ExtrusionReader::readRays()
{
    struct RaysXML {
        QString file;
    } xml;

    readParameter("file", xml.file);
    xml.file = QDir(m_xml.inputPath).filePath(xml.file);

    cout << "Reading rays" << endl;
    cout << "file = " << xml.file << endl;
    cout << endl;

    m_extrusion.setRays(xml.file);

    skipElement();
}

void ExtrusionReader::readDisk()
{
    struct DiskXML {
        real rMin;
        real rMax;
        int rDivs;
        int phiDivs;
        real zMin;
        real zMax;
    } xml;

    readParameter("rMin", xml.rMin);
    readParameter("rMax", xml.rMax);
    readParameter("rDivs", xml.rDivs);
    Grid gridR(Interval(xml.rMin, xml.rMax), xml.rDivs);
    readParameter("phiDivs", xml.phiDivs);
    Grid gridPhi(Interval(0., 2.*pi), xml.phiDivs);
    readParameter("zMin", xml.zMin);
    readParameter("zMax", xml.zMax);
    Grid gridZ(Interval(xml.zMin, xml.zMax), xml.rDivs);

    cout << "Creating disk" << endl;
    cout << "r = " << gridR << endl;
    cout << "phi = " << gridPhi << endl;
    cout << "z = " << gridZ << endl;
    cout << endl;

    m_extrusion.setGridR(gridR);
    m_extrusion.setGridPhi(gridPhi);
    m_extrusion.setGridZ(gridZ);
    m_extrusion.initLinear();

    skipElement();
}

void ExtrusionReader::readInterception()
{
    struct InterceptionXML {
        real min;
        QString output;
    } xml;


    readParameter("min", xml.min);
    readParameter("output", xml.output);
    xml.output = QDir(m_xml.outputPath).filePath(xml.output);

    cout << "Writing shapes" << endl;
    cout << "interception = " << xml.min << endl;
    cout << "output = " << xml.output << endl;
    cout << endl;

    m_extrusion.receiver().save(xml.output);

    skipElement();
}
