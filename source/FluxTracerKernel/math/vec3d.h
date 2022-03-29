#ifndef VEC3D_H
#define VEC3D_H

#include <immintrin.h> // https://software.intel.com/sites/landingpage/IntrinsicsGuide/

#include <QVector3D>
#include <QString>
#include "math.h"
#include "vec3i.h"

namespace cyi {



#ifdef USE_AVX

class vec3d
{
public:
    // data
    union  {
        __m256d mv;
        struct {double mx, my, mz, mw;};
        struct {double mr, mg, mb, ma;};
    };

public:
    // constructors
    vec3d() {}
    vec3d(double x): mv(_mm256_setr_pd(x, x, x, 0.)) {}
    vec3d(double x, double y, double z): mv(_mm256_setr_pd(x, y, z, 0.)) {}
    vec3d(const vec3i& v): mv(_mm256_setr_pd(v.mx, v.my, v.mz, 0.)) {}
    vec3d(const QVector3D& v): mv(_mm256_setr_pd(double(v.x()), double(v.y()), double(v.z()), 0.)) {}
    vec3d(const __m256d& v): mv(v) {}

    // constants
    static const vec3f Zero;
    static const vec3d Null;
    static const vec3d UnitX;
    static const vec3d UnitY;
    static const vec3d UnitZ;
    static const vec3f PosInf;
    static const vec3f NegInf;

    // components
    double x() const {return mx;}
    double y() const {return my;}
    double z() const {return mz;}

    double& x() {return mx;}
    double& y() {return my;}
    double& z() {return mz;}

    double operator()(int n) const {return (&mx)[n];}
    double& operator()(int n) {return (&mx)[n];}

    double minComponent() const;
    double maxComponent() const;

    // assignments
    vec3d& operator+=(const vec3d& v) {mv = _mm256_add_pd(mv, v.mv); return *this;}
    vec3d& operator-=(const vec3d& v) {mv = _mm256_sub_pd(mv, v.mv); return *this;}
    vec3d& operator*=(double f) {mv = _mm256_mul_pd(mv, _mm256_set1_pd(f)); return *this;}
    vec3d& operator/=(double f) {mv = _mm256_div_pd(mv, _mm256_set1_pd(f)); return *this;}

    // comparisons
    bool operator==(const vec3d& v) const {return _mm256_movemask_pd(_mm256_cmp_pd(mv, v.mv, _CMP_EQ_OQ)) == 0xf;}
    bool operator<=(const vec3d& v) const {return _mm256_movemask_pd(_mm256_cmp_pd(mv, v.mv, _CMP_LE_OQ)) == 0xf;}
    // todo last component can be nonzero
    bool equal(const vec3d& v, double eps = 1e-10) const;

    // functions
    double norm() const;
    double norm2() const;
    vec3d normalized() const;
    void normalize() {*this /= norm();}

    vec3d abs() const {return _mm256_and_pd(mv, _mm256_castsi256_pd(_mm256_set1_epi64x(0x7fffffffffffffff)));}
    vec3d floor() const {return _mm256_floor_pd(mv);}
    vec3d ceil() const {return _mm256_ceil_pd(mv);}

    // conversions
    operator __m256d() const {return mv;}
    operator QVector3D() const {return QVector3D(float(mx), float(my), float(mz));}
    operator QString() const {return QString("vec3d(%1, %2, %3)").arg(mx).arg(my).arg(mz);}
    operator vec3i() const {return vec3i(int(mx), int(my), int(mz));}
};



// global
inline vec3d operator+(const vec3d& a, const vec3d& b) {return _mm256_add_pd(a, b);}
inline vec3d operator-(const vec3d& a, const vec3d& b) {return _mm256_sub_pd(a, b);}
inline vec3d operator-(const vec3d& v) {return _mm256_sub_pd(_mm256_set1_pd(0.), v);}

inline vec3d operator*(const vec3d& a, const vec3d& b) {return _mm256_mul_pd(a, b);}
inline vec3d operator*(const vec3d& v, double f) {return _mm256_mul_pd(v, _mm256_set1_pd(f));}
inline vec3d operator*(double f, const vec3d& v) {return _mm256_mul_pd(v, _mm256_set1_pd(f));}

inline vec3d operator/(const vec3d& a, const vec3d& b) {return _mm256_div_pd(a, b);}
inline vec3d operator/(const vec3d& v, double f) {return _mm256_div_pd(v, _mm256_set1_pd(f));}
inline vec3d operator/(double f, const vec3d& v) {return _mm256_div_pd(_mm256_set1_pd(f), v);}

// dot product
double dot(const vec3d& a, const vec3d& b);
// cross product
vec3d cross(const vec3d& a, const vec3d& b);
// triple (mixed) product
inline double triple(const vec3d& a, const vec3d& b, const vec3d& c) {return dot(a, cross(b, c));}

// minimal components
inline vec3d min(const vec3d& a, const vec3d& b) {return _mm256_min_pd(a, b);}
// maximal components
inline vec3d max(const vec3d& a, const vec3d& b) {return _mm256_max_pd(a, b);}
// clamp
inline vec3d clamp(const vec3d& v, const vec3d& min, const vec3d& max) {return _mm256_min_pd(_mm256_max_pd(v, min), max);}

// depended
inline double vec3d::norm() const {return std::sqrt(dot(*this, *this));}
inline double vec3d::norm2() const {return dot(*this, *this);}
inline vec3d vec3d::normalized() const {return *this/norm();}

#else

class vec3d
{
public:
    // data
    union  {
        struct {double mx, my, mz;};
        struct {double mr, mg, mb;};
    };
public:
    // constructors
    vec3d() {}
    vec3d(double x): mx(x), my(0.), mz(0.) {}
    vec3d(double x, double y, double z): mx(x), my(y), mz(z) {}

    // constants
    static const vec3d Zero;
    static const vec3d Null;
    static const vec3d UnitX;
    static const vec3d UnitY;
    static const vec3d UnitZ;
    static const vec3d PosInf;
    static const vec3d NegInf;

    // components
    double x() const {return mx;}
    double y() const {return my;}
    double z() const {return mz;}

    double& x() {return mx;}
    double& y() {return my;}
    double& z() {return mz;}

    double operator()(int n) const {return (&mx)[n];}
    double& operator()(int n) {return (&mx)[n];}
};

#endif



// stream
QTextStream& operator<<(QTextStream& out, const vec3d& v);
QTextStream& operator>>(QTextStream& in, vec3d& v);



} // namespace cyi

#endif
