#ifndef PHOTONREADER_H
#define PHOTONREADER_H

#include "common/qiostream.h"
#include "PhotonInfo.h"

namespace cyi {



class PhotonReader
{
public:
    PhotonReader(QDir dir);
    bool readPhoton(PhotonInfo& photonInfo);

private:
    bool openFile();
    static bool fileOrder(const QString& a, const QString& b);
    static int fileNumber(const QString& fileName);

private:
    QDir m_dir;
    QStringList m_files;
    int m_fileNumber;

    QFile m_file;
    QDataStream m_stream;  
};



} // namespace cyi

#endif // PHOTONREADER_H
