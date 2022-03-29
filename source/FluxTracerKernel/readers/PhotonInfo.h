#ifndef PHOTONINFO_H
#define PHOTONINFO_H

#include "math/vec3r.h"

namespace cyi {



struct PhotonInfo
{
    double id;
    vec3r v;
    double side;
    double idPrevious;
    double idNext;
    double idSurface;
};



QDataStream& operator>>(QDataStream& in, PhotonInfo& photonInfo);



} // namespace cyi

#endif // PHOTONINFO_H
