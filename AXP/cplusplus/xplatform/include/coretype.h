
#ifndef __CORETYPE_H__
#define __CORETYPE_H__

#ifdef PLATFORM_WIN32
#include <malloc.h>
#elif defined(PLATFORM_IOS)
#include <new>
#include <memory>
#elif defined(PLATFORM_LINUX)
#include <malloc.h>
#include <string.h>
#endif // PLATFORM_WIN32

#include "type.h"
#include "define.h"
#include "aresult.h"
#include "object.h"

namespace AXP {
    template <typename T>
    class Array : public CObject
    {
    public:

        VIRTUAL ~Array()
        {
            if (mData)
                ::free(mData);

            mData = NULL;
        }

        T * GetPayload() CONST
        {
            return (T*)mData;
        }

        Int32 GetCapacity() CONST
        {
            return mCapacity;
        }

        Int32 GetUsed() CONST
        {
            return (mData ? mUsed : 0);
        }

        Int32 SetUsed(
            IN Int32 sUsed)
        {
            sUsed = MIN(sUsed, GetCapacity());
            mUsed = sUsed;

            return sUsed;
        }

        Boolean IsEmpty() CONST
        {
            return (NULL == (PVoid)mData);
        }

        ARESULT Copy(
            IN CONST Sp<Array<T> > & src)
        {
            return Copy(src, 0, src->mUsed);
        }

        ARESULT Copy(
            IN CONST Sp<Array<T> > & src,
            IN Int32 count)
        {
            if (count > src->mUsed)
                return AE_INVALIDARG;

            return Copy((CONST T *)src->mData, 0, count);
        }

        ARESULT Copy(
            IN CONST Sp<Array<T> > & src,
            IN Int32 start,
            IN Int32 end)
        {
            if ((start > src->mUsed) || (end > src->mUsed))
                return AE_INVALIDARG;

            Int32 count = end - start;
            if (count > src->mUsed)
                return AE_INVALIDARG;

            return Copy((CONST T *)src->mData, start, end);
        }

        ARESULT Copy(
            IN CONST T * p,
            IN Int32 count)
        {
            return Copy(p, 0, count);
        }

        ARESULT Copy(
            IN CONST T * p,
            IN Int32 start,
            IN Int32 end)
        {
            if ((!p) || (start < 0) || (start > end))
                return AE_INVALIDARG;

            Int32 count = end - start;
            if (count <= 0) {
                mUsed = 0;
                return AS_OK;
            }
            else if (count > mCapacity) {
                return AE_BUFFERTOOSMALL;
            }
            else {
                ::memcpy(mData, (PVoid)(p + start), sizeof(T)* count);
                mUsed = count;
                return AS_OK;
            }
        }

        ARESULT Append(
            IN CONST Sp<Array<T> > & src)
        {
            return Append(src, 0, src->mUsed);
        }

        ARESULT Append(
            IN CONST Sp<Array<T> > & src,
            IN Int32 count)
        {
            if (count > src->mUsed)
                return AE_INVALIDARG;

            return Append(src, 0, count);
        }

        ARESULT Append(
            IN CONST Sp<Array<T> > & src,
            IN Int32 start,
            IN Int32 end)
        {
            if ((start > src->mUsed) || (end > src->mUsed))
                return AE_INVALIDARG;

            Int32 count = end - start;
            if (count > src->mUsed)
                return AE_INVALIDARG;

            return Append((CONST T *)src->mData, start, end);
        }

        ARESULT Append(
            IN CONST T * p,
            IN Int32 count)
        {
            return Append(p, 0, count);
        }

        ARESULT Append(
            IN CONST T * p,
            IN Int32 start,
            IN Int32 end)
        {
            if ((!p) || (start < 0) || (start > end))
                return AE_INVALIDARG;

            Int32 count = end - start;
            if (count <= 0)
                return AS_OK;

            if ((mCapacity - mUsed) < count) {
                Int32 newSize = mUsed + count;
                Address p = ::realloc(mData, newSize * sizeof(T));
                if (!p)
                    return AE_OUTOFMEMORY;

                mData = p;
                mCapacity = newSize;
            }

            ::memcpy(
                (PVoid)((T*)mData + mUsed),
                (PVoid)(p + start),
                sizeof(T)* count);

            mUsed += count;

            return AS_OK;
        }

