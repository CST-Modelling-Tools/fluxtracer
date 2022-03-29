#ifndef MATRIX_H
#define MATRIX_H

#include <QVector>

namespace cyi {



/**
 * Matrix stores a matrix
 * Essentially, it is a QVector class with a modified indexing
 * The matrix indices starts from 0
 *
 * Usage:
 * Matrix<double> m(3, 5);
 * m.fill(0.);
 * m(2, 3) = 1.;
 */

template <class T>
class Matrix
{
public:
    // constructors
    Matrix() {}
    Matrix(int rows, int cols);

    // components
    int rows() const {return m_rows;}
    int cols() const {return m_cols;}
    void setDimensions(int rows, int cols);

    QVector<T>& data() {return m_data;}
    const QVector<T>& data() const {return m_data;}
    void setData(const QVector<T>& data) {m_data = data;}

    inline int index(int n, int m) const {return n*m_cols + m;}
    T& operator()(int n, int m) {return m_data[index(n, m)];}
    const T& operator()(int n, int m) const {return m_data[index(n, m)];}

    // functions
    void fill(const T& value) {m_data.fill(value);}

    Matrix<T>& operator+=(const Matrix<T>& other);
    bool operator==(const Matrix<T>& other) const;

protected:
    int m_rows;
    int m_cols;
    QVector<T> m_data;
};



template<class T>
Matrix<T>::Matrix(int rows, int cols)
{
    setDimensions(rows, cols);
}

template<class T>
void Matrix<T>::setDimensions(int rows, int cols)
{
    Q_ASSERT(rows > 0);
    Q_ASSERT(cols > 0);
    m_rows = rows;
    m_cols = cols;
    m_data.resize(rows*cols);
    m_data.fill(0.); // not always
}

template<class T>
Matrix<T>& Matrix<T>::operator+=(const Matrix<T>& other)
{
    Q_ASSERT(m_rows == other.m_rows);
    Q_ASSERT(m_cols == other.m_cols);
    for (int n = 0; n < m_data.size(); ++n)
        m_data[n] += other.m_data[n];
    return *this;
}

template<class T>
bool Matrix<T>::operator==(const Matrix<T>& other) const
{
    if (m_rows != other.m_rows) return false;
    if (m_cols != other.m_cols) return false;
    if (m_data != other.m_data) return false;
    return true;
}



} // namespace cyi

#endif // MATRIX_H
