#include "PhotonInfo.h"

using namespace cyi;



QDataStream& cyi::operator>>(QDataStream& in, PhotonInfo& photonInfo)
{
    in >> photonInfo.id;
    in >> photonInfo.v.mx;
    in >> photonInfo.v.my;
    in >> photonInfo.v.mz;
    in >> photonInfo.side;
    in >> photonInfo.idPrevious;
    in >> photonInfo.idNext;
    in >> photonInfo.idSurface;
    return in;
}
