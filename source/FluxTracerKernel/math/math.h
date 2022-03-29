#ifndef MATH_H
#define MATH_H

#include <QtMath>

namespace cyi {



extern const double pi;
extern const double degree;
extern const double epsilon;
extern const double infinity;
extern const double qnan;

inline bool eqz(double x) {return std::abs(x) < epsilon;}

// clamp x between xMin and xMax
template<class T>
T clamp(T x, T xMin, T xMax)
{
    if (x <= xMin) return xMin;
    if (x >= xMax) return xMax;
    return x;
}

inline double pow2(double x) {return x*x;}



} // namespace cyi

#endif // MATH_H
