#include "vec3i.h"

#include <smmintrin.h>

/*
Debug helpers for vec3i

C:\Qt\Qt5.11.0\Tools\QtCreator\share\qtcreator\debugger\personaltypes.py

def qdump__cyi__vec3i(d, value):
   #d.putValue("[{0}, {1}, {2}]".format(value["mx"], value["my"], value["mz"]))
   d.putNumChild(3)
   if d.isExpanded():
       with Children(d):
           d.putSubItem("mx", value["mx"])
           d.putSubItem("my", value["my"])
           d.putSubItem("mz", value["mz"])
*/

using namespace cyi;



int cyi::dot(const vec3i& a, const vec3i& b)
{
    __m128i t = _mm_mullo_epi32(a, b);
    __m128i r = _mm_add_epi32(t, _mm_shuffle_epi32(t, _MM_SHUFFLE(0, 0, 0, 1)));
    r = _mm_add_epi32(r, _mm_shuffle_epi32(t, _MM_SHUFFLE(0, 0, 0, 2)));
    return _mm_cvtsi128_si32(r);
}

QTextStream& cyi::operator<<(QTextStream& out, const vec3i& v)
{
    out << QString("vec3i(%1, %2, %3)")
           .arg(v.mx)
           .arg(v.my)
           .arg(v.mz);
    return out;
}

QTextStream& cyi::operator>>(QTextStream& in, vec3i& v)
{
    in >> v.mx;
    in >> v.my;
    in >> v.mz;
    return in;
}
