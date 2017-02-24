
#ifndef __STREAM_BUF_H__
#define __STREAM_BUF_H__

#ifdef PLATFORM_WIN32
#include <malloc.h>
#include <wchar.h>
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
#include <memory.h>
#endif // PLATFORM_WIN32

#include "type.h"
#include "define.h"
#include "object.h"

namespace AXP {
    template <UInt32 capacity>
    class StreamBuffer : public CObject
    {
    public:

        StreamBuffer()
        {
            Init();
        }

    public:
        
        Void Init()
        {
            mUsed = 0;
            mCapacity = capacity;
        }

        Void PushBack(PCByte data, Int64 length)
        {
            if ((mUsed + length) > mCapacity) {
                if (length >= mCapacity) {
                    ::memcpy(mBuffer, data + (length - mCapacity), (Size)mCapacity);
                }
                else {
                    Int64 move = mCapacity - length;
                    Int64 offset = mUsed - move;
                    ::memmove(mBuffer, mBuffer + offset, (Size)move);
                    ::memcpy(mBuffer + move, data, (Size)length);
                }

                mUsed = mCapacity;
            }
            else {
                ::memcpy(mBuffer + mUsed, data, (Size)length);
                mUsed += length;
            }
        }

        Void PopFirst(Int64 length)
        {
            if (length > mUsed)
                return;

            ::memmove(mBuffer, mBuffer + length, (Size)(mUsed - length));
            mUsed -= length;
        }
        
        Int64 PopFirst(PByte buffer, Int64 length)
        {
            if (mUsed == 0)
                return 0;
            
            if (length > mUsed)
                length = mUsed;
            
            ::memcpy(buffer, mBuffer, (Size)length);
            ::memmove(mBuffer, mBuffer + length, (Size)(mUsed - length));
            mUsed -= length;
            
            return length;
        }

        Void Reset()
        {
            mUsed = 0;
        }

        INLINE PCByte GetPayload() CONST
        {
            return mBuffer;
        }

        INLINE Boolean IsEmpty() CONST
        {
            return (mUsed == 0);
        }

        INLINE Int64 GetUsed() CONST
        {
            return mUsed;
        }

        INLINE Int64 GetCapacity() CONST
        {
            return mCapacity;
        }

    protected:

        Byte mBuffer[capacity];
        Int64 mUsed;
        Int64 mCapacity;
    };
}

#endif // __STREAM_BUF_H__
