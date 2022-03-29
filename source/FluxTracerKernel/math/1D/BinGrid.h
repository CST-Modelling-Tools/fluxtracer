#ifndef BINGRID_H
#define BINGRID_H

#include "Grid.h"
#include <QVector>

namespace cyi {


/**
 * @brief BinGrid considers 1d grid as a set of bins
 *
 * The bins at the ends are extended to infinity
 */
class BinGrid
{
public:
    // constructors
    BinGrid() {}
    BinGrid(int) {} // fix
    BinGrid(const Grid& grid);
    void set(const Grid& grid);

    // components
    const Grid& grid() const {return m_grid;}
    int size() const {return m_grid.divisions();}

    double& operator()(double x) {return m_bins[m_grid.index(x)];}
    double operator()(double x) const {return m_bins[m_grid.index(x)];}

    void addKahan(double x, double value);

    const QVector<double>& bins() const {return m_bins;}
    double bin(int n) const {return m_bins[n];}

    // functions
    void integrate();
    void integrateKahan();
    double fraction(double eta) const;
    void reset();

    BinGrid& operator+=(const BinGrid& other);

    // export
    bool save(const QString& fileName) const;

protected:
    Grid m_grid;
    QVector<double> m_bins;
    QVector<double> m_corrections;
};



} // namespace cyi

#endif // BINGRID_H

