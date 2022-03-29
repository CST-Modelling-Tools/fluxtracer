#include "Interval.h"

using namespace cyi;



Interval::Interval(double min, double max)
{
    set(min, max);
}

void Interval::set(double min, double max)
{
    Q_ASSERT(min < max);
    m_min = min;
    m_max = max;
}

bool Interval::isInside(double x) const
{
    return m_min <= x && x <= m_max;
}

QVector<double> Interval::subdivide(int divs, bool centers) const
{
    Q_ASSERT(divs > 0);
    QVector<double> ans;

    ans << m_min;
    if (divs > 1) {
        double step = (m_max - m_min)/divs;
        for (int n = 1; n < divs; ++n)
            ans << m_min + n*step;
    }
    if (m_min != m_max) ans << m_max;

    if (centers) {
        for (int n = 0; n < divs; ++n)
            ans[n] = (ans[n] + ans[n + 1])/2.;
        ans.removeLast();
    }

    return ans;
}

Interval::operator QString() const
{
    return QString("Interval(%1, %2)").arg(m_min).arg(m_max);
}

QTextStream& cyi::operator<<(QTextStream& out, const Interval& interval)
{
    out << QString(interval);
    return out;
}
