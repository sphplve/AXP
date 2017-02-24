
#ifdef PLATFORM_WIN32
#include <malloc.h>
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
#include <stdlib.h>
#include <wchar.h>
#endif // PLATFORM_WIN32

#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "../include/astring.h"

namespace AXP {
    ARESULT Strcpy_s(
        IN PStr pszDst,
        IN Int32 sSize,
        IN PCStr pcszSrc,
        OUT Int32 * psLength)
    {
        if (!pszDst || !pcszSrc || 0 == sSize) {
            return AE_INVALIDARG;
        }

        Char * p = pszDst;

        while ((sSize > 0) && ((*p = *pcszSrc) != 0)) {
            sSize--;
            p++;
            pcszSrc++;
        }

        if (0 == sSize) {
            return AE_BUFFERTOOSMALL;
        }

        if (psLength)
            *psLength = (Int32)(p - pszDst);

        return AS_OK;
    }

    ARESULT Strncpy_s(
        IN PStr pszDst,
        IN Int32 sSize,
        IN PCStr pcszSrc,
        IN Int32 sSrcSize,
        OUT Int32 * psLength)
    {
        if (!pszDst || !pcszSrc || 0 == sSize || 0 == sSrcSize) {
            return AE_INVALIDARG;
        }

        Char * p = pszDst;

        while ((sSize > 0) && (sSrcSize > 0) && ((*p = *pcszSrc) != 0)) {
            sSize--;
            sSrcSize--;
            p++;
            pcszSrc++;
        }

        if (0 == sSize) {
            return AE_BUFFERTOOSMALL;
        }

        *p = EOAS;
        if (psLength)
            *psLength = (Int32)(p - pszDst);

        return AS_OK;
    }

    ARESULT Strcat_s(
        IN PStr pszDst,
        IN Int32 sSize,
        IN PCStr pcszSrc,
        OUT Int32 * psLength)
    {
        if ((!pszDst) || (sSize == 0))
            return AE_INVALIDARG;

        if (!pcszSrc) {
            if (psLength)
                *psLength = sSize - 1;
            return AS_OK;
        }

        Char * p = pszDst;

        while ((*p != 0) && (sSize > 0)) {
            p++;
            sSize--;
        }

        if (*p != 0) {
            return AE_INVALIDARG;
        }

        while ((sSize > 0) && ((*p = *pcszSrc) != 0)) {
            sSize--;
            p++;
            pcszSrc++;
        }

        if (0 == sSize) {
            return AE_BUFFERTOOSMALL;
        }

        if (psLength)
            *psLength = (Int32)(p - pszDst);

        return AS_OK;
    }

    ARESULT Strncat_s(
        IN PStr pszDst,
        IN Int32 sSize,
        IN PCStr pcszSrc,
        IN Int32 sSrcSize,
        OUT Int32 * psLength)
    {
        if ((!pszDst) || (sSize == 0))
            return AE_INVALIDARG;

        if ((!pcszSrc) || (0 == sSrcSize)) {
            if (psLength)
                *psLength = sSize - 1;
            return AS_OK;
        }

        Char * p = pszDst;

        while ((*p != 0) && (sSize > 0)) {
            p++;
            sSize--;
        }

        if (*p != 0) {
            return AE_INVALIDARG;
        }

        while ((sSize > 0) && (sSrcSize > 0) && ((*p = *pcszSrc) != 0)) {
            sSize--;
            sSrcSize--;
            p++;
            pcszSrc++;
        }

        if (0 == sSize) {
            return AE_BUFFERTOOSMALL;
        }

        *p = EOAS;
        if (psLength)
            *psLength = (Int32)(p - pszDst);

        return AS_OK;
    }

