
#ifndef __Hash_TABLE_H__
#define __Hash_TABLE_H__

#include "../../../xplatform/include/list.h"
#include "pair.h"
#include "hashFunction.h"
#include "equalToFunction.h"

namespace AXP {
    // Key:结点的Key类型(只支持基本数据类型)
    // Value:结点的Value类型(只支持RefBase派生的子类)
    template <typename Key, class Value>
    class HashTable : public CObject
    {
    private:

        typedef Pair<Key, Value> KeyValue;

    public:

        // 迭代器
        class Iterator : public CObject
        {
        private:

            typedef typename List<List<KeyValue> >::Iterator BacketsIterator;
            typedef typename List<KeyValue>::Iterator PairIterator;

        public:

            Boolean MoveNext()
            {
                if (mBucketsIterator == NULL)
                    return FALSE;

                if (mPairIterator == NULL) {
                    while (mBucketsIterator->MoveNext()) {
                        Sp<List<KeyValue> > pairList = mBucketsIterator->GetValue();
                        if ((pairList != NULL) && (pairList->GetCount() > 0)) {
                            mPairIterator = pairList->GetIterator();
                            if (mPairIterator != NULL)
                                return mPairIterator->MoveNext();

                            return FALSE;
                        }
                    }

                    return FALSE;
                }
                else {
                    if (!mPairIterator->MoveNext()) {
                        while (mBucketsIterator->MoveNext()) {
                            Sp<List<KeyValue> > pairList = mBucketsIterator->GetValue();
                            if ((pairList != NULL) && (pairList->GetCount() > 0)) {
                                mPairIterator = pairList->GetIterator();
                                if (mPairIterator != NULL)
                                    return mPairIterator->MoveNext();

                                return FALSE;
                            }
                        }

                        return FALSE;
                    }

                    return TRUE;
                }
            }

            Void Reset()
            {
                mPairIterator = NULL;
                mBucketsIterator->Reset();
            }

            Sp<Value> GetValue()
            {
                if (mPairIterator != NULL) {
                    Sp<KeyValue> pair = mPairIterator->GetValue();
                    if (pair != NULL)
                        return pair->GetValue();
                    else
                        return NULL;
                }
                else
                    return NULL;
            }

            Key GetKey()
            {
                if (mPairIterator != NULL) {
                    Sp<KeyValue> pair = mPairIterator->GetValue();
                    if (pair != NULL)
                        return pair->GetKey();
                    else
                        return NULL;
                }
                else
                    return NULL;
            }

        private:

            explicit Iterator(
                IN CONST Sp<BacketsIterator> & bucketsIterator)
            {
                mBucketsIterator = bucketsIterator;
            }

        private:

            FRIEND HashTable;
            Sp<BacketsIterator> mBucketsIterator;
            Sp<PairIterator> mPairIterator;
        };

    public:

        explicit HashTable(
            IN Int32 n)
        {
            InitializeBuckets(n);
        }

        HashTable(
            IN CONST HashTable & hashTable)
        {
            mNumElements = 0;
            CopyFrom(hashTable);
        }

        HashTable & operator = (
            IN CONST HashTable & hashTable)
        {
            if (&hashTable != this) {
                Clear();
                CopyFrom(hashTable);
            }

            return *this;
        }

        ~HashTable()
        {
            Clear();
        }

        // 总数
        Int32 Size() CONST
        {
            return mNumElements;
        }

        // 判断是否为空
        Boolean Empty() CONST
        {
            return Size() == 0;
        }

        STATIC Sp<HashTable> Clone(
            IN CONST Sp<HashTable> & hashTable)
        {
            if (hashTable == NULL)
                return NULL;

            return new HashTable(*hashTable);
        }

    public:

        // 插入操作, 不允许重复
        Boolean InsertUnique(
            IN CONST Key key,
            IN CONST Sp<Value> & value)
        {
            if (!ReSize(mNumElements + 1))
                return FALSE;

            // 获取待插入元素在Hashtable中的索引
            CONST Int32 n = GetMapLocationByKey(key);
            if (mBuckets->GetCount() <= n)
                return FALSE;

            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    // 如果key重复, 在不进行插入, 并告知用户插入失败
                    if (mEquals(pair->GetKey(), key))
                        return FALSE;
                }
            }
            else {
                pairList = new List<KeyValue>();
                if (pairList == NULL)
                    return FALSE;

                mBuckets->SetValue(n, pairList);
            }

            // 插入结点
            Sp<KeyValue> tmp = KeyValue::Create(key, value);
            if (tmp == NULL)
                return FALSE;

            if (!mValueList->PushBack(tmp))
                return FALSE;

            if (!pairList->PushBack(tmp))
                return FALSE;

            ++mNumElements;

