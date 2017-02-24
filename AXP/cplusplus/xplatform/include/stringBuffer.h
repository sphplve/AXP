
#ifndef __STRING_BUFFER_H__
#define __STRING_BUFFER_H__

#include "vbuf.h"
#include "astring.h"

namespace AXP {
#define STRING_BUFFER_DEFAULT (16)

    class StringBuffer : public CObject
    {
    public:

        STATIC Sp<StringBuffer> Create()
        {
            return StringBuffer::Create<STRING_BUFFER_DEFAULT>();
        }

        template <UInt32 DefaultLength>
        STATIC Sp<StringBuffer> Create()
        {
            Sp<StringBuffer> obj = new StringBuffer();
            if (!obj)
                return NULL;

            obj->mBuf = new VBuf_<DefaultLength>();
            if (obj->mBuf == NULL)
                return NULL;

            ::memset((PVoid)obj->GetPayload(), '\0', sizeof(WChar));

            return obj;
        }

        STATIC Sp<StringBuffer> Create(PCStr src)
        {
            return StringBuffer::Create(String::Create(src));
        }

        STATIC Sp<StringBuffer> Create(PCWStr src)
        {
            return StringBuffer::Create(String::Create(src));
        }

        STATIC Sp<StringBuffer> Create(CONST Sp<String> & string)
        {
            if (string == NULL)
                return NULL;

            Sp<StringBuffer> obj = Create();
            if (!obj)
                return NULL;

            Boolean boolean = obj->Append(string);
            if (!boolean)
                return NULL;

            return obj;
        }

    public:

        Boolean Append(
            IN PCStr src)
        {
            return Append(String::Create(src));
        }

        Boolean Append(
            IN PCWStr src)
        {
            Int32 used = mBuf->GetUsed();
            if (used > 0)
                mBuf->SetUsed(used - sizeof(WChar));

            return mBuf->Append((PByte)src, (Wcslen(src) + 1) * sizeof(WChar));
        }

        Boolean Append(
            IN CONST Sp<String> & src)
        {
            if (src == NULL)
                return FALSE;

            return Append((PCWStr)*src);
        }

        Boolean Append(
            IN CONST Int32 capacity,
            IN PCStr format,
            IN ...)
        {
            Sp<ByteArray> buffer = ByteArray::Create(
                (capacity + 1) * sizeof(Char));
            if (buffer == NULL)
                return FALSE;

            va_list arglist;
            va_start(arglist, format);

#ifdef PLATFORM_WIN32
            AXP::Int32 retLength = ::vsprintf_s(
                (PChar)buffer->GetPayload(),
                capacity + 1,
                format,
                arglist);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
            AXP::Int32 retLength = ::vsprintf(
                (PChar)buffer->GetPayload(), format, arglist);
#endif // PLATFORM_WIN32

            va_end(arglist);

            if (retLength < 0)
                return FALSE;

            buffer->SetUsed((retLength + 1) * sizeof(Char));
            Sp<String> str = String::Create(
                buffer->GetPayload(),
                sizeof(Char),
                retLength,
                EncodingType_ASCII);

            return Append((PCWStr)*str);
        }

        Boolean Append(
            IN CONST Int32 capacity,
            IN PCWStr format,
            IN ...)
        {
            Sp<ByteArray> buffer = ByteArray::Create(
                (capacity + 1) * sizeof(WChar));
            if (buffer == NULL)
                return FALSE;

            va_list arglist;
            va_start(arglist, format);

#ifdef PLATFORM_WIN32
            AXP::Int32 retLength = ::vswprintf_s(
                (PWStr)buffer->GetPayload(),
                capacity + 1,
                format,
                arglist);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
            AXP::Int32 retLength = ::vswprintf(
                (PWStr)buffer->GetPayload(),
                capacity + 1,
                format,
                arglist);
#endif // PLATFORM_WIN32

            va_end(arglist);

            if (retLength < 0)
                return FALSE;

            buffer->SetUsed((retLength + 1) * sizeof(WChar));
            return Append((PCWStr)buffer->GetPayload());
        }

