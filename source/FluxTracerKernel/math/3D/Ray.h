#ifndef RAYSEGMENT_H
#define RAYSEGMENT_H

#include "math/vec3r.h"

namespace cyi {



class Ray
{
public:
    Ray(const vec3r& start = vec3r::Null, const vec3r& end = vec3r::UnitZ, double energy = 1.);

    void setPoints(const vec3r& start, const vec3r& end);
    void setEnergy(double energy) {m_energy = energy;}

    const vec3r& start() const {return m_start;}
    const vec3r& end() const {return m_end;}

    const vec3r& vector() const {return m_vector;}
    const vec3r& rvector() const {return m_rvector;}

    double energy() const {return m_energy;}

    vec3r point(double t) const;

private:
    vec3r m_start;
    vec3r m_end;
    vec3r m_vector;
    vec3r m_rvector;
    double m_energy;

public:
    double surface;
    QString id;
};



} // namespace cyi

#endif // RAYSEGMENT_H
