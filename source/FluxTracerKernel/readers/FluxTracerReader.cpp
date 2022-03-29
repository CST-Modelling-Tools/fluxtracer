#include "FluxTracerReader.h"

#include <QCoreApplication>
#include <QThreadPool>

#include "functors/SphericalReceiver.h"
#include "functors/SphericalReceiverVoxelized.h"
#include "functors/SphericalReceiverAngular.h"
#include "functors/TubularReceiverAngular.h"
#include "functors/TubularReceiver.h"
#include "functors/TubularReceiverVoxelized.h"
#include "functors/CylindricalReceiver.h"
#include "functors/HeliostatAngles.h"
#include "functors/VoxelTraversal.h"
#include "functors/FluxPlane.h"
#include "functors/FluxCylinder.h"
#include "functors/AngleWithPlane.h"
#include "functors/AngularMatrix.h"
#include "readers/RayReader.h"

using namespace cyi;



FluxTracerReader::FluxTracerReader(const QString& fileName, int nodes, int node):
    InputReader(fileName)
{
    m_nodes = nodes;
    m_node = node;

    try {
        readFluxTracer();
    } catch (const char* msg) {
        throw QString("Error in reading\n%1\n%2").arg(fileName).arg(msg);
    }

    if (nodes > 1) {
        QVector<QString> rayFolders;
        for (int n = m_node; n < m_rayFolders.size(); n += m_nodes)
            rayFolders << m_rayFolders[n];
        m_rayFolders = rayFolders;

        QString temp = QString("_%1").arg(m_node);
        for (QString& name : m_outputFiles)
            name.insert(name.lastIndexOf("."), temp);
    }
}

void FluxTracerReader::run()
{
    for (QString& folder : m_rayFolders) {
        cout << "Processing rays" << endl;
        cout << "folder = " << folder << endl << endl;
        RayReader rayReader(folder);
        Ray ray;
        while (rayReader.read(ray))
            for (RayFunctor* rf : m_rayFunctors) rf->process(ray);
    }
    cout << endl;

    cout << "Writing output" << endl;
    for (int n = 0; n < m_rayFunctors.size(); n++) {
        cout << m_outputFiles[n] << endl;
        m_rayFunctors[n]->save(m_outputFiles[n]);
        delete(m_rayFunctors[n]);
    }
    cout << endl;
}

void FluxTracerReader::readFluxTracer()
{
    FluxTracerXML& xml = m_fluxTracerXML;
    readElement("FluxTracer");

    readParameter("version", xml.version);
    if (xml.version != 0.3) throw "FluxTracer of version 0.3 required";

    readParameter("inputPath", xml.inputPath);
    readParameter("outputPath", xml.outputPath);
    if (xml.outputPath == "")
        xml.outputPath = xml.inputPath;
    readParameter("threads", xml.threads);
    if (xml.threads == -1)
        xml.threads = QThreadPool::globalInstance()->maxThreadCount();

    cout << "Reading FluxTracer" << endl;
    cout << "inputPath = " << xml.inputPath << endl;
    cout << "outputPath = " << xml.outputPath << endl;
    cout << "threads = " << xml.threads << endl;
    cout << endl;

    while (readNextStartElement()) {
        if (name() == "Rays")
            readRays();
        else if (name() == "SphericalReceiver")
            readSphericalReceiver();
        else if (name() == "SphericalReceiverAngular")
            readSphericalReceiverAngular();
        else if (name() == "SphericalReceiverVoxelized")
            readSphericalReceiverVoxelized();
        else if (name() == "TubularReceiver")
            readTubularReceiver();
        else if (name() == "TubularReceiverAngular")
            readTubularReceiverAngular();
        else if (name() == "TubularReceiverVoxelized")
            readTubularReceiverVoxelized();
        else if (name() == "CylindricalReceiver")
            readCylindricalReceiver();
        else if (name() == "VoxelTraversal")
            readVoxelTraversal();
        else if (name() == "FluxPlane")
            readFluxPlane();
        else if (name() == "FluxCylinder")
            readFluxCylinder();
        else if (name() == "HeliostatAngles")
            readHeliostatAngles();
        else if (name() == "AngleWithPlane")
            readAngleWithPlane();
        else if (name() == "AngularMatrix")
            readAngularMatrix();
        else
            throw "Unknown xml-tag";
    }

    skipElement();
}

