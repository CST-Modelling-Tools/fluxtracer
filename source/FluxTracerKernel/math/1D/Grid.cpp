#include "Grid.h"

#include "math/math.h"

using namespace cyi;



Grid::Grid():
    m_interval{Interval(-1., 1.)},
    m_divisions(1)
{

}

Grid::Grid(const Interval& interval, int divisions):
    m_interval(interval),
    m_divisions(divisions)
{
    Q_ASSERT(divisions > 0);
    m_step = (interval.max() - interval.min())/divisions;
}

Interval Grid::interval(int n) const
{
    double temp = m_interval.min() + n*m_step;
    return Interval(temp, temp + m_step);
}

int Grid::index(double x) const
{
    int n = std::floor((x - m_interval.min())/m_step);
    n = clamp(n, 0, m_divisions - 1);
    return n;
}

Grid::operator QString() const
{
    return QString("Grid(%1, %2)").arg(QString(m_interval)).arg(m_divisions);
}

QTextStream& cyi::operator<<(QTextStream& out, const Grid& grid)
{
    out << QString(grid);
    return out;
}
