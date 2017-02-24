
#ifndef __PAIR_H__
#define __PAIR_H__

#ifdef PLATFORM_LINUX
#include <wchar.h>
#endif // PLATFORM_LINUX

#include "../object.h"
#include "../coretype.h"

namespace AXP {
    // Key:           结点的Key类型(只支持基本数据类型)
    // Value:         结点的Value类型(只支持RefBase派生的子类)
    template <typename Key, class Value>
    class Pair : public CObject
    {
    public:

        STATIC Sp<Pair> Create(CONST Key & key, CONST Sp<Value> & value)
        {
            Sp<Pair> pair = new Pair();
            if (pair == NULL)
                return NULL;

            pair->mKey = key;
            pair->mValue = value;

            return pair;
        }

        Key GetKey()
        {
            return mKey;
        }

        CONST Key GetKey() CONST
        {
            return mKey;
        }

        Sp<Value> GetValue()
        {
            return mValue;
        }

        CONST Sp<Value> GetValue() CONST
        {
            return mValue;
        }

        Boolean SetKey(CONST Key & key)
        {
            mKey = key;

            return TRUE;
        }

        Boolean SetValue(CONST Sp<Value> & value)
        {
            mValue = value;

            return TRUE;
        }

    protected:

        Pair()
        {
        }

    protected:

        Key mKey;
        Sp<Value> mValue;
    };

    template <class Value>
    class Pair<PStr, Value> : public CObject
    {
    public:

        STATIC Sp<Pair> Create(CONST PStr & key, CONST Sp<Value> & value)
        {
            if (!key)
                return NULL;

            Sp<Pair> pair = new Pair();
            if (pair == NULL)
                return NULL;

            Int32 len = (Int32)strlen(key);
            pair->mKey = ByteArray::Create(len + 1);
            if (pair->mKey == NULL)
                return NULL;

            ARESULT ar = pair->mKey->Append((PByte)key, len);
            if (AFAILED(ar))
                return NULL;

            ar = pair->mKey->Append((PByte)"\0", 1);
            if (AFAILED(ar))
                return NULL;

            pair->mValue = value;

            return pair;
        }

        PStr GetKey()
        {
            if (mKey == NULL)
                return NULL;
            else
                return (PStr)mKey->GetPayload();
        }

        CONST PStr GetKey() CONST
        {
            if (mKey == NULL)
                return NULL;
            else
                return (PStr)mKey->GetPayload();
        }

        Sp<Value> GetValue()
        {
            return mValue;
        }

        CONST Sp<Value> GetValue() CONST
        {
            return mValue;
        }

        Boolean SetKey(CONST PStr & key)
        {
            if (!key)
                return NULL;

            Int32 len = (Int32)strlen(key);
            Sp<ByteArray> temp = ByteArray::Create(len + 1);
            if (temp == NULL)
                return FALSE;

            ARESULT ar = temp->Append((PByte)key, len);
            if (AFAILED(ar))
                return FALSE;

            ar = temp->Append((PByte)"\0", 1);
            if (AFAILED(ar))
                return FALSE;

            return TRUE;
        }

        Boolean SetValue(CONST Sp<Value> & value)
        {
            mValue = value;

            return TRUE;
        }

    protected:

        Pair()
        {
        }

    protected:

        Sp<ByteArray> mKey;
        Sp<Value> mValue;
    };

    template <class Value>
    class Pair<PCStr, Value> : public CObject
    {
    public:

        STATIC Sp<Pair> Create(CONST PCStr & key, CONST Sp<Value> & value)
        {
            if (!key)
                return NULL;

            Sp<Pair> pair = new Pair();
            if (pair == NULL)
                return NULL;

            Int32 len = (Int32)strlen(key);
            pair->mKey = ByteArray::Create(len + 1);
            if (pair->mKey == NULL)
                return NULL;

            ARESULT ar = pair->mKey->Append((PByte)key, len);
            if (AFAILED(ar))
                return NULL;

            ar = pair->mKey->Append((PByte)"\0", 1);
            if (AFAILED(ar))
                return NULL;

            pair->mValue = value;

            return pair;
        }

        PCStr GetKey()
        {
            if (mKey == NULL)
                return NULL;
            else
                return (PCStr)mKey->GetPayload();
        }

        CONST PCStr GetKey() CONST
        {
            if (mKey == NULL)
            return NULL;
            else
                return (PCStr)mKey->GetPayload();
        }

        Sp<Value> GetValue()
        {
            return mValue;
        }

        CONST Sp<Value> GetValue() CONST
        {
            return mValue;
        }

        Boolean SetKey(CONST PCStr & key)
        {
            if (!key)
                return NULL;

            Int32 len = (Int32)strlen(key);
            Sp<ByteArray> temp = ByteArray::Create(len + 1);
            if (temp == NULL)
                return FALSE;

            ARESULT ar = temp->Append((PByte)key, len);
            if (AFAILED(ar))
                return FALSE;

            ar = temp->Append((PByte)"\0", 1);
            if (AFAILED(ar))
                return FALSE;

            return TRUE;
        }