    Int32 Strlen_s(
        IN PCStr pcszSrc,
        IN Int32 sSize)
    {
        if (!pcszSrc || 0 == sSize) {
            return 0;
        }

        Int32 sLength = 0;

        while ((*pcszSrc != 0) && (sSize > 0)) {
            pcszSrc++;
            sLength++;
            sSize--;
        }

        if (*pcszSrc != 0) {
            return 0;
        }

        return sLength;
    }

    Int32 Strlen(
        IN PCStr pcszSrc)
    {
        if (!pcszSrc)
            return 0;

        PCStr p = pcszSrc;

        while (*p++);

        return ((Int32)(p - pcszSrc - 1));
    }

    Int32 Strcmp_s(
        IN PCStr pcszStr1,
        IN Int32 sSize1,
        IN PCStr pcszStr2,
        IN Int32 sSize2)
    {
        if (!pcszStr1 || !pcszStr2 || (sSize1 == 0) || (sSize2 == 0))
            return -1;

        if (sSize1 != sSize2)
            return -1;

        for (Int32 i = 0; i < sSize1; ++i) {
            Int32 ret = *pcszStr1++ - *pcszStr2++;
            if (ret != 0)
                return ret;
        }

        return 0;
    }

    Int32 Strcmp(
        IN PCStr pcszStr1,
        IN PCStr pcszStr2)
    {
        if (!pcszStr1 || !pcszStr2)
            return -1;

        do {
            Char a = *pcszStr1++;
            Char b = *pcszStr2++;

            Int32 ret = a - b;
            if (ret != 0)
                return ret;

            if ((a == EOAS) || (b == EOAS))
                break;
        } while (TRUE);

        return 0;
    }

    Int32 Strncmp_s(
        IN PCStr pcszStr1,
        IN Int32 sSize1,
        IN PCStr pcszStr2,
        IN Int32 sSize2,
        IN Int32 length)
    {
        if (!pcszStr1 || !pcszStr2 || (sSize1 == 0) || (sSize2 == 0))
            return -1;

        if ((length > sSize1) || (length > sSize2))
            return -1;

        for (Int32 i = 0; i < length; ++i) {
            Int32 ret = *pcszStr1++ - *pcszStr2++;
            if (ret != 0)
                return ret;
        }

        return 0;
    }

    Int32 Strnicmp_s(
        IN PCStr pcszStr1,
        IN Int32 sSize1,
        IN PCStr pcszStr2,
        IN Int32 sSize2,
        IN Int32 length)
    {
        if (!pcszStr1 || !pcszStr2 || (sSize1 == 0) || (sSize2 == 0))
            return -1;

        if ((length > sSize1) || (length > sSize2))
            return -1;

        for (Int32 i = 0; i < length; ++i) {
            Char a = *pcszStr1++;
            Char b = *pcszStr2++;

            if ((a >= 'A') && (a <= 'Z'))
                a = a - 'A' + 'a';

            if ((b >= 'A') && (b <= 'Z'))
                b = b - 'A' + 'a';

            Int32 ret = a - b;
            if (ret != 0)
                return ret;
        }

        return 0;
    }

    PStr Strchr_s(
        IN PCStr pcszStr,
        IN Int32 sSize,
        IN Char ch)
    {
        if (!pcszStr || (sSize == 0))
            return NULL;

        for (Int32 i = 0; i < sSize; ++i, ++pcszStr) {
            if (*pcszStr == ch)
                return (PStr)pcszStr;
        }

        return NULL;
    }

    PStr Strchr(
        IN PCStr pcszStr,
        IN Char ch)
    {
        if (!pcszStr)
            return NULL;

        while (*pcszStr != EOAS) {
            if (*pcszStr == ch)
                return (PStr)pcszStr;

            pcszStr++;
        }

        return NULL;
    }

    PStr Strstr_s(
        IN PCStr pcszStr,
        IN Int32 sSize1,
        IN PCStr pcszSubStr,
        IN Int32 sSize2)
    {
        if (!pcszStr || !pcszSubStr || (sSize1 == 0) || (sSize2 == 0))
            return NULL;

        for (Int32 i = 0; i < sSize1; ++i, ++pcszStr) {
            if (Strncmp_s(
                pcszStr, sSize1 - i, pcszSubStr, sSize2, sSize2 - 1) == 0)
                return (PStr)pcszStr;
        }

        return NULL;
    }

