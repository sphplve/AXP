//
//  byteOrder.h
//  TestObjectiveCParcel
//
//  Created by Alex on 15/8/12.
//  Copyright (c) 2015年 Alex. All rights reserved.
//

#ifndef __BYTE_ORDER_H__
#define __BYTE_ORDER_H__

#ifndef SWAP_16
#define SWAP_16(s) \
    ((((s)& 0xff) << 8) | (((s) >> 8) & 0xff))
#endif // SWAP_16
    
#ifndef SWAP_32
#define SWAP_32(l) (((l) >> 24) | \
    (((l)& 0x00ff0000) >> 8) | \
    (((l)& 0x0000ff00) << 8) | \
    ((l) << 24))
#endif // SWAP_32
    
#ifndef SWAP_64
#define SWAP_64(ll) (((ll) >> 56) | \
    (((ll)& 0x00ff000000000000) >> 40) | \
    (((ll)& 0x0000ff0000000000) >> 24) | \
    (((ll)& 0x000000ff00000000) >> 8) | \
    (((ll)& 0x00000000ff000000) << 8) | \
    (((ll)& 0x0000000000ff0000) << 24) | \
    (((ll)& 0x000000000000ff00) << 40) | \
    (((ll) << 56)))
#endif // SWAP_64
    
#ifndef BYTE_ORDER_EQUAL
#ifdef ARCH_LITTLE_ENDIAN
#ifndef AXP_LITTLE_ENDIAN
#define AXP_LITTLE_ENDIAN (TRUE)
#endif // LITTLE_ENDIAN
#define BYTE_ORDER_EQUAL(b) \
    ((AXP_LITTLE_ENDIAN) == (b) ? (TRUE) : (FALSE))
#elif defined(ARCH_BIG_ENDIAN)
#ifndef AXP_LITTLE_ENDIAN
#define AXP_LITTLE_ENDIAN (FALSE)
#endif // LITTLE_ENDIAN
#define BYTE_ORDER_EQUAL(b) \
    ((AXP_LITTLE_ENDIAN) == (b) ? (TRUE) : (FALSE))
#else
#ifndef AXP_LITTLE_ENDIAN
#define AXP_LITTLE_ENDIAN (TRUE)
#endif // LITTLE_ENDIAN
#define BYTE_ORDER_EQUAL(b) \
    ((AXP_LITTLE_ENDIAN) == (b) ? (TRUE) : (FALSE))
#endif // ARCH_LITTLE_ENDIAN
#endif // BYTE_ORDER_EQUAL

#endif // __BYTE_ORDER_H__
