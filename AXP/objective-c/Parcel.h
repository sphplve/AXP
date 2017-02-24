//
//  CParcel.h
//  TestObjectiveCParcel
//
//  Created by Alex on 15/8/12.
//  Copyright (c) 2015年 Alex. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AResult.h"

@interface CParcel : NSObject

- (AXP::ARESULT)Reset;

- (AXP::ARESULT)Reset: (const Byte *)buffer
               Length: (int64_t)length;

- (int64_t)GetPosition;

- (void)Seek: (int64_t)position;

- (const Byte *)GetPayload;

- (int64_t)GetLength;

- (AXP::ARESULT)Read: (Byte *)dst
           DstLength: (int64_t)dstLength
           Length: (int64_t)length;

- (AXP::ARESULT)ReadBoolean: (Boolean*)value;

- (AXP::ARESULT)ReadByte: (int8_t*)value;

- (AXP::ARESULT)ReadInt8: (int8_t*)value;

- (AXP::ARESULT)ReadUInt8: (uint8_t*)value;

- (AXP::ARESULT)ReadInt16: (int16_t*)value;

- (AXP::ARESULT)ReadUInt16: (uint16_t*)value;

- (AXP::ARESULT)ReadInt32: (int32_t*)value;

- (AXP::ARESULT)ReadUInt32: (uint32_t*)value;

- (AXP::ARESULT)ReadInt64: (int64_t*)value;

- (AXP::ARESULT)ReadUInt64: (uint64_t*)value;

- (AXP::ARESULT)ReadFloat: (float_t*)value;

- (AXP::ARESULT)ReadDouble: (double_t*)value;

- (NSNumber*)ReadNullableBoolean;

- (NSNumber*)ReadNullableByte;

- (NSNumber*)ReadNullableInt8;

- (NSNumber*)ReadNullableUInt8;

- (NSNumber*)ReadNullableInt16;

- (NSNumber*)ReadNullableUInt16;

- (NSNumber*)ReadNullableInt32;

- (NSNumber*)ReadNullableUInt32;

- (NSNumber*)ReadNullableInt64;

- (NSNumber*)ReadNullableUInt64;

- (NSNumber*)ReadNullableFloat;

- (NSNumber*)ReadNullableDouble;

- (NSData*)ReadByteArray;

- (NSString*)ReadString;

- (AXP::ARESULT)Write: (const Byte *)data
           Length: (int64_t)length;

- (AXP::ARESULT)WriteBoolean: (Boolean)value;

- (AXP::ARESULT)WriteByte: (int8_t)value;

- (AXP::ARESULT)WriteInt8: (int8_t)value;

- (AXP::ARESULT)WriteUInt8: (uint8_t)value;

- (AXP::ARESULT)WriteInt16: (int16_t)value;

- (AXP::ARESULT)WriteUInt16: (uint16_t)value;

- (AXP::ARESULT)WriteInt32: (int32_t)value;

- (AXP::ARESULT)WriteUInt32: (uint32_t)value;

- (AXP::ARESULT)WriteInt64: (int64_t)value;

- (AXP::ARESULT)WriteUInt64: (uint64_t)value;

- (AXP::ARESULT)WriteFloat: (float_t)value;

- (AXP::ARESULT)WriteDouble: (double_t)value;

- (AXP::ARESULT)WriteNullableBoolean: (NSNumber*)value;

- (AXP::ARESULT)WriteNullableByte: (NSNumber*)value;

- (AXP::ARESULT)WriteNullableInt8: (NSNumber*)value;

- (AXP::ARESULT)WriteNullableUInt8: (NSNumber*)value;

- (AXP::ARESULT)WriteNullableInt16: (NSNumber*)value;

- (AXP::ARESULT)WriteNullableUInt16: (NSNumber*)value;

- (AXP::ARESULT)WriteNullableInt32: (NSNumber*)value;

- (AXP::ARESULT)WriteNullableUInt32: (NSNumber*)value;

- (AXP::ARESULT)WriteNullableInt64: (NSNumber*)value;

- (AXP::ARESULT)WriteNullableUInt64: (NSNumber*)value;

- (AXP::ARESULT)WriteNullableFloat: (NSNumber*)value;

- (AXP::ARESULT)WriteNullableDouble: (NSNumber*)value;

- (AXP::ARESULT)WriteByteArray: (NSData*)value;

- (AXP::ARESULT)WriteString: (NSString*)value;

@end
