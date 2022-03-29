#ifndef MATRIXGRID_H
#define MATRIXGRID_H

#include "Matrix.h"

namespace cyi {



class MatrixGrid: public Matrix<double>
{
public:
    // constructors
    MatrixGrid(const QVector<double>& xPoints, const QVector<double>& yPoints);

    // components
    const QVector<double>& xPoints() const {return m_xPoints;}
    const QVector<double>& yPoints() const {return m_yPoints;}

    MatrixGrid& operator=(const Matrix<double>& other);
    MatrixGrid& operator=(const MatrixGrid& other);

    // export
    bool save(const QString& fileName) const;

protected:
    QVector<double> m_xPoints;
    QVector<double> m_yPoints;
};



} // namespace cyi

#endif // MATRIXGRID_H