        Boolean SetValue(CONST Sp<Value> & value)
        {
            mValue = value;

            return TRUE;
        }

    protected:

        Pair()
        {
        }

    protected:

        Sp<ByteArray> mKey;
        Sp<Value> mValue;
    };


    template <class Value>
    class Pair<PWStr, Value> : public CObject
    {
    public:

        STATIC Sp<Pair> Create(CONST PWStr & key, CONST Sp<Value> & value)
        {
            if (!key)
                return NULL;

            Sp<Pair> pair = new Pair();
            if (pair == NULL)
                return NULL;

            Int32 len = (Int32)wcslen(key);
            pair->mKey = ByteArray::Create((len + 1) * sizeof(WChar));
            if (pair->mKey == NULL)
                return NULL;

            ARESULT ar = pair->mKey->Append((PByte)key, len * sizeof(WChar));
            if (AFAILED(ar))
                return NULL;

            ar = pair->mKey->Append((PByte)L"\0", sizeof(WChar));
            if (AFAILED(ar))
                return NULL;

            pair->mValue = value;

            return pair;
        }

        PWStr GetKey()
        {
            if (mKey == NULL)
                return NULL;
            else
                return (PWStr)mKey->GetPayload();
        }

        CONST PWStr GetKey() CONST
        {
            if (mKey == NULL)
            return NULL;
            else
                return (PWStr)mKey->GetPayload();
        }

        Sp<Value> GetValue()
        {
            return mValue;
        }

        CONST Sp<Value> GetValue() CONST
        {
            return mValue;
        }

        Boolean SetKey(CONST PWStr & key)
        {
            if (!key)
                return NULL;

            Int32 len = (Int32)wcslen(key);
            Sp<ByteArray> temp = ByteArray::Create((len + 1) * sizeof(WChar));
            if (temp == NULL)
                return FALSE;

            ARESULT ar = temp->Append((PByte)key, len * sizeof(WChar));
            if (AFAILED(ar))
                return FALSE;

            ar = temp->Append((PByte)L"\0", sizeof(WChar));
            if (AFAILED(ar))
                return FALSE;

            return TRUE;
        }

        Boolean SetValue(CONST Sp<Value> & value)
        {
            mValue = value;

            return TRUE;
        }

    protected:

        Pair()
        {
        }

    protected:

        Sp<ByteArray> mKey;
        Sp<Value> mValue;
    };

    template <class Value>
    class Pair<PCWStr, Value> : public CObject
    {
    public:

        STATIC Sp<Pair> Create(CONST PCWStr & key, CONST Sp<Value> & value)
        {
            if (!key)
                return NULL;

            Sp<Pair> pair = new Pair();
            if (pair == NULL)
                return NULL;

            Int32 len = (Int32)wcslen(key);
            pair->mKey = ByteArray::Create((len + 1) * sizeof(WChar));
            if (pair->mKey == NULL)
                return NULL;

            ARESULT ar = pair->mKey->Append((PByte)key, len * sizeof(WChar));
            if (AFAILED(ar))
                return NULL;

            ar = pair->mKey->Append((PByte)L"\0", sizeof(WChar));
            if (AFAILED(ar))
                return NULL;

            pair->mValue = value;

            return pair;
        }

        PCWStr GetKey()
        {
            if (mKey == NULL)
                return NULL;
            else
                return (PCWStr)mKey->GetPayload();
        }

        CONST PCWStr GetKey() CONST
        {
            if (mKey == NULL)
            return NULL;
            else
                return (PCWStr)mKey->GetPayload();
        }

        Sp<Value> GetValue()
        {
            return mValue;
        }

        CONST Sp<Value> GetValue() CONST
        {
            return mValue;
        }

        Boolean SetKey(CONST PCWStr & key)
        {
            if (!key)
                return NULL;

            Int32 len = (Int32)wcslen(key);
            Sp<ByteArray> temp = ByteArray::Create((len + 1) * sizeof(WChar));
            if (temp == NULL)
                return FALSE;

            ARESULT ar = temp->Append((PByte)key, len * sizeof(WChar));
            if (AFAILED(ar))
                return FALSE;

            ar = temp->Append((PByte)L"\0", sizeof(WChar));
            if (AFAILED(ar))
                return FALSE;

            return TRUE;
        }

        Boolean SetValue(CONST Sp<Value> & value)
        {
            mValue = value;

            return TRUE;
        }

    protected:

        Pair()
        {
        }

    protected:

        Sp<ByteArray> mKey;
        Sp<Value> mValue;
    };

} // namespace AXP

#endif // __PAIR_H__
