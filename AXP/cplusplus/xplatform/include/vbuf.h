
#ifndef __VBUF_H__
#define __VBUF_H__

#ifdef PLATFORM_WIN32
#include <malloc.h>
#include <memory.h>
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
#include <memory.h>
#endif // PLATFORM_WIN32

#ifdef PLATFORM_LINUX
#include <malloc.h>
#endif

#include "type.h"
#include "define.h"
#include "object.h"

#define VBUF_ALIGN (4 * 1024)

namespace AXP {
    class IVBuf : public CObject
    {
    public:

        VIRTUAL Boolean STDCALL Append(PCByte data, Int64 length) = 0;
        VIRTUAL Void STDCALL Clear() = 0;
        VIRTUAL Void STDCALL Reset() = 0;
        VIRTUAL PCByte STDCALL GetPayload() CONST = 0;
        VIRTUAL Int64 STDCALL GetLastError() CONST = 0;
        VIRTUAL Int64 STDCALL GetUsed() CONST = 0;
        VIRTUAL Int64 STDCALL GetCapacity() CONST = 0;
        VIRTUAL Int64 STDCALL SetUsed(Int64 used) = 0;
    };

    template <UInt32 DefaultLength>
    class VBuf_ : public IVBuf
    {
    public:

        VBuf_()
        {
            mUseBuffer1 = TRUE;
            mBuffer2 = NULL;
            mCapacity = DefaultLength;
            mUsed = 0;
            mLastError = 0;
        }

        VIRTUAL ~VBuf_()
        {
            if (mBuffer2)
                free(mBuffer2);
        }

        Boolean STDCALL Append(PCByte data, Int64 length)
        {
            if (mUsed + length >= mCapacity) {
                if (mUseBuffer1) {
                    mCapacity = RoundUp(mUsed + length, VBUF_ALIGN);
                    mBuffer2 = (PByte)::malloc((Size)mCapacity);
                    if (!mBuffer2) {
                        mLastError = -1;
                        return FALSE;
                    }
                    else {
                        ::memcpy((PVoid)mBuffer2, (PVoid)mBuffer1, (Size)mUsed);
                        ::memcpy((PVoid)(mBuffer2 + mUsed), data, (Size)length);
                        mUsed += length;
                        mUseBuffer1 = FALSE;
                    }
                }
                else {
                    mCapacity = RoundUp(mUsed + length, VBUF_ALIGN);
                    PByte tmp = (PByte)::malloc((Size)mCapacity);
                    if (!tmp) {
                        mLastError = -1;
                        return FALSE;
                    }
                    else {
                        ::memcpy((PVoid)tmp, (PVoid)mBuffer2, (Size)mUsed);
                        ::memcpy((PVoid)(tmp + mUsed), data, (Size)length);
                        free(mBuffer2);
                        mBuffer2 = tmp;
                        mUsed += length;
                    }
                }
            }
            else {
                if (mUseBuffer1)
                    ::memcpy((PVoid)(mBuffer1 + mUsed), data, (Size)length);
                else
                    ::memcpy((PVoid)(mBuffer2 + mUsed), data, (Size)length);

                mUsed += length;
            }

            return TRUE;
        }

        Void STDCALL Clear()
        {
            mUsed = 0;
            mLastError = 0;
        }

        Void STDCALL Reset()
        {
            if (mBuffer2) {
                free(mBuffer2);
                mBuffer2 = NULL;
            }

            mUseBuffer1 = TRUE;
            mCapacity = DefaultLength;
            mUsed = 0;
            mLastError = 0;
        }

        PCByte STDCALL GetPayload() CONST
        {
            if (mUseBuffer1)
                return mBuffer1;
            else
                return mBuffer2;
        }

        Int64 STDCALL GetLastError() CONST
        {
            return mLastError;
        }

        Int64 STDCALL GetUsed() CONST
        {
            return mUsed;
        }

        Int64 STDCALL GetCapacity() CONST
        {
            return mCapacity;
        }

        VIRTUAL Int64 STDCALL SetUsed(Int64 used)
        {
            used = MIN(used, GetCapacity());
            mUsed = used;

            return used;
        }

    protected:

        Byte mBuffer1[DefaultLength];
        PByte mBuffer2;
        Boolean mUseBuffer1;
        Int64 mCapacity;
        Int64 mUsed;
        Int64 mLastError;
    };

    class VBuf : public VBuf_<VBUF_ALIGN>
    {
    };
} // namespace AXP

#endif // __VBUF_H__
