
#ifdef PLATFORM_WIN32
#include <Windows.h>
#elif defined(PLATFORM_IOS)
#include <libkern/OSAtomic.h>
#endif // PLATFORM_WIN32

#include "../include/atom.h"

namespace AXP {
    Int32 CDECL CInterlocked::Increment(Int32 VOLATILE * lpAddend)
    {
#ifdef PLATFORM_WIN32
        return ::InterlockedIncrement((Long VOLATILE *)lpAddend);
#elif defined(PLATFORM_IOS)
        return OSAtomicAdd32Barrier(1, lpAddend);
#elif defined(PLATFORM_LINUX)
        return __sync_add_and_fetch(lpAddend, 1);
#endif // PLATFORM_WIN32
    }

    Int32 CDECL CInterlocked::Decrement(Int32 VOLATILE * lpAddend)
    {
#ifdef PLATFORM_WIN32
        return ::InterlockedDecrement((Long VOLATILE *)lpAddend);
#elif defined(PLATFORM_IOS)
        return OSAtomicAdd32Barrier(-1, lpAddend);
#elif defined(PLATFORM_LINUX)
        return __sync_sub_and_fetch(lpAddend, 1);
#endif // PLATFORM_WIN32
    }

#ifdef PLATFORM_WIN32
    Int32 CDECL CInterlocked::Exchange(Int32 VOLATILE * target, Int32 value)
    {
        return ::InterlockedExchange((Long VOLATILE *)target, value);
    }
#endif // PLATFORM_WIN32
}