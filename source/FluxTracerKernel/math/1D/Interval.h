#ifndef INTERVAL_H
#define INTERVAL_H

#include "common/qiostream.h"
#include <QVector>

namespace cyi {


/**
 * @brief Interval represernts an interval between two points
 */
class Interval
{
public:
    // constructors
    Interval() {}
    Interval(double min, double max);
    void set(double min, double max);

    // components
    double min() const {return m_min;}
    double max() const {return m_max;}
    double extent() const {return m_max - m_min;}

    // functions
    bool isInside(double x) const;
    QVector<double> subdivide(int divs, bool centers = false) const;

    // conversions
    operator QString() const;

private:
    double m_min;
    double m_max;
};



// stream
QTextStream& operator<<(QTextStream& out, const Interval& interval);



} // namespace cyi

#endif // INTERVAL_H
