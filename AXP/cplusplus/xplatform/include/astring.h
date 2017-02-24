
#ifndef __ASTRING_H__
#define __ASTRING_H__

#include <stdarg.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>

#include "type.h"
#include "define.h"
#include "coretype.h"
#include "aresult.h"
#include "refBase.h"
#include "list.h"
#include "../../ThirdParty/IConv/Libiconv/IConv.h"

namespace AXP {
#define EOS                 (L'\0')      // UNICODE字符串结束标志
#define EOAS                ('\0')       // ASCII字符串结束标志
#define ASCII_CHAR_MAX      (0x7F)       // ASCII字符上限

    /**
     * 复制字符串,包括尾0,安全函数
     *
     * @param IN PStr pszDst 目标字符串
     * @param IN Int32 sSize 目标字符串缓存长度,包括尾0
     * @param IN PCStr pcszSrc 源字符串
     * @param IN Int32 * plength 完成操作后目标字符串有效字符数,不包括尾0
     *
     * @return E_INVALIDARG 传入参数错误
     * @return E_BUFFERTOOSMALL 目标字符串缓冲区长度不足
     */
    EXTERN_C ARESULT Strcpy_s(
        IN PStr pszDst,
        IN Int32 sSize,
        IN PCStr pcszSrc,
        OUT Int32 * plength);

    /**
     * 复制指定长度的字符串,包括尾0,安全函数
     *
     * @param IN PStr pszDst 目标字符串
     * @param IN Int32 sSize 目标字符串缓存长度,包括尾0
     * @param IN PCStr pcszSrc 源字符串
     * @param IN Int32 sSrcSize 需要复制源串长度,不包括尾0
     * @param IN Int32 * plength 完成操作后目标字符串有效字符数,不包括尾0
     *
     * @return E_INVALIDARG 传入参数错误
     * @return E_BUFFERTOOSMALL 目标字符串缓冲区长度不足
     */
    EXTERN_C ARESULT Strncpy_s(
        IN PStr pszDst,
        IN Int32 sSize,
        IN PCStr pcszSrc,
        IN Int32 sSrcSize,
        OUT Int32 * plength);

    /**
     * 拼接字符串,包括尾0,安全函数
     *
     * @param IN PStr pszDst 目标字符串
     * @param IN Int32 sSize 目标字符串缓存长度,包括尾0
     * @param IN PCStr pcszSrc 源字符串
     * @param IN Int32 * plength 完成操作后目标字符串有效字符数,不包括尾0
     *
     * @return E_INVALIDARG 传入参数错误
     * @return E_BUFFERTOOSMALL 目标字符串缓冲区长度不足
     */
    EXTERN_C ARESULT Strcat_s(
        IN PStr pszDst,
        IN Int32 sSize,
        IN PCStr pcszSrc,
        OUT Int32 * plength);

    /**
     * 拼接指定长度的字符串,包括尾0,安全函数
     *
     * @param IN PStr pszDst 目标字符串
     * @param IN Int32 sSize 目标字符串缓存长度,包括尾0
     * @param IN PCStr pcszSrc 源字符串
     * @param IN Int32 sSrcSize 需要拼接源串长度,不包括尾0
     * @param IN Int32 * plength 完成操作后目标字符串有效字符数,不包括尾0
     *
     * @return E_INVALIDARG 传入参数错误
     * @return E_BUFFERTOOSMALL 目标字符串缓冲区长度不足
     */
    EXTERN_C ARESULT Strncat_s(
        IN PStr pszDst,
        IN Int32 sSize,
        IN PCStr pcszSrc,
        IN Int32 sSrcSize,
        OUT Int32 * plength);

    /**
     * 返回字符串有效字符个数,不包括尾0,安全函数
     *
     * @param IN PCStr pcszSrc 字符串
     * @param IN Int32 sSize 字符串缓存长度,包括尾0
     *
     * @return Int32 有效字符个数,不包括尾0,若有效字符数超过缓存长度,则返回0
     */
    EXTERN_C Int32 Strlen_s(
        IN PCStr pcszSrc,
        IN Int32 sSize);

    /**
     * 返回字符串有效字符个数,不包括尾0
     *
     * @param IN PCStr pcszSrc 字符串
     *
     * @return Int32 有效字符个数,不包括尾0
     */
    EXTERN_C Int32 Strlen(
        IN PCStr pcszSrc);

    /**
     * 比较字符串,安全函数
     *
     * @param IN PCStr pcszStr1 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCStr pcszStr2 字符串
     * @param IN Int32 sSize2 字符串缓存长度,包括尾0
     *
     * @return Int32 比较结果,为0表示两字符串相同
     */
    EXTERN_C Int32 Strcmp_s(
        IN PCStr pcszStr1,
        IN Int32 sSize1,
        IN PCStr pcszStr2,
        IN Int32 sSize2);

    /**
     * 比较字符串
     *
     * @param IN PCStr pcszStr1 字符串
     * @param IN PCStr pcszStr2 字符串
     *
     * @return Int32 比较结果,为0表示两字符串相同
     */
    EXTERN_C Int32 Strcmp(
        IN PCStr pcszStr1,
        IN PCStr pcszStr2);

    /**
     * 比较指定长度字符串,安全函数
     *
     * @param IN PCStr pcszStr1 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCStr pcszStr2 字符串
     * @param IN Int32 sSize2 字符串缓存长度,包括尾0
     * @param IN Int32 length 比较指定长度
     *
     * @return Int32 比较结果,为0表示两字符串相同
     */
    EXTERN_C Int32 Strncmp_s(
        IN PCStr pcwszStr1,
        IN Int32 sSize1,
        IN PCStr pcwszStr2,
        IN Int32 sSize2,
        IN Int32 length);

    /**
     * 忽略英文大小写比较指定长度字符串,安全函数
     *
     * @param IN PCStr pcszStr1 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCStr pcszStr2 字符串
     * @param IN Int32 sSize2 字符串缓存长度,包括尾0
     * @param IN Int32 length 比较指定长度
     *
     * @return Int32 比较结果,为0表示两字符串相同
     */
    EXTERN_C Int32 Strnicmp_s(
        IN PCStr pcwszStr1,
        IN Int32 sSize1,
        IN PCStr pcwszStr2,
        IN Int32 sSize2,
        IN Int32 length);

    /**
     * 在字符串中定位字符位置,安全函数
     *
     * @param IN PCStr pcszStr 字符串
     * @param IN Int32 sSize 字符串缓存长度,包括尾0
     * @param IN Char ch 字符
     *
     * @return PStr 首次发现字符的位置,若为空表示未查找到
     */
    EXTERN_C PStr Strchr_s(
        IN PCStr pcszStr,
        IN Int32 sSize,
        IN Char ch);

    /**
     * 在字符串中定位字符位置
     *
     * @param IN PCStr pcszStr 字符串
     * @param IN Char ch 字符
     *
     * @return PStr 首次发现字符的位置,若为空表示未查找到
     */
    EXTERN_C PStr Strchr(
        IN PCStr pcszStr,
        IN Char ch);

