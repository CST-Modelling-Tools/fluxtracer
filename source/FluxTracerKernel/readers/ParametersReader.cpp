#include "ParametersReader.h"

using namespace cyi;



ParametersReader::ParametersReader(const QString& fileName)
{
    try {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
            throw "File cannot be opened";
        QTextStream in(&file);
        QString line;

        if (in.readLine() != "START PARAMETERS")
            throw "START PARAMETERS not found";
        m_parameters = 0;
        while (in.readLineInto(&line)) {
            if (line == "END PARAMETERS") break;
            m_parameters++;
        }

        if (in.readLine() != "START SURFACES")
            throw "START SURFACES not found";
        while (in.readLineInto(&line)) {
            if (line == "END SURFACES") break;
            int n = line.indexOf(' ');
            if (n == -1) throw line.toStdString().c_str();
//            m_surfaces << line.mid(n + 1);
            m_surfaces << line;
        }

        if (!in.readLineInto(&line))
            throw "Photon energy not found";
        bool ok;
        m_photonEnergy = line.toDouble(&ok);
        if (!ok) throw "Photon energy cannot be read";

        findHeliostats();
    }
    catch (const char* msg) {
        throw QString("Error in reading\n%1\n%2").arg(fileName).arg(msg);
    }
}

int ParametersReader::surface(const QString& name) const
{
    QRegExp rx("*" + name + "*");
    rx.setPatternSyntax(QRegExp::Wildcard);
    int id = m_surfaces.indexOf(rx);
    if (id < 0)
        throw QString("Surface not found\n*%1*").arg(name);
    return id + 1;
}

void ParametersReader::findHeliostats()
{
    for (QString line : m_surfaces) {
        int n = line.indexOf(" "); // separate id and path
        if (n < 0) continue;
        QString id = line.left(n);
        int key = id.toInt();

        int b = line.indexOf("/HelioTracker/MirrorSurface"); // signature of heliostat
        if (b < 0) continue;
        int a = line.lastIndexOf("/", b - 1); // start of heliostat name
        if (a < 0) continue;
        QString name = line.mid(a + 1, b - a - 1);

        m_heliostatNames[key] = name;
    }
}

