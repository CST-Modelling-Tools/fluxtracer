#ifndef RAYTHREAD_H
#define RAYTHREAD_H

#include <QThread>

#include "RayServer.h"

namespace cyi {



class RayThread: public QThread
{
    Q_OBJECT

public:
    RayThread(RayServer* server, const QVector<RayFunctor*>& rayFunctors);
    ~RayThread();

    void run();

    const QVector<RayFunctor*>& rayFunctors() const {return m_rayFunctors;}

private:
    RayServer* m_server;
    QVector<RayFunctor*> m_rayFunctors;
};



} // namespace cyi

#endif // RAYTHREAD_H
