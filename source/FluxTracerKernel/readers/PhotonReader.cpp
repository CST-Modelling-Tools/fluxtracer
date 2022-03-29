#include "PhotonReader.h"

using namespace cyi;



PhotonReader::PhotonReader(QDir dir):
    m_dir(dir)
{
    // get file names in the directory and sort them
    m_files = m_dir.entryList(QStringList() << "*.dat", QDir::Files);
    std::sort(m_files.begin(), m_files.end(), fileOrder);

    // prepare to read the first file
    m_fileNumber = 0;
    m_stream.setStatus(QDataStream::ReadPastEnd);
}

bool PhotonReader::readPhoton(PhotonInfo& photonInfo)
{
    // read data
    if (m_stream.status() == QDataStream::Ok) {
        m_stream >> photonInfo;
        if (m_stream.status() == QDataStream::Ok) return true;
    }

    // open next file
    if (m_stream.status() == QDataStream::ReadPastEnd) {
        if (openFile()) return readPhoton(photonInfo);
    }

    return false;
}

bool PhotonReader::openFile()
{
    if (m_file.isOpen()) m_file.close();

    if (m_fileNumber == m_files.size()) return false;
    QString& name = m_files[m_fileNumber++];
    cout << name << endl;

    m_file.setFileName(m_dir.filePath(name));
    if (!m_file.open(QIODevice::ReadOnly)) {
        cout << "File cannot be opened" << endl;
        return false;
    }

    m_stream.resetStatus();
    m_stream.setDevice(&m_file);
//    m_stream.setFloatingPointPrecision(QDataStream::DoublePrecision);
//    m_stream.setByteOrder(QDataStream::BigEndian);
    return true;
}

bool PhotonReader::fileOrder(const QString& a, const QString& b)
{
    return fileNumber(a) < fileNumber(b);
}

// Example "Results_5.dat" -> 5
int PhotonReader::fileNumber(const QString& fileName)
{
    int iA = fileName.lastIndexOf('_') + 1;
    int iB = fileName.lastIndexOf('.');
    int n = fileName.mid(iA, iB - iA).toInt();
    return n;
}
