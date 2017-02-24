
#ifndef __LIMITS_H__
#define __LIMITS_H__

#ifndef MB_LEN_MAX
#define MB_LEN_MAX   (4)                // max. # bytes in multibyte char
#endif // MB_LEN_MAX

#ifndef CHAR_BIT
#define CHAR_BIT     (8)                // number of bits in a char
#endif // CHAR_BIT

#ifndef SCHAR_MIN
#define SCHAR_MIN    (-128)             // minimum signed char value
#endif // SCHAR_MIN

#ifndef SCHAR_MAX
#define SCHAR_MAX    (127)              // maximum signed char value
#endif // SCHAR_MAX

#ifndef UCHAR_MAX
#define UCHAR_MAX    (0xFF)             // maximum unsigned char value
#endif // UCHAR_MAX

#ifndef SHRT_BIT
#define SHRT_BIT     (16)
#endif // SHRT_BIT

#ifndef SHRT_MIN
#define SHRT_MIN     (-32768)
#endif // SHRT_MIN

#ifndef SHRT_MAX
#define SHRT_MAX     (32767)
#endif // SHRT_MAX

#ifndef USHRT_MAX
#define USHRT_MAX    (0xFFFF)
#endif // USHRT_MAX

#ifndef INT32_BIT
#define INT32_BIT    (32)
#endif // INT32_BIT

#ifndef INT32_MIN
#define INT32_MIN    (-2147483648)
#endif // INT32_MIN

#ifndef INT32_MAX
#define INT32_MAX    (2147483647)
#endif // INT32_MAX

#ifndef UINT32_MAX
#define UINT32_MAX   (0xFFFFFFFF)
#endif // UINT32_MAX

#ifndef LONG_BIT
#define LONG_BIT     (32)
#endif // LONG_BIT

#ifndef LONG_MIN
#define LONG_MIN     (-2147483648)
#endif // LONG_MIN

#ifndef LONG_MAX
#define LONG_MAX     (2147483647)
#endif // LONG_MAX

#ifndef ULONG_MAX
#define ULONG_MAX    (0xFFFFFFFF)
#endif // ULONG_MAX

#ifndef INT32_STR_MAX
#define INT32_STR_MAX (12)
#endif // INT32_STR_MAX

#ifndef INT64_STR_MAX
#define INT64_STR_MAX (21)
#endif // INT64_STR_MAX

#ifndef FLOAT_STR_MAX
#define FLOAT_STR_MAX (48)
#endif // FLOAT_STR_MAX

#endif // __LIMITS_H__