    /**
     * 在字符串中定位子字符串中字符位置,安全函数
     *
     * @param IN PCStr pcszStr 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCStr pcszSubStr 子字符串
     * @param IN Int32 sSize2 子字符串缓存长度,不包括尾0
     *
     * @return PStr 首次发现字符串的位置,若为空表示未查找到
     */
    EXTERN_C PStr Strstr_s(
        IN PCStr pcszStr,
        IN Int32 sSize1,
        IN PCStr pcszSubStr,
        IN Int32 sSize2);

    /**
     * 在字符串中定位子字符串中字符位置
     *
     * @param IN PCStr pcszStr 字符串
     * @param IN PCStr pcszSubStr 子字符串
     *
     * @return PStr 首次发现字符串的位置,若为空表示未查找到
     */
    EXTERN_C PStr Strstr(
        IN PCStr pcszStr,
        IN PCStr pcszSubStr);

    /**
     * 在字符串中定位字符列表中字符位置,安全函数
     *
     * @param IN PCStr pcszStr 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCStr pcszChars 字符列表
     * @param IN Int32 sSize2 字符列表缓存长度,不包括尾0
     *
     * @return PStr 首次发现字符的位置,若为空表示未查找到
     */
    EXTERN_C PStr Strpbk_s(
        IN PCStr pcszStr,
        IN Int32 sSize1,
        IN PCStr pcszChars,
        IN Int32 sSize2);

    /**
     * 在字符串中定位字符列表中字符位置
     *
     * @param IN PCStr pcszStr 字符串
     * @param IN PCStr pcszChars 字符列表
     *
     * @return PStr 首次发现字符的位置,若为空表示未查找到
     */
    EXTERN_C PStr Strpbk(
        IN PCStr pcszStr,
        IN PCStr pcszChars);

    /**
     * 判断字符串的开头是否符合指定子字符串
     *
     * @param IN PCStr pcszStr 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCStr pcszSubStr 子字符串
     * @param IN Int32 sSize2 子字符串缓存长度,包括尾0
     *
     * @return 字符串的开头是否符合指定子字符串
     */
    EXTERN_C Boolean StrStartsWith(
        IN PCStr pcszStr,
        IN Int32 sSize1,
        IN PCStr pcszSubStr,
        IN Int32 sSize2);

    /**
     * 忽略英文大小写判断字符串的开头是否符合指定子字符串
     *
     * @param IN PCStr pcszStr 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCStr pcszSubStr 子字符串
     * @param IN Int32 sSize2 子字符串缓存长度,包括尾0
     *
     * @return 字符串的开头是否符合指定子字符串
     */
    EXTERN_C Boolean StriStartsWith(
        IN PCStr pcszStr,
        IN Int32 sSize1,
        IN PCStr pcszSubStr,
        IN Int32 sSize2);

    /**
     * 复制字符串,安全函数
     *
     * @param IN PCStr source 字符串
     * @param IN Int32 size 字符串长度,包括尾0
     *
     * @return 复制后字符串,失败返回空
     */
    EXTERN_C PStr Strdup_s(
        IN PCStr source,
        IN Int32 size);

    /**
     * 在字符串中定位字符位置,安全函数
     *
     * @param IN PCStr pcszStr 字符串
     * @param IN Int32 sSize 字符串缓存长度,包括尾0
     * @param IN Char ch 字符
     *
     * @return PStr 从末尾开始首次发现字符的位置,若为空表示未查找到
     */
    EXTERN_C PStr Strrchr_s(
        IN PCStr pcszStr,
        IN Int32 sSize,
        IN Char ch);

    /**
     * 在字符串中定位字符位置
     *
     * @param IN PCStr pcszStr 字符串
     * @param IN Char ch 字符
     *
     * @return PStr 从末尾开始首次发现字符的位置,若为空表示未查找到
     */
    EXTERN_C PStr Strrchr(
        IN PCStr pcszStr,
        IN Char ch);

    /**
     * 在字符串中定位子字符串中字符位置,安全函数
     *
     * @param IN PCStr pcszStr 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCStr pcszSubStr 子字符串
     * @param IN Int32 sSize2 子字符串缓存长度,不包括尾0
     *
     * @return PStr 从末尾首次发现字符串的位置,若为空表示未查找到
     */
    EXTERN_C PStr Strrstr_s(
        IN PCStr pcszStr,
        IN Int32 sSize1,
        IN PCStr pcszSubStr,
        IN Int32 sSize2);

    /**
     * 在字符串中定位字符列表中字符位置,安全函数
     *
     * @param IN PCStr pcszStr 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCStr pcszChars 字符列表
     * @param IN Int32 sSize2 字符列表缓存长度,不包括尾0
     *
     * @return PStr 从末尾开始首次发现字符的位置,若为空表示未查找到
     */
    EXTERN_C PStr Strrpbk_s(
        IN PCStr pcszStr,
        IN Int32 sSize1,
        IN PCStr pcszChars,
        IN Int32 sSize2);

    /**
     * 复制字符串,包括尾0,安全函数
     *
     * @param IN PWStr pwszDst 目标字符串
     * @param IN Int32 sSize 目标字符串缓存长度,包括尾0
     * @param IN PCWStr pcwszSrc 源字符串
     * @param IN Int32 * plength 完成操作后目标字符串有效字符数,不包括尾0
     *
     * @return E_INVALIDARG 传入参数错误
     * @return E_BUFFERTOOSMALL 目标字符串缓冲区长度不足
     */
    EXTERN_C ARESULT Wcscpy_s(
        IN PWStr pwszDst,
        IN Int32 sSize,
        IN PCWStr pcwszSrc,
        OUT Int32 * plength);

    /**
     * 复制指定长度的字符串,包括尾0,安全函数
     *
     * @param IN PWStr pwszDst 目标字符串
     * @param IN Int32 sSize 目标字符串缓存长度,包括尾0
     * @param IN PCWStr pcwszSrc 源字符串
     * @param IN Int32 sSrcSize 需要复制源串长度,不包括尾0
     * @param IN Int32 * plength 完成操作后目标字符串有效字符数,不包括尾0
     *
     * @return E_INVALIDARG 传入参数错误
     * @return E_BUFFERTOOSMALL 目标字符串缓冲区长度不足
     */
    EXTERN_C ARESULT Wcsncpy_s(
        IN PWStr pwszDst,
        IN Int32 sSize,
        IN PCWStr pcwszSrc,
        IN Int32 sSrcSize,
        OUT Int32 * plength);

    /**
     * 拼接字符串,包括尾0,安全函数
     *
     * @param IN PWStr pwszDst 目标字符串
     * @param IN Int32 sSize 目标字符串缓存长度,包括尾0
     * @param IN PCWStr pcwszSrc 源字符串
     * @param IN Int32 * plength 完成操作后目标字符串有效字符数,不包括尾0
     *
     * @return E_INVALIDARG 传入参数错误
     * @return E_BUFFERTOOSMALL 目标字符串缓冲区长度不足
     */
    EXTERN_C ARESULT Wcscat_s(
        IN PWStr pwszDst,
        IN Int32 sSize,
        IN PCWStr pcwszSrc,
        OUT Int32 * plength);