    PStr Strpbk_s(
        IN PCStr pcszStr,
        IN Int32 sSize1,
        IN PCStr pcszChars,
        IN Int32 sSize2)
    {
        if (!pcszStr || !pcszChars || (sSize1 == 0) || (sSize2 == 0))
            return NULL;

        for (Int32 i = 0; i < sSize1; ++i, ++pcszStr) {
            if (Strchr_s(pcszChars, sSize2, *pcszStr) != NULL)
                return (PStr)pcszStr;
        }

        return NULL;
    }

    Boolean StrStartsWith(
        IN PCStr pcszStr,
        IN Int32 sSize1,
        IN PCStr pcszSubStr,
        IN Int32 sSize2)
    {
        if (!pcszStr || !pcszSubStr || (sSize1 == 0) || (sSize2 == 0))
            return FALSE;

        if (sSize1 < sSize2)
            return FALSE;

        return (Strncmp_s(
            pcszStr, sSize2, pcszSubStr, sSize2, sSize2 - 1) == 0);
    }

    Boolean StriStartsWith(
        IN PCStr pcszStr,
        IN Int32 sSize1,
        IN PCStr pcszSubStr,
        IN Int32 sSize2)
    {
        if (!pcszStr || !pcszSubStr || (sSize1 == 0) || (sSize2 == 0))
            return FALSE;

        if (sSize1 < sSize2)
            return FALSE;

        return (Strnicmp_s(
            pcszStr, sSize2, pcszSubStr, sSize2, sSize2 - 1) == 0);
    }

    PStr Strdup_s(
        IN PCStr source,
        IN Int32 size)
    {
        PStr ret = (PStr)::malloc(sizeof(Char) * size);
        if (!ret)
            return NULL;

        memcpy((PVoid)ret, (PVoid)source, sizeof(Char) * size);

        return ret;
    }

    PStr Strrchr_s(
        IN PCStr pcszStr,
        IN Int32 sSize,
        IN Char ch)
    {
        if (!pcszStr || (sSize == 0))
            return NULL;

        pcszStr += sSize - 1;
        if (!pcszStr)
            return NULL;

        for (Int32 i = sSize; i != 0; --i, --pcszStr) {
            if (*pcszStr == ch)
                return (PStr)pcszStr;
        }

        return NULL;
    }

    PStr Strrchr(
        IN PCStr pcszStr,
        IN Char ch)
    {
        if (!pcszStr)
            return NULL;

        PStr start = (PStr)pcszStr;
        while (*start++);

        while (--start != pcszStr && *start != ch);
        if (*start == ch)
            return start;

        return NULL;
    }

    PStr Strrstr_s(
        IN PCStr pcszStr,
        IN Int32 sSize1,
        IN PCStr pcszSubStr,
        IN Int32 sSize2)
    {
        if (!pcszStr || !pcszSubStr || (sSize1 == 0) || (sSize2 == 0))
            return NULL;

        pcszStr += sSize1 - 1;
        if (!pcszStr)
            return NULL;

        for (Int32 i = sSize1; i != 0; --i, --pcszStr) {
            if (Strncmp_s(
                pcszStr, sSize1 - i, pcszSubStr, sSize2, sSize2 - 1) == 0)
                return (PStr)pcszStr;
        }

        return NULL;
    }

    PStr Strrpbk_s(
        IN PCStr pcszStr,
        IN Int32 sSize1,
        IN PCStr pcszChars,
        IN Int32 sSize2)
    {
        if (!pcszStr || !pcszChars || (sSize1 == 0) || (sSize2 == 0))
            return NULL;

        pcszStr += sSize1 - 1;
        if (!pcszStr)
            return NULL;

        for (Int32 i = sSize1; i != 0; --i, --pcszStr) {
            if (Strchr_s(pcszChars, sSize2, *pcszStr) != NULL)
                return (PStr)pcszStr;
        }

        return NULL;
    }

