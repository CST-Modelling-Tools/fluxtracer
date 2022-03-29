#include "vec3f.h"

/*
Debug helpers for vec3f

C:\Qt\Qt5.11.0\Tools\QtCreator\share\qtcreator\debugger\personaltypes.py

def qdump__cyi__vec3f(d, value):
   #d.putValue("[{0}, {1}, {2}]".format(value["mx"], value["my"], value["mz"]))
   d.putNumChild(3)
   if d.isExpanded():
       with Children(d):
           d.putSubItem("mx", value["mx"])
           d.putSubItem("my", value["my"])
           d.putSubItem("mz", value["mz"])
*/

using namespace cyi;



const vec3f vec3f::Zero(0.f);
const vec3f vec3f::Null(0.f);
const vec3f vec3f::UnitX(1.f, 0.f, 0.f);
const vec3f vec3f::UnitY(0.f, 1.f, 0.f);
const vec3f vec3f::UnitZ(0.f, 0.f, 1.f);
const vec3f vec3f::PosInf(1e30f);
const vec3f vec3f::NegInf(-1e30f);


float vec3f::minComponent() const {
    __m128 t = _mm_min_ss(mv, _mm_shuffle_ps(mv, mv, _MM_SHUFFLE(0, 0, 0, 1)));
    t = _mm_min_ss(t, _mm_shuffle_ps(mv, mv, _MM_SHUFFLE(0, 0, 0, 2)));
    return _mm_cvtss_f32(t);
}
// todo shuffle of 01 components is faster

float vec3f::maxComponent() const {
    __m128 t = _mm_max_ss(mv, _mm_shuffle_ps(mv, mv, _MM_SHUFFLE(0, 0, 0, 1)));
    t = _mm_max_ss(t, _mm_shuffle_ps(mv, mv, _MM_SHUFFLE(0, 0, 0, 2)));
    return _mm_cvtss_f32(t);
}

bool vec3f::equal(const vec3f& v, float eps) const
{
    return (v - *this).abs() <= vec3f(eps);
}

vec3f cyi::cross(const vec3f& a, const vec3f& b) {
    __m128 t = _mm_sub_ps(
        _mm_mul_ps(a, _mm_shuffle_ps(b, b, _MM_SHUFFLE(3, 0, 2, 1))),
        _mm_mul_ps(b, _mm_shuffle_ps(a, a, _MM_SHUFFLE(3, 0, 2, 1)))
    );
    return _mm_shuffle_ps(t, t, _MM_SHUFFLE(3, 0, 2, 1));
}

QTextStream& cyi::operator<<(QTextStream &out, const vec3f &v)
{
    out << QString("vec3f(%1, %2, %3)")
           .arg(v.mx)
           .arg(v.my)
           .arg(v.mz);
    return out;
}

QTextStream& cyi::operator>>(QTextStream& in, vec3f& v)
{
    in >> v.mx;
    in >> v.my;
    in >> v.mz;
    return in;
}
