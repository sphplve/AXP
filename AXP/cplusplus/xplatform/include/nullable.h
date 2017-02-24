//
//  nullable.h
//  AXP
//
//  Created by Liu Alex on 14-7-25.
//  Copyright (c) 2014年 Liu Alex. All rights reserved.
//

#ifndef __NULLABLE_H__
#define __NULLABLE_H__

#include "type.h"

namespace AXP {
    template <typename T>
    class Nullable : public CObject
    {
    public:

        Nullable()
        {
            mIsNull = TRUE;
        }

        Nullable(CONST T value)
        {
            mIsNull = FALSE;
            mValue = value;
        }

        Nullable(CONST Nullable & nullable)
        {
            mIsNull = nullable.mIsNull;
            mValue = nullable.mValue;
        }

        INLINE Boolean HasValue() CONST
        {
            return (!mIsNull);
        }

        INLINE Void SetNull()
        {
            mIsNull = TRUE;
        }

        INLINE T GetValue() CONST
        {
            return mValue;
        }

        INLINE Void SetValue(
            IN CONST T value)
        {
            mValue = value;
            mIsNull = FALSE;
        }

        INLINE Boolean Equals(
            IN CONST Nullable & nullable) CONST
        {
            if (mIsNull)
                return FALSE;

            return mValue == nullable.mValue;
        }

        INLINE Boolean Equals(
            IN CONST T value) CONST
        {
            if (mIsNull)
                return FALSE;

            return mValue == value;
        }

        STATIC Nullable Clone(
            IN CONST Nullable & nullable)
        {
            return nullable;
        }

    public:

        Nullable<T> & operator = (
            IN CONST T value)
        {
            SetValue(value);

            return *this;
        }

        Nullable<T> & operator = (
            IN CONST Nullable & nullable)
        {
            mIsNull = nullable.mIsNull;
            mValue = nullable.mValue;

            return *this;
        }

        INLINE Boolean operator == (
            IN CONST T value) CONST
        {
            if (mIsNull)
                return FALSE;

            return mValue == value;
        }

        INLINE Boolean operator != (
            IN CONST T value) CONST
        {
            if (mIsNull)
                return FALSE;

            return mValue != value;
        }

        INLINE Boolean operator == (
            IN CONST Nullable & nullable) CONST
        {
            if (mIsNull)
                return FALSE;

            return mValue == nullable.mValue;
        }

        INLINE Boolean operator != (
            IN CONST Nullable & nullable) CONST
        { 
            if (mIsNull)
                return FALSE;

            return mValue != nullable.mValue;
        }

    private:

        Boolean mIsNull;
        T mValue;
    };

    typedef Nullable<Boolean> Boolean$;
    typedef Nullable<Byte> Byte$;
    typedef Nullable<Char> Char$;
    typedef Nullable<UChar> UChar$;
    typedef Nullable<Short> Short$;
    typedef Nullable<UShort> UShort$;
    typedef Nullable<Word> Word$;
    typedef Nullable<Int> Int$;
    typedef Nullable<UInt> UInt$;
    typedef Nullable<Long> Long$;
    typedef Nullable<ULong> ULong$;
    typedef Nullable<DWord> DWord$;
    typedef Nullable<Int8> Int8$;
    typedef Nullable<UInt8> UInt8$;
    typedef Nullable<Int16> Int16$;
    typedef Nullable<UInt16> UInt16$;
    typedef Nullable<Int32> Int32$;
    typedef Nullable<UInt32> UInt32$;
    typedef Nullable<Int64> Int64$;
    typedef Nullable<UInt64> UInt64$;
    typedef Nullable<Float> Float$;
    typedef Nullable<Double> Double$;
    typedef Nullable<WChar> WChar$;
} // namespace AXP

#endif // __NULLABLE_H__
