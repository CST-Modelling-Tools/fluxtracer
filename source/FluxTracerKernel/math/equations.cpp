#include "equations.h"

#include "math.h"

using namespace cyi;



 /**
 * Solves the quadratic equation a*x^2 + b*x + c = 0
 * Returns the number of roots
 *
 * Example:
 * QuadraticEquation eq;
 * eq.a = 2.;
 * eq.b = -38.;
 * eq.c = 168.;
 * int n = eq.solve();
 * cout << n << " roots: " << endl;
 * if (n > 0) cout << "x = " << eq.x1 << endl;
 * if (n > 1) cout << "x = " << eq.x2 << endl;
 * cout << endl;
 */
int QuadraticEquation::solve()
{
    if (!eqz(a)) { // quadratic equation
        double disc = b*b - 4.*a*c; // discriminant
        if (disc >= 0.) { // real roots
            // use Vieta's formulas
            // avoid subtraction of near numbers for the first root
            int sgnb = b >= 0. ? 1 : -1;
            double temp = -(b + sgnb*std::sqrt(disc))/2.;
            if (!eqz(temp)) {
                x1 = temp/a;
                x2 = c/temp;
                if (x1 > x2) std::swap(x1, x2); // sort
            } else { // b = 0 and c = 0
                x1 = 0.;
                x2 = 0.;
            }
            return 2;
        } else { // complex roots
            x1 = -b/(2.*a); // real parts
            x2 = x1;
            return 0;
        }
    } else if(!eqz(b)) { // linear equation
        x1 = -c/b;
        x2 = x1;
        return 1;
    } else if (!eqz(c)) { // no roots
        x1 = qnan; // quiet not a number
        x2 = qnan;
        return 0;
    } else { // any roots
        x1 = -infinity; // infinity
        x2 = infinity;
        return 3;
    }
}
