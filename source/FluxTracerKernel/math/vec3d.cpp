#include "vec3d.h"

/*
Debug helpers for vec3d

C:\Qt\Qt5.11.0\Tools\QtCreator\share\qtcreator\debugger\personaltypes.py

def qdump__cyi__vec3d(d, value):
   #d.putValue("[{0}, {1}, {2}]".format(value["mx"], value["my"], value["mz"]))
   d.putNumChild(3)
   if d.isExpanded():
       with Children(d):
           d.putSubItem("mx", value["mx"])
           d.putSubItem("my", value["my"])
           d.putSubItem("mz", value["mz"])
*/

using namespace cyi;



const vec3d vec3d::Zero(0.f);
const vec3d vec3d::Null(0.);
const vec3d vec3d::UnitX(1., 0., 0.);
const vec3d vec3d::UnitY(0., 1., 0.);
const vec3d vec3d::UnitZ(0., 0., 1.);
const vec3d vec3d::PosInf(1e300);
const vec3d vec3d::NegInf(-1e300);


#ifdef USE_AVX

double vec3d::minComponent() const {
    __m256d t = _mm256_min_pd(mv, _mm256_permute4x64_pd(mv, _MM_SHUFFLE(0, 0, 0, 1)));
    t = _mm256_min_pd(t, _mm256_permute4x64_pd(mv, _MM_SHUFFLE(0, 0, 0, 2)));
    return _mm_cvtsd_f64(_mm256_castpd256_pd128(t));
}
// todo shuffle of 01 components is faster

double vec3d::maxComponent() const {
    __m256d t = _mm256_max_pd(mv, _mm256_permute4x64_pd(mv, _MM_SHUFFLE(0, 0, 0, 1)));
    t = _mm256_max_pd(t, _mm256_permute4x64_pd(mv, _MM_SHUFFLE(0, 0, 0, 2)));
    return _mm_cvtsd_f64(_mm256_castpd256_pd128(t));
}

bool vec3d::equal(const vec3d& v, double eps) const
{
    return (v - *this).abs() <= vec3d(eps);
}

double cyi::dot(const vec3d& a, const vec3d& b)
{
    __m256d t = _mm256_mul_pd(a, b);
    __m256d r = _mm256_add_pd(t, _mm256_permute4x64_pd(t, _MM_SHUFFLE(0, 0, 0, 1)));
    r = _mm256_add_pd(r, _mm256_permute4x64_pd(t, _MM_SHUFFLE(0, 0, 0, 2)));
    return _mm_cvtsd_f64(_mm256_castpd256_pd128(r));
}

vec3d cyi::cross(const vec3d& a, const vec3d& b) {
    __m256d t = _mm256_sub_pd(
        _mm256_mul_pd(a, _mm256_permute4x64_pd(b, _MM_SHUFFLE(3, 0, 2, 1))),
        _mm256_mul_pd(b, _mm256_permute4x64_pd(a, _MM_SHUFFLE(3, 0, 2, 1)))
    );
    return _mm256_permute4x64_pd(t, _MM_SHUFFLE(3, 0, 2, 1));
}

#else

#endif

QTextStream& cyi::operator<<(QTextStream &out, const vec3d &v)
{
    out << QString("vec3d(%1, %2, %3)")
           .arg(v.mx)
           .arg(v.my)
           .arg(v.mz);
    return out;
}

QTextStream& cyi::operator>>(QTextStream& in, vec3d& v)
{
    in >> v.mx;
    in >> v.my;
    in >> v.mz;
    return in;
}


