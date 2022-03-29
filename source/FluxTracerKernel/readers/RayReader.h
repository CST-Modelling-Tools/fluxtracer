#ifndef RAYREADER_H
#define RAYREADER_H

#include "PhotonReader.h"
#include "math/3D/Ray.h"

namespace cyi {

class ParametersReader;



class RayReader: public PhotonReader
{
public:
    RayReader(QDir dir);
    ~RayReader();

    bool read(Ray& ray);

private:
    double m_idSurfaceLight;
    double m_idSurfaceVirtual;
    double m_photonEnergy;

    PhotonInfo m_start;
    PhotonInfo m_end;

    long m_photons;
    long m_rays;

    ParametersReader* m_parametersReader;
};



} // namespace cyi

#endif // RAYREADER_H