void FluxTracerReader::readRays()
{
    struct RaysXML {
        QString folder;
    } xml;

    readParameter("folder", xml.folder);
    QDir dir = QDir(m_fluxTracerXML.inputPath).filePath(xml.folder);

    bool sub = dir.entryList(QStringList() << "*_parameters.txt", QDir::Files).isEmpty();
    if (sub) {
        QStringList subdirs = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        for (QString subdir : subdirs)
            m_rayFolders << dir.filePath(subdir);
    } else {
        m_rayFolders << dir.path();
    }

    skipElement();
}

void FluxTracerReader::readSphericalReceiver()
{
    struct XML {
        vec3r normal;
        real angleMin;
        real angleMax;
        int angleDivs;
        QString output;
    } xml;

    readParameter("center", xml.normal);
    readParameter("rMin", xml.angleMin);
    readParameter("rMax", xml.angleMax);
    readParameter("rDivs", xml.angleDivs);
    Grid rGrid(Interval(xml.angleMin, xml.angleMax), xml.angleDivs);
    readParameter("output", xml.output);
    xml.output = QDir(m_fluxTracerXML.outputPath).filePath(xml.output);
    skipElement();

    m_rayFunctors << new SphericalReceiver(xml.normal, rGrid);
    m_outputFiles << xml.output;

    cout << "Creating receiver" << endl;
    cout << "center = " << xml.normal << endl;
    cout << "rGrid = " << rGrid << endl;
    cout << "output = " << xml.output << endl;
    cout << endl;
}

void FluxTracerReader::readSphericalReceiverAngular()
{
    struct XML {
        vec3r normal;
        int phiDivs;
        int thetaDivs;
        double angleMin;
        double angleMax;
        int angleDivs;
        double eta;
        QString output;
    } xml;

    readParameter("center", xml.normal);
    readParameter("phiDivs", xml.phiDivs);
    readParameter("thetaDivs", xml.thetaDivs);
    readParameter("rMin", xml.angleMin);
    readParameter("rMax", xml.angleMax);
    readParameter("rDivs", xml.angleDivs);
    Grid rGrid(Interval(xml.angleMin, xml.angleMax), xml.angleDivs);
    readParameter("eta", xml.eta);
    readParameter("output", xml.output);
    xml.output = QDir(m_fluxTracerXML.outputPath).filePath(xml.output);
    skipElement();

    m_rayFunctors << new SphericalReceiverAngular(xml.normal, xml.phiDivs, xml.thetaDivs, rGrid, xml.eta);
    m_outputFiles << xml.output;

    cout << "Creating SphericalReceiverAngular" << endl;
    cout << "center = " << xml.normal << endl;
    cout << "phiDivs = " << xml.phiDivs << endl;
    cout << "thetaDivs = " << xml.thetaDivs << endl;
    cout << "rGrid = " << rGrid << endl;
    cout << "eta = " << xml.eta << endl;
    cout << "output = " << xml.output << endl;
    cout << endl;
}

void FluxTracerReader::readSphericalReceiverVoxelized()
{
    struct XML {
        vec3r normal;
        vec3r cornerMin;
        vec3r cornerMax;
        vec3i dimensions;
        QString output;
    } xml;

    readParameter("center", xml.normal);
    readParameter("cornerMin", xml.cornerMin);
    readParameter("cornerMax", xml.cornerMax);
    BoundingBox box(xml.cornerMin, xml.cornerMax);
    readParameter("dimensions", xml.dimensions);
    readParameter("output", xml.output);
    xml.output = QDir(m_fluxTracerXML.outputPath).filePath(xml.output);
    skipElement();

    m_rayFunctors << new SphericalReceiverVoxelized(xml.normal, box, xml.dimensions);
    m_outputFiles << xml.output;

    cout << "Creating SphericalReceiverVoxelized" << endl;
    cout << "center = " << xml.normal << endl;
    cout << "box = " << box << endl;
    cout << "dimensions = " << xml.dimensions << endl;
    cout << "output = " << xml.output << endl;
    cout << endl;
}


