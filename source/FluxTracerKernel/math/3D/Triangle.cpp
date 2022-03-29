#include "Triangle.h"

using namespace cyi;



Triangle::Triangle(const vec3r& a, const vec3r& b, const vec3r& c):
    m_a(a),
    m_b(b),
    m_c(c)
{

}

real Triangle::area() const
{
    return cross(m_a - m_c, m_b - m_c).norm()/2.;
}

bool Triangle::isIntersected(const Ray& ray) const
{
    vec3r eu = m_a - m_c;
    vec3r ev = m_b - m_c;
    vec3r qt = ray.start() - m_c;
    vec3r qu = cross(qt, eu);
    vec3r qv = cross(ray.vector(), ev);

    real q = dot(qv, eu);
    if (eqz(q)) return false;
    real t = dot(qu, ev)/q;
    if (t < 0.) return false;
    real u = dot(qv, qt)/q;
    if (u < 0. || u > 1.) return false;
    real v = dot(qu, ray.vector())/q;
    if (v < 0. || v > 1. - u) return false;
    return true;
}

Triangle::operator QString() const
{
    return QString("Triangle(%1, %2, %3)").arg(m_a).arg(m_b).arg(m_c);
}

QTextStream& cyi::operator<<(QTextStream& out, const Triangle& triangle)
{
    out << QString(triangle);
    return out;
}
