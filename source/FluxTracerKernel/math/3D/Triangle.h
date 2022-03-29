#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Ray.h"

namespace cyi {



struct TriangleIndexed
{
    TriangleIndexed() {}
    TriangleIndexed(int a, int b, int c): a(a), b(b), c(c) {}

    int a;
    int b;
    int c;
};



class Triangle
{
public:
    // constructors
    Triangle() {}
    Triangle(const vec3r& a, const vec3r& b, const vec3r& c);

    // components
    const vec3r& a() const {return m_a;}
    const vec3r& b() const {return m_b;}
    const vec3r& c() const {return m_c;}

    void setA(const vec3r& p) {m_a = p;}
    void setB(const vec3r& p) {m_b = p;}
    void setC(const vec3r& p) {m_c = p;}

    // functions
    real area() const;
    bool isIntersected(const Ray& ray) const;

    // conversions
    operator QString() const;

private:
    vec3r m_a;
    vec3r m_b;
    vec3r m_c;
};



// stream
QTextStream& operator<<(QTextStream& out, const Triangle& triangle);



} // namespace cyi

#endif // TRIANGLE_H
