#ifndef TENSOR3_H
#define TENSOR3_H

#include <QVector>
#include "math/vec3i.h"

namespace cyi {



/**
 * Tensor3 stores tensors of rank 3
 * Essentially, it is a QVector class with a modified indexing
 * The tensor indices start from 0
 *
 * Usage:
 * Tensor3<double> tensor(3, 5, 2);
 * tensor.fill(0.);
 * tensor(1, 2, 0) = 1.;
 */

template <class T>
class Tensor3
{
public:
    // constructors
    Tensor3() {}
    Tensor3(int nx, int ny, int nz);
    Tensor3(const vec3i& n);

    // components
    const vec3i& dimensions() const {return m_dimensions;}
    void setDimensions(const vec3i& n);

    QVector<T>& data() {return m_data;}
    const QVector<T>& data() const {return m_data;}

    inline int index(int nx, int ny, int nz) const;
    T& operator()(int nx, int ny, int nz) {return m_data[index(nx, ny, nz)];}
    const T& operator()(int nx, int ny, int nz) const {return m_data[index(nx, ny, nz)];}

    inline int index(const vec3i& n) const;
    T& operator()(const vec3i& n) {return m_data[index(n)];}
    const T& operator()(const vec3i& n) const {return m_data[index(n)];}

    // functions
    void fill(const T& value) {m_data.fill(value);}

    Tensor3<T>& operator+=(const Tensor3<T>& other);
    bool operator==(const Tensor3<T>& other) const;

private:
    vec3i m_dimensions;
    vec3i m_strides;
    QVector<T> m_data;
};



template<class T>
Tensor3<T>::Tensor3(int nx, int ny, int nz)
{
    setDimensions(vec3i(nx, ny, nz));
}

template<class T>
Tensor3<T>::Tensor3(const vec3i& n)
{
    setDimensions(n);
}

template<class T>
void Tensor3<T>::setDimensions(const vec3i& n)
{
    Q_ASSERT(n.mx > 0);
    Q_ASSERT(n.my > 0);
    Q_ASSERT(n.mz > 0);

    m_dimensions = n;
//    m_strides = vec3i(n.y*n.z, n.z, 1);
    m_strides = vec3i(1, n.mx, n.mx*n.my); // for VTK
    m_data.resize(n.mx*n.my*n.mz);
    m_data.fill(0.);
}

template<class T>
inline int Tensor3<T>::index(int nx, int ny, int nz) const
{
    int index = m_strides.mx*nx + m_strides.my*ny + m_strides.mz*nz;
    return index;
}

template<class T>
inline int Tensor3<T>::index(const vec3i& n) const
{
    int index = dot(m_strides, n);
    return index;
}

template<class T>
Tensor3<T>& Tensor3<T>::operator+=(const Tensor3<T>& other)
{
    Q_ASSERT(m_dimensions == other.m_dimensions);
    for (int n = 0; n < m_data.size(); ++n)
        m_data[n] += other.m_data[n];
    return *this;
}

template<class T>
bool Tensor3<T>::operator==(const Tensor3<T>& other) const
{
    if (m_dimensions != other.m_dimensions) return false;
    if (m_data != other.m_data) return false;
    return true;
}



} // namespace cyi

#endif // TENSOR3_H
