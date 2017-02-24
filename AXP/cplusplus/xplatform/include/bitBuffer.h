
#ifndef __BIT_BUFFER_H__
#define __BIT_BUFFER_H__

#include "type.h"
#include "object.h"
#include "astring.h"
#include "parcelable.h"
#include "parcel.h"

namespace AXP {
    class BitBuffer : public CObject, public IParcelable
    {
    public:

        STATIC Sp<BitBuffer> Create(
            IN Int32 length)
        {
            Sp<BitBuffer> obj = new BitBuffer();
            if (obj == NULL)
                return NULL;

            if (AFAILED(obj->Init(length)))
                return NULL;

            return obj;
        }

        STATIC Sp<BitBuffer> Create(
            IN PCByte data,
            IN Int32 dataLength,
            IN Int32 bitLength,
            IN Int32 bitStartOffset,
            IN Boolean isByteHigh)
        {
            Sp<BitBuffer> obj = new BitBuffer();
            if (obj == NULL)
                return NULL;

            if (AFAILED(obj->Init(
                data, dataLength, bitLength, bitStartOffset, isByteHigh)))
                return NULL;

            return obj;
        }

        VIRTUAL ~BitBuffer()
        {
            if (mBitBuffer)
                free(mBitBuffer);
        }

        Sp<BitBuffer> GetBitBuffer(
            IN Int32 bitLength)
        {
            if ((!mBitBuffer)
                || (bitLength < 1)
                || ((mPosition + bitLength) > mBitLength)) {
                if ((mPosition + bitLength) > mBitLength)
                    mPosition = mBitLength;

                return NULL;
            }

            Sp<BitBuffer> bitBuffer = new BitBuffer();
            if (bitBuffer == NULL)
                return NULL;

            bitBuffer->mBitBuffer = (PByte)malloc(bitLength);
            if (!bitBuffer->mBitBuffer)
                return NULL;

            bitBuffer->mStartOffset = mStartOffset;
            bitBuffer->mBitLength = bitLength;
            bitBuffer->mIsByteHigh = mIsByteHigh;
            PByte bits = mBitBuffer + mPosition;
            ::memcpy(bitBuffer->mBitBuffer, bits, bitLength);
            mPosition += bitLength;

            return bitBuffer;
        }

        Sp<BitBuffer> Append(
            IN CONST Sp<BitBuffer> & bitBuffer)
        {
            if ((bitBuffer == NULL) || (mIsByteHigh != bitBuffer->mIsByteHigh))
                return NULL;

            Sp<BitBuffer> temp = new BitBuffer();
            if (temp == NULL)
                return NULL;

            if ((mBitLength + bitBuffer->mBitLength) <= 0)
                return NULL;

            temp->mBitBuffer = (PByte)malloc(mBitLength + bitBuffer->mBitLength);
            if (!temp->mBitBuffer)
                return NULL;

            temp->mStartOffset = 0;
            temp->mBitLength = mBitLength + bitBuffer->mBitLength;
            temp->mIsByteHigh = mIsByteHigh;
            ::memcpy(temp->mBitBuffer, mBitBuffer, mBitLength);
            ::memcpy(temp->mBitBuffer + mBitLength, bitBuffer->mBitBuffer, bitBuffer->mBitLength);
            temp->mPosition = mPosition;

            return temp;
        }

        UInt8 GetUInt8(
            IN Int32 length,
            IN Boolean isPos)
        {
            if ((!mBitBuffer)
                || (length < 1)
                || (length > 8)) {
                return 0;
            }

            if ((mPosition + length) > mBitLength)
                length = mBitLength - mPosition;

            PByte bits = mBitBuffer + mPosition;
            AXP::UInt8 value = 0;
            if (mIsByteHigh) {
                for (Int32 i = length - 1, j = 0; i >= 0; --i, ++j) {
                    value = value | (bits[i] << j);
                }
            }
            else {
                for (Int32 i = 0; i < length; i++) {
                    value = value | (bits[i] << i);
                }
            }

            if (isPos)
                mPosition += length;

            return value;
        }

