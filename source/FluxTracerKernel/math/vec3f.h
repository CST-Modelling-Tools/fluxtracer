#ifndef VEC3F_H
#define VEC3F_H

#include <xmmintrin.h> // https://software.intel.com/sites/landingpage/IntrinsicsGuide/
#include <smmintrin.h> // for SSE4.1 _mm_dp_ps

#include <QVector3D>
#include <QString>
#include "math.h"
#include "vec3i.h"

namespace cyi {



class vec3f
{
public:
    // data
    union  {
        __m128 mv;
        struct {float mx, my, mz, mw;};
        struct {float mr, mg, mb, ma;};
    };

public:
    // constructors
    vec3f() {}
    vec3f(float x): mv(_mm_setr_ps(x, x, x, 0.f)) {}
    vec3f(float x, float y, float z): mv(_mm_setr_ps(x, y, z, 0.f)) {}
    vec3f(const vec3i& v): mv(_mm_setr_ps(v.mx, v.my, v.mz, 0.f)) {}
    vec3f(const QVector3D& v): mv(_mm_setr_ps(v.x(), v.y(), v.z(), 0.f)) {}
    vec3f(const __m128& v): mv(v) {}

    // constants
    static const vec3f Zero;
    static const vec3f Null;
    static const vec3f UnitX;
    static const vec3f UnitY;
    static const vec3f UnitZ;
    static const vec3f PosInf;
    static const vec3f NegInf;

    // components
    float x() const {return mx;}
    float y() const {return my;}
    float z() const {return mz;}

    float& x() {return mx;}
    float& y() {return my;}
    float& z() {return mz;}

    float operator()(int n) const {return (&mx)[n];}
    float& operator()(int n) {return (&mx)[n];}

    float minComponent() const;
    float maxComponent() const;

    // assignments
    vec3f& operator+=(const vec3f& v) {mv = _mm_add_ps(mv, v.mv); return *this;}
    vec3f& operator-=(const vec3f& v) {mv = _mm_sub_ps(mv, v.mv); return *this;}
    vec3f& operator*=(float f) {mv = _mm_mul_ps(mv, _mm_set1_ps(f)); return *this;}
    vec3f& operator/=(float f) {mv = _mm_div_ps(mv, _mm_set1_ps(f)); return *this;}

    // comparisons
    bool operator==(const vec3f& v) const {return _mm_movemask_ps(_mm_cmpeq_ps(mv, v.mv)) == 0xf;}
    bool operator<=(const vec3f& v) const {return _mm_movemask_ps(_mm_cmple_ps(mv, v.mv)) == 0xf;}
    // todo last component can be nonzero
    bool equal(const vec3f& v, float eps = 1e-10f) const;

    // functions
    float norm() const {return _mm_cvtss_f32(_mm_sqrt_ss(_mm_dp_ps(mv, mv, 0x71)));}
    float norm2() const {return _mm_cvtss_f32(_mm_dp_ps(mv, mv, 0x71));}
    vec3f normalized() const;
    void normalize() {*this /= norm();}

    vec3f abs() const {return _mm_and_ps(mv, _mm_castsi128_ps(_mm_set1_epi32(0x7fffffff)));}
    vec3f floor() const {return _mm_floor_ps(mv);}
    vec3f ceil() const {return _mm_ceil_ps(mv);}

    // conversions
    operator __m128() const {return mv;}
    operator QVector3D() const {return QVector3D(mx, my, mz);}
    operator QString() const {return QString("vec3f(%1, %2, %3)").arg(mx).arg(my).arg(mz);}
    operator vec3i() const {return vec3i(int(mx), int(my), int(mz));}
};



// global
inline vec3f operator+(const vec3f& a, const vec3f& b) {return _mm_add_ps(a, b);}
inline vec3f operator-(const vec3f& a, const vec3f& b) {return _mm_sub_ps(a, b);}
inline vec3f operator-(const vec3f& v) {return _mm_xor_ps(v, _mm_castsi128_ps(_mm_set1_epi32(0x80000000)));}

inline vec3f operator*(const vec3f& a, const vec3f& b) {return _mm_mul_ps(a, b);}
inline vec3f operator*(const vec3f& v, float f) {return _mm_mul_ps(v, _mm_set1_ps(f));}
inline vec3f operator*(float f, const vec3f& v) {return _mm_mul_ps(v, _mm_set1_ps(f));}

inline vec3f operator/(const vec3f& a, const vec3f& b) {return _mm_div_ps(a, b);}
inline vec3f operator/(const vec3f& v, float f) {return _mm_div_ps(v, _mm_set1_ps(f));}
inline vec3f operator/(float f, const vec3f& v) {return _mm_div_ps(_mm_set1_ps(f), v);}

// dot product
inline float dot(const vec3f& a, const vec3f& b) {return _mm_cvtss_f32(_mm_dp_ps(a, b, 0x71));}
// cross product
vec3f cross(const vec3f& a, const vec3f& b);
// triple (mixed) product
inline float triple(const vec3f& a, const vec3f& b, const vec3f& c) {return dot(a, cross(b, c));}

// minimal components
inline vec3f min(const vec3f& a, const vec3f& b) {return _mm_min_ps(a, b);}
// maximal components
inline vec3f max(const vec3f& a, const vec3f& b) {return _mm_max_ps(a, b);}
// clamp
inline vec3f clamp(const vec3f& v, const vec3f& min, const vec3f& max) {return _mm_min_ps(_mm_max_ps(v, min), max);}

// depended
inline vec3f vec3f::normalized() const {return *this/norm();}

// stream
QTextStream& operator<<(QTextStream& out, const vec3f& v);
QTextStream& operator>>(QTextStream& in, vec3f& v);


} // namespace cyi

#endif // VEC3F_H
