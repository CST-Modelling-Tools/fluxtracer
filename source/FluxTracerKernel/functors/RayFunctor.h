#ifndef RAYFUNCTOR_H
#define RAYFUNCTOR_H

#include "math/3D/Ray.h"

namespace cyi {


/**
 * @brief RayFunctor provides an interface for post-processing of rays
 */
class RayFunctor
{
public:
    // constructors
    RayFunctor() {}
    virtual RayFunctor* copy() const = 0;
//    VIRTUAL_COPY(RayFunctor)
    virtual ~RayFunctor() {}

    // calculations
    virtual void process(const Ray& ray) = 0;
    virtual void combine(const RayFunctor& rf) = 0;

    // export
    virtual bool save(const QString& fileName) const = 0;

    // alinged allocation of dynamic memory
    void* operator new(size_t s);
    void operator delete(void* p);
};



} // namespace cyi

#endif // RAYFUNCTOR_H