    /**
     * 拼接指定长度的字符串,包括尾0,安全函数
     *
     * @param IN PWStr pwszDst 目标字符串
     * @param IN Int32 sSize 目标字符串缓存长度,包括尾0
     * @param IN PCWStr pcwszSrc 源字符串
     * @param IN Int32 sSrcSize 需要拼接源串长度,不包括尾0
     * @param IN Int32 * plength 完成操作后目标字符串有效字符数,不包括尾0
     *
     * @return E_INVALIDARG 传入参数错误
     * @return E_BUFFERTOOSMALL 目标字符串缓冲区长度不足
     */
    EXTERN_C ARESULT Wcsncat_s(
        IN PWStr pwszDst,
        IN Int32 sSize,
        IN PCWStr pcwszSrc,
        IN Int32 sSrcSize,
        OUT Int32 * plength);

    /**
     * 返回字符串有效字符个数,不包括尾0,安全函数
     *
     * @param IN PCWStr pcwszSrc 字符串
     * @param IN Int32 sSize 字符串缓存长度,包括尾0
     *
     * @return Int32 有效字符个数,不包括尾0,若有效字符数超过缓存长度,则返回0
     */
    EXTERN_C Int32 Wcslen_s(
        IN PCWStr pcwszSrc,
        IN Int32 sSize);

    /**
     * 返回字符串有效字符个数,不包括尾0
     *
     * @param IN PCWStr pcwszSrc 字符串
     *
     * @return Int32 有效字符个数,不包括尾0
     */
    EXTERN_C Int32 Wcslen(
        IN PCWStr pcwszSrc);

    /**
     * 比较字符串,安全函数
     *
     * @param IN PCWStr pcwszStr1 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCWStr pcwszStr2 字符串
     * @param IN Int32 sSize2 字符串缓存长度,包括尾0
     *
     * @return Int32 比较结果,为0表示两字符串相同
     */
    EXTERN_C Int32 Wcscmp_s(
        IN PCWStr pcwszStr1,
        IN Int32 sSize1,
        IN PCWStr pcwszStr2,
        IN Int32 sSize2);

    /**
     * 比较字符串
     *
     * @param IN PCWStr pcwszStr1 字符串
     * @param IN PCWStr pcwszStr2 字符串
     *
     * @return Int32 比较结果,为0表示两字符串相同
     */
    EXTERN_C Int32 Wcscmp(
        IN PCWStr pcwszStr1,
        IN PCWStr pcwszStr2);

    /**
     * 比较指定长度字符串,安全函数
     *
     * @param IN PCWStr pcwszStr1 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCWStr pcwszStr2 字符串
     * @param IN Int32 sSize2 字符串缓存长度,包括尾0
     * @param IN Int32 length 比较指定长度
     *
     * @return Int32 比较结果,为0表示两字符串相同
     */
    EXTERN_C Int32 Wcsncmp_s(
        IN PCWStr pcwszStr1,
        IN Int32 sSize1,
        IN PCWStr pcwszStr2,
        IN Int32 sSize2,
        IN Int32 length);

    /**
     * 忽略英文大小写比较指定长度字符串,安全函数
     *
     * @param IN PCWStr pcwszStr1 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCWStr pcwszStr2 字符串
     * @param IN Int32 sSize2 字符串缓存长度,包括尾0
     * @param IN Int32 length 比较指定长度
     *
     * @return Int32 比较结果,为0表示两字符串相同
     */
    EXTERN_C Int32 Wcsnicmp_s(
        IN PCWStr pcwszStr1,
        IN Int32 sSize1,
        IN PCWStr pcwszStr2,
        IN Int32 sSize2,
        IN Int32 length);

    /**
     * 在字符串中定位字符位置,安全函数
     *
     * @param IN PCWStr pcwszStr 字符串
     * @param IN Int32 sSize 字符串缓存长度,包括尾0
     * @param IN WChar ch 字符
     *
     * @return PWStr 首次发现字符的位置,若为空表示未查找到
     */
    EXTERN_C PCWStr Wcschr_s(
        IN PCWStr pcwszStr,
        IN Int32 sSize,
        IN WChar ch);

    /**
     * 在字符串中定位字符位置
     *
     * @param IN PCWStr pcwszStr 字符串
     * @param IN WChar ch 字符
     *
     * @return PWStr 首次发现字符的位置,若为空表示未查找到
     */
    EXTERN_C PWStr Wcschr(
        IN PCWStr pcwszStr,
        IN WChar ch);

    /**
     * 在字符串中定位子字符串中字符位置,安全函数
     *
     * @param IN PCWStr pcwszStr 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCWStr pcwszSubStr 子字符串
     * @param IN Int32 sSize2 子字符串缓存长度,不包括尾0
     *
     * @return PWStr 首次发现字符串的位置,若为空表示未查找到
     */
    EXTERN_C PCWStr Wcsstr_s(
        IN PCWStr pcwszStr,
        IN Int32 sSize1,
        IN PCWStr pcwszSubStr,
        IN Int32 sSize2);

    /**
     * 在字符串中定位子字符串中字符位置
     *
     * @param IN PCWStr pcwszStr 字符串
     * @param IN PCWStr pcwszSubStr 子字符串
     *
     * @return PWStr 首次发现字符串的位置,若为空表示未查找到
     */
    EXTERN_C PWStr Wcsstr(
        IN PCWStr pcwszStr,
        IN PCWStr pcwszSubStr);

    /**
     * 在字符串中定位字符列表中字符位置,安全函数
     *
     * @param IN PCWStr pcwszStr 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCWStr pcwszChars 字符列表
     * @param IN Int32 sSize2 字符列表缓存长度,不包括尾0
     *
     * @return PWStr 首次发现字符的位置,若为空表示未查找到
     */
    EXTERN_C PWStr Wcspbrk_s(
        IN PCWStr pcwszStr,
        IN Int32 sSize1,
        IN PCWStr pcwszChars,
        IN Int32 sSize2);

    /**
     * 在字符串中定位字符列表中字符位置
     *
     * @param IN PCWStr pcwszStr 字符串
     * @param IN PCWStr pcwszChars 字符列表
     *
     * @return PWStr 首次发现字符的位置,若为空表示未查找到
     */
    EXTERN_C PWStr Wcspbrk(
        IN PCWStr pcwszStr,
        IN PCWStr pcwszChars);

    /**
     * 判断字符串的开头是否符合指定子字符串
     *
     * @param IN PCWStr pcwszStr 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCWStr pcwszSubStr 子字符串
     * @param IN Int32 sSize2 子字符串缓存长度,包括尾0
     *
     * @return 字符串的开头是否符合指定子字符串
     */
    EXTERN_C Boolean WcsStartsWith(
        IN PCWStr pcwszStr,
        IN Int32 sSize1,
        IN PCWStr pcwszSubStr,
        IN Int32 sSize2);

