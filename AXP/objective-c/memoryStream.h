//
//  memoryBuffer.h
//  TestObjectiveCParcel
//
//  Created by Alex on 15/8/12.
//  Copyright (c) 2015年 Alex. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AObject.h"

@interface MemoryStream : AObject

- (const Byte *)GetPayload;

- (int64_t)GetCapacity;

- (int64_t)GetLength;

- (int64_t)GetPosition;

- (int64_t)SetPosition: (int64_t)position;

- (int32_t)GetLastError;

- (Boolean)Write: (const Byte *)data
           Length: (int64_t)length;

- (Boolean)Read: (Byte *)dst
           DstLength: (int64_t)dstLength
           Length: (int64_t)length;

- (Boolean)Read: (int64_t)length;

- (void)Reset;

- (Boolean) IsEmpty;

@end
