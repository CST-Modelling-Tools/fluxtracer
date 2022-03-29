#include "RayThread.h"

using namespace cyi;



RayThread::RayThread(RayServer* server,const QVector<RayFunctor*>& rayFunctors):
    QThread(server),
    m_server(server)
{  
    for (RayFunctor* rf : rayFunctors)
        m_rayFunctors << rf->copy();
}

RayThread::~RayThread()
{
    for (RayFunctor* rf : m_rayFunctors)
        delete rf;
}

void RayThread::run()
{
    Rays rays;
    while (true) {
        if (m_server->getRays(rays))
            for (const Ray& ray : rays)
                for (RayFunctor* rf : m_rayFunctors) rf->process(ray);
        else if (m_server->isReading())
            sleep(1);
        else
            break;
    }
}