    /**
     * 忽略英文大小写判断字符串的开头是否符合指定子字符串
     *
     * @param IN PCWStr pcwszStr 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCWStr pcwszSubStr 子字符串
     * @param IN Int32 sSize2 子字符串缓存长度,包括尾0
     *
     * @return 字符串的开头是否符合指定子字符串
     */
    EXTERN_C Boolean WcsiStartsWith(
        IN PCWStr pcwszStr,
        IN Int32 sSize1,
        IN PCWStr pcwszSubStr,
        IN Int32 sSize2);

    /**
     * 复制字符串,安全函数
     *
     * @param IN PWStr source 字符串
     * @param IN Int32 size 字符串长度,包括尾0
     *
     * @return 复制后字符串,失败返回空
     */
    EXTERN_C PWStr Wcsdup_s(
        IN PCWStr source,
        IN Int32 size);

    /**
     * 将多字节字符串转换为宽字符串
     *
     * @param IN PWStr dst 宽字符串
     * @param IN PCStr src 多字节字符串
     * @param IN Int32 count 宽字符串缓冲区长度,包括尾0
     *
     * @return 转换后宽字符串长度,失败返回-1
     */
    EXTERN_C Int32 MbsToWcs(
        IN PWStr dst,
        IN PCStr src,
        IN Int32 count);

    /**
     * 将宽字符串转换为多字节字符串
     *
     * @param IN PStr dst 多字节字符串
     * @param IN PCWStr src 宽字符串
     * @param IN Int32 count 多字节字符串缓冲区长度,包括尾0
     *
     * @return 转换后多字节字符串长度,失败返回-1
     */
    EXTERN_C Int32 WcsToMbs(
        IN PStr dst,
        IN PCWStr src,
        IN Int32 count);

    /**
     * 在字符串中定位字符位置,安全函数
     *
     * @param IN PCWStr pcwszStr 字符串
     * @param IN Int32 sSize 字符串缓存长度,包括尾0
     * @param IN WChar ch 字符
     *
     * @return PWStr 从末尾开始首次发现字符的位置,若为空表示未查找到
     */
    EXTERN_C PWStr Wcsrchr_s(
        IN PCWStr pcwszStr,
        IN Int32 sSize,
        IN WChar ch);

    /**
     * 在字符串中定位字符位置
     *
     * @param IN PCWStr pcwszStr 字符串
     * @param IN WChar ch 字符
     *
     * @return PWStr 从末尾开始首次发现字符的位置,若为空表示未查找到
     */
    EXTERN_C PWStr Wcsrchr(
        IN PCWStr pcwszStr,
        IN WChar ch);

    /**
     * 在字符串中定位子字符串中字符位置,安全函数
     *
     * @param IN PCWStr pcwszStr 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCWStr pcwszSubStr 子字符串
     * @param IN Int32 sSize2 子字符串缓存长度,不包括尾0
     *
     * @return PWStr 从末尾开始首次发现字符串的位置,若为空表示未查找到
     */
    EXTERN_C PWStr Wcsrstr_s(
        IN PCWStr pcwszStr,
        IN Int32 sSize1,
        IN PCWStr pcwszSubStr,
        IN Int32 sSize2);

    /**
     * 在字符串中定位字符列表中字符位置,安全函数
     *
     * @param IN PCWStr pcwszStr 字符串
     * @param IN Int32 sSize1 字符串缓存长度,包括尾0
     * @param IN PCWStr pcwszChars 字符列表
     * @param IN Int32 sSize2 字符列表缓存长度,不包括尾0
     *
     * @return PWStr 从末尾开始首次发现字符的位置,若为空表示未查找到
     */
    EXTERN_C PWStr Wcsrpbrk_s(
        IN PCWStr pcwszStr,
        IN Int32 sSize1,
        IN PCWStr pcwszChars,
        IN Int32 sSize2);

    enum EncodingType {
        EncodingType_ASCII = 0,
        EncodingType_GB2312,
        EncodingType_GBK,
        EncodingType_GB18030,
        EncodingType_BIG5,
        EncodingType_ISO2022JP,
        EncodingType_EUCJP,
        EncodingType_WCHAR_T,
        EncodingType_UTF8,
        EncodingType_UTF16,
        EncodingType_UTF16BE,
        EncodingType_UTF16LE,
        EncodingType_UTF32,
        EncodingType_UTF32BE,
        EncodingType_UTF32LE,
        EncodingType_UCS2,
        EncodingType_UCS2BE,
        EncodingType_UCS2LE,
        EncodingType_UCS4,
        EncodingType_UCS4BE,
        EncodingType_UCS4LE
    };

    STATIC PCStr sEncodingTypeList[] = {
        "ASCII",
        "GB2312",
        "GBK",
        "GB18030",
        "BIG-5",
        "ISO-2022-JP",
        "EUC-JP",
        "WCHAR_T",
        "UTF-8",
        "UTF-16",
        "UTF-16BE",
        "UTF-16LE",
        "UTF-32",
        "UTF-32BE",
        "UTF-32LE",
        "UCS-2-INTERNAL",
        "UCS-2BE",
        "UCS-2LE",
        "UCS-4-INTERNAL",
        "UCS-4BE",
        "UCS-4LE"
    };

    /**
     * 字符串编码转换
     *
     * @param IN fromEncoding 源编码
     * @param IN toEncoding 目的编码
     * @param INOUT inBuffer 源字符串
     * @param IN inLength 源字符串字符内存占用大小，包括尾0
     * @param INOUT outBuffer 目的字符串
     * @param INOUT outLength 目的字符串内存占用大小,包括尾0
     *
     * @return ARESULT 转换成功返回AS_OK
     */
    EXTERN_C ARESULT Convert(
        IN CONST EncodingType fromEncoding,
        IN CONST EncodingType toEncoding,
        IN PCByte inBuffer,
        IN CONST Int32 inLength,
        INOUT PByte outBuffer,
        INOUT Int32 * outLength);

    class String;
    typedef String * PString;

    class String : public ByteArray
    {
    public:

        STATIC Sp<String> Create(
            IN PCStr src)
        {
            return Create(
                (PCByte)src, sizeof(Char), Strlen(src), EncodingType_ASCII);
        }

        STATIC Sp<String> Create(
            IN PCStr src,
            IN Int32 length)
        {
            return Create(
                (PCByte)src, sizeof(Char), length, EncodingType_ASCII);
        }

        STATIC Sp<String> Create(
            IN PCWStr src)
        {
            return Create(src, Wcslen(src));
        }

        STATIC Sp<String> Create(
            IN PCWStr src,
            IN Int32 length)
        {
            if (!src)
                return NULL;

            Sp<String> ret = String::Create(length + 1);
            if (ret == NULL)
                return NULL;

            PByte data = ret->GetPayload();

            ::memcpy(data, src, length * sizeof(WChar));
            ((PWStr)data)[length] = EOS;
            ret->SetUsed(length + 1);

            return ret;
        }

