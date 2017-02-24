
#ifndef __MEMORY_STREAM_H__
#define __MEMORY_STREAM_H__

#if defined(PLATFORM_WIN32) || defined(PLATFORM_LINUX)
#include <malloc.h>
#include <memory.h>
#elif defined(PLATFORM_IOS) 
#include <memory.h>
#endif // PLATFORM_WIN32

#include "type.h"
#include "define.h"
#include "object.h"

#define MEMORY_STREAM_ALIGN (4 * 1024)

namespace AXP {
    enum SeekOrigin
    {
        Begin = 0,
        Current,
        End
    };

    template <UInt32 DefaultCapacity>
    class MemoryStream : public CObject
    {
    public:

        MemoryStream()
        {
            mUseBuffer1 = TRUE;
            mBuffer2 = NULL;
            mCapacity = DefaultCapacity;
            mLength = 0;
            mPosition = 0;
            mLastError = 0;
        }

        VIRTUAL ~MemoryStream()
        {
            if (mBuffer2)
                free(mBuffer2);
        }

        PCByte STDCALL GetPayload() CONST
        {
            if (mUseBuffer1)
                return mBuffer1;
            else
                return mBuffer2;
        }

        Int64 STDCALL GetCapacity() CONST
        {
            return mCapacity;
        }

        Int64 STDCALL GetLength() CONST
        {
            return mLength;
        }

        Int64 STDCALL GetPosition() CONST
        {
            return mPosition;
        }

        Int64 STDCALL SetPosition(Int64 position)
        {
            if (position < 0)
                mPosition = 0;
            else
                mPosition = MIN(position, mCapacity);

            return mPosition;
        }

        Int64 STDCALL GetLastError() CONST
        {
            return mLastError;
        }

        Int64 STDCALL Seek(Int64 offset, SeekOrigin loc)
        {
            if (loc == Begin)
                return SetPosition(offset);
            else if (loc == Current)
                return SetPosition(mPosition + offset);
            else if (loc == End)
                return SetPosition(mLength + offset);

            return -1;
        }

        Boolean STDCALL Write(PCByte data, Int64 length)
        {
            if (mPosition + length >= mCapacity) {
                if (mUseBuffer1) {
                    mCapacity = RoundUp(mPosition + length, MEMORY_STREAM_ALIGN);
                    mBuffer2 = (PByte)::malloc((Size)mCapacity);
                    if (!mBuffer2) {
                        mLastError = -1;
                        return FALSE;
                    }
                    else {
                        ::memcpy((PVoid)mBuffer2, (PVoid)mBuffer1, (Size)mPosition);
                        ::memcpy((PVoid)(mBuffer2 + mPosition), data, (Size)length);
                        mPosition += length;
                        mUseBuffer1 = FALSE;
                    }
                }
                else {
                    mCapacity = RoundUp(mPosition + length, MEMORY_STREAM_ALIGN);
                    PByte tmp = (PByte)::malloc((Size)mCapacity);
                    if (!tmp) {
                        mLastError = -1;
                        return FALSE;
                    }
                    else {
                        ::memcpy((PVoid)tmp, (PVoid)mBuffer2, (Size)mPosition);
                        ::memcpy((PVoid)(tmp + mPosition), data, (Size)length);
                        free(mBuffer2);
                        mBuffer2 = tmp;
                        mPosition += length;
                    }
                }
            }
            else {
                if (mUseBuffer1)
                    ::memcpy((PVoid)(mBuffer1 + mPosition), data, (Size)length);
                else
                    ::memcpy((PVoid)(mBuffer2 + mPosition), data, (Size)length);

                mPosition += length;
            }

            mLength = mPosition;

            return TRUE;
        }

        Boolean STDCALL Read(PByte dst, Int64 dstLength, Int64 length)
        {
            if ((dstLength < length)
                || (mLength < (mPosition + length)))
                return FALSE;

            PCByte data = GetPayload();
            ::memcpy((PVoid)dst, (PVoid)(data + mPosition), (Size)length);

            mPosition += length;

            return TRUE;
        }

        Boolean STDCALL Read(Int64 length)
        {
            if ((length > mLength)
                || (mLength < (mPosition + length)))
                return FALSE;

            mPosition += length;

            return TRUE;
        }

        Void STDCALL Reset()
        {
            if (mBuffer2) {
                free(mBuffer2);
                mBuffer2 = NULL;
            }

            mUseBuffer1 = TRUE;
            mCapacity = DefaultCapacity;
            mLength = 0;
            mPosition = 0;
            mLastError = 0;
        }

        INLINE Boolean STDCALL IsEmpty() CONST
        {
            return (mLength == 0);
        }

    protected:

        Byte mBuffer1[DefaultCapacity];
        PByte mBuffer2;
        Boolean mUseBuffer1;
        Int64 mCapacity;
        Int64 mLength;
        Int64 mPosition;
        Int64 mLastError;
    };
} // namespace AXP

#endif // __MEMORY_STREAM_H__