    ARESULT Wcscpy_s(
        IN PWStr pwszDst,
        IN Int32 sSize,
        IN PCWStr pcwszSrc,
        OUT Int32 * psLength)
    {
        if (!pwszDst || !pcwszSrc || 0 == sSize) {
            return AE_INVALIDARG;
        }

        WChar * p = pwszDst;

        while ((sSize > 0) && ((*p = *pcwszSrc) != 0)) {
            sSize--;
            p++;
            pcwszSrc++;
        }

        if (0 == sSize) {
            return AE_BUFFERTOOSMALL;
        }

        if (psLength)
            *psLength = (Int32)(p - pwszDst);

        return AS_OK;
    }

    ARESULT Wcsncpy_s(
        IN PWStr pwszDst,
        IN Int32 sSize,
        IN PCWStr pcwszSrc,
        IN Int32 sSrcSize,
        OUT Int32 * psLength)
    {
        if (!pwszDst || !pcwszSrc || 0 == sSize || 0 == sSrcSize) {
            return AE_INVALIDARG;
        }

        WChar * p = pwszDst;

        while ((sSize > 0) && (sSrcSize > 0) && ((*p = *pcwszSrc) != 0)) {
            sSize--;
            sSrcSize--;
            p++;
            pcwszSrc++;
        }

        if (0 == sSize) {
            return AE_BUFFERTOOSMALL;
        }

        *p = EOS;
        if (psLength)
            *psLength = (Int32)(p - pwszDst);

        return AS_OK;
    }

    ARESULT Wcscat_s(
        IN PWStr pwszDst,
        IN Int32 sSize,
        IN PCWStr pcwszSrc,
        OUT Int32 * psLength)
    {
        if ((!pwszDst) || (sSize == 0))
            return AE_INVALIDARG;

        if (!pcwszSrc) {
            if (psLength)
                *psLength = sSize - 1;
            return AS_OK;
        }

        WChar * p = pwszDst;

        while ((*p != 0) && (sSize > 0)) {
            p++;
            sSize--;
        }

        if (*p != 0) {
            return AE_INVALIDARG;
        }

        while ((sSize > 0) && ((*p = *pcwszSrc) != 0)) {
            sSize--;
            p++;
            pcwszSrc++;
        }

        if (0 == sSize) {
            return AE_BUFFERTOOSMALL;
        }

        if (psLength)
            *psLength = (Int32)(p - pwszDst);

        return AS_OK;
    }

    ARESULT Wcsncat_s(
        IN PWStr pwszDst,
        IN Int32 sSize,
        IN PCWStr pcwszSrc,
        IN Int32 sSrcSize,
        OUT Int32 * psLength)
    {
        if ((!pwszDst) || (sSize == 0))
            return AE_INVALIDARG;

        if ((!pcwszSrc) || (0 == sSrcSize)) {
            if (psLength)
                *psLength = sSize - 1;
            return AS_OK;
        }

        WChar * p = pwszDst;

        while ((*p != 0) && (sSize > 0)) {
            p++;
            sSize--;
        }

        if (*p != 0) {
            return AE_INVALIDARG;
        }

        while ((sSize > 0) && (sSrcSize > 0) && ((*p = *pcwszSrc) != 0)) {
            sSize--;
            sSrcSize--;
            p++;
            pcwszSrc++;
        }

        if (0 == sSize) {
            return AE_BUFFERTOOSMALL;
        }

        *p = EOS;
        if (psLength)
            *psLength = (Int32)(p - pwszDst);

        return AS_OK;
    }