        STATIC Sp<String> Create(
            IN PCByte src,
            IN Int32 size,
            IN Int32 length,
            IN EncodingType encoding)
        {
            if (!src)
                return NULL;

            Sp<String> ret = String::Create(length + 1);
            if (ret == NULL)
                return NULL;

            PByte data = ret->GetPayload();
            Int32 used = ret->GetCapacity() * sizeof(WChar);
            Int32 sizeofWChar = sizeof(WChar);
            if (sizeofWChar == 2) {
                if (AFAILED(Convert(
                    encoding,
                    EncodingType_UTF16,
                    src,
                    length * size,
                    data,
                    &used)))
                {
                    return NULL;
                }
            }
            else if (sizeofWChar == 4){
                if (AFAILED(Convert(
                    encoding,
                    EncodingType_UTF32,
                    src,
                    length * size,
                    data,
                    &used)))
                {
                    return NULL;
                }
            }
            else
                return NULL;

            memset(data + used, EOS, sizeofWChar);
            ret->SetUsed(used / sizeof(WChar) + 1);

            return ret;
        }

        STATIC Sp<String> Create(
            IN PCByte src,
            IN Int32 capacity,
            IN EncodingType encoding)
        {
            return NULL;
        }

        STATIC Sp<String> Create(
            IN CONST Sp<String> & src)
        {
            if (src == NULL)
                return NULL;

            Int32 length = src->Length();
            Sp<String> ret = String::Create(length + 1);
            if (ret == NULL)
                return NULL;

            PByte data = ret->GetPayload();
            Int32 used = src->GetUsed();

            ::memcpy(data, src->GetPayload(), used * sizeof(WChar));
            ret->SetUsed(used);

            return ret;
        }
        
        STATIC Sp<String> Clone(
            IN CONST Sp<String> & src)
        {
            return Create(src);
        }

        STATIC Sp<String> Create(
            IN Int32 length,
            IN PCStr format,
            IN ...)
        {
            Sp<ByteArray> buffer = ByteArray::Create(
                (length + 1) * sizeof(Char));
            if (buffer == NULL)
                return NULL;

            va_list arglist;
            va_start(arglist, format);

#ifdef PLATFORM_WIN32
            AXP::Int32 retLength = ::vsprintf_s(
                (PChar)buffer->GetPayload(),
                length + 1,
                format,
                arglist);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
            AXP::Int32 retLength = ::vsprintf(
                (PChar)buffer->GetPayload(), format, arglist);
#endif // PLATFORM_WIN32

            va_end(arglist);

            if (retLength < 0)
                return NULL;

            buffer->SetUsed((retLength + 1) * sizeof(Char));

            return Create(
                buffer->GetPayload(),
                sizeof(Char),
                retLength,
                EncodingType_ASCII);
        }

        STATIC Sp<String> Create(
            IN EncodingType encoding,
            IN Int32 size,
            IN Int32 length,
            IN PCStr format,
            IN ...)
        {
            Sp<ByteArray> buffer = ByteArray::Create((length + 1) * size);
            if (buffer == NULL)
                return NULL;

            va_list arglist;
            va_start(arglist, format);

#ifdef PLATFORM_WIN32
            AXP::Int32 retLength = ::vsprintf_s(
                (PChar)buffer->GetPayload(),
                length + 1,
                format,
                arglist);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
            AXP::Int32 retLength = ::vsprintf(
                (PChar)buffer->GetPayload(), format, arglist);
#endif // PLATFORM_WIN32

            va_end(arglist);

            if (retLength < 0)
                return NULL;

            buffer->SetUsed((retLength + 1) * size);

            return Create(buffer->GetPayload(), size, retLength, encoding);
        }

        STATIC Sp<String> Create(
            IN Int32 length,
            IN PCWStr format,
            IN ...)
        {
            Sp<ByteArray> buffer = ByteArray::Create(
                (length + 1) * sizeof(WChar));
            if (buffer == NULL)
                return NULL;

            va_list arglist;
            va_start(arglist, format);

#ifdef PLATFORM_WIN32
            AXP::Int32 retLength = ::vswprintf_s(
                (PWStr)buffer->GetPayload(),
                length + 1,
                format,
                arglist);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
            AXP::Int32 retLength = ::vswprintf(
                (PWStr)buffer->GetPayload(),
                length + 1,
                format,
                arglist);
#endif // PLATFORM_WIN32

            va_end(arglist);

            if (retLength < 0)
                return NULL;

            PWStr data = (PWStr)buffer->GetPayload();
            data[retLength] = EOS;

            buffer->SetUsed((retLength + 1) * sizeof(WChar));

            return Create((PCWStr)buffer->GetPayload());
        }

    public:

        Sp<ByteArray> GetBytes(
            IN EncodingType encoding) CONST
        {
            Int32 length = (Length() + 1) * 4;

            Sp<ByteArray> obj = ByteArray::Create(length);
            if (obj == NULL)
                return NULL;

            Int32 used = obj->GetCapacity();
            Int32 sizeofWChar = sizeof(WChar);
            if (sizeofWChar == 2) {
                ARESULT ar = Convert(
                    EncodingType_UTF16,
                    encoding,
                    (PCByte)GetPayload(),
                    GetUsed() * sizeof(WChar),
                    obj->GetPayload(),
                    &used);
                if (AFAILED(ar))
                    return NULL;
            }
            else if (sizeofWChar == 4) {
                ARESULT ar = Convert(
                    EncodingType_UTF32,
                    encoding,
                    (PCByte)GetPayload(),
                    GetUsed() * sizeof(WChar),
                    obj->GetPayload(),
                    &used);
                if (AFAILED(ar))
                    return NULL;
            }
            else
                return NULL;

            obj->SetUsed(used);

            return obj;
        }

        Sp<ByteArray> GetBytes() CONST
        {
            return GetBytes(EncodingType_UTF8);
        }

    public:

        Sp<String> Append(
            IN PCStr str) CONST
        {
            Sp<String> temp = String::Create(str);
            if (temp == NULL)
                return NULL;

            return Append(temp);
        }

        Sp<String> Append(
            IN PCStr str,
            IN Int32 length) CONST
        {
            Sp<String> temp = String::Create(str, length);
            if (temp == NULL)
                return NULL;

            return Append(temp);
        }

        Sp<String> Append(
            IN PCByte buffer,
            IN Int32 size,
            IN Int32 length,
            IN EncodingType encoding) CONST
        {
            Sp<String> temp = String::Create(buffer, size, length, encoding);
            if (temp == NULL)
                return NULL;

            return Append(temp);
        }

        Sp<String> Append(
            IN PCByte buffer,
            IN Int32 capacity,
            IN EncodingType encoding)
        {
            return NULL;
        }

        Sp<String> Append(
            IN PCWStr str) CONST
        {
            Sp<String> temp = String::Create(str);
            if (temp == NULL)
                return NULL;

            return Append(temp);
        }

        Sp<String> Append(
            IN PCWStr str,
            IN Int32 length) CONST
        {
            Sp<String> temp = String::Create(str, length);
            if (temp == NULL)
                return NULL;

            return Append(temp);
        }