        PCWStr GetPayload() CONST
        {
            return (PCWStr)mBuf->GetPayload();
        }

        Int32 GetCapacity() CONST
        {
            return (mBuf->GetCapacity() / sizeof(WChar));
        }

        Int32 Length() CONST
        {
            return (mBuf->GetUsed() / sizeof(WChar) - 1);
        }

        Boolean Contains(PCWStr subStr) CONST
        {
            if (!subStr)
            return FALSE;

            Int32 length = Wcslen((PCWStr)subStr);
            return (NULL != Wcsstr_s(
                (PCWStr)mBuf->GetPayload(), mBuf->GetUsed(), (PCWStr)subStr, length));
        }

        Boolean Equals(
            IN CONST StringBuffer * stringBuffer) CONST
        {
            return (0 == Wcscmp((PCWStr)mBuf->GetPayload(), (PCWStr)stringBuffer->GetPayload()));
        }

        Boolean Equals(
            IN PCWStr string) CONST
        {
            return (0 == Wcscmp((PCWStr)mBuf->GetPayload(), (PCWStr)string));
        }

        Int32 IndexOf(
            IN CONST WChar ch) CONST
        {
            return IndexOf(ch, 0);
        }

        Int32 IndexOf(
            IN CONST WChar ch,
            IN CONST Int32 sStart) CONST
        {
            if (sStart >= Length())
                return -1;

            PVoid data = (PVoid)mBuf->GetPayload();
            PCWStr pwszStr = Wcschr_s(
                (PCWStr)data + sStart, Length() + 1 - sStart, (WChar)ch);
            if (!pwszStr)
                return -1;
            else
                return ((Int32)(pwszStr - (PCWStr)data));

        }

        Int32 IndexOfString(
            IN PCWStr src) CONST
        {
            return IndexOfString(src, 0);
        }

        Int32 IndexOfString(
            IN PCWStr src,
            IN CONST Int32 sStart) CONST
        {
            if (!src)
                return -1;

            if (sStart >= Length())
                return -1;

            PVoid data = (PVoid)mBuf->GetPayload();
            Int32 length = Wcslen(src);

            PCWStr pwszStr = Wcsstr_s(
                (PCWStr)data + sStart, Length() + 1 - sStart, (PCWStr)src, length);
            if (!pwszStr)
                return -1;
            else
                return ((Int32)(pwszStr - (PCWStr)data));
        }

        Int32 IndexOfAny(
            IN PCWStr pChars) CONST
        {
            if (!pChars)
            return -1;

            PVoid data = (PVoid)mBuf->GetPayload();
            Int32 length = Wcslen((PCWStr)pChars);

            PWStr pwszStr = Wcspbrk_s(
                (PCWStr)data, Length() + 1, (PCWStr)pChars, length);
            if (!pwszStr)
                return -1;
            else
                return ((Int32)(pwszStr - (PCWStr)data));
        }

        Int32 IndexOfAny(
            IN CONST Sp<String> & string) CONST
        {
            return IndexOfAny((PCWStr)*string);
        }

        Sp<String> SubString(
            IN CONST Int32 start) CONST
        {
            return SubString(start, Length() - start);
        }

        Sp<String> SubString(
            IN CONST Int32 start,
            IN CONST Int32 length) CONST
        {
            if ((start > Length())
            || ((start + length) > Length()))
                return NULL;

            return String::Create((PCWStr)mBuf->GetPayload() + start, length);
        }

        Boolean IsNullOrEmpty() CONST
        {
            PVoid data = (PVoid)mBuf->GetPayload();

            return ((data == NULL) || (((PCWStr)data)[0] == EOS));
        }

        Boolean StartsWith(
            IN PCWStr str) CONST
        {
            if (!str)
                return FALSE;

            return StartsWith(str, Wcslen(str));
        }