void FluxTracerReader::readTubularReceiver()
{
    struct XML {
        vec3r normal;
        vec3r direction;
        real angleMin;
        real angleMax;
        int angleDivs;
        QString output;
    } xml;

    readParameter("center", xml.normal);
    readParameter("direction", xml.direction);
    readParameter("rMin", xml.angleMin);
    readParameter("rMax", xml.angleMax);
    readParameter("rDivs", xml.angleDivs);
    Grid rGrid(Interval(xml.angleMin, xml.angleMax), xml.angleDivs);
    readParameter("output", xml.output);
    xml.output = QDir(m_fluxTracerXML.outputPath).filePath(xml.output);
    skipElement();

    m_rayFunctors << new TubularReceiver(xml.normal, xml.direction, rGrid);
    m_outputFiles << xml.output;

    cout << "Creating TubularReceiver" << endl;
    cout << "center = " << xml.normal << endl;
    cout << "direction = " << xml.direction << endl;
    cout << "rGrid = " << rGrid << endl;
    cout << "output = " << xml.output << endl;
    cout << endl;
}

void FluxTracerReader::readTubularReceiverAngular()
{
    struct XML {
        vec3r normal;
        vec3r direction;
//        vec3r normal;
        int phiDivs;
        double hMin;
        double hMax;
        int hDivs;
        double angleMin;
        double angleMax;
        int angleDivs;
        double eta;
        QString output;
    } xml;

    readParameter("center", xml.normal);
    readParameter("direction", xml.direction);
    xml.direction.normalize();
    readParameter("normal", xml.normal);
    xml.normal.normalize();
    readParameter("phiDivs", xml.phiDivs);
    readParameter("hMin", xml.hMin);
    readParameter("hMax", xml.hMax);
    readParameter("hDivs", xml.hDivs);
    Grid hGrid(Interval(xml.hMin, xml.hMax), xml.hDivs);
    readParameter("rMin", xml.angleMin);
    readParameter("rMax", xml.angleMax);
    readParameter("rDivs", xml.angleDivs);
    Grid rGrid(Interval(xml.angleMin, xml.angleMax), xml.angleDivs);
    readParameter("eta", xml.eta);
    readParameter("output", xml.output);
    xml.output = QDir(m_fluxTracerXML.outputPath).filePath(xml.output);
    skipElement();

    m_rayFunctors << new TubularReceiverAngular(xml.normal, xml.direction, xml.normal, xml.phiDivs, hGrid, rGrid, xml.eta);
    m_outputFiles << xml.output;

    cout << "Creating TubularReceiverAngular" << endl;
    cout << "center = " << xml.normal << endl;
    cout << "direction = " << xml.direction << endl;
    cout << "normal = " << xml.normal << endl;
    cout << "phiDivs = " << xml.phiDivs << endl;
    cout << "hGrid = " << hGrid << endl;
    cout << "rGrid = " << rGrid << endl;
    cout << "eta = " << xml.eta << endl;
    cout << "output = " << xml.output << endl;
    cout << endl;
}

void FluxTracerReader::readTubularReceiverVoxelized()
{
    struct XML {
        vec3r normal;
        vec3r direction;
        vec3r cornerMin;
        vec3r cornerMax;
        vec3i dimensions;
        QString output;
    } xml;

    readParameter("center", xml.normal);
    readParameter("direction", xml.direction);
    readParameter("cornerMin", xml.cornerMin);
    readParameter("cornerMax", xml.cornerMax);
    BoundingBox box(xml.cornerMin, xml.cornerMax);
    readParameter("dimensions", xml.dimensions);
    readParameter("output", xml.output);
    xml.output = QDir(m_fluxTracerXML.outputPath).filePath(xml.output);
    skipElement();

    m_rayFunctors << new TubularReceiverVoxelized(xml.normal, xml.direction, box, xml.dimensions);
    m_outputFiles << xml.output;

    cout << "Creating TubularReceiverVoxelized" << endl;
    cout << "center = " << xml.normal << endl;
    cout << "direction = " << xml.direction << endl;
    cout << "box = " << box << endl;
    cout << "dimensions = " << xml.dimensions << endl;
    cout << "output = " << xml.output << endl;
    cout << endl;
}

