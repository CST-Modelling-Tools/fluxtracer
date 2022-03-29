#ifndef VEC3R_H
#define VEC3R_H

#include "vec3f.h"
#include "vec3d.h"

namespace cyi {



#ifdef USE_FLOATS
    typedef float real;
    typedef vec3f vec3r;
#else
    typedef double real;
    typedef vec3d vec3r;
#endif



} // namespace cyi

#endif // VEC3R_H