        Sp<String> Append(
            IN CONST Sp<String> & string) CONST
        {
            if (string == NULL)
            return NULL;

            return Append(string, string->Length());
        }

        Sp<String> Append(
            IN CONST Sp<String> & string,
            IN Int32 length) CONST
        {
            return Append(string, 0, length);
        }

        Sp<String> Append(
            IN CONST Sp<String> & string,
            IN CONST Int32 startOffset,
            IN CONST Int32 length) CONST
        {
            if (string == NULL)
                return NULL;

            Sp<String> sub = string->SubString(startOffset, length);
            if (sub == NULL)
                return NULL;

            Int32 newLength = Length() + sub->Length() + 1;
            Sp<String> ret = String::Create(newLength);
            if (ret == NULL)
                return NULL;

            Int32 len;

            if (AFAILED(Wcscpy_s(
                (PWStr)ret->GetPayload(),
                newLength,
                (PWStr)GetPayload(),
                &len)))
            {
                return NULL;
            }

            Int32 used = len;
            if (AFAILED(Wcscpy_s(
                (PWStr)ret->GetPayload() + used,
                newLength - used,
                (PWStr)sub->GetPayload(),
                &len)))
            {
                return NULL;
            }

            ret->SetUsed(used + len + 1);

            return ret;
        }

        Sp<String> Append(
            IN Int32 length,
            IN PCStr format,
            IN ...) CONST
        {
            Sp<ByteArray> buffer = ByteArray::Create(
                (length + 1) * sizeof(Char));
            if (buffer == NULL)
                return NULL;

            va_list arglist;
            va_start(arglist, format);

#ifdef PLATFORM_WIN32
            AXP::Int32 retLength = ::vsprintf_s(
                (PChar)buffer->GetPayload(),
                length + 1,
                format,
                arglist);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
            AXP::Int32 retLength = ::vsprintf(
                (PChar)buffer->GetPayload(), format, arglist);
#endif // PLATFORM_WIN32

            va_end(arglist);

            if (retLength < 0)
                return NULL;

            buffer->SetUsed((retLength + 1) * sizeof(Char));

            Sp<String> temp = Create(
                buffer->GetPayload(),
                sizeof(Char),
                retLength,
                EncodingType_ASCII);
            if (temp == NULL)
                return NULL;

            return Append(temp);
        }

        Sp<String> Append(
            IN EncodingType encoding,
            IN Int32 size,
            IN Int32 length,
            IN PCStr format,
            IN ...) CONST
        {
            Sp<ByteArray> buffer = ByteArray::Create((length + 1) * size);
            if (buffer == NULL)
                return NULL;

            va_list arglist;
            va_start(arglist, format);

#ifdef PLATFORM_WIN32
            AXP::Int32 retLength = ::vsprintf_s(
                (PChar)buffer->GetPayload(),
                length + 1,
                format,
                arglist);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
            AXP::Int32 retLength = ::vsprintf(
                (PChar)buffer->GetPayload(), format, arglist);
#endif // PLATFORM_WIN32

            va_end(arglist);

            if (retLength < 0)
                return NULL;

            buffer->SetUsed((retLength + 1) * size);

            Sp<String> temp = Create(
                buffer->GetPayload(), size, retLength, encoding);
            if (temp == NULL)
                return NULL;

            return Append(temp);
        }

        Sp<String> Append(
            IN EncodingType encoding,
            IN Int32 capacity,
            IN PCStr format,
            IN ...) CONST
        {
            return NULL;
        }

        Sp<String> Append(
            IN Int32 length,
            IN PCWStr format,
            IN ...) CONST
        {
            Sp<ByteArray> buffer = ByteArray::Create(
                (length + 1) * sizeof(WChar));
            if (buffer == NULL)
                return NULL;

            va_list arglist;
            va_start(arglist, format);

#ifdef PLATFORM_WIN32
            AXP::Int32 retLength = ::vswprintf_s(
                (PWStr)buffer->GetPayload(),
                length + 1,
                format,
                arglist);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
            AXP::Int32 retLength = ::vswprintf(
                (PWStr)buffer->GetPayload(),
                length + 1,
                format,
                arglist);
#endif // PLATFORM_WIN32

            va_end(arglist);

            if (retLength < 0)
                return NULL;

            PWStr data = (PWStr)buffer->GetPayload();
            data[retLength] = EOS;

            buffer->SetUsed((retLength + 1) * sizeof(WChar));

            Sp<String> temp = Create((PCWStr)buffer->GetPayload());
            if (temp == NULL)
                return NULL;

            return Append(temp);
        }

        Boolean Contains(
            IN PCStr str) CONST
        {
            Sp<String> temp = String::Create(str);
            if (temp == NULL)
                return FALSE;

            return Contains(temp);
        }

        Boolean Contains(
            IN PCByte buffer,
            IN Int32 size,
            IN Int32 length,
            IN EncodingType encoding) CONST
        {
            Sp<String> temp = String::Create(buffer, size, length, encoding);
            if (temp == NULL)
                return FALSE;

            return Contains(temp);
        }

        Boolean Contains(
            IN PCByte buffer,
            IN Int32 capacity,
            IN EncodingType encoding) CONST
        {
            return FALSE;
        }

        Boolean Contains(
            IN PCWStr str) CONST
        {
            Sp<String> temp = String::Create(str);
            if (temp == NULL)
                return FALSE;

            return Contains(temp);
        }

        Boolean Contains(
            IN CONST Sp<String> & string) CONST
        {
            if (string == NULL)
                return FALSE;

            return (NULL != Wcsstr_s(
                (PCWStr)GetPayload(),
                Length() + 1,
                (PCWStr)string->GetPayload(),
                string->Length()));
        }

        Int32 IndexOf(
            IN Char ch) CONST
        {
            WChar c = ch;

            return IndexOf(c, 0);
        }

        Int32 IndexOf(
            IN WChar ch) CONST
        {
            return IndexOf(ch, 0);
        }

        Int32 IndexOf(
            IN Char ch,
            IN Int32 startOffset) CONST
        {
            WChar c = ch;

            return IndexOf(c, startOffset);
        }

        Int32 IndexOf(
            IN WChar ch,
            IN Int32 startOffset) CONST
        {
            if (startOffset >= Length())
                return -1;

            PCWStr p = Wcschr_s(
                (PCWStr)GetPayload() + startOffset,
                Length() + 1,
                ch);
            if (!p)
                return -1;

            return (Int32)(((PCByte)p - GetPayload()) / sizeof(WChar));
        }

        Int32 IndexOfString(
            IN PCStr str) CONST
        {
            Sp<String> temp = String::Create(str);
            if (temp == NULL)
                return -1;

            return IndexOfString(temp, 0);
        }

        Int32 IndexOfString(
            IN PCByte buffer,
            IN Int32 size,
            IN Int32 length,
            IN EncodingType encoding) CONST
        {
            Sp<String> temp = String::Create(buffer, size, length, encoding);
            if (temp == NULL)
                return -1;

            return IndexOfString(temp, 0);
        }

