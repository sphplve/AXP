//
//  IParcelable.h
//  TestObjectiveCParcel
//
//  Created by Alex on 15/8/12.
//  Copyright (c) 2015年 Alex. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "AResult.h"
#import "Parcel.h"

@protocol IParcelable

- (AXP::ARESULT)ReadFromParcel: (CParcel*)parcel;

- (AXP::ARESULT)WriteToParcel: (CParcel*)parcel;

@end
