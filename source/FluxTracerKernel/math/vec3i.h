#ifndef VEC3I_H
#define VEC3I_H

#include <emmintrin.h> // https://software.intel.com/sites/landingpage/IntrinsicsGuide/
#include "common/qiostream.h"

namespace cyi {



class vec3i
{
public:
    // data
    union  {
        __m128i mv;
        struct {int mx, my, mz, mw;};
    };

public:
    // constructors
    vec3i() {}
    vec3i(int x): mv(_mm_setr_epi32(x, x, x, 0)) {}
    vec3i(int x, int y, int z): mv(_mm_setr_epi32(x, y, z, 0)) {}
    vec3i(const __m128i& v): mv(v) {}

    // components
    int x() const {return mx;}
    int y() const {return my;}
    int z() const {return mz;}

    int& x() {return mx;}
    int& y() {return my;}
    int& z() {return mz;}

    int operator()(int n) const {return (&mx)[n];}
    int& operator()(int n) {return (&mx)[n];}

    // comparisons
    bool operator==(const vec3i& v) const {return mx == v.mx && my == v.my && mz == v.mz;}
    bool operator!=(const vec3i& v) const {return mx != v.mx || my != v.my || mz != v.mz;}

    bool operator>(const vec3i& v) const {return (_mm_movemask_epi8(_mm_cmpgt_epi32(mv, v.mv)) & 0x0fff) == 0x0fff;}
    bool operator<=(const vec3i& v) const {return (_mm_movemask_epi8(_mm_cmpgt_epi32(mv, v.mv)) & 0x0fff) == 0x0000;}
    bool operator<(const vec3i& v) const {return (_mm_movemask_epi8(_mm_cmplt_epi32(mv, v.mv)) & 0x0fff) == 0x0fff;}
    bool operator>=(const vec3i& v) const {return (_mm_movemask_epi8(_mm_cmplt_epi32(mv, v.mv)) & 0x0fff) == 0x0000;}

    // conversions
    operator __m128i() const {return mv;}
};



// global
inline vec3i operator+(const vec3i& a, const vec3i& b) {return _mm_add_epi32(a, b);}
inline vec3i operator-(const vec3i& a, const vec3i& b) {return _mm_sub_epi32(a, b);}

// dot product
int dot(const vec3i& a, const vec3i& b);

// stream
QTextStream& operator<<(QTextStream& out, const vec3i& v);
QTextStream& operator>>(QTextStream& in, vec3i& v);



} // namespace cyi

#endif // VEC3I_H
