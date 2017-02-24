//
//  AException.h
//  IPC
//
//  Created by Alex on 15/8/13.
//  Copyright (c) 2015年 Alex. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AResult.h"

@interface AException : NSException
 
+ (void)Raise: (AXP::ARESULT)ar;

@end
 
@interface ARemoteException : AException

+ (void)Raise: (AXP::ARESULT)ar;

@end

@interface ARemoteRefException : AException

+ (void)Raise: (AXP::ARESULT)ar;

@end

@interface AServerBusyException : AException

+ (void)Raise: (AXP::ARESULT)ar;

@end
