//
//  AObject.h
//  TestObjectiveCParcel
//
//  Created by Alex on 15/8/12.
//  Copyright (c) 2015年 Alex. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * 定义基础对象
 */
@interface AObject : NSObject<NSCoding>

/**
 * 构造函数
 */
- (id)init;

/**
 * 自定义构造函数
 */
- (void)ctor;

@end