        Sp<Array<T> > Clone()
        {
            Sp<Array<T> > obj = Create(mUsed);
            if (obj == NULL)
                return NULL;

            if (AFAILED(obj->Copy(this)))
                return NULL;

            return obj;
        }

        T & operator [](
            IN Int32 index)
        {
            ASSERT(mData && (index >= 0) && ((UInt32)index < GetUsed()));

            return ((T*)mData)[index];
        }

        CONST T & operator [](
            IN Int32 index) CONST
        {
            ASSERT(mData && (index >= 0) && ((UInt32)index < GetUsed()));

            return ((T*)mData)[index];
        }

        STATIC Sp<Array<T> > Create(
            IN Int32 capacity)
        {
            Sp<Array<T> > obj = new Array<T>();
            if (obj == NULL)
                return NULL;

            obj->mData = ::malloc(sizeof(T) * capacity);
            if (!obj->mData)
                return NULL;

            obj->mUsed = 0;
            obj->mCapacity = capacity;

            return obj;
        }

    protected:

        Array() {}
        Array & operator = (CONST Array & rArray) {}
        Array(CONST Array & rArray) {}

    protected:

        Address mData;
        Int32 mUsed;
        Int32 mCapacity;
    };   

    class ByteArray : public Array<Byte>
    {
    public:

        ARESULT Copy(
            IN CONST Sp<ByteArray> & src)
        {
            if (src == NULL)
                return AE_INVALIDARG;

            return Array<Byte>::Copy(src->GetPayload(), 0, src->GetUsed());
        }

        ARESULT Copy(
            IN CONST Sp<ByteArray> & src,
            IN Int32 start,
            IN Int32 end)
        {
            if (src == NULL)
                return AE_INVALIDARG;

            return Array<Byte>::Copy(src->GetPayload(), start, end);
        }

        ARESULT Copy(
            IN PCByte p,
            IN Int32 count)
        {
            return Array<Byte>::Copy(p, count);
        }

        ARESULT Append(
            IN CONST Sp<ByteArray> & src)
        {
            if (src == NULL)
                return AE_INVALIDARG;

            return Array<Byte>::Append(src->GetPayload(), src->GetUsed());
        }

        ARESULT Append(
            IN PCByte p,
            IN Int32 count)
        {
            return Array<Byte>::Append(p, count);
        }

        Sp<ByteArray> Clone()
        {
            Sp<ByteArray> obj = Create(mUsed);
            if (obj == NULL)
                return NULL;

            if (AFAILED(obj->Copy(this)))
                return NULL;

            return obj;
        }

        STATIC Sp<ByteArray> Create(
            IN Int32 capacity)
        {
            if (capacity <= 0)
                return NULL;

            Sp<ByteArray> obj = new ByteArray();
            if (obj == NULL)
                return NULL;

            obj->mData = ::malloc(sizeof(Byte) * capacity);
            if (!obj->mData)
                return NULL;

            obj->mUsed = 0;
            obj->mCapacity = capacity;

            return obj;
        }

        STATIC Sp<ByteArray> Clone(
            IN CONST Sp<ByteArray> & src)
        {   
            if (src == NULL) 
                return NULL;

            Sp<ByteArray> obj = Create(src->mUsed);
            if (obj == NULL) 
                return NULL;

            obj->Copy(src);
            return obj;
        }


    protected:

        ByteArray() {}
        ByteArray(CONST ByteArray & rByteArray) {}

    public:

        operator CONST PCByte() CONST
        {
            return (PByte)mData;
        }
    };

    typedef ByteArray * PByteArray;
} // namespace AXP

#endif // __CORETYPE_H__
