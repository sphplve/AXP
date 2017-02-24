//
//  CParcel.m
//  TestObjectiveCParcel
//
//  Created by Alex on 15/8/12.
//  Copyright (c) 2015年 Alex. All rights reserved.
//

#import "Parcel.h"
#import "byteOrder.h"
#import "memoryStream.h"

@interface CParcel () {
@private
    
    MemoryStream * mStream;
    Boolean mIsLittleEndian;
}

@end

@implementation CParcel

- (id)init
{
    if (!(self = [super init]))
        return self;
    
    @try {
        [self ctor];
    }
    @catch (NSException * e) {
        return nil;
    }
    
    return self;
}

- (void)ctor
{
    mStream = [[MemoryStream alloc] init];
    if (mStream == NULL)
        return;
    
    mIsLittleEndian = AXP_LITTLE_ENDIAN;
    if (![mStream Write: (const Byte *)&mIsLittleEndian Length: sizeof(Boolean)])
        return;
    
    int32_t len = 0;
    if (![mStream Write: (const Byte *)&len Length: sizeof(int32_t)])
        return;
    
    [self UpdateHeaderLength];
}

- (AXP::ARESULT) UpdateHeaderLength
{
    Byte * data = (Byte *)[self GetPayload];
    if (!data)
        return AXP::AE_FAIL;

    *(int32_t*)(data + 1) = (int32_t)([self GetPosition] - 5);
    return AXP::AS_OK;
}

- (AXP::ARESULT)Reset
{
    [mStream SetPosition: 0];
    if (![mStream Read: (Byte*)&mIsLittleEndian DstLength: sizeof(Boolean) Length: sizeof(Boolean)])
        return AXP::AE_FAIL;
    
    int32_t len = 0;
    if (![mStream Read: (Byte*)&len DstLength: sizeof(int32_t) Length: sizeof(int32_t)])
        return AXP::AE_FAIL;
    
    return AXP::AS_OK;
}

- (AXP::ARESULT)Reset: (const Byte *)buffer
               Length: (int64_t)length
{
    [mStream SetPosition: 0];
    
    if (![mStream Write: buffer Length: length])
        return AXP::AE_OUTOFMEMORY;

    return [self Reset];
}

- (int64_t)GetPosition
{
    return [mStream GetPosition];
}

- (void)Seek: (int64_t)position
{
    [mStream SetPosition: position];
}

- (const Byte *)GetPayload
{
    return [mStream GetPayload];
}

- (int64_t)GetLength
{
    return [mStream GetLength];
}

- (AXP::ARESULT)Read: (Byte *)dst
           DstLength: (int64_t)dstLength
           Length: (int64_t)length
{
    if (![mStream Read: (Byte*)dst DstLength: dstLength Length: length])
        return AXP::AE_OUTOFMEMORY;
    else
        return AXP::AS_OK;
}           

- (AXP::ARESULT)ReadBoolean: (Boolean*)value
{
    if (![mStream Read: 1])
        return AXP::AE_OUTOFMEMORY;
    
    if (![mStream Read: (Byte*)value DstLength: sizeof(Boolean) Length: sizeof(Boolean)])
        return AXP::AE_OUTOFMEMORY;
    else
        return AXP::AS_OK;
}

