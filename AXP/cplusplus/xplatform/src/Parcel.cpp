//
//  parcel.cpp
//  CommAnalyzerCore
//
//  Created by zhanwenshu on 14-7-31.
//  Copyright (c) 2014年 Liu Alex. All rights reserved.
//

#include <stdio.h>
#include "../include/parcel.h"
#include "../include/byteOrder.h"

namespace AXP {
    STATIC ARESULT UpdateParcelHeaderLength(
        IN CParcel * parcel)
    {
        ASSERT(parcel);

        PByte data = (PByte)parcel->GetPayload();
        if (!data)
            return AE_FAIL;

        *(Int32*)(data + 1) = (Int32)(parcel->GetPosition() - 5);

        return AS_OK;
    }

    STATIC ARESULT ReadInt32FromMemoryStream(
        IN CONST Sp<MemoryStream<PARCEL_BUFFER_DEFAULT_LENGTH> > & stream,
        INOUT Int32 & value)
    {
        if (stream == NULL)
            return AE_FAIL;

        if (!stream->Read(
            (PByte)&value, sizeof(Int32), sizeof(Int32)))
            return AE_OUTOFMEMORY;

        return AS_OK;
    }

    STATIC ARESULT WriteInt32ToMemoryStream(
        IN CONST Sp<MemoryStream<PARCEL_BUFFER_DEFAULT_LENGTH> > & stream,
        IN CONST Int32 value)
    {
        if (stream == NULL)
            return AE_FAIL;
        
        if (!stream->Write((PCByte)&value, sizeof(Int32)))
            return AE_OUTOFMEMORY;

        return AS_OK;
    }

    CParcel::CParcel()
    {
        mStream = new MemoryStream<PARCEL_BUFFER_DEFAULT_LENGTH>;
        if (mStream == NULL)
            return;

        mIsLittleEndian = AXP_LITTLE_ENDIAN;
        if (!mStream->Write((PCByte)&mIsLittleEndian, sizeof(Boolean)))
            return;

        Int32 length = 0;
        if (!mStream->Write((PCByte)&length, sizeof(Int32)))
            return;

        UpdateParcelHeaderLength(this);
    }

    CParcel::CParcel(
        IN CONST Sp<MemoryStream<PARCEL_BUFFER_DEFAULT_LENGTH> > & stream)
    {
        mStream = stream;
        Reset();
    }

