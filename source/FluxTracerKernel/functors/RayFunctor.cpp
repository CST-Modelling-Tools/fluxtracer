#include "RayFunctor.h"

#include <stdlib.h>
#include <malloc.h>

using namespace cyi;



void* RayFunctor::operator new(size_t s)
{
    #if defined(Q_OS_WIN)
        return _aligned_malloc(s, 32);
    #elif defined(Q_OS_LINUX)
        return memalign(32, s); // aligned_alloc C++11
    #endif
}

void RayFunctor::operator delete(void* p)
{
    #if defined(Q_OS_WIN)
        if (p) _aligned_free(p);
    #elif defined(Q_OS_LINUX)
        if (p) free(p);
    #endif
}
