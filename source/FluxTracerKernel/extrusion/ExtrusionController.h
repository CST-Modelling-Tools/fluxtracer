#ifndef EXTRUSIONCONTROLLER_H
#define EXTRUSIONCONTROLLER_H

#include "ExtrudedReceiver.h"
#include "math/3D/Ray.h"
#include "math/1D/Grid.h"

namespace cyi {



class ExtrusionController
{
public:
    // constructors
    ExtrusionController();

    // components
    ExtrudedReceiver& receiver() {return m_receiver;}
    void setGridR(const Grid& gridR) {m_gridR = gridR;}
    void setGridPhi(const Grid& gridPhi) {m_gridPhi = gridPhi;}
    void setGridZ(const Grid& gridZ) {m_gridZ = gridZ;}
    void initLinear();
    void setRays(const QString& fileName);

private:
    ExtrudedReceiver m_receiver;
    Grid m_gridR;
    Grid m_gridPhi;
    Grid m_gridZ;
    QVector<Ray> m_rays;
};



} // namespace cyi

#endif // EXTRUSIONCONTROLLER_H