void FluxTracerReader::readCylindricalReceiver()
{
    struct XML {
        vec3r normal;
        real dMin;
        real dMax;
        int dDivs;
        real hMin;
        real hMax;
        int hDivs;
        QString output;
    } xml;

    readParameter("center", xml.normal);
    readParameter("dMin", xml.dMin);
    readParameter("dMax", xml.dMax);
    readParameter("dDivs", xml.dDivs);
    Grid dGrid(Interval(xml.dMin, xml.dMax), xml.dDivs);
    readParameter("hMin", xml.hMin);
    readParameter("hMax", xml.hMax);
    readParameter("hDivs", xml.hDivs);
    Grid hGrid(Interval(xml.hMin, xml.hMax), xml.hDivs);
    readParameter("output", xml.output);
    xml.output = QDir(m_fluxTracerXML.outputPath).filePath(xml.output);
    skipElement();

    m_rayFunctors << new CylindricalReceiver(xml.normal, dGrid, hGrid);
    m_outputFiles << xml.output;

    cout << "Creating CylindricalReceiver" << endl;
    cout << "center = " << xml.normal << endl;
    cout << "dGrid = " << dGrid << endl;
    cout << "hGrid = " << hGrid << endl;
    cout << "output = " << xml.output << endl;
    cout << endl;
}

void FluxTracerReader::readVoxelTraversal()
{
    struct XML {
        vec3r cornerMin;
        vec3r cornerMax;
        vec3i dimensions;
        QString output;
    } xml;

    readParameter("cornerMin", xml.cornerMin);
    readParameter("cornerMax", xml.cornerMax);
    BoundingBox box(xml.cornerMin, xml.cornerMax);
    readParameter("dimensions", xml.dimensions);
    readParameter("output", xml.output);
    xml.output = QDir(m_fluxTracerXML.outputPath).filePath(xml.output);
    skipElement();

    m_rayFunctors << new VoxelTraversal(box, xml.dimensions);
    m_outputFiles << xml.output;

    cout << "Creating VoxelTraversal" << endl;
    cout << "box = " << box << endl;
    cout << "dimensions = " << xml.dimensions << endl;
    cout << "output = " << xml.output << endl;
    cout << endl;
}

void FluxTracerReader::readFluxPlane()
{
    struct XML {
        real xMin;
        real xMax;
        int xDivs;
        real yMin;
        real yMax;
        int yDivs;
        real z;
        QString output;
    } xml;

    readParameter("xMin", xml.xMin);
    readParameter("xMax", xml.xMax);
    readParameter("xDivs", xml.xDivs);
    Grid xGrid(Interval(xml.xMin, xml.xMax), xml.xDivs);
    readParameter("yMin", xml.yMin);
    readParameter("yMax", xml.yMax);
    readParameter("yDivs", xml.yDivs);
    Grid yGrid(Interval(xml.yMin, xml.yMax), xml.yDivs);
    readParameter("z", xml.z);
    readParameter("output", xml.output);
    xml.output = QDir(m_fluxTracerXML.outputPath).filePath(xml.output);
    skipElement();

    m_rayFunctors << new FluxPlane(xGrid, yGrid, xml.z);
    m_outputFiles << xml.output;

    cout << "Creating FluxPlane" << endl;
    cout << "xGrid = " << xGrid << endl;
    cout << "yGrid = " << yGrid << endl;
    cout << "z = " << xml.z << endl;
    cout << "output = " << xml.output << endl;
    cout << endl;
}

