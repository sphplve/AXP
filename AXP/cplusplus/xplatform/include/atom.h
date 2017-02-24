
#ifndef __ATOM_H__
#define __ATOM_H__

#include "type.h"

namespace AXP {
    class CInterlocked
    {
    public:

        STATIC Int32 CDECL Increment(Int32 VOLATILE * lpAddend);
        STATIC Int32 CDECL Decrement(Int32 VOLATILE * lpAddend);
#ifdef PLATFORM_WIN32
        STATIC Int32 CDECL Exchange(Int32 VOLATILE * target, Int32 value);
#endif // PLATFORM_WIN32

        CInterlocked() : mCount(0) {}

        Int32 Increment()
        {
            return Increment(&mCount);
        }

        Int32 Decrement()
        {
            return Decrement(&mCount);
        }

#ifdef PLATFORM_WIN32
        Int32 Exchange(
            IN Int32 VOLATILE lValue)
        {
            return Exchange(&mCount, lValue);
        }
#endif // PLATFORM_WIN32

    private:

        Int32 mCount;
    };
} // namespace AXP

#endif // __ATOM_H__
