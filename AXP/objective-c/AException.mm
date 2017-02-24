//
//  AIPCException.m
//  IPC
//
//  Created by Alex on 15/8/13.
//  Copyright (c) 2015年 Alex. All rights reserved.
//

#import "AException.h"

@implementation AException

+ (void)Raise: (AXP::ARESULT)ar
{
    @throw [AException exceptionWithName: @"AException" reason: @"" userInfo: nil];
}

@end

@implementation ARemoteException

+ (void)Raise: (AXP::ARESULT)ar
{
    @throw [ARemoteException exceptionWithName: @"ARemoteException" reason: @"" userInfo: nil];
}

@end

@implementation ARemoteRefException

+ (void)Raise: (AXP::ARESULT)ar
{
    @throw [ARemoteRefException exceptionWithName: @"ARemoteRefException" reason: @"" userInfo: nil];
}

@end

@implementation AServerBusyException

+ (void)Raise: (AXP::ARESULT)ar
{
    @throw [AServerBusyException exceptionWithName: @"AServerBusyException" reason: @"" userInfo: nil];
}

@end