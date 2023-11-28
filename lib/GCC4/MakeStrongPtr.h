#ifndef GCC4_MAKE_STRONG_PTR_H_INCLUDED
#define GCC4_MAKE_STRONG_PTR_H_INCLUDED

#include <memory>

template<class ClassType>
static inline std::shared_ptr<ClassType>
MakeStrongPtr(std::weak_ptr<ClassType> pWeakPtr)
{
    if (!pWeakPtr.expired()) {
        return std::shared_ptr<ClassType>(pWeakPtr);
    }
    return std::shared_ptr<ClassType>();
}

#endif // GCC4_MAKE_STRONG_PTR_H_INCLUDED

