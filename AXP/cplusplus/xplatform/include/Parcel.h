//
//  parcel.h
//  CommAnalyzerCore
//
//  Created by zhanwenshu on 14-7-31.
//  Copyright (c) 2014年 Liu Alex. All rights reserved.
//

#ifndef __PARCEL_H__
#define __PARCEL_H__

#include "type.h"
#include "aresult.h"
#include "memoryStream.h"
#include "astring.h"
#include "nullable.h"

#define PARCEL_BUFFER_DEFAULT_LENGTH (1024)

namespace AXP {
    class CParcel : public CObject
    {
    public:

        CParcel();

        CParcel(
            IN CONST Sp<MemoryStream<PARCEL_BUFFER_DEFAULT_LENGTH> > & stream);

    public:

        ARESULT STDCALL Reset();

        ARESULT STDCALL Reset(
            IN PCByte buffer,
            IN CONST Int64 length);

        Int64 STDCALL GetPosition()
        {
            if (mStream == NULL)
                return -1;

            return mStream->GetPosition();
        }

        Void STDCALL Seek(Int64 position)
        {
            if (mStream == NULL)
                return;

            mStream->SetPosition(position);
        }

        PCByte STDCALL GetPayload();

        Int64 STDCALL GetLength()
        {
            if (mStream == NULL)
                return -1;

            return mStream->GetLength();
        }

        ARESULT STDCALL Read(
            INOUT PByte dst,
            IN Int64 dstLength,
            IN Int64 length);

        ARESULT STDCALL ReadBoolean(
            INOUT Boolean & value);

        ARESULT STDCALL ReadByte(
            INOUT Int8 & value);

        ARESULT STDCALL ReadInt8(
            INOUT Int8 & value);

        ARESULT STDCALL ReadUInt8(
            INOUT UInt8 & value);

        ARESULT STDCALL ReadInt16(
            INOUT Int16 & value);

        ARESULT STDCALL ReadUInt16(
            INOUT UInt16 & value);

        ARESULT STDCALL ReadInt32(
            INOUT Int32 & value);

        ARESULT STDCALL ReadUInt32(
            INOUT UInt32 & value);

        ARESULT STDCALL ReadInt64(
            INOUT Int64 & value);

        ARESULT STDCALL ReadUInt64(
            INOUT UInt64 & value);

        ARESULT STDCALL ReadFloat(
            INOUT Float & value);

        ARESULT STDCALL ReadDouble(
            INOUT Double & value);

        ARESULT STDCALL ReadNullableBoolean(
            INOUT Boolean$ & value);

        ARESULT STDCALL ReadNullableByte(
            INOUT Int8$ & value);

        ARESULT STDCALL ReadNullableInt8(
            INOUT Int8$ & value);

        ARESULT STDCALL ReadNullableUInt8(
            INOUT UInt8$ & value);

        ARESULT STDCALL ReadNullableInt16(
            INOUT Int16$ & value);

        ARESULT STDCALL ReadNullableUInt16(
            INOUT UInt16$ & value);

        ARESULT STDCALL ReadNullableInt32(
            INOUT Int32$ & value);

        ARESULT STDCALL ReadNullableUInt32(
            INOUT UInt32$ & value);

        ARESULT STDCALL ReadNullableInt64(
            INOUT Int64$ & value);

        ARESULT STDCALL ReadNullableUInt64(
            INOUT UInt64$ & value);

        ARESULT STDCALL ReadNullableFloat(
            INOUT Float$ & value);

        ARESULT STDCALL ReadNullableDouble(
            INOUT Double$ & value);

        ARESULT STDCALL ReadByteArray(
            INOUT Sp<ByteArray> & value);

        ARESULT STDCALL ReadString(
            INOUT Sp<String> & value);

        ARESULT STDCALL Write(
            IN PCByte data,
            IN Int64 length);

        ARESULT STDCALL WriteBoolean(
            IN CONST Boolean value);

        ARESULT STDCALL WriteByte(
            IN CONST Int8 value);

        ARESULT STDCALL WriteInt8(
            IN CONST Int8 value);

        ARESULT STDCALL WriteUInt8(
            IN CONST UInt8 value);

        ARESULT STDCALL WriteInt16(
            IN CONST Int16 value);

        ARESULT STDCALL WriteUInt16(
            IN CONST UInt16 value);

        ARESULT STDCALL WriteInt32(
            IN CONST Int32 value);

        ARESULT STDCALL WriteUInt32(
            IN CONST UInt32 value);

        ARESULT STDCALL WriteInt64(
            IN CONST Int64 value);

        ARESULT STDCALL WriteUInt64(
            IN CONST UInt64 value);

        ARESULT STDCALL WriteFloat(
            IN CONST Float value);

        ARESULT STDCALL WriteDouble(
            IN CONST Double value);

        ARESULT STDCALL WriteNullableBoolean(
            IN CONST Boolean$ value);

        ARESULT STDCALL WriteNullableByte(
            IN CONST Int8$ value);

        ARESULT STDCALL WriteNullableInt8(
            IN CONST Int8$ value);

        ARESULT STDCALL WriteNullableUInt8(
            IN CONST UInt8$ value);

        ARESULT STDCALL WriteNullableInt16(
            IN CONST Int16$ value);

        ARESULT STDCALL WriteNullableUInt16(
            IN CONST UInt16$ value);

        ARESULT STDCALL WriteNullableInt32(
            IN CONST Int32$ value);

        ARESULT STDCALL WriteNullableUInt32(
            IN CONST UInt32$ value);

        ARESULT STDCALL WriteNullableInt64(
            IN CONST Int64$ value);

        ARESULT STDCALL WriteNullableUInt64(
            IN CONST UInt64$ value);

        ARESULT STDCALL WriteNullableFloat(
            IN CONST Float$ value);

        ARESULT STDCALL WriteNullableDouble(
            IN CONST Double$ value);

        ARESULT STDCALL WriteByteArray(
            IN CONST Sp<ByteArray> & value);

        ARESULT STDCALL WriteString(
            IN CONST Sp<String> & value);

    private:

        Sp<MemoryStream<PARCEL_BUFFER_DEFAULT_LENGTH> > mStream;
        Boolean mIsLittleEndian;
    };
} // namespace AXP

#endif // __PARCEL_H__