        Boolean StartsWith(
            IN PCWStr str,
            IN CONST Int32 length) CONST
        {
            return WcsStartsWith((PCWStr)mBuf->GetPayload(),
                Length() + 1,
                (PCWStr)str,
                length + 1);
        }

        Boolean StartsWithIgnoreCase(
            IN PCWStr str) CONST
        {
            if (!str)
                return FALSE;

            return StartsWithIgnoreCase(str, Wcslen(str));
        }

        Boolean StartsWithIgnoreCase(
            IN PCWStr str,
            IN CONST Int32 length) CONST
        {
            PVoid data = (PVoid)mBuf->GetPayload();
            return WcsiStartsWith((PCWStr)data,
                Length() + 1,
                (PCWStr)str,
                length + 1);
        }

        Int32 LastIndexOf(
            IN WChar ch) CONST
        {
            return LastIndexOf(ch, 0);
        }

        Int32 LastIndexOf(
            IN CONST WChar ch,
            IN CONST Int32 sStart) CONST
        {
            if (sStart >= Length())
                return -1;

            PVoid data = (PVoid)mBuf->GetPayload();
            PWStr pwszStr = Wcsrchr_s(
                (PCWStr)data + sStart, Length() + 1 - sStart, (WChar)ch);
            if (!pwszStr)
                return -1;
            else
                return ((Int32)(pwszStr - (PCWStr)data));
        }

        Int32 LastIndexOfString(
            IN PCWStr str) CONST
        {
            return LastIndexOfString(str, 0);
        }

        Int32 LastIndexOfString(
            IN PCWStr str,
            IN CONST Int32 sStart) CONST
        {
            if (!str || sStart >= Length())
                return -1;

            PVoid data = (PVoid)mBuf->GetPayload();
            Int32 length = Wcslen((PCWStr)str);

            PWStr pwszStr = Wcsrstr_s(
                (PCWStr)data + sStart, Length() + 1 - sStart, (PCWStr)str, length);
            if (!pwszStr)
                return -1;
            else
                return ((Int32)(pwszStr - (PCWStr)data));
        }

        Int32 LastIndexOfAny(
            IN PCWStr pChars) CONST
        {
            if (!pChars)
                return -1;

            PVoid data = (PVoid)mBuf->GetPayload();
            Int32 length = Wcslen((PCWStr)pChars);

            PWStr pwszStr = Wcsrpbrk_s(
                (PCWStr)data, Length(), (PCWStr)pChars, length);
            if (!pwszStr)
                return -1;
            else
                return ((Int32)(pwszStr - (PCWStr)data));
        }

        Int32 LastIndexOfAny(
            IN CONST Sp<String> & string) CONST
        {
            if (!string)
                return -1;

            return LastIndexOfAny((PCWStr)*string);
        }

        Boolean Remove(
            IN CONST Int32 start)
        {
            if (start >= Length())
                return FALSE;

            PVoid data = (PVoid)mBuf->GetPayload();
            if (!data)
                return FALSE;

            ::memset((PWStr)data + start, EOS, sizeof(WChar));

            mBuf->SetUsed(start * sizeof(WChar));

            return TRUE;
        }

        Boolean Remove(
            IN CONST Int32 start,
            IN CONST Int32 length)
        {
            if ((start > Length())
                || ((start + length) > Length()))
                return FALSE;

            PWStr data = (PWStr)GetPayload() + start;
            PWStr temp = (PWStr)GetPayload() + start + length;
            Int32 len = Length() - start - length;

            while (len > 0) {
                *data++ = *temp++;
                len--;
            }

            *data = EOS;

            mBuf->SetUsed((Length() + 1 - length) * sizeof(WChar));

            return TRUE;
        }

    public:

        operator PCWStr ()
        {
            return (PWStr)mBuf->GetPayload();
        }


    protected:

        StringBuffer()
        {}

    private:

        Sp<IVBuf> mBuf;
    };

    typedef StringBuffer * PStringBuffer;
}

#endif // __STRING_BUFFER_H__