    Int32 Wcslen_s(
        IN PCWStr pcwszSrc,
        IN Int32 sSize)
    {
        if (!pcwszSrc || 0 == sSize) {
            return 0;
        }

        Int32 sLength = 0;

        while ((*pcwszSrc != 0) && (sSize > 0)) {
            pcwszSrc++;
            sLength++;
            sSize--;
        }

        if (*pcwszSrc != 0) {
            return 0;
        }

        return sLength;
    }

    Int32 Wcslen(
        IN PCWStr pcwszSrc)
    {
        if (!pcwszSrc) {
            return 0;
        }

        PCWStr p = pcwszSrc;

        while (*p++);

        return ((Int32)(p - pcwszSrc - 1));
    }

    Int32 Wcscmp_s(
        IN PCWStr pcwszStr1,
        IN Int32 sSize1,
        IN PCWStr pcwszStr2,
        IN Int32 sSize2)
    {
        if (!pcwszStr1 || !pcwszStr2 || (sSize1 == 0) || (sSize2 == 0))
            return -1;

        if (sSize1 != sSize2)
            return -1;

        for (Int32 i = 0; i < sSize1; ++i) {
            Int32 ret = *pcwszStr1++ - *pcwszStr2++;
            if (ret != 0)
                return ret;
        }

        return 0;
    }

    Int32 Wcscmp(
        IN PCWStr pcwszStr1,
        IN PCWStr pcwszStr2)
    {
        if (!pcwszStr1 || !pcwszStr2)
            return -1;

        do {
            WChar a = *pcwszStr1++;
            WChar b = *pcwszStr2++;

            Int32 ret = a - b;
            if (ret != 0)
                return ret;

            if ((a == EOS) || (b == EOS))
                break;
        } while (TRUE);

        return 0;
    }

    Int32 Wcsncmp_s(
        IN PCWStr pcwszStr1,
        IN Int32 sSize1,
        IN PCWStr pcwszStr2,
        IN Int32 sSize2,
        IN Int32 length)
    {
        if (!pcwszStr1 || !pcwszStr2 || (sSize1 == 0) || (sSize2 == 0))
            return -1;

        if ((length > sSize1) || (length > sSize2))
            return -1;

        for (Int32 i = 0; i < length; ++i) {
            Int32 ret = *pcwszStr1++ - *pcwszStr2++;
            if (ret != 0)
                return ret;
        }

        return 0;
    }

    Int32 Wcsnicmp_s(
        IN PCWStr pcwszStr1,
        IN Int32 sSize1,
        IN PCWStr pcwszStr2,
        IN Int32 sSize2,
        IN Int32 length)
    {
        if (!pcwszStr1 || !pcwszStr2 || (sSize1 == 0) || (sSize2 == 0))
            return -1;

        if ((length > sSize1) || (length > sSize2))
            return -1;

        for (Int32 i = 0; i < length; ++i) {
            WChar a = *pcwszStr1++;
            WChar b = *pcwszStr2++;

            if ((a >= 'A') && (a <= 'Z'))
                a = a - 'A' + 'a';

            if ((b >= 'A') && (b <= 'Z'))
                b = b - 'A' + 'a';

            Int32 ret = a - b;
            if (ret != 0)
                return ret;
        }

        return 0;
    }

    PCWStr Wcschr_s(
        IN PCWStr pcwszStr,
        IN Int32 sSize,
        IN WChar ch)
    {
        if (!pcwszStr || (sSize == 0))
            return NULL;

        for (Int32 i = 0; i < sSize; ++i, ++pcwszStr) {
            if (*pcwszStr == ch)
                return (PWStr)pcwszStr;
        }

        return NULL;
    }

    PWStr Wcschr(
        IN PCWStr pcwszStr,
        IN WChar ch)
    {
        if (!pcwszStr)
            return NULL;

        while (*pcwszStr != EOS) {
            if (*pcwszStr == ch)
                return (PWStr)pcwszStr;

            pcwszStr++;
        }

        return NULL;
    }

