//
//  vbuf.h
//  TestObjectiveCParcel
//
//  Created by Alex on 15/8/12.
//  Copyright (c) 2015年 Alex. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AObject.h"

@interface VBuf : AObject

- (Boolean)Append: (const Byte *)data
           Length: (int64_t)length;

- (void)Clear;

- (void)Reset;

- (const Byte *)GetPayload;

- (int32_t)GetLastError;

- (int64_t)GetCapacity;

- (int64_t)GetUsed;

- (int64_t)SetUsed: (int64_t)used;

@end
