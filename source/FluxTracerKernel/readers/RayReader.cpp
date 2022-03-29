#include "RayReader.h"

#include "ParametersReader.h"

using namespace cyi;



static QString toString(long n)
{
    return QLocale().toString(qlonglong(n));
}



RayReader::RayReader(QDir dir):
    PhotonReader(dir)
{
    QStringList list = dir.entryList(QStringList() << "*_parameters.txt", QDir::Files);
    QString fileName = list.first();
    m_parametersReader = new ParametersReader(dir.filePath(fileName));
    m_idSurfaceLight = m_parametersReader->surface("Light");
    m_idSurfaceVirtual = m_parametersReader->surface("ReceiverAperture");
    m_photonEnergy = m_parametersReader->photonEnergy();

    cout << "Extracting parameters" << endl;
    cout << "file = " << fileName << endl;
    cout << "idSurfaceLight = " << m_idSurfaceLight << endl;
    cout << "idSurfaceVirtual = " << m_idSurfaceVirtual << endl;
    cout << "photonEnergy = " << m_photonEnergy << endl;
    cout << endl;

    readPhoton(m_end);
    m_photons = 1;
    m_rays = 0;
}

RayReader::~RayReader()
{
    delete m_parametersReader;
}

bool RayReader::read(Ray& ray)
{
    while (m_start = m_end, readPhoton(m_end)) {
        m_photons++;
        if (m_start.idNext != m_end.id) continue;
        if (m_start.idSurface == m_idSurfaceLight) continue;
        if (m_start.idSurface == m_idSurfaceVirtual) continue;
        if (m_end.idSurface != m_idSurfaceVirtual) continue;

        m_rays++;
        ray.setPoints(m_start.v, m_end.v);
        ray.setEnergy(m_photonEnergy);
        ray.surface = m_start.idSurface;
        ray.id = m_parametersReader->getName(ray.surface);
        return true;
    }

    cout << endl;
    cout << "photons = " << toString(m_photons) << endl;
    cout << "rays = " << toString(m_rays) << endl;
    cout << endl;
    return false;
}
