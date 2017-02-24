
#ifndef __HASH_FUNCTION_H__
#define __HASH_FUNCTION_H__

#include "../define.h"

namespace AXP {
    INLINE UInt32 HashAString(CONST Char * __s)
    {
        UInt32 hash = 0;
        for (; *__s; ++__s)
            hash = 5 * hash + *__s;

        return (hash & 0x7FFFFFFF);
    }

    INLINE UInt32 HashString(PCWStr str)
    {
        UInt32 seed = 131; // 31 131 1313 13131 131313 etc..
        UInt32 hash = 0;

        while (*str) {
            hash = hash * seed + (*str++);
        }

        return (hash & 0x7FFFFFFF);
    }

    INLINE UInt32 HashWChar(WChar str)
    {
        Byte l = LOBYTE(str);
        UInt32  h = HIBYTE(str);

        UInt32 hash = h << 8;
        hash += l;

        return (hash & 0x7FFFFFFF);
    }

    INLINE UInt32 HashFloat(CONST PVoid __ptr, UInt32 __clength)
    {
        UInt32 hash = 0;
        CONST Char * __cptr = (CONST Char*)__ptr;
        for (; __clength; --__clength)
            hash = 5 * hash + (UInt32)(*__cptr++);

        return (hash & 0x7FFFFFFF);
    }

    template <class _Key>
    struct Hash
    {
        UInt32 operator()(CONST _Key __x) CONST{ return (UInt32)__x; }
    };

    template <>
    struct Hash<WChar>
    {
        UInt32 operator()(CONST WChar __x) CONST{ return HashWChar(__x); }
    };

    template <>
    struct Hash<Float>
    {
        UInt32 operator()(CONST Float __x) CONST
        { return __x != 0.0f ? HashFloat((PVoid)&__x, sizeof(__x)) : 0; }
    };

    template <>
    struct Hash<Double>
    {
        UInt32 operator()(CONST Double __x) CONST
        { return __x != 0.0f ? HashFloat((PVoid)&__x, sizeof(__x)) : 0; }
    };

    template <>
    struct Hash<PChar>
    {
        UInt32 operator()(PCStr __s) CONST { return HashAString(__s); }
    };

    template <>
    struct Hash<PCStr>
    {
        UInt32 operator()(PCStr __s) CONST { return HashAString(__s); }
    };

    template <>
    struct Hash<PWStr>
    {
        UInt32 operator()(PCWStr __s) CONST { return HashString(__s); }
    };

    template <>
    struct Hash<PCWStr>
    {
        UInt32 operator()(PCWStr __s) CONST { return HashString(__s); }
    };
} // namespace AXP

#endif // __HASH_FUNCTION_H__