        Int32 IndexOfString(
            IN PCByte buffer,
            IN Int32 capacity,
            IN EncodingType encoding) CONST
        {
            return -1;
        }

        Int32 IndexOfString(
            IN PCWStr str) CONST
        {
            Sp<String> temp = String::Create(str);
            if (temp == NULL)
                return -1;

            return IndexOfString(temp, 0);
        }

        Int32 IndexOfString(
            IN PCStr str,
            IN Int32 startOffset) CONST
        {
            Sp<String> temp = String::Create(str);
            if (temp == NULL)
                return -1;

            return IndexOfString(temp, startOffset);
        }

        Int32 IndexOfString(
            IN PCByte buffer,
            IN Int32 size,
            IN Int32 length,
            IN EncodingType encoding,
            IN Int32 startOffset) CONST
        {
            Sp<String> temp = String::Create(buffer, size, length, encoding);
            if (temp == NULL)
                return -1;

            return IndexOfString(temp, startOffset);
        }

        Int32 IndexOfString(
            IN PCByte buffer,
            IN Int32 capacity,
            IN EncodingType encoding,
            IN Int32 startOffset) CONST
        {
            return -1;
        }

        Int32 IndexOfString(
            IN PCWStr str,
            IN Int32 startOffset) CONST
        {
            Sp<String> temp = String::Create(str);
            if (temp == NULL)
                return -1;

            return IndexOfString(temp, startOffset);
        }

        Int32 IndexOfString(
            IN CONST Sp<String> & str,
            IN Int32 startOffset) CONST
        {
            if ((str == NULL) || (startOffset > Length()))
                return -1;

            PCWStr p = Wcsstr_s(
                (PCWStr)GetPayload() + startOffset,
                Length() + 1 - startOffset,
                (PCWStr)str->GetPayload(),
                str->Length());
            if (!p)
                return -1;

            return (Int32)(((PCByte)p - GetPayload()) / sizeof(WChar));
        }

        Int32 IndexOfAny(
            IN PCStr pChars) CONST
        {
            Sp<String> temp = String::Create(pChars);
            if (temp == NULL)
                return -1;

            return IndexOfAny(temp);
        }

        Int32 IndexOfAny(
            IN PCByte buffer,
            IN Int32 size,
            IN Int32 length,
            IN EncodingType encoding) CONST
        {
            Sp<String> temp = String::Create(buffer, size, length, encoding);
            if (temp == NULL)
                return -1;

            return IndexOfAny(temp);
        }

        Int32 IndexOfAny(
            IN PCByte buffer,
            IN Int32 capacity,
            IN EncodingType encodingt) CONST
        {
            return -1;
        }

        Int32 IndexOfAny(
            IN PCWStr pChars) CONST
        {
            Sp<String> temp = String::Create(pChars);
            if (temp == NULL)
                return -1;

            return IndexOfAny(temp);
        }

        Int32 IndexOfAny(
            IN CONST Sp<String> & string) CONST
        {
            if (string == NULL)
                return -1;

            PWStr p = Wcspbrk_s(
                (PCWStr)GetPayload(),
                Length() + 1,
                (PCWStr)string->GetPayload(),
                string->Length());
            if (!p)
                return -1;

            return (Int32)(((PCByte)p - GetPayload()) / sizeof(WChar));
        }

        Sp<String> SubString(
            IN CONST Int32 startOffset) CONST
        {
            return SubString(startOffset, Length() - startOffset);
        }

        Sp<String> SubString(
            IN CONST Int32 startOffset,
            IN CONST Int32 length) CONST
        {
            if ((startOffset > Length())
                || ((startOffset + length) > Length()))
                return NULL;

            return String::Create((PCWStr)GetPayload() + startOffset, length);
        }

        Boolean IsNullOrEmpty() CONST
        {
            return ((GetPayload() == NULL) || (((PCWStr)GetPayload())[0] == EOS));
        }

        Boolean StartsWith(
            IN PCStr str,
            IN CONST Int32 length) CONST
        {
            Sp<String> temp = String::Create(str, length);
            if (temp == NULL)
                return FALSE;

            return StartsWith(temp);
        }

        Boolean StartsWith(
            IN PCByte buffer,
            IN Int32 size,
            IN Int32 length,
            IN EncodingType encoding) CONST
        {
            Sp<String> temp = String::Create(buffer, size, length, encoding);
            if (temp == NULL)
                return FALSE;

            return StartsWith(temp);
        }

        Boolean StartsWith(
            IN PCByte buffer,
            IN Int32 capacity,
            IN EncodingType encodingt) CONST
        {
            return FALSE;
        }

        Boolean StartsWith(
            IN PCWStr str,
            IN CONST Int32 length) CONST
        {
            Sp<String> temp = String::Create(str, length);
            if (temp == NULL)
                return FALSE;

            return StartsWith(temp);
        }

        Boolean StartsWith(
            IN CONST Sp<String> & string) CONST
        {
            return WcsStartsWith(
                (PCWStr)GetPayload(), Length(), (PCWStr)string->GetPayload(), string->Length() + 1);
        }

        Boolean StartsWithIgnoreCase(
            IN PCStr str,
            IN CONST Int32 length) CONST
        {
            Sp<String> temp = String::Create(str, length);
            if (temp == NULL)
                return FALSE;

            return StartsWithIgnoreCase(temp);
        }

        Boolean StartsWithIgnoreCase(
            IN PCByte buffer,
            IN Int32 size,
            IN CONST Int32 length,
            IN EncodingType encoding) CONST
        {
            Sp<String> temp = String::Create(buffer, size, length, encoding);
            if (temp == NULL)
                return FALSE;

            return StartsWithIgnoreCase(temp);
        }

        Boolean StartsWithIgnoreCase(
            IN PCByte buffer,
            IN Int32 capacity,
            IN EncodingType encodingt) CONST
        {
            return FALSE;
        }

        Boolean StartsWithIgnoreCase(
            IN PCWStr str,
            IN CONST Int32 length) CONST
        {
            Sp<String> temp = String::Create(str, length);
            if (temp == NULL)
                return FALSE;

            return StartsWithIgnoreCase(temp);
        }

        Boolean StartsWithIgnoreCase(
            IN CONST Sp<String> & string) CONST
        {
            if (string == NULL)
                return FALSE;

            return WcsiStartsWith((PCWStr)GetPayload(), Length(), (PCWStr)string->GetPayload(),
                string->Length() + 1);
        }

        Boolean Equals(
            IN CONST Sp<String> & string) CONST
        {
            if (string == NULL)
                return FALSE;

            return (0 == Wcscmp_s(
                (PCWStr)GetPayload(), Length() + 1,
                (PCWStr)string->GetPayload(), string->Length() + 1));
        }

        Boolean Equals(
            IN PCStr string) CONST
        {
            Sp<String> temp = String::Create(string);
            if (temp == NULL)
                return FALSE;

            return Equals(temp);
        }

