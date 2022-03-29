#ifndef FLUXTRACERREADER_H
#define FLUXTRACERREADER_H

#include "common/InputReader.h"
#include "functors/RayFunctor.h"

namespace cyi {



struct FluxTracerXML
{
    double version;
    QString inputPath;
    QString outputPath;
    int threads;
};



class FluxTracerReader: public InputReader
{
public:
    FluxTracerReader(const QString& fileName, int nodes = 1, int node = 0);

    void run();

    const FluxTracerXML& fluxTracerXML() const {return m_fluxTracerXML;}
    const QVector<QString>& rayFolders() const {return m_rayFolders;}
    const QVector<RayFunctor*>& rayFunctors() const {return m_rayFunctors;}
    const QVector<QString>& outputFiles() const {return m_outputFiles;}

protected:
    void readFluxTracer();
    void readRays();
    void readSphericalReceiver();
    void readSphericalReceiverVoxelized();
    void readSphericalReceiverAngular();
    void readTubularReceiver();
    void readTubularReceiverAngular();
    void readTubularReceiverVoxelized();
    void readCylindricalReceiver();
    void readVoxelTraversal();
    void readFluxPlane();
    void readFluxCylinder();
    void readHeliostatAngles();
    void readAngleWithPlane();
    void readAngularMatrix();

private:
    int m_nodes;
    int m_node;
    FluxTracerXML m_fluxTracerXML;
    QVector<QString> m_rayFolders;
    QVector<RayFunctor*> m_rayFunctors;
    QVector<QString> m_outputFiles;
};



} // namespace cyi

#endif // FLUXTRACERREADER_H

