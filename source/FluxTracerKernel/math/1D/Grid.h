#ifndef GRID_H
#define GRID_H

#include "Interval.h"

namespace cyi {


/**
 * @brief Grid represents an equidistant grid
 */
class Grid
{
public:
    Grid(); // TODO inherit from Interval
    Grid(const Interval& interval, int divisions);

    // components
    const Interval& interval() const {return m_interval;}
    int divisions() const {return m_divisions;}
    double step() const {return m_step;}

    // functions
    Interval interval(int n) const;
    double point(int n) const {return m_interval.min() + n*m_step;}
    QVector<double> points() const { return m_interval.subdivide(m_divisions);}
    QVector<double> centers() const { return m_interval.subdivide(m_divisions, true);}

    int index(double x) const;

    // conversions
    operator QString() const;

private:
    Interval m_interval;
    int m_divisions;
    double m_step;
};



// stream
QTextStream& operator<<(QTextStream& out, const Grid& grid);



} // namespace cyi

#endif // GRID_H
