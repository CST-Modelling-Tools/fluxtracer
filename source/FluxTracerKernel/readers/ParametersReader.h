#ifndef PARAMETERSREADER_H
#define PARAMETERSREADER_H

#include "common/qiostream.h"

namespace cyi {



class ParametersReader
{
public:
    ParametersReader(const QString& fileName);

    int parameters() const {return m_parameters;}
    int surface(const QString& name) const;
    double photonEnergy() const {return m_photonEnergy;}

    QString getName(int id) {return m_heliostatNames.value(id, "NAN");}

private:
    int m_parameters;
    QStringList m_surfaces;
    double m_photonEnergy;

    void findHeliostats();
    QMap<int, QString> m_heliostatNames;
};



} // namespace cyi

#endif // PARAMETERSREADER_H