        UInt16 GetUInt16(
            IN Int32 length,
            IN Boolean isPos)
        {
            if ((!mBitBuffer)
                || (length < 1)
                || (length > 16)
                || ((mPosition + length) > mBitLength)) {
                if ((mPosition + length) > mBitLength)
                    mPosition = mBitLength;

                return 0;
            }

            PByte bits = mBitBuffer + mPosition;
            AXP::UInt16 value = 0;
            if (mIsByteHigh) {
                for (Int32 i = length - 1, j = 0; i >= 0; --i, ++j) {
                    value = value | (bits[i] << j);
                }
            }
            else {
                for (Int32 i = 0; i < length; i++) {
                    value = value | (bits[i] << i);
                }
            }

            if (isPos)
                mPosition += length;

            return value;
        }

        UInt32 GetUInt32(
            IN Int32 length,
            IN Boolean isPos)
        {
            if ((!mBitBuffer)
                || (length < 1)
                || (length > 32)
                || ((mPosition + length) > mBitLength)) {
                if ((mPosition + length) > mBitLength)
                    mPosition = mBitLength;

                return 0;
            }

            PByte bits = mBitBuffer + mPosition;
            AXP::UInt32 value = 0;
            if (mIsByteHigh) {
                for (Int32 i = length - 1, j = 0; i >= 0; --i, ++j) {
                    value = value | (bits[i] << j);
                }
            }
            else {
                for (Int32 i = 0; i < length; i++) {
                    value = value | (bits[i] << i);
                }
            }

            if (isPos)
                mPosition += length;

            return value;
        }

        UInt64 GetUInt64(
            IN Int32 length,
            IN Boolean isPos)
        {
            if ((!mBitBuffer)
                || (length < 1)
                || (length > 64)
                || ((mPosition + length) > mBitLength)) {
                if ((mPosition + length) > mBitLength)
                    mPosition = mBitLength;

                return 0;
            }

            PByte bits = mBitBuffer + mPosition;
            AXP::UInt64 value = 0;
            if (mIsByteHigh) {
                for (Int32 i = length - 1, j = 0; i >= 0; --i, ++j) {
                    value = value | (bits[i] << j);
                }
            }
            else {
                for (Int32 i = 0; i < length; i++) {
                    value = value | (bits[i] << i);
                }
            }

            if (isPos)
                mPosition += length;

            return value;
        }

        Void PopFirst(
            IN Int32 length)
        {
            mPosition += length;
        }

        Int32 GetPosition()
        {
            return mPosition;
        }

        Void SetPosition(Int32 pos)
        {
            mPosition = MIN(pos, mBitLength);
        }

        Int32 GetLength()
        {
            return (mBitLength - mPosition);
        }

        Int32 GetBytePosition()
        {
            return ((mPosition + mStartOffset) & 0x07);
        }

        VIRTUAL ARESULT ReadFromParcel(
            IN CONST Sp<CParcel> & parcel)
        {
            if (parcel == NULL)
                return AE_INVALIDARG;

            if (AFAILED(parcel->ReadBoolean(mIsByteHigh)))
                return AE_FAIL;

            if (AFAILED(parcel->ReadInt32(mPosition)))
                return AE_FAIL;

            if (AFAILED(parcel->ReadInt32(mStartOffset)))
                return AE_FAIL;

            Int64 pos = parcel->GetPosition();
            if (AFAILED(parcel->ReadInt32(mBitLength)))
                return AE_FAIL;

            if (mBitLength <= 0)
                return AS_OK;

            parcel->Seek(pos);
            Sp<ByteArray> byteArray;
            if (AFAILED(parcel->ReadByteArray(byteArray)))
                return AE_FAIL;

            if (byteArray == NULL)
                return AE_FAIL;

            mBitLength = byteArray->GetUsed();
            if (mBitLength <= 0)
                return AS_OK;

            mBitBuffer = (PByte)malloc(mBitLength);
            if (!mBitBuffer)
                return AE_OUTOFMEMORY;

            memcpy(mBitBuffer, byteArray->GetPayload(), mBitLength);

            return AS_OK;
        }