            return TRUE;
        }

        // 插入操作, 允许重复
        Boolean InsertEqual(
            IN CONST Key key,
            IN CONST Sp<Value> & value)
        {
            if (!ReSize(mNumElements + 1))
                return FALSE;

            CONST Int32 n = GetMapLocationByKey(key);
            if (mBuckets->GetCount() <= n)
                return FALSE;

            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList == NULL) {
                pairList = new List<KeyValue>();
                if (pairList == NULL)
                    return FALSE;

                mBuckets->SetValue(n, pairList);
            }

            Sp<KeyValue> tmp = KeyValue::Create(key, value);
            if (tmp == NULL)
                return FALSE;

            if (!mValueList->PushBack(tmp))
                return FALSE;

            if (!pairList->PushBack(tmp))
                return FALSE;

            ++mNumElements;

            return TRUE;
        }

        // 查找指定key是否存在
        Boolean Contains(
            CONST Key key)
        {
            Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key))
                        return TRUE;
                }
            }

            return FALSE;
        }

        // 返回key的个数
        Int32 ContainsKeyCount(
            IN CONST Key key) CONST
        {
            CONST Int32 n = GetMapLocationByKey(key);
            Int32 result = 0;
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key))
                        ++result;
                }
            }

            return result;
        }

        // 如果key存在则修改首次的Value, 否则插入
        Boolean SetValue(
            IN CONST Key key,
            IN CONST Sp<Value> & value)
        {
            if (!ReSize(mNumElements + 1))
                return FALSE;

            // 获取待插入元素在Hashtable中的索引
            CONST Int32 n = GetMapLocationByKey(key);
            if (mBuckets->GetCount() <= n)
                return FALSE;

            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    // 如果key重复, 在不进行插入, 并告知用户插入失败
                    if (mEquals(pair->GetKey(), key)) {
                        pair->SetValue(value);
                        return TRUE;
                    }
                }
            }
            else {
                pairList = new List<KeyValue>();
                if (pairList == NULL)
                    return FALSE;

                mBuckets->SetValue(n, pairList);
            }

            // 插入结点
            Sp<KeyValue> tmp = KeyValue::Create(key, value);
            if (tmp == NULL)
                return FALSE;

            if (!mValueList->PushBack(tmp))
                return FALSE;

            if (!pairList->PushBack(tmp))
                return FALSE;

            ++mNumElements;

            return TRUE;
        }

        Sp<Value> GetValue(
            IN CONST Key key)
        {
            Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key))
                        return pair->GetValue();
                }
            }

            return NULL;
        }

        Sp<List<Value> > GetValues(
            IN CONST Key key)
        {
            Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Sp<List<Value> > values = new List<Value>();
                if (values == NULL)
                    return NULL;

                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key)) {
                        if (!values->PushBack(pair->GetValue()))
                            return NULL;
                    }
                }

                return values;
            }

            return NULL;
        }

        // 移除指定key的元素
        Int32 Remove(
            IN CONST Key key)
        {
            // 计算映射位置
            CONST Int32 n = GetMapLocationByKey(key);
            Int32 erased = 0;
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList == NULL)
                return erased;

            // 开始查找并删除
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key)) {
                        pairList->Detach(pair);
                        mValueList->Detach(pair);
                        ++erased;
                        --mNumElements;
                    }
                }
            }

            return erased;
        }

        // 移除指定迭代器的元素
        Int32 Remove(
            IN CONST Sp<Iterator> & it)
        {
            CONST Key key = it->GetKey();

            // 计算映射位置
            CONST Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList == NULL)
                return 0;

            // 开始查找并删除
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key)) {
                        pairList->Detach(pair);
                        mValueList->Detach(pair);
                        --mNumElements;
                        return 1;
                    }
                }
            }

            return 0;
        }

        // 调整Hashtable的容量
        Boolean ReSize(
            IN Int32 numElementsHint)
        {
            Int32 old = mBuckets->GetCount();
            if (numElementsHint > old) {
                for (Int32 i = 0; i < old; i++) {
                    Sp<List<KeyValue> > pairList = mBuckets->Get(i);
                    if (pairList != NULL)
                        pairList->Clear();
                }

                numElementsHint = numElementsHint + (numElementsHint >> 1);
                for (Int32 i = old; i < numElementsHint; i++) {
                    if (!mBuckets->PushBack(NULL))
                        return FALSE;
                }

                mNumElements = 0;
                Foreach(KeyValue, pair, mValueList) {
                    Int32 newBucket = GetMapLocationByKey(pair->GetKey(), numElementsHint);
                    Sp<List<KeyValue> > newPairList = mBuckets->Get(newBucket);
                    if (newPairList != NULL) {
                        if (!newPairList->PushBack(pair))
                            return FALSE;

                        ++mNumElements;
                    }
                    else {
                        newPairList = new List<KeyValue>();
                        if (newPairList == NULL)
                            return FALSE;

                        if (!newPairList->PushBack(pair))
                            return FALSE;

                        ++mNumElements;
                        mBuckets->SetValue(newBucket, newPairList);
                    }
                }
            }

            return TRUE;
        }

        // 清空Hashtable,但是不释放list的内存
        Void Clear()
        {
            for (Int32 i = 0; i < mBuckets->GetCount(); i++) {
                Sp<List<KeyValue> > pairList = mBuckets->Get(i);
                if (pairList != NULL)
                    pairList->Clear();
            }

            mValueList->Clear();
            mNumElements = 0;
        }

        Sp<List<Value> > GetValues()
        {
            if (mValueList == NULL)
                return NULL;

            Sp<List<Value> > values = new List<Value>();
            if (values == NULL)
                return NULL;

            Foreach(KeyValue, pair, mValueList) {
                if (!values->PushBack(pair->GetValue()))
                    return NULL;
            }

            return values;
        }

        Sp<Iterator> GetIterator()
        {
            if (mBuckets != NULL)
                return new Iterator(mBuckets->GetIterator());
            else
                return NULL;
        }

        Sp<Value> operator [](CONST Key key)
        {
            return GetValue(key);
        }

    private:

        // 预留空间,并进行初始化
        Void InitializeBuckets(
            IN Int32 n)
        {
            mValueList = new List<KeyValue>();
            if (mValueList == NULL)
                return;

            mBuckets = new List<List<KeyValue> >();
            if (mBuckets == NULL)
                return;

            for (Int32 i = 0; i < n; i++)
                mBuckets->PushBack(NULL);

            mNumElements = 0;
        }

        // 通过Key获取映射位置
        Int32 GetMapLocationByKey(
            IN CONST Key key) CONST
        {
            return GetMapLocationByKey(key, mBuckets->GetCount());
        }

        // 通过Key获取映射位置
        Int32 GetMapLocationByKey(
            IN CONST Key key,
            Int32 n) CONST
        {
            return mHash(key) % n;
        }

        // 复制HashTable
        Void CopyFrom(
            IN CONST HashTable & hashTable)
        {
            // 预留足够容量
            InitializeBuckets(hashTable.mBuckets->GetCount());

            // 开始copy操作
            for (Int32 i = 0; i < hashTable.mBuckets->GetCount(); i++) {
                Sp<List<KeyValue> > pairList = hashTable.mBuckets->Get(i);
                if (pairList != NULL) {
                    Sp<List<KeyValue> > newPairList = new List<KeyValue>();
                    if (newPairList == NULL)
                        return;

                    Foreach(KeyValue, pair, pairList) {
                        Sp<KeyValue> newPair = KeyValue::Create(pair->GetKey(), pair->GetValue());
                        if (newPair == NULL)
                            return;

                        if (!mValueList->PushBack(newPair))
                            return;

                        if (!newPairList->PushBack(newPair))
                            return;

                        ++mNumElements;
                    }

                    mBuckets->SetValue(i, newPairList);
                }
            }
        }

    public:

        Sp<List<List<KeyValue> > > mBuckets;
        Sp<List<KeyValue> > mValueList;
        Int32 mNumElements;
        Hash<Key> mHash;
        EqualTo<Key> mEquals;
    };

    template <class Value>
    class HashTable<PStr, Value> : public CObject
    {
    private:

        typedef Pair<PStr, Value> KeyValue;

    public:

        // 迭代器
        class Iterator : public CObject
        {
        private:

            typedef typename List<List<KeyValue> >::Iterator BacketsIterator;
            typedef typename List<KeyValue>::Iterator PairIterator;

        public:

            Boolean MoveNext()
            {
                if (mBucketsIterator == NULL)
                    return FALSE;

                if (mPairIterator == NULL) {
                    while (mBucketsIterator->MoveNext()) {
                        Sp<List<KeyValue> > pairList = mBucketsIterator->GetValue();
                        if ((pairList != NULL) && (pairList->GetCount() > 0)) {
                            mPairIterator = pairList->GetIterator();
                            if (mPairIterator != NULL)
                                return mPairIterator->MoveNext();

                            return FALSE;
                        }
                    }

                    return FALSE;
                }
                else {
                    if (!mPairIterator->MoveNext()) {
                        while (mBucketsIterator->MoveNext()) {
                            Sp<List<KeyValue> > pairList = mBucketsIterator->GetValue();
                            if ((pairList != NULL) && (pairList->GetCount() > 0)) {
                                mPairIterator = pairList->GetIterator();
                                if (mPairIterator != NULL)
                                    return mPairIterator->MoveNext();

                                return FALSE;
                            }
                        }

                        return FALSE;
                    }

                    return TRUE;
                }
            }

            Void Reset()
            {
                mPairIterator = NULL;
                mBucketsIterator->Reset();
            }

            Sp<Value> GetValue()
            {
                if (mPairIterator != NULL) {
                    Sp<KeyValue> pair = mPairIterator->GetValue();
                    if (pair != NULL)
                        return pair->GetValue();
                    else
                        return NULL;
                }
                else
                    return NULL;
            }

            PStr GetKey()
            {
                if (mPairIterator != NULL) {
                    Sp<KeyValue> pair = mPairIterator->GetValue();
                    if (pair != NULL)
                        return pair->GetKey();
                    else
                        return NULL;
                }
                else
                    return NULL;
            }

        private:

            explicit Iterator(
                IN CONST Sp<BacketsIterator> & bucketsIterator)
            {
                mBucketsIterator = bucketsIterator;
            }

        private:

            FRIEND HashTable;
            Sp<BacketsIterator> mBucketsIterator;
            Sp<PairIterator> mPairIterator;
        };

    public:

        explicit HashTable(
            IN Int32 n)
        {
            InitializeBuckets(n);
        }

        HashTable(
            IN CONST HashTable & hashTable)
        {
            mNumElements = 0;
            CopyFrom(hashTable);
        }

        HashTable & operator = (
            IN CONST HashTable & hashTable)
        {
            if (&hashTable != this) {
                Clear();
                CopyFrom(hashTable);
            }

            return *this;
        }

        ~HashTable()
        {
            Clear();
        }

        // 总数
        Int32 Size() CONST
        {
            return mNumElements;
        }

        // 判断是否为空
        Boolean Empty() CONST
        {
            return Size() == 0;
        }

        STATIC Sp<HashTable> Clone(
            IN CONST Sp<HashTable> & hashTable)
        {
            if (hashTable == NULL)
                return NULL;

            return new HashTable(*hashTable);
        }

    public:

        // 插入操作, 不允许重复
        Boolean InsertUnique(
            IN CONST PStr key,
            IN CONST Sp<Value> & value)
        {
            if (!key)
                return FALSE;

            if (!ReSize(mNumElements + 1))
                return FALSE;

            // 获取待插入元素在Hashtable中的索引
            CONST Int32 n = GetMapLocationByKey(key);
            if (mBuckets->GetCount() <= n)
                return FALSE;

            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    // 如果key重复, 在不进行插入, 并告知用户插入失败
                    if (mEquals(pair->GetKey(), key))
                        return FALSE;
                }
            }
            else {
                pairList = new List<KeyValue>();
                if (pairList == NULL)
                    return FALSE;

                mBuckets->SetValue(n, pairList);
            }

            // 插入结点
            Sp<KeyValue> tmp = KeyValue::Create(key, value);
            if (tmp == NULL)
                return FALSE;

            if (!mValueList->PushBack(tmp))
                return FALSE;

            if (!pairList->PushBack(tmp))
                return FALSE;

            ++mNumElements;

            return TRUE;
        }

        // 插入操作, 允许重复
        Boolean InsertEqual(
            IN CONST PStr key,
            IN CONST Sp<Value> & value)
        {
            if (!key)
                return FALSE;

            if (!ReSize(mNumElements + 1))
                return FALSE;

            CONST Int32 n = GetMapLocationByKey(key);
            if (mBuckets->GetCount() <= n)
                return FALSE;

            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList == NULL) {
                pairList = new List<KeyValue>();
                if (pairList == NULL)
                    return FALSE;

                mBuckets->SetValue(n, pairList);
            }

            Sp<KeyValue> tmp = KeyValue::Create(key, value);
            if (tmp == NULL)
                return FALSE;

            if (!mValueList->PushBack(tmp))
                return FALSE;

            if (!pairList->PushBack(tmp))
                return FALSE;

            ++mNumElements;

            return TRUE;
        }

        // 查找指定key是否存在
        Boolean Contains(
            CONST PStr key)
        {
            Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key))
                        return TRUE;
                }
            }

            return FALSE;
        }

        // 返回key的个数
        Int32 ContainsKeyCount(
            IN CONST PStr key) CONST
        {
            CONST Int32 n = GetMapLocationByKey(key);
            Int32 result = 0;
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key))
                        ++result;
                }
            }

            return result;
        }

        // 如果key存在则修改首次的Value, 否则插入
        Boolean SetValue(
            IN CONST PStr key,
            IN CONST Sp<Value> & value)
        {
            if (!key)
                return FALSE;

            if (!ReSize(mNumElements + 1))
                return FALSE;

            // 获取待插入元素在Hashtable中的索引
            CONST Int32 n = GetMapLocationByKey(key);
            if (mBuckets->GetCount() <= n)
                return FALSE;

            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    // 如果key重复, 在不进行插入, 并告知用户插入失败
                    if (mEquals(pair->GetKey(), key)) {
                        pair->SetValue(value);
                        return TRUE;
                    }
                }
            }
            else {
                pairList = new List<KeyValue>();
                if (pairList == NULL)
                    return FALSE;

                mBuckets->SetValue(n, pairList);
            }

            // 插入结点
            Sp<KeyValue> tmp = KeyValue::Create(key, value);
            if (tmp == NULL)
                return FALSE;

            if (!mValueList->PushBack(tmp))
                return FALSE;

            if (!pairList->PushBack(tmp))
                return FALSE;

            ++mNumElements;

            return TRUE;
        }

        Sp<Value> GetValue(
            IN CONST PStr key)
        {
            Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key))
                        return pair->GetValue();
                }
            }

            return NULL;
        }

        Sp<List<Value> > GetValues(
            IN CONST PStr key)
        {
            Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Sp<List<Value> > values = new List<Value>();
                if (values == NULL)
                    return NULL;

                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key)) {
                        if (!values->PushBack(pair->GetValue()))
                            return NULL;
                    }
                }

                return values;
            }

            return NULL;
        }

        // 移除指定key的元素
        Int32 Remove(
            IN CONST PStr key)
        {
            // 计算映射位置
            CONST Int32 n = GetMapLocationByKey(key);
            Int32 erased = 0;
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList == NULL)
                return erased;

            // 开始查找并删除
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key)) {
                        pairList->Detach(pair);
                        mValueList->Detach(pair);
                        ++erased;
                        --mNumElements;
                    }
                }
            }

            return erased;
        }

        // 移除指定迭代器的元素
        Int32 Remove(
            IN CONST Sp<Iterator> & it)
        {
            CONST PStr key = it->GetKey();

            // 计算映射位置
            CONST Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList == NULL)
                return 0;

            // 开始查找并删除
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key)) {
                        pairList->Detach(pair);
                        mValueList->Detach(pair);
                        --mNumElements;
                        return 1;
                    }
                }
            }

            return 0;
        }

        // 调整Hashtable的容量
        Boolean ReSize(
            IN Int32 numElementsHint)
        {
            Int32 old = mBuckets->GetCount();
            if (numElementsHint > old) {
                for (Int32 i = 0; i < old; i++) {
                    Sp<List<KeyValue> > pairList = mBuckets->Get(i);
                    if (pairList != NULL)
                        pairList->Clear();
                }

                numElementsHint = numElementsHint + (numElementsHint >> 1);
                for (Int32 i = old; i < numElementsHint; i++) {
                    if (!mBuckets->PushBack(NULL))
                        return FALSE;
                }

                mNumElements = 0;
                Foreach(KeyValue, pair, mValueList) {
                    Int32 newBucket = GetMapLocationByKey(pair->GetKey(), numElementsHint);
                    Sp<List<KeyValue> > newPairList = mBuckets->Get(newBucket);
                    if (newPairList != NULL) {
                        if (!newPairList->PushBack(pair))
                            return FALSE;

                        ++mNumElements;
                    }
                    else {
                        newPairList = new List<KeyValue>();
                        if (newPairList == NULL)
                            return FALSE;

                        if (!newPairList->PushBack(pair))
                            return FALSE;

                        ++mNumElements;
                        mBuckets->SetValue(newBucket, newPairList);
                    }
                }
            }

            return TRUE;
        }

        // 清空Hashtable,但是不释放list的内存
        Void Clear()
        {
            for (Int32 i = 0; i < mBuckets->GetCount(); i++) {
                Sp<List<KeyValue> > pairList = mBuckets->Get(i);
                if (pairList != NULL)
                    pairList->Clear();
            }

            mValueList->Clear();
            mNumElements = 0;
        }

        Sp<List<Value> > GetValues()
        {
            if (mValueList == NULL)
                return NULL;

            Sp<List<Value> > values = new List<Value>();
            if (values == NULL)
                return NULL;

            Foreach(KeyValue, pair, mValueList) {
                if (!values->PushBack(pair->GetValue()))
                    return NULL;
            }

            return values;
        }

        Sp<Iterator> GetIterator()
        {
            if (mBuckets != NULL)
                return new Iterator(mBuckets->GetIterator());
            else
                return NULL;
        }

        Sp<Value> operator [](CONST PStr key)
        {
            return GetValue(key);
        }

    private:

        // 预留空间,并进行初始化
        Void InitializeBuckets(
            IN Int32 n)
        {
            mValueList = new List<KeyValue>();
            if (mValueList == NULL)
                return;

            mBuckets = new List<List<KeyValue> >();
            if (mBuckets == NULL)
                return;

            for (Int32 i = 0; i < n; i++)
                mBuckets->PushBack(NULL);

            mNumElements = 0;
        }

        // 通过Key获取映射位置
        Int32 GetMapLocationByKey(
            IN CONST PStr key) CONST
        {
            return GetMapLocationByKey(key, mBuckets->GetCount());
        }

        // 通过Key获取映射位置
        Int32 GetMapLocationByKey(
            IN CONST PStr key,
            Int32 n) CONST
        {
            return mHash(key) % n;
        }

        // 复制HashTable
        Void CopyFrom(
            IN CONST HashTable & hashTable)
        {
            // 预留足够容量
            InitializeBuckets(hashTable.mBuckets->GetCount());

            // 开始copy操作
            for (Int32 i = 0; i < hashTable.mBuckets->GetCount(); i++) {
                Sp<List<KeyValue> > pairList = hashTable.mBuckets->Get(i);
                if (pairList != NULL) {
                    Sp<List<KeyValue> > newPairList = new List<KeyValue>();
                    if (newPairList == NULL)
                        return;

                    Foreach(KeyValue, pair, pairList) {
                        Sp<KeyValue> newPair = KeyValue::Create(pair->GetKey(), pair->GetValue());
                        if (newPair == NULL)
                            return;

                        if (!mValueList->PushBack(newPair))
                            return;

                        if (!newPairList->PushBack(newPair))
                            return;

                        ++mNumElements;
                    }

                    mBuckets->SetValue(i, newPairList);
                }
            }
        }

    public:

        Sp<List<List<KeyValue> > > mBuckets;
        Sp<List<KeyValue> > mValueList;
        Int32 mNumElements;
        Hash<PStr> mHash;
        EqualTo<PStr> mEquals;
    };

    template <class Value>
    class HashTable<PCStr, Value> : public CObject
    {
    private:

        typedef Pair<PCStr, Value> KeyValue;

    public:

        // 迭代器
        class Iterator : public CObject
        {
        private:

            typedef typename List<List<KeyValue> >::Iterator BacketsIterator;
            typedef typename List<KeyValue>::Iterator PairIterator;

        public:

            Boolean MoveNext()
            {
                if (mBucketsIterator == NULL)
                    return FALSE;

                if (mPairIterator == NULL) {
                    while (mBucketsIterator->MoveNext()) {
                        Sp<List<KeyValue> > pairList = mBucketsIterator->GetValue();
                        if ((pairList != NULL) && (pairList->GetCount() > 0)) {
                            mPairIterator = pairList->GetIterator();
                            if (mPairIterator != NULL)
                                return mPairIterator->MoveNext();

                            return FALSE;
                        }
                    }

                    return FALSE;
                }
                else {
                    if (!mPairIterator->MoveNext()) {
                        while (mBucketsIterator->MoveNext()) {
                            Sp<List<KeyValue> > pairList = mBucketsIterator->GetValue();
                            if ((pairList != NULL) && (pairList->GetCount() > 0)) {
                                mPairIterator = pairList->GetIterator();
                                if (mPairIterator != NULL)
                                    return mPairIterator->MoveNext();

                                return FALSE;
                            }
                        }

                        return FALSE;
                    }

                    return TRUE;
                }
            }

            Void Reset()
            {
                mPairIterator = NULL;
                mBucketsIterator->Reset();
            }

            Sp<Value> GetValue()
            {
                if (mPairIterator != NULL) {
                    Sp<KeyValue> pair = mPairIterator->GetValue();
                    if (pair != NULL)
                        return pair->GetValue();
                    else
                        return NULL;
                }
                else
                    return NULL;
            }

            PCStr GetKey()
            {
                if (mPairIterator != NULL) {
                    Sp<KeyValue> pair = mPairIterator->GetValue();
                    if (pair != NULL)
                        return pair->GetKey();
                    else
                        return NULL;
                }
                else
                    return NULL;
            }

        private:

            explicit Iterator(
                IN CONST Sp<BacketsIterator> & bucketsIterator)
            {
                mBucketsIterator = bucketsIterator;
            }

        private:

            FRIEND HashTable;
            Sp<BacketsIterator> mBucketsIterator;
            Sp<PairIterator> mPairIterator;
        };

    public:

        explicit HashTable(
            IN Int32 n)
        {
            InitializeBuckets(n);
        }

        HashTable(
            IN CONST HashTable & hashTable)
        {
            mNumElements = 0;
            CopyFrom(hashTable);
        }

        HashTable & operator = (
            IN CONST HashTable & hashTable)
        {
            if (&hashTable != this) {
                Clear();
                CopyFrom(hashTable);
            }

            return *this;
        }

        ~HashTable()
        {
            Clear();
        }

        // 总数
        Int32 Size() CONST
        {
            return mNumElements;
        }

        // 判断是否为空
        Boolean Empty() CONST
        {
            return Size() == 0;
        }

        STATIC Sp<HashTable> Clone(
            IN CONST Sp<HashTable> & hashTable)
        {
            if (hashTable == NULL)
                return NULL;

            return new HashTable(*hashTable);
        }

    public:

        // 插入操作, 不允许重复
        Boolean InsertUnique(
            IN CONST PCStr key,
            IN CONST Sp<Value> & value)
        {
            if (!key)
                return FALSE;

            if (!ReSize(mNumElements + 1))
                return FALSE;

            // 获取待插入元素在Hashtable中的索引
            CONST Int32 n = GetMapLocationByKey(key);
            if (mBuckets->GetCount() <= n)
                return FALSE;

            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    // 如果key重复, 在不进行插入, 并告知用户插入失败
                    if (mEquals(pair->GetKey(), key))
                        return FALSE;
                }
            }
            else {
                pairList = new List<KeyValue>();
                if (pairList == NULL)
                    return FALSE;

                mBuckets->SetValue(n, pairList);
            }

            // 插入结点
            Sp<KeyValue> tmp = KeyValue::Create(key, value);
            if (tmp == NULL)
                return FALSE;

            if (!mValueList->PushBack(tmp))
                return FALSE;

            if (!pairList->PushBack(tmp))
                return FALSE;

            ++mNumElements;

            return TRUE;
        }

        // 插入操作, 允许重复
        Boolean InsertEqual(
            IN CONST PCStr key,
            IN CONST Sp<Value> & value)
        {
            if (!key)
                return FALSE;

            if (!ReSize(mNumElements + 1))
                return FALSE;

            CONST Int32 n = GetMapLocationByKey(key);
            if (mBuckets->GetCount() <= n)
                return FALSE;

            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList == NULL) {
                pairList = new List<KeyValue>();
                if (pairList == NULL)
                    return FALSE;

                mBuckets->SetValue(n, pairList);
            }

            Sp<KeyValue> tmp = KeyValue::Create(key, value);
            if (tmp == NULL)
                return FALSE;

            if (!mValueList->PushBack(tmp))
                return FALSE;

            if (!pairList->PushBack(tmp))
                return FALSE;

            ++mNumElements;

            return TRUE;
        }

        // 查找指定key是否存在
        Boolean Contains(
            CONST PCStr key)
        {
            Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key))
                        return TRUE;
                }
            }

            return FALSE;
        }

        // 返回key的个数
        Int32 ContainsKeyCount(
            IN CONST PCStr key) CONST
        {
            CONST Int32 n = GetMapLocationByKey(key);
            Int32 result = 0;
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key))
                        ++result;
                }
            }

            return result;
        }

        // 如果key存在则修改首次的Value, 否则插入
        Boolean SetValue(
            IN CONST PCStr key,
            IN CONST Sp<Value> & value)
        {
            if (!key)
                return FALSE;

            if (!ReSize(mNumElements + 1))
                return FALSE;

            // 获取待插入元素在Hashtable中的索引
            CONST Int32 n = GetMapLocationByKey(key);
            if (mBuckets->GetCount() <= n)
                return FALSE;

            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    // 如果key重复, 在不进行插入, 并告知用户插入失败
                    if (mEquals(pair->GetKey(), key)) {
                        pair->SetValue(value);
                        return TRUE;
                    }
                }
            }
            else {
                pairList = new List<KeyValue>();
                if (pairList == NULL)
                    return FALSE;

                mBuckets->SetValue(n, pairList);
            }

            // 插入结点
            Sp<KeyValue> tmp = KeyValue::Create(key, value);
            if (tmp == NULL)
                return FALSE;

            if (!mValueList->PushBack(tmp))
                return FALSE;

            if (!pairList->PushBack(tmp))
                return FALSE;

            ++mNumElements;

            return TRUE;
        }

        Sp<Value> GetValue(
            IN CONST PCStr key)
        {
            Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key))
                        return pair->GetValue();
                }
            }

            return NULL;
        }

        Sp<List<Value> > GetValues(
            IN CONST PCStr key)
        {
            Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Sp<List<Value> > values = new List<Value>();
                if (values == NULL)
                    return NULL;

                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key)) {
                        if (!values->PushBack(pair->GetValue()))
                            return NULL;
                    }
                }

                return values;
            }

            return NULL;
        }

        // 移除指定key的元素
        Int32 Remove(
            IN CONST PCStr key)
        {
            // 计算映射位置
            CONST Int32 n = GetMapLocationByKey(key);
            Int32 erased = 0;
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList == NULL)
                return erased;

            // 开始查找并删除
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key)) {
                        pairList->Detach(pair);
                        mValueList->Detach(pair);
                        ++erased;
                        --mNumElements;
                    }
                }
            }

            return erased;
        }

        // 移除指定迭代器的元素
        Int32 Remove(
            IN CONST Sp<Iterator> & it)
        {
            CONST PCStr key = it->GetKey();

            // 计算映射位置
            CONST Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList == NULL)
                return 0;

            // 开始查找并删除
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key)) {
                        pairList->Detach(pair);
                        mValueList->Detach(pair);
                        --mNumElements;
                        return 1;
                    }
                }
            }

            return 0;
        }

        // 调整Hashtable的容量
        Boolean ReSize(
            IN Int32 numElementsHint)
        {
            Int32 old = mBuckets->GetCount();
            if (numElementsHint > old) {
                for (Int32 i = 0; i < old; i++) {
                    Sp<List<KeyValue> > pairList = mBuckets->Get(i);
                    if (pairList != NULL)
                        pairList->Clear();
                }

                numElementsHint = numElementsHint + (numElementsHint >> 1);
                for (Int32 i = old; i < numElementsHint; i++) {
                    if (!mBuckets->PushBack(NULL))
                        return FALSE;
                }

                mNumElements = 0;
                Foreach(KeyValue, pair, mValueList) {
                    Int32 newBucket = GetMapLocationByKey(pair->GetKey(), numElementsHint);
                    Sp<List<KeyValue> > newPairList = mBuckets->Get(newBucket);
                    if (newPairList != NULL) {
                        if (!newPairList->PushBack(pair))
                            return FALSE;

                        ++mNumElements;
                    }
                    else {
                        newPairList = new List<KeyValue>();
                        if (newPairList == NULL)
                            return FALSE;

                        if (!newPairList->PushBack(pair))
                            return FALSE;

                        ++mNumElements;
                        mBuckets->SetValue(newBucket, newPairList);
                    }
                }
            }

            return TRUE;
        }

        // 清空Hashtable,但是不释放list的内存
        Void Clear()
        {
            for (Int32 i = 0; i < mBuckets->GetCount(); i++) {
                Sp<List<KeyValue> > pairList = mBuckets->Get(i);
                if (pairList != NULL)
                    pairList->Clear();
            }

            mValueList->Clear();
            mNumElements = 0;
        }

        Sp<List<Value> > GetValues()
        {
            if (mValueList == NULL)
                return NULL;

            Sp<List<Value> > values = new List<Value>();
            if (values == NULL)
                return NULL;

            Foreach(KeyValue, pair, mValueList) {
                if (!values->PushBack(pair->GetValue()))
                    return NULL;
            }

            return values;
        }

        Sp<Iterator> GetIterator()
        {
            if (mBuckets != NULL)
                return new Iterator(mBuckets->GetIterator());
            else
                return NULL;
        }

        Sp<Value> operator [](CONST PCStr key)
        {
            return GetValue(key);
        }

    private:

        // 预留空间,并进行初始化
        Void InitializeBuckets(
            IN Int32 n)
        {
            mValueList = new List<KeyValue>();
            if (mValueList == NULL)
                return;

            mBuckets = new List<List<KeyValue> >();
            if (mBuckets == NULL)
                return;

            for (Int32 i = 0; i < n; i++)
                mBuckets->PushBack(NULL);

            mNumElements = 0;
        }

        // 通过Key获取映射位置
        Int32 GetMapLocationByKey(
            IN CONST PCStr key) CONST
        {
            return GetMapLocationByKey(key, mBuckets->GetCount());
        }

        // 通过Key获取映射位置
        Int32 GetMapLocationByKey(
            IN CONST PCStr key,
            Int32 n) CONST
        {
            return mHash(key) % n;
        }

        // 复制HashTable
        Void CopyFrom(
            IN CONST HashTable & hashTable)
        {
            // 预留足够容量
            InitializeBuckets(hashTable.mBuckets->GetCount());

            // 开始copy操作
            for (Int32 i = 0; i < hashTable.mBuckets->GetCount(); i++) {
                Sp<List<KeyValue> > pairList = hashTable.mBuckets->Get(i);
                if (pairList != NULL) {
                    Sp<List<KeyValue> > newPairList = new List<KeyValue>();
                    if (newPairList == NULL)
                        return;

                    Foreach(KeyValue, pair, pairList) {
                        Sp<KeyValue> newPair = KeyValue::Create(pair->GetKey(), pair->GetValue());
                        if (newPair == NULL)
                            return;

                        if (!mValueList->PushBack(newPair))
                            return;

                        if (!newPairList->PushBack(newPair))
                            return;

                        ++mNumElements;
                    }

                    mBuckets->SetValue(i, newPairList);
                }
            }
        }

    public:

        Sp<List<List<KeyValue> > > mBuckets;
        Sp<List<KeyValue> > mValueList;
        Int32 mNumElements;
        Hash<PCStr> mHash;
        EqualTo<PCStr> mEquals;
    };

    template <class Value>
    class HashTable<PWStr, Value> : public CObject
    {
    private:

        typedef Pair<PWStr, Value> KeyValue;

    public:

        // 迭代器
        class Iterator : public CObject
        {
        private:

            typedef typename List<List<KeyValue> >::Iterator BacketsIterator;
            typedef typename List<KeyValue>::Iterator PairIterator;

        public:

            Boolean MoveNext()
            {
                if (mBucketsIterator == NULL)
                    return FALSE;

                if (mPairIterator == NULL) {
                    while (mBucketsIterator->MoveNext()) {
                        Sp<List<KeyValue> > pairList = mBucketsIterator->GetValue();
                        if ((pairList != NULL) && (pairList->GetCount() > 0)) {
                            mPairIterator = pairList->GetIterator();
                            if (mPairIterator != NULL)
                                return mPairIterator->MoveNext();

                            return FALSE;
                        }
                    }

                    return FALSE;
                }
                else {
                    if (!mPairIterator->MoveNext()) {
                        while (mBucketsIterator->MoveNext()) {
                            Sp<List<KeyValue> > pairList = mBucketsIterator->GetValue();
                            if ((pairList != NULL) && (pairList->GetCount() > 0)) {
                                mPairIterator = pairList->GetIterator();
                                if (mPairIterator != NULL)
                                    return mPairIterator->MoveNext();

                                return FALSE;
                            }
                        }

                        return FALSE;
                    }

                    return TRUE;
                }
            }

            Void Reset()
            {
                mPairIterator = NULL;
                mBucketsIterator->Reset();
            }

            Sp<Value> GetValue()
            {
                if (mPairIterator != NULL) {
                    Sp<KeyValue> pair = mPairIterator->GetValue();
                    if (pair != NULL)
                        return pair->GetValue();
                    else
                        return NULL;
                }
                else
                    return NULL;
            }

            PWStr GetKey()
            {
                if (mPairIterator != NULL) {
                    Sp<KeyValue> pair = mPairIterator->GetValue();
                    if (pair != NULL)
                        return pair->GetKey();
                    else
                        return NULL;
                }
                else
                    return NULL;
            }

        private:

            explicit Iterator(
                IN CONST Sp<BacketsIterator> & bucketsIterator)
            {
                mBucketsIterator = bucketsIterator;
            }

        private:

            FRIEND HashTable;
            Sp<BacketsIterator> mBucketsIterator;
            Sp<PairIterator> mPairIterator;
        };

    public:

        explicit HashTable(
            IN Int32 n)
        {
            InitializeBuckets(n);
        }

        HashTable(
            IN CONST HashTable & hashTable)
        {
            mNumElements = 0;
            CopyFrom(hashTable);
        }

        HashTable & operator = (
            IN CONST HashTable & hashTable)
        {
            if (&hashTable != this) {
                Clear();
                CopyFrom(hashTable);
            }

            return *this;
        }

        ~HashTable()
        {
            Clear();
        }

        // 总数
        Int32 Size() CONST
        {
            return mNumElements;
        }

        // 判断是否为空
        Boolean Empty() CONST
        {
            return Size() == 0;
        }

        STATIC Sp<HashTable> Clone(
            IN CONST Sp<HashTable> & hashTable)
        {
            if (hashTable == NULL)
                return NULL;

            return new HashTable(*hashTable);
        }

    public:

        // 插入操作, 不允许重复
        Boolean InsertUnique(
            IN CONST PWStr key,
            IN CONST Sp<Value> & value)
        {
            if (!key)
                return FALSE;

            if (!ReSize(mNumElements + 1))
                return FALSE;

            // 获取待插入元素在Hashtable中的索引
            CONST Int32 n = GetMapLocationByKey(key);
            if (mBuckets->GetCount() <= n)
                return FALSE;

            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    // 如果key重复, 在不进行插入, 并告知用户插入失败
                    if (mEquals(pair->GetKey(), key))
                        return FALSE;
                }
            }
            else {
                pairList = new List<KeyValue>();
                if (pairList == NULL)
                    return FALSE;

                mBuckets->SetValue(n, pairList);
            }

            // 插入结点
            Sp<KeyValue> tmp = KeyValue::Create(key, value);
            if (tmp == NULL)
                return FALSE;

            if (!mValueList->PushBack(tmp))
                return FALSE;

            if (!pairList->PushBack(tmp))
                return FALSE;

            ++mNumElements;

            return TRUE;
        }

        // 插入操作, 允许重复
        Boolean InsertEqual(
            IN CONST PWStr key,
            IN CONST Sp<Value> & value)
        {
            if (!key)
                return FALSE;

            if (!ReSize(mNumElements + 1))
                return FALSE;

            CONST Int32 n = GetMapLocationByKey(key);
            if (mBuckets->GetCount() <= n)
                return FALSE;

            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList == NULL) {
                pairList = new List<KeyValue>();
                if (pairList == NULL)
                    return FALSE;

                mBuckets->SetValue(n, pairList);
            }

            Sp<KeyValue> tmp = KeyValue::Create(key, value);
            if (tmp == NULL)
                return FALSE;

            if (!mValueList->PushBack(tmp))
                return FALSE;

            if (!pairList->PushBack(tmp))
                return FALSE;

            ++mNumElements;

            return TRUE;
        }

        // 查找指定key是否存在
        Boolean Contains(
            CONST PWStr key)
        {
            Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key))
                        return TRUE;
                }
            }

            return FALSE;
        }

        // 返回key的个数
        Int32 ContainsKeyCount(
            IN CONST PWStr key) CONST
        {
            CONST Int32 n = GetMapLocationByKey(key);
            Int32 result = 0;
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key))
                        ++result;
                }
            }

            return result;
        }

        // 如果key存在则修改首次的Value, 否则插入   
        Boolean SetValue(
            IN CONST PWStr key,
            IN CONST Sp<Value> & value)
        {
            if (!key)
                return FALSE;

            if (!ReSize(mNumElements + 1))
                return FALSE;

            // 获取待插入元素在Hashtable中的索引
            CONST Int32 n = GetMapLocationByKey(key);
            if (mBuckets->GetCount() <= n)
                return FALSE;

            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    // 如果key重复, 在不进行插入, 并告知用户插入失败
                    if (mEquals(pair->GetKey(), key)) {
                        pair->SetValue(value);
                        return TRUE;
                    }
                }
            }
            else {
                pairList = new List<KeyValue>();
                if (pairList == NULL)
                    return FALSE;

                mBuckets->SetValue(n, pairList);
            }

            // 插入结点
            Sp<KeyValue> tmp = KeyValue::Create(key, value);
            if (tmp == NULL)
                return FALSE;

            if (!mValueList->PushBack(tmp))
                return FALSE;

            if (!pairList->PushBack(tmp))
                return FALSE;

            ++mNumElements;

            return TRUE;
        }

        Sp<Value> GetValue(
            IN CONST PWStr key)
        {
            Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key))
                        return pair->GetValue();
                }
            }

            return NULL;
        }

        Sp<List<Value> > GetValues(
            IN CONST PWStr key)
        {
            Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Sp<List<Value> > values = new List<Value>();
                if (values == NULL)
                    return NULL;

                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key)) {
                        if (!values->PushBack(pair->GetValue()))
                            return NULL;
                    }
                }

                return values;
            }

            return NULL;
        }

        // 移除指定key的元素
        Int32 Remove(
            IN CONST PWStr key)
        {
            // 计算映射位置
            CONST Int32 n = GetMapLocationByKey(key);
            Int32 erased = 0;
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList == NULL)
                return erased;

            // 开始查找并删除
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key)) {
                        pairList->Detach(pair);
                        mValueList->Detach(pair);
                        ++erased;
                        --mNumElements;
                    }
                }
            }

            return erased;
        }

        // 移除指定迭代器的元素
        Int32 Remove(
            IN CONST Sp<Iterator> & it)
        {
            CONST PWStr key = it->GetKey();

            // 计算映射位置
            CONST Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList == NULL)
                return 0;

            // 开始查找并删除
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key)) {
                        pairList->Detach(pair);
                        mValueList->Detach(pair);
                        --mNumElements;
                        return 1;
                    }
                }
            }

            return 0;
        }

        // 调整Hashtable的容量
        Boolean ReSize(
            IN Int32 numElementsHint)
        {
            Int32 old = mBuckets->GetCount();
            if (numElementsHint > old) {
                for (Int32 i = 0; i < old; i++) {
                    Sp<List<KeyValue> > pairList = mBuckets->Get(i);
                    if (pairList != NULL)
                        pairList->Clear();
                }

                numElementsHint = numElementsHint + (numElementsHint >> 1);
                for (Int32 i = old; i < numElementsHint; i++) {
                    if (!mBuckets->PushBack(NULL))
                        return FALSE;
                }

                mNumElements = 0;
                Foreach(KeyValue, pair, mValueList) {
                    Int32 newBucket = GetMapLocationByKey(pair->GetKey(), numElementsHint);
                    Sp<List<KeyValue> > newPairList = mBuckets->Get(newBucket);
                    if (newPairList != NULL) {
                        if (!newPairList->PushBack(pair))
                            return FALSE;

                        ++mNumElements;
                    }
                    else {
                        newPairList = new List<KeyValue>();
                        if (newPairList == NULL)
                            return FALSE;

                        if (!newPairList->PushBack(pair))
                            return FALSE;

                        ++mNumElements;
                        mBuckets->SetValue(newBucket, newPairList);
                    }
                }
            }

            return TRUE;
        }

        // 清空Hashtable,但是不释放list的内存
        Void Clear()
        {
            for (Int32 i = 0; i < mBuckets->GetCount(); i++) {
                Sp<List<KeyValue> > pairList = mBuckets->Get(i);
                if (pairList != NULL)
                    pairList->Clear();
            }

            mValueList->Clear();
            mNumElements = 0;
        }

        Sp<List<Value> > GetValues()
        {
            if (mValueList == NULL)
                return NULL;

            Sp<List<Value> > values = new List<Value>();
            if (values == NULL)
                return NULL;

            Foreach(KeyValue, pair, mValueList) {
                if (!values->PushBack(pair->GetValue()))
                    return NULL;
            }

            return values;
        }

        Sp<Iterator> GetIterator()
        {
            if (mBuckets != NULL)
                return new Iterator(mBuckets->GetIterator());
            else
                return NULL;
        }

        Sp<Value> operator [](CONST PWStr key)
        {
            return GetValue(key);
        }

    private:

        // 预留空间,并进行初始化
        Void InitializeBuckets(
            IN Int32 n)
        {
            mValueList = new List<KeyValue>();
            if (mValueList == NULL)
                return;

            mBuckets = new List<List<KeyValue> >();
            if (mBuckets == NULL)
                return;

            for (Int32 i = 0; i < n; i++)
                mBuckets->PushBack(NULL);

            mNumElements = 0;
        }

        // 通过Key获取映射位置
        Int32 GetMapLocationByKey(
            IN CONST PWStr key) CONST
        {
            return GetMapLocationByKey(key, mBuckets->GetCount());
        }

        // 通过Key获取映射位置
        Int32 GetMapLocationByKey(
            IN CONST PWStr key,
            Int32 n) CONST
        {
            return mHash(key) % n;
        }

        // 复制HashTable
        Void CopyFrom(
            IN CONST HashTable & hashTable)
        {
            // 预留足够容量
            InitializeBuckets(hashTable.mBuckets->GetCount());

            // 开始copy操作
            for (Int32 i = 0; i < hashTable.mBuckets->GetCount(); i++) {
                Sp<List<KeyValue> > pairList = hashTable.mBuckets->Get(i);
                if (pairList != NULL) {
                    Sp<List<KeyValue> > newPairList = new List<KeyValue>();
                    if (newPairList == NULL)
                        return;

                    Foreach(KeyValue, pair, pairList) {
                        Sp<KeyValue> newPair = KeyValue::Create(pair->GetKey(), pair->GetValue());
                        if (newPair == NULL)
                            return;

                        if (!mValueList->PushBack(newPair))
                            return;

                        if (!newPairList->PushBack(newPair))
                            return;

                        ++mNumElements;
                    }

                    mBuckets->SetValue(i, newPairList);
                }
            }
        }

    public:

        Sp<List<List<KeyValue> > > mBuckets;
        Sp<List<KeyValue> > mValueList;
        Int32 mNumElements;
        Hash<PWStr> mHash;
        EqualTo<PWStr> mEquals;
    };

    template <class Value>
    class HashTable<PCWStr, Value> : public CObject
    {
    private:

        typedef Pair<PCWStr, Value> KeyValue;

    public:

        // 迭代器
        class Iterator : public CObject
        {
        private:

            typedef typename List<List<KeyValue> >::Iterator BacketsIterator;
            typedef typename List<KeyValue>::Iterator PairIterator;

        public:

            Boolean MoveNext()
            {
                if (mBucketsIterator == NULL)
                    return FALSE;

                if (mPairIterator == NULL) {
                    while (mBucketsIterator->MoveNext()) {
                        Sp<List<KeyValue> > pairList = mBucketsIterator->GetValue();
                        if ((pairList != NULL) && (pairList->GetCount() > 0)) {
                            mPairIterator = pairList->GetIterator();
                            if (mPairIterator != NULL)
                                return mPairIterator->MoveNext();

                            return FALSE;
                        }
                    }

                    return FALSE;
                }
                else {
                    if (!mPairIterator->MoveNext()) {
                        while (mBucketsIterator->MoveNext()) {
                            Sp<List<KeyValue> > pairList = mBucketsIterator->GetValue();
                            if ((pairList != NULL) && (pairList->GetCount() > 0)) {
                                mPairIterator = pairList->GetIterator();
                                if (mPairIterator != NULL)
                                    return mPairIterator->MoveNext();

                                return FALSE;
                            }
                        }

                        return FALSE;
                    }

                    return TRUE;
                }
            }

            Void Reset()
            {
                mPairIterator = NULL;
                mBucketsIterator->Reset();
            }

            Sp<Value> GetValue()
            {
                if (mPairIterator != NULL) {
                    Sp<KeyValue> pair = mPairIterator->GetValue();
                    if (pair != NULL)
                        return pair->GetValue();
                    else
                        return NULL;
                }
                else
                    return NULL;
            }

            PCWStr GetKey()
            {
                if (mPairIterator != NULL) {
                    Sp<KeyValue> pair = mPairIterator->GetValue();
                    if (pair != NULL)
                        return pair->GetKey();
                    else
                        return NULL;
                }
                else
                    return NULL;
            }

        private:

            explicit Iterator(
                IN CONST Sp<BacketsIterator> & bucketsIterator)
            {
                mBucketsIterator = bucketsIterator;
            }

        private:

            FRIEND HashTable;
            Sp<BacketsIterator> mBucketsIterator;
            Sp<PairIterator> mPairIterator;
        };

    public:

        explicit HashTable(
            IN Int32 n)
        {
            InitializeBuckets(n);
        }

        HashTable(
            IN CONST HashTable & hashTable)
        {
            mNumElements = 0;
            CopyFrom(hashTable);
        }

        HashTable & operator = (
            IN CONST HashTable & hashTable)
        {
            if (&hashTable != this) {
                Clear();
                CopyFrom(hashTable);
            }

            return *this;
        }

        ~HashTable()
        {
            Clear();
        }

        // 总数
        Int32 Size() CONST
        {
            return mNumElements;
        }

        // 判断是否为空
        Boolean Empty() CONST
        {
            return Size() == 0;
        }

        STATIC Sp<HashTable> Clone(
            IN CONST Sp<HashTable> & hashTable)
        {
            if (hashTable == NULL)
                return NULL;

            return new HashTable(*hashTable);
        }

    public:

        // 插入操作, 不允许重复
        Boolean InsertUnique(
            IN CONST PCWStr key,
            IN CONST Sp<Value> & value)
        {
            if (!key)
                return FALSE;

            if (!ReSize(mNumElements + 1))
                return FALSE;

            // 获取待插入元素在Hashtable中的索引
            CONST Int32 n = GetMapLocationByKey(key);
            if (mBuckets->GetCount() <= n)
                return FALSE;

            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    // 如果key重复, 在不进行插入, 并告知用户插入失败
                    if (mEquals(pair->GetKey(), key))
                        return FALSE;
                }
            }
            else {
                pairList = new List<KeyValue>();
                if (pairList == NULL)
                    return FALSE;

                mBuckets->SetValue(n, pairList);
            }

            // 插入结点
            Sp<KeyValue> tmp = KeyValue::Create(key, value);
            if (tmp == NULL)
                return FALSE;

            if (!mValueList->PushBack(tmp))
                return FALSE;

            if (!pairList->PushBack(tmp))
                return FALSE;

            ++mNumElements;

            return TRUE;
        }

        // 插入操作, 允许重复
        Boolean InsertEqual(
            IN CONST PCWStr key,
            IN CONST Sp<Value> & value)
        {
            if (!key)
                return FALSE;

            if (!ReSize(mNumElements + 1))
                return FALSE;

            CONST Int32 n = GetMapLocationByKey(key);
            if (mBuckets->GetCount() <= n)
                return FALSE;

            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList == NULL) {
                pairList = new List<KeyValue>();
                if (pairList == NULL)
                    return FALSE;

                mBuckets->SetValue(n, pairList);
            }

            Sp<KeyValue> tmp = KeyValue::Create(key, value);
            if (tmp == NULL)
                return FALSE;

            if (!mValueList->PushBack(tmp))
                return FALSE;

            if (!pairList->PushBack(tmp))
                return FALSE;

            ++mNumElements;

            return TRUE;
        }

        // 查找指定key是否存在
        Boolean Contains(
            CONST PCWStr key)
        {
            Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key))
                        return TRUE;
                }
            }

            return FALSE;
        }

        // 返回key的个数
        Int32 ContainsKeyCount(
            IN CONST PCWStr key) CONST
        {
            CONST Int32 n = GetMapLocationByKey(key);
            Int32 result = 0;
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key))
                        ++result;
                }
            }

            return result;
        }

        // 如果key存在则修改首次的Value, 否则插入
        Boolean SetValue(
            IN CONST PCWStr key,
            IN CONST Sp<Value> & value)
        {
            if (!key)
                return FALSE;

            if (!ReSize(mNumElements + 1))
                return FALSE;

            // 获取待插入元素在Hashtable中的索引
            CONST Int32 n = GetMapLocationByKey(key);
            if (mBuckets->GetCount() <= n)
                return FALSE;

            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    // 如果key重复, 在不进行插入, 并告知用户插入失败
                    if (mEquals(pair->GetKey(), key)) {
                        pair->SetValue(value);
                        return TRUE;
                    }
                }
            }
            else {
                pairList = new List<KeyValue>();
                if (pairList == NULL)
                    return FALSE;

                mBuckets->SetValue(n, pairList);
            }

            // 插入结点
            Sp<KeyValue> tmp = KeyValue::Create(key, value);
            if (tmp == NULL)
                return FALSE;

            if (!mValueList->PushBack(tmp))
                return FALSE;

            if (!pairList->PushBack(tmp))
                return FALSE;

            ++mNumElements;

            return TRUE;
        }

        Sp<Value> GetValue(
            IN CONST PCWStr key)
        {
            Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key))
                        return pair->GetValue();
                }
            }

            return NULL;
        }

        Sp<List<Value> > GetValues(
            IN CONST PCWStr key)
        {
            Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList != NULL) {
                Sp<List<Value> > values = new List<Value>();
                if (values == NULL)
                    return NULL;

                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key)) {
                        if (!values->PushBack(pair->GetValue()))
                            return NULL;
                    }
                }

                return values;
            }

            return NULL;
        }

        // 移除指定key的元素
        Int32 Remove(
            IN CONST PCWStr key)
        {
            // 计算映射位置
            CONST Int32 n = GetMapLocationByKey(key);
            Int32 erased = 0;
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList == NULL)
                return erased;

            // 开始查找并删除
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key)) {
                        pairList->Detach(pair);
                        mValueList->Detach(pair);
                        ++erased;
                        --mNumElements;
                    }
                }
            }

            return erased;
        }

        // 移除指定迭代器的元素
        Int32 Remove(
            IN CONST Sp<Iterator> & it)
        {
            CONST PCWStr key = it->GetKey();

            // 计算映射位置
            CONST Int32 n = GetMapLocationByKey(key);
            Sp<List<KeyValue> > pairList = mBuckets->Get(n);
            if (pairList == NULL)
                return 0;

            // 开始查找并删除
            if (pairList != NULL) {
                Foreach(KeyValue, pair, pairList) {
                    if (mEquals(pair->GetKey(), key)) {
                        pairList->Detach(pair);
                        mValueList->Detach(pair);
                        --mNumElements;
                        return 1;
                    }
                }
            }

            return 0;
        }

        // 调整Hashtable的容量
        Boolean ReSize(
            IN Int32 numElementsHint)
        {
            Int32 old = mBuckets->GetCount();
            if (numElementsHint > old) {
                for (Int32 i = 0; i < old; i++) {
                    Sp<List<KeyValue> > pairList = mBuckets->Get(i);
                    if (pairList != NULL)
                        pairList->Clear();
                }

                numElementsHint = numElementsHint + (numElementsHint >> 1);
                for (Int32 i = old; i < numElementsHint; i++) {
                    if (!mBuckets->PushBack(NULL))
                        return FALSE;
                }

                mNumElements = 0;
                Foreach(KeyValue, pair, mValueList) {
                    Int32 newBucket = GetMapLocationByKey(pair->GetKey(), numElementsHint);
                    Sp<List<KeyValue> > newPairList = mBuckets->Get(newBucket);
                    if (newPairList != NULL) {
                        if (!newPairList->PushBack(pair))
                            return FALSE;

                        ++mNumElements;
                    }
                    else {
                        newPairList = new List<KeyValue>();
                        if (newPairList == NULL)
                            return FALSE;

                        if (!newPairList->PushBack(pair))
                            return FALSE;

                        ++mNumElements;
                        mBuckets->SetValue(newBucket, newPairList);
                    }
                }
            }

            return TRUE;
        }

        // 清空Hashtable,但是不释放list的内存
        Void Clear()
        {
            for (Int32 i = 0; i < mBuckets->GetCount(); i++) {
                Sp<List<KeyValue> > pairList = mBuckets->Get(i);
                if (pairList != NULL)
                    pairList->Clear();
            }

            mValueList->Clear();
            mNumElements = 0;
        }

        Sp<List<Value> > GetValues()
        {
            if (mValueList == NULL)
                return NULL;

            Sp<List<Value> > values = new List<Value>();
            if (values == NULL)
                return NULL;

            Foreach(KeyValue, pair, mValueList) {
                if (!values->PushBack(pair->GetValue()))
                    return NULL;
            }

            return values;
        }

        Sp<Iterator> GetIterator()
        {
            if (mBuckets != NULL)
                return new Iterator(mBuckets->GetIterator());
            else
                return NULL;
        }

        Sp<Value> operator [](CONST PCWStr key)
        {
            return GetValue(key);
        }

    private:

        // 预留空间,并进行初始化
        Void InitializeBuckets(
            IN Int32 n)
        {
            mValueList = new List<KeyValue>();
            if (mValueList == NULL)
                return;

            mBuckets = new List<List<KeyValue> >();
            if (mBuckets == NULL)
                return;

            for (Int32 i = 0; i < n; i++)
                mBuckets->PushBack(NULL);

            mNumElements = 0;
        }

        // 通过Key获取映射位置
        Int32 GetMapLocationByKey(
            IN CONST PCWStr key) CONST
        {
            return GetMapLocationByKey(key, mBuckets->GetCount());
        }

        // 通过Key获取映射位置
        Int32 GetMapLocationByKey(
            IN CONST PCWStr key,
            Int32 n) CONST
        {
            return mHash(key) % n;
        }

        // 复制HashTable
        Void CopyFrom(
            IN CONST HashTable & hashTable)
        {
            // 预留足够容量
            InitializeBuckets(hashTable.mBuckets->GetCount());

            // 开始copy操作
            for (Int32 i = 0; i < hashTable.mBuckets->GetCount(); i++) {
                Sp<List<KeyValue> > pairList = hashTable.mBuckets->Get(i);
                if (pairList != NULL) {
                    Sp<List<KeyValue> > newPairList = new List<KeyValue>();
                    if (newPairList == NULL)
                        return;

                    Foreach(KeyValue, pair, pairList) {
                        Sp<KeyValue> newPair = KeyValue::Create(pair->GetKey(), pair->GetValue());
                        if (newPair == NULL)
                            return;

                        if (!mValueList->PushBack(newPair))
                            return;

                        if (!newPairList->PushBack(newPair))
                            return;

                        ++mNumElements;
                    }

                    mBuckets->SetValue(i, newPairList);
                }
            }
        }

    public:

        Sp<List<List<KeyValue> > > mBuckets;
        Sp<List<KeyValue> > mValueList;
        Int32 mNumElements;
        Hash<PCWStr> mHash;
        EqualTo<PCWStr> mEquals;
    };
} // namespace AXP

#endif // __Hash_TABLE_H__