void FluxTracerReader::readFluxCylinder()
{
    struct XML {
        real rho;
        int phiDivs;
        real zMin;
        real zMax;
        int zDivs;
        QString output;
    } xml;

    readParameter("rho", xml.rho);
    readParameter("phiDivs", xml.phiDivs);
    readParameter("zMin", xml.zMin);
    readParameter("zMax", xml.zMax);
    readParameter("zDivs", xml.zDivs);
    Grid zGrid(Interval(xml.zMin, xml.zMax), xml.zDivs);
    readParameter("output", xml.output);
    xml.output = QDir(m_fluxTracerXML.outputPath).filePath(xml.output);
    skipElement();

    m_rayFunctors << new FluxCylinder(xml.rho, xml.phiDivs, zGrid);
    m_outputFiles << xml.output;

    cout << "Creating FluxCylinder" << endl;
    cout << "rho = " << xml.rho << endl;
    cout << "phiDivs = " << xml.phiDivs << endl;
    cout << "zGrid = " << zGrid << endl;
    cout << "output = " << xml.output << endl;
    cout << endl;
}

void FluxTracerReader::readHeliostatAngles()
{
    struct XML {
        vec3r axis;
        real aMin;
        real aMax;
        int aDivs;
        QString output;
    } xml;

    readParameter("axis", xml.axis);
    readParameter("aMin", xml.aMin);
    readParameter("aMax", xml.aMax);
    readParameter("aDivs", xml.aDivs);
    Grid aGrid(Interval(xml.aMin, xml.aMax), xml.aDivs);
    readParameter("output", xml.output);
    xml.output = QDir(m_fluxTracerXML.outputPath).filePath(xml.output);
    skipElement();

    m_rayFunctors << new HeliostatAngles(xml.axis, aGrid);
    m_outputFiles << xml.output;

    cout << "Creating HeliostatAngles" << endl;
    cout << "axis = " << xml.axis << endl;
    cout << "aGrid = " << aGrid << endl;
    cout << "output = " << xml.output << endl;
    cout << endl;
}

void FluxTracerReader::readAngleWithPlane()
{
    struct XML {
        vec3r normal;
        real angleMin;
        real angleMax;
        int angleDivs;
        QString output;
    } xml;

    readParameter("normal", xml.normal);
    readParameter("angleMin", xml.angleMin);
    readParameter("angleMax", xml.angleMax);
    readParameter("angleDivs", xml.angleDivs);
    Grid angleGrid(Interval(xml.angleMin, xml.angleMax), xml.angleDivs);
    readParameter("output", xml.output);
    xml.output = QDir(m_fluxTracerXML.outputPath).filePath(xml.output);
    skipElement();

    m_rayFunctors << new AngleWithPlane(xml.normal, angleGrid);
    m_outputFiles << xml.output;

    cout << "Computing angles with the plane" << endl;
    cout << "normal = " << xml.normal << endl;
    cout << "angleGrid = " << angleGrid << endl;
    cout << "output = " << xml.output << endl;
    cout << endl;
}

void FluxTracerReader::readAngularMatrix()
{
    struct XML {
        vec3r axis;
        real azimuthMin;
        real azimuthMax;
        int azimuthDivs;
        real zenithMin;
        real zenithMax;
        int zenithDivs;
        QString output;
    } xml;

    readParameter("axis", xml.axis);

    readParameter("azimuthMin", xml.azimuthMin);
    readParameter("azimuthMax", xml.azimuthMax);
    readParameter("azimuthDivs", xml.azimuthDivs);
    Grid azimuthGrid(Interval(xml.azimuthMin, xml.azimuthMax), xml.azimuthDivs);

    readParameter("zenithMin", xml.zenithMin);
    readParameter("zenithMax", xml.zenithMax);
    readParameter("zenithDivs", xml.zenithDivs);
    Grid zenithGrid(Interval(xml.zenithMin, xml.zenithMax), xml.zenithDivs);

    readParameter("output", xml.output);
    xml.output = QDir(m_fluxTracerXML.outputPath).filePath(xml.output);

    skipElement();

    m_rayFunctors << new AngularMatrix(xml.axis, azimuthGrid, zenithGrid);
    m_outputFiles << xml.output;

    cout << "Computing AngularMatrix" << endl;
    cout << "axis = " << xml.axis << endl;
    cout << "output = " << xml.output << endl;
    cout << endl;
}