        VIRTUAL ARESULT WriteToParcel(
            IN CONST Sp<CParcel> & parcel)
        {
            if (parcel == NULL)
                return AE_INVALIDARG;

            if (AFAILED(parcel->WriteBoolean(mIsByteHigh)))
                return AE_FAIL;

            if (AFAILED(parcel->WriteInt32(mPosition)))
                return AE_FAIL;

            if (AFAILED(parcel->WriteInt32(mStartOffset)))
                return AE_FAIL;

            if (mBitLength <= 0) {
                if (AFAILED(parcel->WriteInt32(mBitLength)))
                    return AE_FAIL;
            }
            else {
                Sp<ByteArray> byteArray = ByteArray::Create(mBitLength);
                if (byteArray == NULL)
                    return AE_FAIL;

                if (AFAILED(byteArray->Append(mBitBuffer, mBitLength)))
                    return AE_FAIL;

                if (AFAILED(parcel->WriteByteArray(byteArray)))
                    return AE_FAIL;
            }

            return AS_OK;
        }

        Sp<String> ToString()
        {
            Sp<String> str;
            Int32 position = mPosition;

            while (mPosition < mBitLength) {
                if (str == NULL) {
                    Int32 i = mBitLength & 0x07;
                    if (i == 0)
                        str = String::Create(6, L"0x%02X", GetUInt8(8, TRUE));
                    else
                        str = String::Create(6, L"0x%02X", GetUInt8(i, TRUE));
                }
                else {
                    str = str->Append(6, L" 0x%02X", GetUInt8(8, TRUE));
                }

                if (str == NULL)
                    return NULL;
            }

            mPosition = position;

            return str;
        }

        BitBuffer()
        {
            mBitBuffer = NULL;
            mPosition = 0;
            mStartOffset = 0;
            mBitLength = 0;
        }

    protected:

        ARESULT Init(
            IN Int32 length)
        {
            if (length <= 0)
                return AE_INVALIDARG;

            mBitBuffer = (PByte)::calloc(length, 1);
            if (!mBitBuffer)
                return AE_OUTOFMEMORY;

            mBitLength = length;

            return AS_OK;
        }

        ARESULT Init(
            IN PCByte data,
            IN Int32 dataLength,
            IN Int32 bitLength,
            IN Int32 bitStartOffset,
            IN Boolean isByteHigh)
        {
            if ((data == NULL)
                || (bitLength < 1)
                || ((dataLength << 3) < (bitStartOffset + bitLength)))
                return AE_INVALIDARG;

            mBitBuffer = (PByte)::calloc(bitLength, 1);
            if (!mBitBuffer)
                return AE_OUTOFMEMORY;

            mBitLength = bitLength;
            mIsByteHigh = isByteHigh;

            Int32 bitEndOffset = bitStartOffset + bitLength;
            for (Int32 i = bitStartOffset, j = 0; i < bitEndOffset; ++i) {
                Int32 index = RoundDown(i, 8) >> 3;
                Int32 pos;
                if (isByteHigh)
                    pos = 7 - (i & 0x07);
                else
                    pos = i & 0x07;

                mBitBuffer[j++] = (UInt8)((data[index] >> pos) & 0x01);
            }

            mStartOffset = bitStartOffset;

            return AS_OK;
        }

    protected:

        Boolean mIsByteHigh;
        Int32 mPosition;
        Int32 mStartOffset;
        Int32 mBitLength;
        PByte mBitBuffer;
    };
} // namespace AXP

#endif // __BIT_BUFFER_H__
