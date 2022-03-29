#include "RayServer.h"

#include "RayThread.h"
#include "readers/RayReader.h"
#include "QCoreApplication"

using namespace cyi;



RayServer::RayServer(QObject* parent, FluxTracerReader* reader):
    QThread(parent),
    m_reader(reader)
{
    m_isRunning = false;
    m_isReading = false;

    connect(this, SIGNAL(close()), (QCoreApplication*) parent, SLOT(quit()));
}

void RayServer::run()
{
    cout << "Starting threads" << endl;
    m_isRunning = true;
    for (int n = 0; n < m_reader->fluxTracerXML().threads - 1; ++n) {
        RayThread* thread = new RayThread(this, m_reader->rayFunctors());
        connect(thread, SIGNAL(finished()), this, SLOT(threadFinished()));
        m_threads << thread;
        thread->start();
    }
    cout << endl;

    m_isReading = true;
    QVector<Ray> rays;
    const int numRays = 16000;
    rays.reserve(numRays);
    for (const QString& folder : m_reader->rayFolders()) {
        cout << "Processing rays" << endl;
        cout << "folder = " << folder << endl << endl;
        RayReader reader(folder);
        Ray ray;
        while (reader.read(ray)) {
            rays << ray;
            if (rays.size() == numRays)
                addRays(rays);
        }
        addRays(rays);
    }
    m_isReading = false;
    cout << endl;
}

void RayServer::addRays(Rays& rays)
{
    QMutexLocker locker(&m_mutexAddRays);
    m_stackRays.push(rays);
    rays.clear();
}

bool RayServer::getRays(Rays& rays)
{
    QMutexLocker locker(&m_mutexGetRays);
    if (m_stackRays.isEmpty()) return false;
    rays = m_stackRays.pop();
    return true;
}

void RayServer::threadFinished()
{   
    RayThread* thread = (RayThread*) sender();
    for (int n = 0; n < m_reader->rayFunctors().size(); n++)
        m_reader->rayFunctors()[n]->combine(*thread->rayFunctors()[n]);
    m_threads.remove(thread);
    thread->deleteLater();

    if (m_threads.isEmpty()) {
        cout << "Writing output" << endl;
        for (int n = 0; n < m_reader->rayFunctors().size(); n++) {
            cout << m_reader->outputFiles()[n] << endl;
            m_reader->rayFunctors()[n]->save(m_reader->outputFiles()[n]);
        }
        cout << endl;
        m_isRunning = false;
        emit close();
    }
}


