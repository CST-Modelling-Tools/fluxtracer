#ifndef TENSORX_H
#define TENSORX_H

#include <QVector>

namespace cyi {



/**
 * TensorX stores tensors with arbitrary number of dimensions
 * Essentially, it is a QVector class with a modified indexing
 *
 * Usage:
 * TensorX<double> tensor({3, 5});
 * tensor.fill(0.);
 * tensor({1, 2}) = 1.;
 */

template <class T>
class TensorX
{
public:
    // constructors
    TensorX(const QVector<int>& dimensions);

    // components
    const QVector<int>& dimensions() const {return m_dimensions;}

    int index(const QVector<int>& indices) const;
    T& operator()(const QVector<int>& indices) {return m_data[index(indices)];}
    const T& operator()(const QVector<int>& indices) const {return m_data[index(indices)];}

    QVector<T>& data() {return m_data;}
    const QVector<T>& data() const {return m_data;}

    // functions
    void fill(const T& value) {m_data.fill(value);}

protected:
    QVector<int> m_dimensions;
    QVector<int> m_strides;
    QVector<T> m_data;
};



template<class T>
TensorX<T>::TensorX(const QVector<int>& dimensions) {
    int dMax = dimensions.size();
    Q_ASSERT(dMax > 0);
    m_dimensions = dimensions;
    m_strides.resize(dMax);

    int size = 1;
    for (int d = dMax - 1; d >= 0; d--) {
        int nMax = m_dimensions[d];
        Q_ASSERT(nMax > 0);
        m_strides[d] = size;
        size *= nMax;
    }
    m_data.resize(size);
}

template<class T>
int TensorX<T>::index(const QVector<int>& indices) const
{
    int index = 0;
    for (int d = 0; d < m_dimensions.size(); d++)
        index += m_strides[d]*indices[d];
    return index;
}



} // namespace cyi

#endif // TENSORX_H