- (AXP::ARESULT)ReadByte: (int8_t*)value
{
    if (![mStream Read: 1])
        return AXP::AE_OUTOFMEMORY;

    if (![mStream Read: (Byte*)value DstLength: sizeof(int8_t) Length: sizeof(int8_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return AXP::AS_OK;
}

- (AXP::ARESULT)ReadInt8: (int8_t*)value
{
    if (![mStream Read: 1])
        return AXP::AE_OUTOFMEMORY;

    if (![mStream Read: (Byte*)value DstLength: sizeof(int8_t) Length: sizeof(int8_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return AXP::AS_OK;
}

- (AXP::ARESULT)ReadUInt8: (uint8_t*)value
{
    if (![mStream Read: 1])
        return AXP::AE_OUTOFMEMORY;
        
    if (![mStream Read: (Byte*)value DstLength: sizeof(uint8_t) Length: sizeof(uint8_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return AXP::AS_OK;
}

- (AXP::ARESULT)ReadInt16: (int16_t*)value
{
    if (![mStream Read: 1])
        return AXP::AE_OUTOFMEMORY;
        
    if (![mStream Read: (Byte*)value DstLength: sizeof(int16_t) Length: sizeof(int16_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return AXP::AS_OK;
}

- (AXP::ARESULT)ReadUInt16: (uint16_t*)value
{
    if (![mStream Read: 1])
        return AXP::AE_OUTOFMEMORY;
        
    if (![mStream Read: (Byte*)value DstLength: sizeof(uint16_t) Length: sizeof(uint16_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return AXP::AS_OK;
}

- (AXP::ARESULT)ReadInt32: (int32_t*)value
{
    if (![mStream Read: 1])
        return AXP::AE_OUTOFMEMORY;
        
    if (![mStream Read: (Byte*)value DstLength: sizeof(int32_t) Length: sizeof(int32_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return AXP::AS_OK;
}

- (AXP::ARESULT)ReadUInt32: (uint32_t*)value
{
    if (![mStream Read: 1])
        return AXP::AE_OUTOFMEMORY;
        
    if (![mStream Read: (Byte*)value DstLength: sizeof(uint32_t) Length: sizeof(uint32_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return AXP::AS_OK;
}

- (AXP::ARESULT)ReadInt64: (int64_t*)value
{
    if (![mStream Read: 1])
        return AXP::AE_OUTOFMEMORY;
        
    if (![mStream Read: (Byte*)value DstLength: sizeof(int64_t) Length: sizeof(int64_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return AXP::AS_OK;
}

- (AXP::ARESULT)ReadUInt64: (uint64_t*)value
{
    if (![mStream Read: 1])
        return AXP::AE_OUTOFMEMORY;
        
    if (![mStream Read: (Byte*)value DstLength: sizeof(uint64_t) Length: sizeof(uint64_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return AXP::AS_OK;
}

- (AXP::ARESULT)ReadFloat: (float_t*)value
{
    if (![mStream Read: 1])
        return AXP::AE_OUTOFMEMORY;
        
    if (![mStream Read: (Byte*)value DstLength: sizeof(float_t) Length: sizeof(float_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return AXP::AS_OK;
}

- (AXP::ARESULT)ReadDouble: (double_t*)value
{
    if (![mStream Read: 1])
        return AXP::AE_OUTOFMEMORY;
        
    if (![mStream Read: (Byte*)value DstLength: sizeof(double_t) Length: sizeof(double_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return AXP::AS_OK;
}

- (NSNumber*)ReadNullableBoolean
{
    if (![mStream Read: 1])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        
    Boolean hasValue;
    if (![mStream Read: (Byte*)&hasValue DstLength: sizeof(Boolean) Length: sizeof(Boolean)])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (hasValue) {
        Boolean value;
        if (![mStream Read: (Byte*)&value DstLength: sizeof(Boolean) Length: sizeof(Boolean)])
            @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        else
            return [NSNumber numberWithBool: value];
    }
    else
        return nil;
}

- (NSNumber*)ReadNullableByte
{
    if (![mStream Read: 1])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        
    Boolean hasValue;
    if (![mStream Read: (Byte*)&hasValue DstLength: sizeof(Boolean) Length: sizeof(Boolean)])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (hasValue) {
        int8_t value;
        if (![mStream Read: (Byte*)&value DstLength: sizeof(int8_t) Length: sizeof(int8_t)])
            @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        else
            return [NSNumber numberWithBool: value];
    }
    else
        return nil;       
}

- (NSNumber*)ReadNullableInt8
{
    if (![mStream Read: 1])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];

    Boolean hasValue;
    if (![mStream Read: (Byte*)&hasValue DstLength: sizeof(Boolean) Length: sizeof(Boolean)])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (hasValue) {
        int8_t value;
        if (![mStream Read: (Byte*)&value DstLength: sizeof(int8_t) Length: sizeof(int8_t)])
            @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        else
            return [NSNumber numberWithChar: value];
    }
    else
        return nil; 
}

- (NSNumber*)ReadNullableUInt8
{
    if (![mStream Read: 1])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        
    Boolean hasValue;
    if (![mStream Read: (Byte*)&hasValue DstLength: sizeof(Boolean) Length: sizeof(Boolean)])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (hasValue) {
        uint8_t value;
        if (![mStream Read: (Byte*)&value DstLength: sizeof(uint8_t) Length: sizeof(uint8_t)])
            @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        else
            return [NSNumber numberWithUnsignedChar: value];
    }
    else
        return nil; 
}

- (NSNumber*)ReadNullableInt16
{
    if (![mStream Read: 1])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        
    Boolean hasValue;
    if (![mStream Read: (Byte*)&hasValue DstLength: sizeof(Boolean) Length: sizeof(Boolean)])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (hasValue) {
        int16_t value;
        if (![mStream Read: (Byte*)&value DstLength: sizeof(int16_t) Length: sizeof(int16_t)])
            @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        else
            return [NSNumber numberWithShort: value];
    }
    else
        return nil;
}

- (NSNumber*)ReadNullableUInt16
{
    if (![mStream Read: 1])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        
    Boolean hasValue;
    if (![mStream Read: (Byte*)&hasValue DstLength: sizeof(Boolean) Length: sizeof(Boolean)])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (hasValue) {
        uint16_t value;
        if (![mStream Read: (Byte*)&value DstLength: sizeof(uint16_t) Length: sizeof(uint16_t)])
            @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        else
            return [NSNumber numberWithUnsignedShort: value];
    }
    else
        return nil;
}

- (NSNumber*)ReadNullableInt32
{
    if (![mStream Read: 1])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        
    Boolean hasValue;
    if (![mStream Read: (Byte*)&hasValue DstLength: sizeof(Boolean) Length: sizeof(Boolean)])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (hasValue) {
        int32_t value;
        if (![mStream Read: (Byte*)&value DstLength: sizeof(int32_t) Length: sizeof(int32_t)])
            @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        else
            return [NSNumber numberWithInt: value];
    }
    else
        return nil;
}

- (NSNumber*)ReadNullableUInt32
{
    if (![mStream Read: 1])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        
    Boolean hasValue;
    if (![mStream Read: (Byte*)&hasValue DstLength: sizeof(Boolean) Length: sizeof(Boolean)])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (hasValue) {
        uint32_t value;
        if (![mStream Read: (Byte*)&value DstLength: sizeof(uint32_t) Length: sizeof(uint32_t)])
            @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        else
            return [NSNumber numberWithUnsignedInt: value];
    }
    else
        return nil;        
}

- (NSNumber*)ReadNullableInt64
{
    if (![mStream Read: 1])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        
    Boolean hasValue;
    if (![mStream Read: (Byte*)&hasValue DstLength: sizeof(Boolean) Length: sizeof(Boolean)])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (hasValue) {
        int64_t value;
        if (![mStream Read: (Byte*)&value DstLength: sizeof(int64_t) Length: sizeof(int64_t)])
            @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        else
            return [NSNumber numberWithLongLong: value];
    }
    else
        return nil;           
}

- (NSNumber*)ReadNullableUInt64
{
    if (![mStream Read: 1])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        
    Boolean hasValue;
    if (![mStream Read: (Byte*)&hasValue DstLength: sizeof(Boolean) Length: sizeof(Boolean)])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (hasValue) {
        uint64_t value;
        if (![mStream Read: (Byte*)&value DstLength: sizeof(uint64_t) Length: sizeof(uint64_t)])
            @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        else
            return [NSNumber numberWithUnsignedLongLong: value];
    }
    else
        return nil;        
}

- (NSNumber*)ReadNullableFloat
{
    if (![mStream Read: 1])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        
    Boolean hasValue;
    if (![mStream Read: (Byte*)&hasValue DstLength: sizeof(Boolean) Length: sizeof(Boolean)])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (hasValue) {
        float_t value;
        if (![mStream Read: (Byte*)&value DstLength: sizeof(float_t) Length: sizeof(float_t)])
            @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        else
            return [NSNumber numberWithFloat: value];
    }
    else
        return nil;  
}

- (NSNumber*)ReadNullableDouble
{
    if (![mStream Read: 1])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        
    Boolean hasValue;
    if (![mStream Read: (Byte*)&hasValue DstLength: sizeof(Boolean) Length: sizeof(Boolean)])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (hasValue) {
        double_t value;
        if (![mStream Read: (Byte*)&value DstLength: sizeof(double_t) Length: sizeof(double_t)])
            @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        else
            return [NSNumber numberWithDouble: value];
    }
    else
        return nil;          
}

- (NSData*)ReadByteArray
{
    if (![mStream Read: 1])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        
    int32_t length;
    if (![mStream Read: (Byte*)length DstLength: sizeof(int32_t) Length: sizeof(int32_t)])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (length == 0)
        return nil;
    
    Byte * buffer = (Byte*)malloc(length);
    if (!buffer)
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (![mStream Read: buffer DstLength: length Length: length]) {
        free(buffer);
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    }
    
    NSData * array = [[NSData alloc] initWithBytes: (const void *)buffer length: length];
    free(buffer);
    if (array)
        return array;
    else
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
}

- (NSString*)ReadString
{
    if (![mStream Read: 1])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
        
    int32_t length;
    if (![mStream Read: (Byte*)&length DstLength: sizeof(int32_t) Length: sizeof(int32_t)])
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (length == 0)
        return nil;
    
    Byte * buffer = (Byte*)malloc(length + 1);
    if (!buffer)
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    
    if (![mStream Read: buffer DstLength: length Length: length]) {
        free(buffer);
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
    }
    
    buffer[length] = '\0';
    
    NSString * string = [[NSString alloc] initWithUTF8String: (const char *)buffer];
    free(buffer);
    if (string)
        return string;
    else
        @throw [NSException exceptionWithName: @"" reason: @"" userInfo: nil];
}

- (AXP::ARESULT)Write: (const Byte *)data
           Length: (int64_t)length
{
    if (![mStream Write: data Length: length])
        return AXP::AE_OUTOFMEMORY;
    else
        return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteBoolean: (Boolean)value
{
    char typeName = 'z';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    if (![mStream Write: (const Byte *)&value Length: sizeof(Boolean)])
        return AXP::AE_OUTOFMEMORY;
    else
        return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteByte: (int8_t)value
{
    char typeName = 'c';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    if (![mStream Write: (const Byte *)&value Length: sizeof(int8_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteInt8: (int8_t)value
{
    char typeName = 'b';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    if (![mStream Write: (const Byte *)&value Length: sizeof(int8_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteUInt8: (uint8_t)value
{
    char typeName = 'e';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    if (![mStream Write: (const Byte *)&value Length: sizeof(UInt8)])
        return AXP::AE_OUTOFMEMORY;
    else
        return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteInt16: (int16_t)value
{
    char typeName = 'r';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    if (![mStream Write: (const Byte *)&value Length: sizeof(int16_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteUInt16: (uint16_t)value
{
    char typeName = 'n';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    if (![mStream Write: (const Byte *)&value Length: sizeof(uint16_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteInt32: (int32_t)value
{
    char typeName = 'i';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    if (![mStream Write: (const Byte *)&value Length: sizeof(int32_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteUInt32: (uint32_t)value
{
    char typeName = 'u';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    if (![mStream Write: (const Byte *)&value Length: sizeof(uint32_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteInt64: (int64_t)value
{
    char typeName = 'g';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    if (![mStream Write: (const Byte *)&value Length: sizeof(int64_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteUInt64: (uint64_t)value
{
    char typeName = 'm';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    if (![mStream Write: (const Byte *)&value Length: sizeof(uint64_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteFloat: (float_t)value
{
    char typeName = 'f';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    if (![mStream Write: (const Byte *)&value Length: sizeof(float_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteDouble: (double_t)value
{
    char typeName = 'd';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    if (![mStream Write: (const Byte *)&value Length: sizeof(double_t)])
        return AXP::AE_OUTOFMEMORY;
    else
        return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteNullableBoolean: (NSNumber*)value
{
    char typeName = 'Z';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    Boolean hasValue = true;
    if (!value)
        hasValue = false;

    if (![mStream Write: (const Byte *)&hasValue Length: sizeof(Boolean)])
        return AXP::AE_OUTOFMEMORY;

    if (!value)
        return [self UpdateHeaderLength];
    
    Boolean tempValue = value.boolValue;
    if (![mStream Write: (const Byte *)&tempValue Length: sizeof(Boolean)])
        return AXP::AE_OUTOFMEMORY;

    return [self UpdateHeaderLength];
}
    
- (AXP::ARESULT)WriteNullableByte: (NSNumber*)value
{
    char typeName = 'C';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    Boolean hasValue = true;
    if (!value)
        hasValue = false;

    if (![mStream Write: (const Byte *)&hasValue Length: sizeof(Boolean)])
        return AXP::AE_OUTOFMEMORY;

    if (!value)
        return [self UpdateHeaderLength];
    
    int8_t tempValue = value.charValue;
    if (![mStream Write: (const Byte *)&tempValue Length: sizeof(int8_t)])
        return AXP::AE_OUTOFMEMORY;

    return [self UpdateHeaderLength];
}   

- (AXP::ARESULT)WriteNullableInt8: (NSNumber*)value
{
    char typeName = 'B';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    Boolean hasValue = true;
    if (!value)
        hasValue = false;

    if (![mStream Write: (const Byte *)&hasValue Length: sizeof(Boolean)])
        return AXP::AE_OUTOFMEMORY;

    if (!value)
        return [self UpdateHeaderLength];
    
    int8_t tempValue = value.charValue;
    if (![mStream Write: (const Byte *)&tempValue Length: sizeof(int8_t)])
        return AXP::AE_OUTOFMEMORY;

    return [self UpdateHeaderLength];
}
- (AXP::ARESULT)WriteNullableUInt8: (NSNumber*)value
{
    char typeName = 'E';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    Boolean hasValue = true;
    if (!value)
        hasValue = false;

    if (![mStream Write: (const Byte *)&hasValue Length: sizeof(Boolean)])
        return AXP::AE_OUTOFMEMORY;

    if (!value)
        return [self UpdateHeaderLength];
    
    uint8_t tempValue = value.unsignedCharValue;
    if (![mStream Write: (const Byte *)&tempValue Length: sizeof(uint8_t)])
        return AXP::AE_OUTOFMEMORY;

    return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteNullableInt16: (NSNumber*)value
{
    char typeName = 'R';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    Boolean hasValue = true;
    if (!value)
        hasValue = false;

    if (![mStream Write: (const Byte *)&hasValue Length: sizeof(Boolean)])
        return AXP::AE_OUTOFMEMORY;

    if (!value)
        return [self UpdateHeaderLength];
    
    int16_t tempValue = value.shortValue;
    if (![mStream Write: (const Byte *)&tempValue Length: sizeof(int16_t)])
        return AXP::AE_OUTOFMEMORY;

    return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteNullableUInt16: (NSNumber*)value
{
    char typeName = 'N';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    Boolean hasValue = true;
    if (!value)
        hasValue = false;

    if (![mStream Write: (const Byte *)&hasValue Length: sizeof(Boolean)])
        return AXP::AE_OUTOFMEMORY;

    if (!value)
        return [self UpdateHeaderLength];
    
    uint16_t tempValue = value.unsignedShortValue;
    if (![mStream Write: (const Byte *)&tempValue Length: sizeof(uint16_t)])
        return AXP::AE_OUTOFMEMORY;

    return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteNullableInt32: (NSNumber*)value
{
    char typeName = 'I';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    Boolean hasValue = true;
    if (!value)
        hasValue = false;

    if (![mStream Write: (const Byte *)&hasValue Length: sizeof(Boolean)])
        return AXP::AE_OUTOFMEMORY;

    if (!value)
        return [self UpdateHeaderLength];
    
    int32_t tempValue = value.intValue;
    if (![mStream Write: (const Byte *)&tempValue Length: sizeof(int32_t)])
        return AXP::AE_OUTOFMEMORY;

    return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteNullableUInt32: (NSNumber*)value
{
    char typeName = 'U';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    Boolean hasValue = true;
    if (!value)
        hasValue = false;

    if (![mStream Write: (const Byte *)&hasValue Length: sizeof(Boolean)])
        return AXP::AE_OUTOFMEMORY;

    if (!value)
        return [self UpdateHeaderLength];
    
    uint32_t tempValue = value.unsignedIntValue;
    if (![mStream Write: (const Byte *)&tempValue Length: sizeof(uint32_t)])
        return AXP::AE_OUTOFMEMORY;

    return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteNullableInt64: (NSNumber*)value
{
    char typeName = 'G';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    Boolean hasValue = true;
    if (!value)
        hasValue = false;

    if (![mStream Write: (const Byte *)&hasValue Length: sizeof(Boolean)])
        return AXP::AE_OUTOFMEMORY;

    if (!value)
        return [self UpdateHeaderLength];
    
    int64_t tempValue = value.longLongValue;
    if (![mStream Write: (const Byte *)&tempValue Length: sizeof(int64_t)])
        return AXP::AE_OUTOFMEMORY;

    return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteNullableUInt64: (NSNumber*)value
{
    char typeName = 'G';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    Boolean hasValue = true;
    if (!value)
        hasValue = false;

    if (![mStream Write: (const Byte *)&hasValue Length: sizeof(Boolean)])
        return AXP::AE_OUTOFMEMORY;

    if (!value)
        return [self UpdateHeaderLength];
    
    uint64_t tempValue = value.unsignedLongLongValue;
    if (![mStream Write: (const Byte *)&tempValue Length: sizeof(uint64_t)])
        return AXP::AE_OUTOFMEMORY;

    return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteNullableFloat: (NSNumber*)value
{
    char typeName = 'F';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    Boolean hasValue = true;
    if (!value)
        hasValue = false;

    if (![mStream Write: (const Byte *)&hasValue Length: sizeof(Boolean)])
        return AXP::AE_OUTOFMEMORY;

    if (!value)
        return [self UpdateHeaderLength];
    
    float_t tempValue = value.floatValue;
    if (![mStream Write: (const Byte *)&tempValue Length: sizeof(float_t)])
        return AXP::AE_OUTOFMEMORY;

    return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteNullableDouble: (NSNumber*)value
{
    char typeName = 'D';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    Boolean hasValue = true;
    if (!value)
        hasValue = false;

    if (![mStream Write: (const Byte *)&hasValue Length: sizeof(Boolean)])
        return AXP::AE_OUTOFMEMORY;

    if (!value)
        return [self UpdateHeaderLength];
    
    double_t tempValue = value.doubleValue;
    if (![mStream Write: (const Byte *)&tempValue Length: sizeof(double_t)])
        return AXP::AE_OUTOFMEMORY;

    return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteByteArray: (NSData*)value
{
    char typeName = 'A';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    if (!value) {
        int32_t length = 0;
        if (![mStream Write: (const Byte *)&length Length: sizeof(int32_t)])
            return AXP::AE_OUTOFMEMORY;
    }
    else {
        int32_t length = (int32_t)value.length;
        if (![mStream Write: (const Byte *)&length Length: sizeof(int32_t)])
            return AXP::AE_OUTOFMEMORY;
            
        if (![mStream Write: (const Byte *)value.bytes Length: length])
            return AXP::AE_OUTOFMEMORY;
    }

    return [self UpdateHeaderLength];
}

- (AXP::ARESULT)WriteString: (NSString*)value
{
    char typeName = 'S';
    if (![mStream Write: (const Byte *)&typeName Length: sizeof(char)])
        return AXP::AE_OUTOFMEMORY;

    if (!value) {        
        int32_t length = 0;
        if (![mStream Write: (const Byte *)&length Length: sizeof(int32_t)])
            return AXP::AE_OUTOFMEMORY;
    }
    else {
        const char * ba = [value cStringUsingEncoding: NSUTF8StringEncoding];
        if (!ba)
        return AXP::AE_OUTOFMEMORY;
    
        int32_t length = (int32_t)strlen(ba);
        if (length == 0) {
            if (![mStream Write: (const Byte *)&length Length: sizeof(int32_t)])
                return AXP::AE_OUTOFMEMORY;
        }
        else {
            if (![mStream Write: (const Byte *)&length Length: sizeof(int32_t)])
                return AXP::AE_OUTOFMEMORY;
        
            if (![mStream Write: (const Byte *)ba Length: length])
                return AXP::AE_OUTOFMEMORY;
            }
        }
    
    return [self UpdateHeaderLength];
}

@end