    PCWStr Wcsstr_s(
        IN PCWStr pcwszStr,
        IN Int32 sSize1,
        IN PCWStr pcwszSubStr,
        IN Int32 sSize2)
    {
        if (!pcwszStr || !pcwszSubStr || (sSize1 == 0) || (sSize2 == 0))
            return NULL;

        for (Int32 i = 0; i < sSize1; ++i, ++pcwszStr) {
            if (Wcsncmp_s(
                pcwszStr, sSize1 - i, pcwszSubStr, sSize2 + 1, sSize2) == 0)
                return (PWStr)pcwszStr;
        }

        return NULL;
    }

    PWStr Wcspbrk_s(
        IN PCWStr pcwszStr,
        IN Int32 sSize1,
        IN PCWStr pcwszChars,
        IN Int32 sSize2)
    {
        if (!pcwszStr || !pcwszChars || (sSize1 == 0) || (sSize2 == 0))
            return NULL;

        for (Int32 i = 0; i < sSize1; ++i, ++pcwszStr) {
            if (Wcschr_s(pcwszChars, sSize2, *pcwszStr) != NULL)
                return (PWStr)pcwszStr;
        }

        return NULL;
    }

    Boolean WcsStartsWith(
        IN PCWStr pcwszStr,
        IN Int32 sSize1,
        IN PCWStr pcwszSubStr,
        IN Int32 sSize2)
    {
        if (!pcwszStr || !pcwszSubStr || (sSize1 == 0) || (sSize2 == 0))
            return FALSE;

        if (sSize1 < sSize2)
            return FALSE;

        return (Wcsncmp_s(
            pcwszStr, sSize2, pcwszSubStr, sSize2, sSize2 - 1) == 0);
    }

    Boolean WcsiStartsWith(
        IN PCWStr pcwszStr,
        IN Int32 sSize1,
        IN PCWStr pcwszSubStr,
        IN Int32 sSize2)
    {
        if (!pcwszStr || !pcwszSubStr || (sSize1 == 0) || (sSize2 == 0))
            return FALSE;

        if (sSize1 < sSize2)
            return FALSE;

        return (Wcsnicmp_s(
            pcwszStr, sSize2, pcwszSubStr, sSize2, sSize2 - 1) == 0);
    }

    PWStr Wcsdup_s(
        IN PCWStr source,
        IN Int32 size)
    {
        PWStr ret = (PWStr)::malloc(sizeof(WChar) * size);
        if (!ret)
            return NULL;

        memcpy((PVoid)ret, (PVoid)source, sizeof(WChar) * size);

        return ret;
    }

    Int32 MbsToWcs(
        IN PWStr dst,
        IN PCStr src,
        IN Int32 count)
    {
#ifdef PLATFORM_WIN32
        Int32 length;

        if (mbstowcs_s((Size*)&length, dst, count + 1, src, count) == 0)
            return length;
        else
            return -1;
#else
        return (Int32)mbstowcs(dst, src, count);
#endif // PLATFORM_WIN32
    }

    Int32 WcsToMbs(
        IN PStr dst,
        IN PCWStr src,
        IN Int32 count)
    {
#ifdef PLATFORM_WIN32
        Int32 length;

        if (wcstombs_s((Size*)&length, dst, count + 1, src, count) == 0)
            return length;
        else
            return -1;
#else
        return (Int32)wcstombs(dst, src, count);
#endif // PLATFORM_WIN32
    }

    PWStr Wcsrchr_s(
        IN PCWStr pcwszStr,
        IN Int32 sSize,
        IN WChar ch)
    {
        if (!pcwszStr || (sSize == 0))
            return NULL;

        pcwszStr += sSize - 1;
        if (!pcwszStr)
            return NULL;

        for (Int32 i = sSize; i != 0; --i, --pcwszStr) {
            if (*pcwszStr == ch)
                return (PWStr)pcwszStr;
        }

        return NULL;
    }

