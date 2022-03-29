#ifndef EQUATIONS_H
#define EQUATIONS_H

namespace cyi {



// a*x^2 + b*x + c = 0
struct QuadraticEquation
{
    double a, b, c;
    double x1, x2;

    int solve();
};



} // namespace cyi

#endif // EQUATIONS_H
