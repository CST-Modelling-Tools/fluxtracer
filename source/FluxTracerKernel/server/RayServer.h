#ifndef RAYSERVER_H
#define RAYSERVER_H

#include <QThread>
#include <QMutex>
#include <QStack>
#include <QVector>
#include <QSet>

#include "readers/FluxTracerReader.h"
#include "math/3D/Ray.h"

namespace cyi {

typedef QVector<Ray> Rays;



class RayServer: public QThread
{
    Q_OBJECT

public:
    explicit RayServer(QObject* parent, FluxTracerReader* reader);

    void run();
    bool isRunning() const {return m_isRunning;}
    bool isReading() const {return m_isReading;}

    bool getRays(Rays& rays);

private:
    void addRays(Rays& rays);

private slots:
    void threadFinished();

signals:
    void close();

private:
    FluxTracerReader* m_reader;
    bool m_isRunning;
    bool m_isReading;

    QSet<QObject*> m_threads;

    QStack<Rays> m_stackRays;

    QMutex m_mutexAddRays;
    QMutex m_mutexGetRays;
};



} // namespace cyi

#endif // RAYSERVER_H
