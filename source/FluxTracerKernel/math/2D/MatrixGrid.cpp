#include "MatrixGrid.h"

#include "common/qiostream.h"

using namespace cyi;



MatrixGrid::MatrixGrid(const QVector<double>& xPoints, const QVector<double>& yPoints):
    Matrix<double>(xPoints.size(), yPoints.size()),
    m_xPoints(xPoints),
    m_yPoints(yPoints)
{

}

MatrixGrid& MatrixGrid::operator=(const Matrix<double>& other)
{
    m_rows = other.rows();
    m_cols = other.cols();
    m_data = other.data();
    return *this;
}

MatrixGrid& MatrixGrid::operator=(const MatrixGrid& other)
{
    m_rows = other.m_rows;
    m_cols = other.m_cols;
    m_data = other.m_data;
    m_xPoints = other.m_xPoints;
    m_yPoints = other.m_yPoints;
    return *this;
}

bool MatrixGrid::save(const QString& fileName) const
{
    // save matrix
    try {
        // open file
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
            throw "File cannot be opened";
        QTextStream out(&file);

        // write x grid
        for (int n = 0; n < m_xPoints.size(); ++n) {
            out << m_xPoints[n];
            if (n < m_xPoints.size() - 1)
                out << ",";
            else
                out << endl;
        }

        // write y grid
        for (int m = 0; m < m_yPoints.size(); ++m) {
            out << m_yPoints[m];
            if (m < m_yPoints.size() - 1)
                out << ",";
            else
                out << endl;
        }

        // write energy
        for (int n = 0; n < m_rows; ++n) {
            for (int m = 0; m < m_cols; ++m) {
                out << (*this)(n, m);
                if (m < m_cols - 1)
                    out << ",";
                else
                    out << endl;
            }
        }
    }
    catch (const char* msg) {
        cout << QString("Error in writing\n%1\n%2").arg(fileName).arg(msg);
        return false;
    }
    return true;
}