    ARESULT CParcel::Reset()
    {
        if (mStream == NULL)
            return AE_FAIL;

        mStream->SetPosition(0);
        if (!mStream->Read(
            (PByte)&mIsLittleEndian, sizeof(Boolean), sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        Int32 length;
        if (!mStream->Read(
            (PByte)&length, sizeof(Int32), sizeof(Int32)))
            return AE_OUTOFMEMORY;

        return AS_OK;
    }

    ARESULT CParcel::Reset(
        IN PCByte buffer,
        IN CONST Int64 length)
    {
        if (mStream == NULL)
            return AE_FAIL;

        mStream->SetPosition(0);
        if (!mStream->Write(buffer, length))
            return AE_OUTOFMEMORY;

        return Reset();
    }

    PCByte CParcel::GetPayload()
    {
        if (mStream == NULL)
            return NULL;
        else
            return mStream->GetPayload();
    }

    ARESULT CParcel::Read(
        INOUT PByte dst,
        IN Int64 dstLength,
        IN Int64 length)
    {
        if ((mStream == NULL)
            || (dstLength < length))
            return AE_FAIL;

        if (!mStream->Read(
            dst, length, length))
            return AE_OUTOFMEMORY;

        return AS_OK;
    }

    ARESULT CParcel::ReadBoolean(
        INOUT Boolean & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        if (!mStream->Read(
            (PByte)&value, sizeof(Boolean), sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        return AS_OK;
    }

    ARESULT CParcel::ReadByte(
        INOUT Int8 & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        if (!mStream->Read(
            (PByte)&value, sizeof(Int8), sizeof(Int8)))
            return AE_OUTOFMEMORY;

        return AS_OK;
    }

    ARESULT CParcel::ReadInt8(
        INOUT Int8 & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        if (!mStream->Read(
            (PByte)&value, sizeof(Int8), sizeof(Int8)))
            return AE_OUTOFMEMORY;

        return AS_OK;
    }

    ARESULT CParcel::ReadUInt8(
        INOUT UInt8 & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        if (!mStream->Read(
            (PByte)&value, sizeof(UInt8), sizeof(UInt8)))
            return AE_OUTOFMEMORY;

        return AS_OK;
    }

    ARESULT CParcel::ReadInt16(
        INOUT Int16 & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        if (!mStream->Read(
            (PByte)&value, sizeof(Int16), sizeof(Int16)))
            return AE_OUTOFMEMORY;

        if (!BYTE_ORDER_EQUAL(mIsLittleEndian)) {
            UInt16 temp = (UInt16)value;
            temp = SWAP_16(temp);
            value = (Int16)temp;
        }

        return AS_OK;
    }

    ARESULT CParcel::ReadUInt16(
        INOUT UInt16 & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        if (!mStream->Read(
            (PByte)&value, sizeof(UInt16), sizeof(UInt16)))
            return AE_OUTOFMEMORY;

        if (!BYTE_ORDER_EQUAL(mIsLittleEndian))
            value = SWAP_16(value);

        return AS_OK;
    }

    ARESULT CParcel::ReadInt32(
        INOUT Int32 & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        if (!mStream->Read(
            (PByte)&value, sizeof(Int32), sizeof(Int32)))
            return AE_OUTOFMEMORY;

        if (!BYTE_ORDER_EQUAL(mIsLittleEndian)) {
            UInt32 temp = (UInt32)value;
            temp = SWAP_32(temp);
            value = (Int32)temp;
        }

        return AS_OK;
    }

    ARESULT CParcel::ReadUInt32(
        INOUT UInt32 & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        if (!mStream->Read(
            (PByte)&value, sizeof(UInt32), sizeof(UInt32)))
            return AE_OUTOFMEMORY;

        if (!BYTE_ORDER_EQUAL(mIsLittleEndian))
            value = SWAP_32(value);

        return AS_OK;
    }

    ARESULT CParcel::ReadInt64(
        INOUT Int64 & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        if (!mStream->Read(
            (PByte)&value, sizeof(Int64), sizeof(Int64)))
            return AE_OUTOFMEMORY;

        if (!BYTE_ORDER_EQUAL(mIsLittleEndian)) {
            UInt64 temp = (UInt64)value;
            temp = SWAP_64(temp);
            value = (Int64)temp;
        }

        return AS_OK;
    }

    ARESULT CParcel::ReadUInt64(
        INOUT UInt64 & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        if (!mStream->Read(
            (PByte)&value, sizeof(UInt64), sizeof(UInt64)))
            return AE_OUTOFMEMORY;

        if (!BYTE_ORDER_EQUAL(mIsLittleEndian))
            value = SWAP_64(value);

        return AS_OK;
    }

    ARESULT CParcel::ReadFloat(
        INOUT Float & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        if (!mStream->Read(
            (PByte)&value, sizeof(Float), sizeof(Float)))
            return AE_OUTOFMEMORY;

        if (!BYTE_ORDER_EQUAL(mIsLittleEndian)) {
            UInt32 temp = *(UInt32*)&value;
            temp = SWAP_32(temp);
            value = *(Float*)&temp;
        }

        return AS_OK;
    }

    ARESULT CParcel::ReadDouble(
        INOUT Double & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        if (!mStream->Read(
            (PByte)&value, sizeof(Double), sizeof(Double)))
            return AE_OUTOFMEMORY;

        if (!BYTE_ORDER_EQUAL(mIsLittleEndian)) {
            UInt64 temp = *(UInt64*)&value;
            temp = SWAP_64(temp);
            value = *(Double*)&temp;
        }

        return AS_OK;
    }

    ARESULT CParcel::ReadNullableBoolean(
        INOUT Boolean$ & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        Boolean hasValue;
        if (!mStream->Read(
            (PByte)&hasValue, sizeof(Boolean), sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Boolean temp;
            if (!mStream->Read(
                (PByte)&temp, sizeof(Boolean), sizeof(Boolean)))
                return AE_OUTOFMEMORY;

            value = temp;
        }
        else
            value.SetNull();


        return AS_OK;
    }

    ARESULT CParcel::ReadNullableByte(
        INOUT Int8$ & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        Boolean hasValue;
        if (!mStream->Read(
            (PByte)&hasValue, sizeof(Boolean), sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Int8 temp;
            if (!mStream->Read(
                (PByte)&temp, sizeof(Int8), sizeof(Int8)))
                return AE_OUTOFMEMORY;

            value = temp;
        }
        else
            value.SetNull();

        return AS_OK;
    }

    ARESULT CParcel::ReadNullableInt8(
        INOUT Int8$ & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        Boolean hasValue;
        if (!mStream->Read(
            (PByte)&hasValue, sizeof(Boolean), sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Int8 temp;
            if (!mStream->Read(
                (PByte)&temp, sizeof(Int8), sizeof(Int8)))
                return AE_OUTOFMEMORY;

            value = temp;
        }
        else
            value.SetNull();

        return AS_OK;
    }

    ARESULT CParcel::ReadNullableUInt8(
        INOUT UInt8$ & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        Boolean hasValue;
        if (!mStream->Read(
            (PByte)&hasValue, sizeof(Boolean), sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            UInt8 temp;
            if (!mStream->Read(
                (PByte)&temp, sizeof(UInt8), sizeof(UInt8)))
                return AE_OUTOFMEMORY;

            value = temp;
        }
        else
            value.SetNull();

        return AS_OK;
    }

    ARESULT CParcel::ReadNullableInt16(
        INOUT Int16$ & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        Boolean hasValue;
        if (!mStream->Read(
            (PByte)&hasValue, sizeof(Boolean), sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Int16 temp;
            if (!mStream->Read(
                (PByte)&temp, sizeof(Int16), sizeof(Int16)))
                return AE_OUTOFMEMORY;

            if (!BYTE_ORDER_EQUAL(mIsLittleEndian)) {
                UInt16 tempValue = (UInt16)temp;
                tempValue = SWAP_16(tempValue);
                temp = (Int16)tempValue;
            }

            value = temp;
        }
        else
            value.SetNull();

        return AS_OK;
    }

    ARESULT CParcel::ReadNullableUInt16(
        INOUT UInt16$ & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        Boolean hasValue;
        if (!mStream->Read(
            (PByte)&hasValue, sizeof(Boolean), sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            UInt16 temp;
            if (!mStream->Read(
                (PByte)&temp, sizeof(UInt16), sizeof(UInt16)))
                return AE_OUTOFMEMORY;

            if (!BYTE_ORDER_EQUAL(mIsLittleEndian))
                temp = SWAP_16(temp);

            value = temp;
        }
        else
            value.SetNull();

        return AS_OK;
    }

    ARESULT CParcel::ReadNullableInt32(
        INOUT Int32$ & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        Boolean hasValue;
        if (!mStream->Read(
            (PByte)&hasValue, sizeof(Boolean), sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Int32 temp;
            if (!mStream->Read(
                (PByte)&temp, sizeof(Int32), sizeof(Int32)))
                return AE_OUTOFMEMORY;

            if (!BYTE_ORDER_EQUAL(mIsLittleEndian)) {
                UInt32 tempValue = (UInt32)temp;
                tempValue = SWAP_32(tempValue);
                temp = (Int32)tempValue;
            }

            value = temp;
        }
        else
            value.SetNull();

        return AS_OK;
    }

    ARESULT CParcel::ReadNullableUInt32(
        INOUT UInt32$ & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        Boolean hasValue;
        if (!mStream->Read(
            (PByte)&hasValue, sizeof(Boolean), sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            UInt32 temp;
            if (!mStream->Read(
                (PByte)&temp, sizeof(UInt32), sizeof(UInt32)))
                return AE_OUTOFMEMORY;

            if (!BYTE_ORDER_EQUAL(mIsLittleEndian))
                temp = SWAP_32(temp);

            value = temp;
        }
        else
            value.SetNull();

        return AS_OK;
    }

    ARESULT CParcel::ReadNullableInt64(
        INOUT Int64$ & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        Boolean hasValue;
        if (!mStream->Read(
            (PByte)&hasValue, sizeof(Boolean), sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Int64 temp;
            if (!mStream->Read(
                (PByte)&temp, sizeof(Int64), sizeof(Int64)))
                return AE_OUTOFMEMORY;

            if (!BYTE_ORDER_EQUAL(mIsLittleEndian)) {
                UInt64 tempValue = (UInt64)temp;
                tempValue = SWAP_64(tempValue);
                temp = (Int64)tempValue;
            }

            value = temp;
        }
        else
            value.SetNull();

        return AS_OK;
    }

    ARESULT CParcel::ReadNullableUInt64(
        INOUT UInt64$ & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        Boolean hasValue;
        if (!mStream->Read(
            (PByte)&hasValue, sizeof(Boolean), sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            UInt64 temp;
            if (!mStream->Read(
                (PByte)&temp, sizeof(UInt64), sizeof(UInt64)))
                return AE_OUTOFMEMORY;

            if (!BYTE_ORDER_EQUAL(mIsLittleEndian))
                temp = SWAP_64(temp);

            value = temp;
        }
        else
            value.SetNull();

        return AS_OK;
    }

    ARESULT CParcel::ReadNullableFloat(
        INOUT Float$ & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        Boolean hasValue;
        if (!mStream->Read(
            (PByte)&hasValue, sizeof(Boolean), sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Float temp;
            if (!mStream->Read(
                (PByte)&temp, sizeof(Float), sizeof(Float)))
                return AE_OUTOFMEMORY;

            if (!BYTE_ORDER_EQUAL(mIsLittleEndian)) {
                UInt32 tempValue = *(UInt32*)&temp;
                tempValue = SWAP_32(tempValue);
                temp = *(Float*)&tempValue;
            }

            value = temp;
        }
        else
            value.SetNull();

        return AS_OK;
    }

    ARESULT CParcel::ReadNullableDouble(
        INOUT Double$ & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        Boolean hasValue;
        if (!mStream->Read(
            (PByte)&hasValue, sizeof(Boolean), sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Double temp;
            if (!mStream->Read(
                (PByte)&temp, sizeof(Double), sizeof(Double)))
                return AE_OUTOFMEMORY;

            if (!BYTE_ORDER_EQUAL(mIsLittleEndian)) {
                UInt64 tempValue = *(UInt64*)&temp;
                tempValue = SWAP_64(tempValue);
                temp = *(Double*)&tempValue;
            }

            value = temp;
        }
        else
            value.SetNull();

        return AS_OK;
    }

    ARESULT CParcel::ReadByteArray(
        INOUT Sp<ByteArray> & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        Int32 length;
        ARESULT ar = ReadInt32FromMemoryStream(mStream, length);
        if (AFAILED(ar))
            return ar;

        if (length == 0) {
            value = NULL;
            return AS_OK;
        }

        PStr buffer = (PStr)malloc(length);
        if (!buffer)
            return AE_OUTOFMEMORY;

        if (!mStream->Read((PByte)buffer, length, length)) {
            free(buffer);
            return AE_OUTOFMEMORY;
        }

        Sp<ByteArray> array = ByteArray::Create(length);
        if (array == NULL) {
            free(buffer);
            return AE_OUTOFMEMORY;
        }

        if (AFAILED(array->Append((PCByte)buffer, length))) {
            free(buffer);
            return AE_OUTOFMEMORY;
        }

        value = array;
        free(buffer);

        return AS_OK;
    }

    ARESULT CParcel::ReadString(
        INOUT Sp<String> & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Read(1))
            return AE_OUTOFMEMORY;

        Int32 length;
        ARESULT ar = ReadInt32FromMemoryStream(mStream, length);
        if (AFAILED(ar))
            return ar;

        if (length == 0) {
            value = NULL;
            return AS_OK;
        }

        PStr buffer = (PStr)malloc(length);
        if (!buffer)
            return AE_OUTOFMEMORY;

        if (!mStream->Read((PByte)buffer, length, length)) {
            free(buffer);
            return AE_OUTOFMEMORY;
        }

        Sp<String> string = String::Create(
            (PCByte)buffer, sizeof(Char), length, EncodingType_UTF8);

        free(buffer);

        if (string == NULL)
            return AE_OUTOFMEMORY;

        value = string;

        return AS_OK;
    }

    ARESULT CParcel::Write(
        IN PCByte data,
        IN Int64 length)
    {
        if (mStream == NULL)
            return AE_FAIL;

        if (!mStream->Write(data, length))
            return AE_OUTOFMEMORY;
        else
            return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteBoolean(
        IN CONST Boolean value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'z';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        if (!mStream->Write((PCByte)&value, sizeof(Boolean)))
            return AE_OUTOFMEMORY;
        else
            return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteByte(
        IN CONST Int8 value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'c';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        if (!mStream->Write((PCByte)&value, sizeof(Int8)))
            return AE_OUTOFMEMORY;
        else
            return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteInt8(
        IN CONST Int8 value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'b';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        if (!mStream->Write((PCByte)&value, sizeof(Int8)))
            return AE_OUTOFMEMORY;
        else
            return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteUInt8(
        IN CONST UInt8 value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'e';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        if (!mStream->Write((PCByte)&value, sizeof(UInt8)))
            return AE_OUTOFMEMORY;
        else
            return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteInt16(
        IN CONST Int16 value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'r';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        if (!mStream->Write((PCByte)&value, sizeof(Int16)))
            return AE_OUTOFMEMORY;
        else
            return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteUInt16(
        IN CONST UInt16 value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'n';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        if (!mStream->Write((PCByte)&value, sizeof(UInt16)))
            return AE_OUTOFMEMORY;
        else
            return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteInt32(
        IN CONST Int32 value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'i';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        if (!mStream->Write((PCByte)&value, sizeof(Int32)))
            return AE_OUTOFMEMORY;
        else
            return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteUInt32(
        IN CONST UInt32 value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'u';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        if (!mStream->Write((PCByte)&value, sizeof(UInt32)))
            return AE_OUTOFMEMORY;
        else
            return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteInt64(
        IN CONST Int64 value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'g';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        if (!mStream->Write((PCByte)&value, sizeof(Int64)))
            return AE_OUTOFMEMORY;
        else
            return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteUInt64(
        IN CONST UInt64 value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'm';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        if (!mStream->Write((PCByte)&value, sizeof(UInt64)))
            return AE_OUTOFMEMORY;
        else
            return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteFloat(
        IN CONST Float value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'f';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        if (!mStream->Write((PCByte)&value, sizeof(Float)))
            return AE_OUTOFMEMORY;
        else
            return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteDouble(
        IN CONST Double value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'd';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        if (!mStream->Write((PCByte)&value, sizeof(Double)))
            return AE_OUTOFMEMORY;
        else
            return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteNullableBoolean(
        IN CONST Boolean$ value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'Z';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        Boolean hasValue = value.HasValue();
        if (!mStream->Write((PCByte)&hasValue, sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Boolean temp = value.GetValue();
            if (!mStream->Write((PCByte)&temp, sizeof(Boolean)))
                return AE_OUTOFMEMORY;
        }

        return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteNullableByte(
        IN CONST Int8$ value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'C';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        Boolean hasValue = value.HasValue();
        if (!mStream->Write((PCByte)&hasValue, sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Int8 temp = value.GetValue();
            if (!mStream->Write((PCByte)&temp, sizeof(Int8)))
                return AE_OUTOFMEMORY;
        }

        return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteNullableInt8(
        IN CONST Int8$ value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'B';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        Boolean hasValue = value.HasValue();
        if (!mStream->Write((PCByte)&hasValue, sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Int8 temp = value.GetValue();
            if (!mStream->Write((PCByte)&temp, sizeof(Int8)))
                return AE_OUTOFMEMORY;
        }

        return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteNullableUInt8(
        IN CONST UInt8$ value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'E';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        Boolean hasValue = value.HasValue();
        if (!mStream->Write((PCByte)&hasValue, sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            UInt8 temp = value.GetValue();
            if (!mStream->Write((PCByte)&temp, sizeof(UInt8)))
                return AE_OUTOFMEMORY;
        }

        return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteNullableInt16(
        IN CONST Int16$ value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'R';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        Boolean hasValue = value.HasValue();
        if (!mStream->Write((PCByte)&hasValue, sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Int16 temp = value.GetValue();
            if (!mStream->Write((PCByte)&temp, sizeof(Int16)))
                return AE_OUTOFMEMORY;
        }

        return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteNullableUInt16(
        IN CONST UInt16$ value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'N';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        Boolean hasValue = value.HasValue();
        if (!mStream->Write((PCByte)&hasValue, sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            UInt16 temp = value.GetValue();
            if (!mStream->Write((PCByte)&temp, sizeof(UInt16)))
                return AE_OUTOFMEMORY;
        }

        return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteNullableInt32(
        IN CONST Int32$ value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'I';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        Boolean hasValue = value.HasValue();
        if (!mStream->Write((PCByte)&hasValue, sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Int32 temp = value.GetValue();
            if (!mStream->Write((PCByte)&temp, sizeof(Int32)))
                return AE_OUTOFMEMORY;
        }

        return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteNullableUInt32(
        IN CONST UInt32$ value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'U';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        Boolean hasValue = value.HasValue();
        if (!mStream->Write((PCByte)&hasValue, sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            UInt32 temp = value.GetValue();
            if (!mStream->Write((PCByte)&temp, sizeof(UInt32)))
                return AE_OUTOFMEMORY;
        }

        return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteNullableInt64(
        IN CONST Int64$ value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'G';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        Boolean hasValue = value.HasValue();
        if (!mStream->Write((PCByte)&hasValue, sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Int64 temp = value.GetValue();
            if (!mStream->Write((PCByte)&temp, sizeof(Int64)))
                return AE_OUTOFMEMORY;
        }

        return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteNullableUInt64(
        IN CONST UInt64$ value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'M';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        Boolean hasValue = value.HasValue();
        if (!mStream->Write((PCByte)&hasValue, sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            UInt64 temp = value.GetValue();
            if (!mStream->Write((PCByte)&temp, sizeof(UInt64)))
                return AE_OUTOFMEMORY;
        }

        return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteNullableFloat(
        IN CONST Float$ value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'F';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        Boolean hasValue = value.HasValue();
        if (!mStream->Write((PCByte)&hasValue, sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Float temp = value.GetValue();
            if (!mStream->Write((PCByte)&temp, sizeof(Float)))
                return AE_OUTOFMEMORY;
        }

        return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteNullableDouble(
        IN CONST Double$ value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'D';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        Boolean hasValue = value.HasValue();
        if (!mStream->Write((PCByte)&hasValue, sizeof(Boolean)))
            return AE_OUTOFMEMORY;

        if (hasValue) {
            Double temp = value.GetValue();
            if (!mStream->Write((PCByte)&temp, sizeof(Double)))
                return AE_OUTOFMEMORY;
        }

        return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteByteArray(
        IN CONST Sp<ByteArray> & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'A';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        ARESULT ar;
        if (value == NULL) {
            ar = WriteInt32ToMemoryStream(mStream, 0);
            if (AFAILED(ar))
                return ar;
		}
		else {
            Int32 length = value->GetUsed();
            ar = WriteInt32ToMemoryStream(mStream, length);
            if (AFAILED(ar))
                return ar;

            if (!mStream->Write(value->GetPayload(), length))
                return AE_OUTOFMEMORY;
		}

        return UpdateParcelHeaderLength(this);
    }

    ARESULT CParcel::WriteString(
        IN CONST Sp<String> & value)
    {
        if (mStream == NULL)
            return AE_FAIL;

        Char typeName = 'S';
        if (!mStream->Write((PCByte)&typeName, sizeof(Char)))
            return AE_OUTOFMEMORY;

        if (value == NULL) {
            return WriteInt32ToMemoryStream(mStream, 0);
        }
		else {
            Sp<ByteArray> ba = value->GetBytes();
            if (ba == NULL)
                return AE_OUTOFMEMORY;

            Int32 length = ba->GetUsed();
            if (length == 0) {
                ARESULT ar = WriteInt32ToMemoryStream(mStream, length);
                if (AFAILED(ar))
                    return ar;
            }
			else {
                length = length - 1;
                ARESULT ar = WriteInt32ToMemoryStream(mStream, length);
                if (AFAILED(ar))
                    return ar;

                if (!mStream->Write(ba->GetPayload(), length))
                    return AE_OUTOFMEMORY;
			}
        }

        return UpdateParcelHeaderLength(this);
    }
} // namespace AXP