    PWStr Wcsrchr(
        IN PCWStr pcwszStr,
        IN WChar ch)
    {
        if (!pcwszStr)
            return NULL;

        PWStr start = (PWStr)pcwszStr;
        while (*start++);

        while (--start != pcwszStr && *start != ch);
        if (*start == ch)
            return start;

        return NULL;
    }

    PWStr Wcsrstr_s(
        IN PCWStr pcwszStr,
        IN Int32 sSize1,
        IN PCWStr pcwszSubStr,
        IN Int32 sSize2)
    {
        if (!pcwszStr || !pcwszSubStr || (sSize1 == 0) || (sSize2 == 0))
            return NULL;

        pcwszStr += sSize1 - 1;
        if (!pcwszStr)
            return NULL;

        for (Int32 i = sSize1; i != 0 ; --i, --pcwszStr) {
            if (Wcsncmp_s(
                pcwszStr, sSize1 - i, pcwszSubStr, sSize2 + 1, sSize2) == 0)
                return (PWStr)pcwszStr;
        }

        return NULL;
    }

    PWStr Wcsrpbrk_s(
        IN PCWStr pcwszStr,
        IN Int32 sSize1,
        IN PCWStr pcwszChars,
        IN Int32 sSize2)
    {
        if (!pcwszStr || !pcwszChars || (sSize1 == 0) || (sSize2 == 0))
            return NULL;

        pcwszStr += sSize1 - 1;
        if (!pcwszStr)
            return NULL;

        for (Int32 i = sSize1; i != 0; --i, --pcwszStr) {
            if (Wcschr_s(pcwszChars, sSize2, *pcwszStr) != NULL)
                return (PWStr)pcwszStr;
        }

        return NULL;
    }

    ARESULT Convert(
        IN CONST EncodingType fromEncoding,
        IN CONST EncodingType toEncoding,
        IN PCByte inBuffer,
        IN CONST Int32 inLength,
        INOUT PByte outBuffer,
        INOUT Int32 * outLength)
    {
        if (!inBuffer || !outBuffer || !outLength)
            return AE_INVALIDARG;

        PByte temp = outBuffer;
        Char **inBufferTemp = (Char**)&inBuffer;
        Char **outBufferTemp = (Char**)&temp;
        Size length = *outLength;
        Size inLen = inLength;
        Size outLen = *outLength;

        EncodingType from, to;
#ifdef ARCH_LITTLE_ENDIAN
        if (toEncoding == EncodingType_UTF16)
            to = EncodingType_UTF16LE;
        else if (toEncoding == EncodingType_UTF32)
            to = EncodingType_UTF32LE;
        else
            to = toEncoding;

        if (fromEncoding == EncodingType_UTF16)
            from = EncodingType_UTF16LE;
        else if (fromEncoding == EncodingType_UTF32)
            from = EncodingType_UTF32LE;
        else
            from = fromEncoding;
#else
        if (toEncoding == EncodingType_UTF16)
            to = EncodingType_UTF16BE;
        else if (toEncoding == EncodingType_UTF32)
            to = EncodingType_UTF32BE;
        else
            to = toEncoding;

        if (fromEncoding == EncodingType_UTF16)
            from = EncodingType_UTF16BE;
        else if (fromEncoding == EncodingType_UTF32)
            from = EncodingType_UTF32BE;
        else
            from = fromEncoding;
#endif

        iconv_t cd;
        if ((cd = iconv_open(
            sEncodingTypeList[to],
            sEncodingTypeList[from])) == (iconv_t)-1)
            return AE_OUTOFMEMORY;

        Size result = iconv(cd, inBufferTemp, &inLen, outBufferTemp, &outLen);
        iconv_close(cd);
        if (result == (Size)-1)
            return AE_OUTOFMEMORY;

        *outLength = (Int32)(length - outLen);

        return AS_OK;
    }
} // namespace AXP