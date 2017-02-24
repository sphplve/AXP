//
//  AObject.m
//  TestObjectiveCParcel
//
//  Created by Alex on 15/8/12.
//  Copyright (c) 2015年 Alex. All rights reserved.
//

#import "AObject.h"

@implementation AObject

- (void)encodeWithCoder: (NSCoder*)encoder
{
}

- (id)initWithCoder: (NSCoder*)decoder
{
    [self ctor];
    
    return self;
}

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
}

@end