        Boolean Equals(
            IN PCByte buffer,
            IN Int32 size,
            IN Int32 length,
            IN EncodingType encoding) CONST
        {
            Sp<String> temp = String::Create(buffer, size, length, encoding);
            if (temp == NULL)
                return FALSE;

            return Equals(temp);
        }

        Boolean Equals(
            IN PCByte buffer,
            IN Int32 capacity,
            IN EncodingType encodingt) CONST
        {
            return FALSE;
        }

        Boolean Equals(IN PCWStr string) CONST
        {
            Sp<String> temp = String::Create(string);
            if (temp == NULL)
                return FALSE;

            return Equals(temp);
        }

        Int32 LastIndexOf(
            IN Char ch) CONST
        {
            WChar c = ch;

            return LastIndexOf(c, 0);
        }

        Int32 LastIndexOf(
            IN WChar ch) CONST
        {
            return LastIndexOf(ch, 0);
        }

        Int32 LastIndexOf(
            IN Char ch,
            IN Int32 startOffset) CONST
        {
            WChar c = ch;

            return LastIndexOf(c, startOffset);
        }

        Int32 LastIndexOf(
            IN WChar ch,
            IN Int32 startOffset) CONST
        {
            if (startOffset >= Length())
                return -1;

            PCWStr p = Wcsrchr_s(
                (PCWStr)GetPayload() + startOffset, Length() + 1, ch);
            if (!p)
                return -1;

            return (Int32)(((PCByte)p - GetPayload()) / sizeof(WChar));
        }

        Int32 LastIndexOfString(
            IN PCStr str) CONST
        {
            Sp<String> temp = String::Create(str);
            if (temp == NULL)
                return -1;

            return LastIndexOfString(temp, 0);
        }

        Int32 LastIndexOfString(
            IN PCByte buffer,
            IN Int32 size,
            IN Int32 length,
            IN EncodingType encoding) CONST
        {
            Sp<String> temp = String::Create(buffer, size, length, encoding);
            if (temp == NULL)
                return -1;

            return LastIndexOfString(temp, 0);
        }

        Int32 LastIndexOfString(
            IN PCByte buffer,
            IN Int32 capacity,
            IN EncodingType encodingt) CONST
        {
            return -1;
        }

        Int32 LastIndexOfString(
            IN PCWStr str) CONST
        {
            Sp<String> temp = String::Create(str);
            if (temp == NULL)
                return -1;

            return LastIndexOfString(temp, 0);
        }

        Int32 LastIndexOfString(
            IN PCStr str,
            IN Int32 startOffset) CONST
        {
            Sp<String> temp = String::Create(str);
            if (temp == NULL)
                return -1;

            return LastIndexOfString(temp, startOffset);
        }

        Int32 LastIndexOfString(
            IN PCByte buffer,
            IN Int32 size,
            IN Int32 length,
            IN EncodingType encoding,
            IN Int32 startOffset) CONST
        {
            Sp<String> temp = String::Create(buffer, size, length, encoding);
            if (temp == NULL)
                return -1;

            return LastIndexOfString(temp, startOffset);
        }

        Int32 LastIndexOfString(
            IN PCWStr str,
            IN Int32 startOffset) CONST
        {
            Sp<String> temp = String::Create(str);
            if (temp == NULL)
                return -1;

            return LastIndexOfString(temp, startOffset);
        }

        Int32 LastIndexOfString(
            IN CONST Sp<String> & str,
            IN Int32 startOffset) CONST
        {
            if ((str == NULL) || (startOffset > Length()))
                return -1;

            PCWStr p = Wcsrstr_s(
                (PCWStr)GetPayload() + startOffset,
                Length() + 1 - startOffset,
                (PCWStr)str->GetPayload(),
                str->Length());
            if (!p)
                return -1;

            return (Int32)(((PCByte)p - GetPayload()) / sizeof(WChar));
        }

        Int32 LastIndexOfAny(
            IN PCStr pChars) CONST
        {
            Sp<String> temp = String::Create(pChars);
            if (temp == NULL)
                return -1;

            return LastIndexOfAny(temp);
        }

        Int32 LastIndexOfAny(
            IN PCByte buffer,
            IN Int32 size,
            IN Int32 length,
            IN EncodingType encoding) CONST
        {
            Sp<String> temp = String::Create(buffer, size, length, encoding);
            if (temp == NULL)
                return -1;

            return LastIndexOfAny(temp);
        }

        Int32 LastIndexOfAny(
            IN PCByte buffer,
            IN Int32 capacity,
            IN EncodingType encodingt) CONST
        {
            return -1;
        }

        Int32 LastIndexOfAny(
            IN PCWStr pChars) CONST
        {
            Sp<String> temp = String::Create(pChars);
            if (temp == NULL)
                return -1;

            return LastIndexOfAny(temp);
        }

        Int32 LastIndexOfAny(
            IN CONST Sp<String> & string) CONST
        {
            if (string == NULL)
                return -1;

            PWStr p = Wcsrpbrk_s(
                (PCWStr)GetPayload(),
                Length() + 1,
                (PCWStr)string->GetPayload(),
                string->Length());
            if (!p)
                return -1;

            return (Int32)(((PCByte)p - GetPayload()) / sizeof(WChar));
        }

        Sp<List<String> >Split(PCWStr seps)
        {
            if (seps == NULL)
                return NULL;

            Sp<String> ret = String::Create(this);
            if (ret == NULL)
                return NULL;

            Sp<List<String> > list = new List<String>();
            if (list == NULL)
                return NULL;

            if (seps[0] == EOS)
                return NULL;

#ifdef PLATFORM_WIN32
            PWStr nextToken = NULL;
            PWStr token = ::wcstok_s((PWStr)ret->GetPayload(), (PCWStr)seps, &nextToken);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
            PWStr last = NULL;
            PWStr token = ::wcstok((PWStr)ret->GetPayload(), (PCWStr)seps, &last);
#endif // PLATFORM_WIN32

            while (token != NULL) {
                Sp<String> str = String::Create(token);
                if (str == NULL)
                    return NULL;

                list->PushBack(str);

#ifdef PLATFORM_WIN32
                token = ::wcstok_s(NULL, (PCWStr)seps, &nextToken);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
                token = ::wcstok(NULL, (PCWStr)seps, &last);
#endif // PLATFORM_WIN32
            }

            return list;
        }

        Int32 Length() CONST
        {
            if (mUsed < 1)
                return 0;
            else
                return (mUsed - 1);
        }

    public:

        operator PCWStr() CONST
        {
            return (PCWStr)GetPayload();
        }

    protected:

        String() {}

    private:

        STATIC Sp<String> Create(
            IN Int32 length)
        {
            if (length == 0)
                return NULL;

            Sp<String> obj = new String();
            if (obj == NULL)
                return NULL;

            obj->mData = ::malloc(sizeof(WChar)* length);
            if (!obj->mData)
                return NULL;

            ::memset(obj->mData, EOS, sizeof(WChar));
            obj->mUsed = 0;
            obj->mCapacity = length;

            return obj;
        }
    };
} // namespace AXP

#endif // __ASTRING_H__
