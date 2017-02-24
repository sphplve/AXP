//
//  memoryBuffer.m
//  TestObjectiveCParcel
//
//  Created by Alex on 15/8/12.
//  Copyright (c) 2015年 Alex. All rights reserved.
//

#import "memoryStream.h"

#define VBUF_ALIGN (4 * 1024)
#define DEFUALT_LENGTH (1024)

static int64_t RoundUp(int64_t n, UInt32 size)
{
    return ((n + size - 1) / size) * size;
}

@interface MemoryStream () {
@private
    
    Byte mBuffer1[DEFUALT_LENGTH];
    Byte * mBuffer2;
    boolean_t mUseBuffer1;
    int64_t mCapacity;
    int64_t mLength;
    int64_t mPosition;
    int32_t mLastError;
}

@end

@implementation MemoryStream

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
    mLength = 0;
    mPosition = 0;
    mLastError = 0;
}

- (void)dealloc
{
    if (mBuffer2)
        free(mBuffer2);
}

- (const Byte *)GetPayload
{
    if (mUseBuffer1)
        return mBuffer1;
    else
        return mBuffer2;
}

- (int64_t)GetCapacity
{
    return mCapacity;
}

- (int64_t)GetLength
{
    return mLength;
}

- (int64_t)GetPosition
{
    return mPosition;
}

- (int64_t)SetPosition: (int64_t)position
{
    if (position < 0)
        mPosition = 0;
    else
        mPosition = MIN(position, [self GetCapacity]);
    
    return mPosition;
}

- (int32_t)GetLastError
{
    return mLastError;
}

- (Boolean)Write: (const Byte *)data
           Length: (int64_t)length
{
    if (mPosition + length >= mCapacity) {
        if (mUseBuffer1) {
            mCapacity = RoundUp(mPosition + length, VBUF_ALIGN);
            mBuffer2 = (Byte*)malloc((Size)mCapacity);
            if (!mBuffer2) {
                mLastError = -1;
                return FALSE;
            }
            else {
                memcpy((void*)mBuffer2, (void*)mBuffer1, (Size)mPosition);
                memcpy((void*)(mBuffer2 + mPosition), data, (Size)length);
                mPosition += length;
                mUseBuffer1 = FALSE;
            }
        }
        else {
            mCapacity = RoundUp(mPosition + length, VBUF_ALIGN);
            Byte * tmp = (Byte*)malloc((Size)mCapacity);
            if (!tmp) {
                mLastError = -1;
                return FALSE;
            }
            else {
                memcpy((void*)tmp, (void*)mBuffer2, (Size)mPosition);
                memcpy((void*)(tmp + mPosition), data, (Size)length);
                free(mBuffer2);
                mBuffer2 = tmp;
                mPosition += length;
            }
        }
    }
    else {
        if (mUseBuffer1)
            memcpy((void*)(mBuffer1 + mPosition), data, (Size)length);
        else
            memcpy((void*)(mBuffer2 + mPosition), data, (Size)length);
        
        mPosition += length;
    }
    
    mLength = mPosition;
    
    return TRUE;
}

- (Boolean)Read: (Byte *)dst
           DstLength: (int64_t)dstLength
           Length: (int64_t)length
{
    if ((dstLength < length)
        || (mLength < (mPosition + length)))
        return FALSE;

    const Byte * data = [self GetPayload];
    ::memcpy((void*)dst, (void*)(data + mPosition), (Size)length);

    mPosition += length;

    return TRUE;
}

- (Boolean)Read: (int64_t)length
{
    if ((length > mLength)
        || (mLength < (mPosition + length)))
        return FALSE;

    mPosition += length;

    return TRUE;
}

- (void)Reset
{
	if (mBuffer2) {
	free(mBuffer2);
	mBuffer2 = NULL;
	}
	
	mUseBuffer1 = TRUE;
	mCapacity = DEFUALT_LENGTH;
	mLength = 0;
	mPosition = 0;
	mLastError = 0;
}

- (Boolean) IsEmpty
{
	return (mLength == 0);
}

@end