
#ifndef __DEFINE_H__
#define __DEFINE_H__

#ifdef COMPILER_GCC
#ifdef ARCH_ARM
#include <assert.h>
#endif // ARCH_ARM
#endif // COMPILER_GCC

#include "type.h"

#ifndef SIGNALED
#define SIGNALED            (1)
#endif

#ifndef UNSIGNALED
#define UNSIGNALED          (0)
#endif

#ifndef QUITSIGNAL
#define QUITSIGNAL          (2)
#endif

#ifndef COMPLIER_ERROR
#define COMPLIER_ERROR()    compiler version error!
#endif

#ifndef KPANIC
#define KPANIC(string)      do {} while (0)
#endif

#ifndef KASSERT
#define KASSERT(string)     do { if (!(string)) ASM("int $3;"); } while (0)
#endif

#ifndef KPRINTF
#define KPRINTF(string)     do {} while (0)
#endif

#ifndef ASSERT
#ifdef COMPILER_GCC
#ifdef ARCH_X86
#define ASSERT(string)      do { if (!(string)) ASM("int $3;"); } while (0)
#else
#define ASSERT(string)      assert(string)
#endif // ARCH_X86
#else
#define ASSERT(string)      do { if (!(string)) __debugbreak(); } while (0)
#endif // COMPILER_GCC
#endif

namespace AXP 
{
#define MAKE_BITS_8(n)      ((UInt8)1u << (n))
#define MAKE_BITS_16(n)     ((UInt16)1u << (n))
#define MAKE_BITS_32(n)     (1ul << (n))

#define HINIBBLE(u)         ((UInt8)(u) >> 4)
#define LONIBBLE(u)         ((UInt8)(u) & 0xF)

#ifndef HIBYTE
#define HIBYTE(u)           ((UInt8)((UInt16)(u) >> 8))
#endif

#ifndef LOBYTE
#define LOBYTE(u)           ((UInt8)((UInt16)(u) & 0xFF))
#endif

#ifndef HIWORD
#define HIWORD(u)           ((UInt16)((UInt32)(u) >> 16))
#endif

#ifndef LOWORD
#define LOWORD(u)           ((UInt16)((UInt32)(u) & 0xFFFF))
#endif

#ifndef HIDWORD
#define HIDWORD(u)          ((UInt32)((UInt64)(u) >> 32))
#endif

#ifndef LODWORD
#define LODWORD(u)          ((UInt32)((UInt64)(u) & 0xFFFFFFFF))
#endif

#ifndef MAKEWORD
#define MAKEWORD(u8l, u8h) \
    ((UInt16)((UInt8)(u8l) | ((UInt16)(UInt8)(u8h) << 8)))
#endif

#ifndef MAKEDWORD
#define MAKEDWORD(u16l, u16h) \
    ((UInt32)((UInt16)(u16l) | ((UInt32)(UInt16)(u16h) << 16)))
#endif

#define PAGE_TO_BYTE(p)     ((p) << PAGE_SHIFT)
#define BYTE_TO_PAGE(b)     ((b) >> PAGE_SHIFT)

#ifndef MAX
#define MAX(a, b)           (((a) > (b))? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b)           (((a) < (b))? (a) : (b))
#endif

#define IS_VALUE_IN_REGION(value, min, max) \
    (((value) >= min) && ((value) <= max))

#define SET_VALUE_IN_REGION(value, min, max) \
    do { \
        if ((value) < (min)) (value) = (min); \
        if ((max) >= (min)) if ((value) > (max)) (value) = (max); \
    } while (0)

#ifdef __cplusplus

    template <class T>
    INLINE CONST T & Max(const T &a, const T &b)
    {
        if (a > b)
            return a;
        else
            return b;
    }

    template <class T>
    INLINE CONST T & Min(const T &a, const T &b)
    {
        if (a < b)
            return a;
        else
            return b;
    }

    template <class T>
    INLINE Void Swap(T &a, T &b)
    {
        T temp = a;
        a = b;
        b = temp;
    }

    template <class T>
    INLINE T RoundUp(T n, UInt32 size)
    {
        return ((n + size - 1) / size) * size;
    }

    template <class T>
    INLINE T RoundUp2(T n, UInt32 size)
    {
        return (n + size - 1) & ~(size - 1);
    }

    template <class T>
    INLINE T RoundDown(T n, UInt32 size)
    {
        return (n / size) * size;
    }

    template <class T>
    INLINE T RoundDown2(T n, UInt32 size)
    {
        return n & ~(size - 1);
    }

    template <class T>
    INLINE Boolean IsAlignment(T n, UInt32 size)
    {
        return 0 == ((UInt32)n % size);
    }

    template <class T>
    INLINE Boolean IsAlignment2(T n, UInt32 size)
    {
        return 0 == ((UInt32)n & (size - 1));
    }

#endif // __cplusplus

#ifndef ABS
#define ABS(n) ((((Int32)(n)) > 0)? (n): -(n))
#endif

    INLINE UInt8 BCDToBinary(
        IN UInt8 u8BCD)
    {
        return (u8BCD >> 4) * 10 + (u8BCD & 0xF);
    }

    INLINE UInt8 BinaryToBCD(
        IN UInt8 u8Binary)
    {
        return ((u8Binary / 10) << 4) + (u8Binary % 10);
    }
} // namespace AXP

#endif // __DEFINE_H__
