#include "BinGrid.h"

#include "common/qiostream.h"
#include "math/math.h"

using namespace cyi;



BinGrid::BinGrid(const Grid& grid)
{
    set(grid);
}

void BinGrid::set(const Grid& grid)
{
    m_grid = grid;
    int divs = grid.divisions();

    m_bins.resize(divs);
    m_bins.fill(0.);

    m_corrections.resize(divs);
    m_corrections.fill(0.);
}

void BinGrid::addKahan(double x, double value)
{
    int n = m_grid.index(x);
    double& correction = m_corrections[n];
    double& sum = m_bins[n];

    double term = value - correction;
    double temp = sum + term;
    correction = (temp - sum) - term;
    sum = temp;
}

void BinGrid::integrate()
{
    for (int n = 1; n < m_bins.size(); ++n)
        m_bins[n] += m_bins[n - 1];
}

void BinGrid::integrateKahan()
{
    double correction = 0.;
    double sum = 0.;
    for (int n = 0; n < m_bins.size(); ++n) {
        double value = m_bins[n];
        double term = value - correction;
        double temp = sum + term;
        correction = (temp - sum) - term;
        sum = temp;
        m_bins[n] = sum;
    }
}

double BinGrid::fraction(double eta) const
{
    Q_ASSERT(eta >= 0 && eta <= 1.);
    BinGrid temp(*this);
    temp.integrateKahan();
    double fMax = temp.bins().last();
    double f = eta*fMax;
    double x = 0;

    double fPrev = 0.;
    for (int n = 0; n < m_bins.size(); ++n) {
        double fNext = temp.bin(n);
        if (f <= fNext) {
            Interval interval = m_grid.interval(n);
            x = interval.min();
            double q = fNext - fPrev;
            if (!eqz(q))
                x += (f - fPrev)/q*interval.extent();
            break;
        }
        fPrev = fNext;
    }
    return x;
}

void BinGrid::reset()
{
    m_bins.fill(0.);
}

BinGrid& BinGrid::operator+=(const BinGrid& other)
{
    Q_ASSERT(m_bins.size() == other.m_bins.size());
    for (int n = 0; n < m_bins.size(); ++n)
        m_bins[n] += other.m_bins[n];
    return *this;
}

bool BinGrid::save(const QString& fileName) const
{
    try {
        // open file
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
            throw "File cannot be opened";
        QTextStream out(&file);

        // write data
        for (int n = 0; n < m_bins.size(); ++n) {
            Interval interval = m_grid.interval(n);
            out << QString("%1, %2, %3\n").
                   arg(interval.min()).
                   arg(interval.max()).
                   arg(m_bins[n]);
        }
    }
    catch (const char* msg) {
        cout << QString("Error in writing\n%1\n%2").arg(fileName).arg(msg);
        return false;
    }
    return true;
}
