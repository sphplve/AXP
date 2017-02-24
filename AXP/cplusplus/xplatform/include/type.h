
#ifndef __TYPE_H__
#define __TYPE_H__

#ifndef PLATFORM_WIN32
#include <stddef.h>
#endif // PLATFORM_WIN32

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef INOUT
#define INOUT
#endif

#if defined(COMPILER_MSVC)
#define DECLSPEC_SELECTANY   __declspec(selectany)
#define DECLSPEC_NOVTABLE    __declspec(novtable)
#define DECLSPEC_UUID(x)     __declspec(uuid(x))
#define DECL_NAKED           __declspec(naked)
#define DECL_PACKED
#define DECL_SECTION(s)
#elif defined(COMPILER_GCC)
#define DECL_REGPARM(n)      __attribute__ ((regparm(n)))
#define DECL_SECTION(s)      __attribute__ ((section(s)))
#define DECL_PACKED          __attribute__ ((packed))
#define DECL_ALIGN(n)        __attribute__ ((aligned(n)))
#define DECL_NORETURN        __attribute__ ((noreturn))
#define DECL_NAKED
#endif // defined(COMPILER_MSVC)

#ifndef CDECL
#ifdef PLATFORM_WIN32
#define CDECL                __cdecl
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
#define CDECL
#endif // PLATFORM_WIN32
#endif

#ifndef STDCALL
#ifdef PLATFORM_WIN32
#define STDCALL              __stdcall
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
#define STDCALL
#endif // PLATFORM_WIN32
#endif

#ifndef INFINITE
#define INFINITE             -1                  // Infinite timeout
#endif

#ifdef __cplusplus
#ifndef EXTERN_C
#define EXTERN_C             extern "C"
#endif

#ifndef EXTERN_C_BEGIN
#define EXTERN_C_BEGIN       EXTERN_C {
#endif

#ifndef EXTERN_C_END
#define EXTERN_C_END         }
#endif

#else

#ifndef EXTERN_C
#define EXTERN_C             extern
#endif

#ifndef EXTERN_C_BEGIN
#define EXTERN_C_BEGIN
#endif

#ifndef EXTERN_C_END
#define EXTERN_C_END
#endif

#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL                 0
#else
#define NULL                 ((void*)0)
#endif
#endif

#ifdef COMPILER_GCC
#ifndef TRUE
#define TRUE                 (1 == 1)
#endif
#ifndef FALSE
#define FALSE                (1 == 0)
#endif
#else
#define TRUE                 1
#define FALSE                0
#endif

#ifdef __cplusplus
#ifndef INLINE
#define INLINE               inline
#endif
#elif defined(COMPILER_MSVC)
#ifndef INLINE
#define INLINE               __inline
#endif
#elif defined(DIAB_COMPILER)
#ifndef INLINE
#define INLINE /* only pragmas supported, don't bother */
#endif
#else
#ifndef INLINE
#define INLINE static       __inline__           /* GNU c style */
#endif
#endif

#ifdef COMPILER_GCC
#ifndef __asm
#define __asm                __asm__
#endif

#ifndef _asm
#define _asm                 __asm
#endif

#ifndef ASM_VOLATILE
#define ASM_VOLATILE         __asm__ __volatile__
#endif

#ifndef ASM
#define ASM                  ASM_VOLATILE
#endif
#endif

#ifndef EXTERN
#define EXTERN               extern
#endif

#ifndef STATIC
#define STATIC               static
#endif

#ifndef FRIEND
#define FRIEND               friend
#endif

#ifndef VIRTUAL
#define VIRTUAL              virtual
#endif

#ifndef CONST
#define CONST                const
#endif

#ifndef VOLATILE
#define VOLATILE             volatile
#endif

#ifndef REGISTER
#define REGISTER             register
#endif

#if defined(COMPILER_GCC)
#define DECL_CODEINIT        DECL_SECTION(".text.init")
#define DECL_DATAINIT        DECL_SECTION(".data.init")
#elif defined(COMPILER_MSVC)
#define DECL_CODEINIT
#define DECL_DATAINIT
#endif

namespace AXP
{
#ifdef COMPILER_GCC
    typedef long long __int64;                       // gnu c does not know __int64
    typedef unsigned long long __uint64;
#else
    typedef unsigned __int64 __uint64;
#endif // COMPILER_GCC

    // intel 80ARCH_X86, win32, GNUC //
    typedef unsigned char        Boolean;
    typedef Boolean *            PBoolean;
    typedef unsigned char        Byte;
    typedef Byte *               PByte;
    typedef const Byte *         PCByte;

    typedef char                 Char;
    typedef Char *               PChar;
    typedef unsigned char        UChar;
    typedef UChar *              PUChar;
    typedef Char *               PStr;
    typedef const Char *         PCStr;

    typedef short                Short;
    typedef Short *              PShort;
    typedef unsigned short       UShort;
    typedef UShort *             PUShort;
    typedef unsigned short       Word;
    typedef Word *               PWord;

    typedef int                  Int;
    typedef Int *                PInt;
    typedef unsigned int         UInt;
    typedef UInt *               PUInt;

    typedef long                 Long;
    typedef Long *               PLong;
    typedef unsigned long        ULong;
    typedef ULong *              PULong;
    typedef unsigned long        DWord;
    typedef DWord *              PDWord;

    typedef signed char          Int8;
    typedef unsigned char        UInt8;
    typedef short                Int16;
    typedef unsigned short       UInt16;
    typedef int                  Int32;
    typedef unsigned int         UInt32;
    typedef __int64              Int64;
    typedef __uint64             UInt64;

    typedef Int8 *               PInt8;
    typedef UInt8 *              PUInt8;
    typedef Int16 *              PInt16;
    typedef UInt16 *             PUInt16;
    typedef Int32 *              PInt32;
    typedef UInt32 *             PUInt32;
    typedef Int64 *              PInt64;
    typedef UInt64 *             PUInt64;

    typedef float                Float;
    typedef Float *              PFloat;
    typedef double               Double;
    typedef Double *             PDouble;

    typedef wchar_t              WChar;
    typedef WChar *              BStr;
    typedef WChar *              PWStr;
    typedef const WChar *        PCWStr;

    typedef UInt32               INetAddr;
    typedef void                 Void;
    typedef void *               PVoid;
    typedef const void *         PCVoid;
    typedef void *               Handle;
    typedef Handle *             PHandle;
    typedef void *               Address;
    typedef size_t               Size;
    typedef ULong                MilliSecond;
} // namespace AXP

#endif // __TYPE_H__
