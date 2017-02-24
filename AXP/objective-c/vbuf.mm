//
//  vbuf.m
//  TestObjectiveCParcel
//
//  Created by Alex on 15/8/12.
//  Copyright (c) 2015年 Alex. All rights reserved.
//

#import "vbuf.h"

#define VBUF_ALIGN (4 * 1024)
#define DEFUALT_LENGTH (1024)

static int64_t RoundUp(int64_t n, UInt32 size)
{
    return ((n + size - 1) / size) * size;
}

@interface VBuf () {
@private
    
    Byte mBuffer1[DEFUALT_LENGTH];
    Byte * mBuffer2;
    boolean_t mUseBuffer1;
    int64_t mCapacity;
    int64_t mUsed;
    int32_t mLastError;
}

@end

@implementation VBuf

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
    mUseBuffer1 = TRUE;
    mBuffer2 = NULL;
    mCapacity = DEFUALT_LENGTH;
    mUsed = 0;
    mLastError = 0;
}

- (void)dealloc
{
    if (mBuffer2)
        free(mBuffer2);
}

- (Boolean)Append: (const Byte *)data
           Length: (int64_t)length
{
    if (mUsed + length >= mCapacity) {
        if (mUseBuffer1) {
            mCapacity = RoundUp(mUsed + length, VBUF_ALIGN);
            mBuffer2 = (Byte*)malloc((Size)mCapacity);
            if (!mBuffer2) {
                mLastError = -1;
                return FALSE;
            }
            else {
                memcpy((void*)mBuffer2, (void*)mBuffer1, (Size)mUsed);
                memcpy((void*)(mBuffer2 + mUsed), data, (Size)length);
                mUsed += length;
                mUseBuffer1 = FALSE;
            }
        }
        else {
            mCapacity = RoundUp(mUsed + length, VBUF_ALIGN);
            Byte * tmp = (Byte*)malloc((Size)mCapacity);
            if (!tmp) {
                mLastError = -1;
                return FALSE;
            }
            else {
                memcpy((void*)tmp, (void*)mBuffer2, (Size)mUsed);
                memcpy((void*)(tmp + mUsed), data, (Size)length);
                free(mBuffer2);
                mBuffer2 = tmp;
                mUsed += length;
            }
        }
    }
    else {
        if (mUseBuffer1)
            memcpy((void*)(mBuffer1 + mUsed), data, (Size)length);
        else
            memcpy((void*)(mBuffer2 + mUsed), data, (Size)length);
        
        mUsed += length;
    }
    
    return TRUE;
}

- (void)Clear
{
    mUsed = 0;
    mLastError = 0;
}

- (void)Reset
{
    if (mBuffer2) {
        free(mBuffer2);
        mBuffer2 = NULL;
    }
    
    mUseBuffer1 = TRUE;
    mCapacity = DEFUALT_LENGTH;
    mUsed = 0;
    mLastError = 0;
}

- (const Byte *)GetPayload
{
    if (mUseBuffer1)
        return mBuffer1;
    else
        return mBuffer2;
}

- (int32_t)GetLastError
{
    return mLastError;
}

- (int64_t)GetCapacity
{
    return mCapacity;
}

- (int64_t)GetUsed
{
    return mUsed;
}

- (int64_t)SetUsed: (int64_t)used
{
    used = MIN(used, [self GetCapacity]);
    mUsed = used;
    
    return used;
}

@end