
#ifndef __QUEUE_H__
#define __QUEUE_H__

#ifdef PLATFORM_WIN32
#include <malloc.h>
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
#include <memory.h>
#endif // PLATFORM_WIN32

#include "type.h"
#include "define.h"
#include "linkedlist.h"
#include "object.h"
#include "mutex.h"

namespace AXP {
    template <typename T>
    class QueueItem : public DoubleLinkNode
    {
    public:

        Int32 tag;
        Int32 length;
        T value;
    };

    class IQueue : public CObject
    {
    public:

        VIRTUAL Boolean STDCALL Initialize() = 0;

        VIRTUAL Boolean STDCALL PushBack(Int32 tag, Int32 length, PCByte value) = 0;

        VIRTUAL Int32 STDCALL PopBack(PInt32 tag, Int32 length, PCByte value) = 0;

        VIRTUAL Int32 STDCALL PopFirst(PInt32 tag, Int32 length, PCByte value) = 0;

        VIRTUAL Void STDCALL Reset() = 0;

        VIRTUAL Boolean STDCALL IsEmpty() = 0;

        VIRTUAL Int32 STDCALL Used() = 0;

        VIRTUAL PCByte STDCALL Get(Int32 index) = 0;
    };

    template <typename T>
    class Queue : public IQueue
    {
    public:

        Queue(UInt32 itemCount) 
        {
            mCache = NULL;
            mCapacity = sizeof(QueueItem<T>) * itemCount;
            mCount = 0;
            mMaxItemCount = itemCount;
        }

        VIRTUAL ~Queue() 
        {
            mFreeList.DetachAll();
            mUsedList.DetachAll();

            if (mCache)
                free(mCache);
        }

        VIRTUAL Boolean STDCALL Initialize()
        {
            mLock.Lock();

            if (mCache) {
                mLock.Unlock();
                return TRUE;
            }

            if (mCapacity == 0)
                return FALSE;

            mCache = malloc(mCapacity);
            if (!mCache)
                return FALSE;

            mCount = 0;
            QueueItem<T> * p = (QueueItem<T>*)mCache;

            for (Int32 i = 0; i < mMaxItemCount; ++i, ++p) {
                p->Initialize();
                mFreeList.InsertLast(p);
            }

            mLock.Unlock();

            return TRUE;
        }

        VIRTUAL Boolean STDCALL PushBack(Int32 tag, Int32 length, PCByte value)
        {
            ASSERT(length <= sizeof(T));

            mLock.Lock();

            QueueItem<T> * item;

            if (!mFreeList.IsEmpty()) {
                item = mFreeList.First();
                mFreeList.Detach(item);
                mCount++;
            }
            else {
                item = mUsedList.First();
                mUsedList.Detach(item);
            }

            item->tag = tag;
            item->length = length;
            SetData(value, (PCByte)&item->value, length);

            mUsedList.InsertLast(item);

            mLock.Unlock();

            return TRUE;
        }

        VIRTUAL Int32 STDCALL PopBack(PInt32 tag, Int32 length, PCByte value)
        {
            ASSERT(length <= sizeof(T));

            mLock.Lock();

            if (mCount == 0) {
                mLock.Unlock();
                return -1;
            }

            QueueItem<T> * item = mUsedList.Last();
            mUsedList.Detach(item);

            if (tag) *tag = item->tag;
            Int32 len = Min(item->length, length);
            GetData((PCByte)&item->value, value, len);
            mFreeList.InsertLast(item);
            mCount--;

            mLock.Unlock();

            return len;
        }

        VIRTUAL Int32 STDCALL PopFirst(PInt32 tag, Int32 length, PCByte value)
        {
            mLock.Lock();

            if (mCount == 0) {
                mLock.Unlock();
                return -1;
            }

            QueueItem<T> * item = mUsedList.First();
            mUsedList.Detach(item);

            if (tag) *tag = item->tag;
            Int32 len = Min(item->length, length);
            GetData((PCByte)&item->value, value, len);
            mFreeList.InsertLast(item);
            mCount--;

            mLock.Unlock();

            return len;
        }

        VIRTUAL Void STDCALL Reset()
        {
            mLock.Lock();

            mCount = 0;

            QueueItem<T> *pNode, *pTempNode;

            ForeachDLinkNodeEx(QueueItem<T>, pNode, pTempNode, &mUsedList) {
                mUsedList.Detach(pNode);
                mFreeList.InsertLast(pNode);
            }

            mLock.Unlock();
        }

        VIRTUAL Boolean STDCALL IsEmpty()
        {
            mLock.Lock();  
            Boolean ret = (mCount == 0);
            mLock.Unlock();

            return ret;
        }

        VIRTUAL Int32 STDCALL Used()
        {
            mLock.Lock();  
            Int32 count = mCount;
            mLock.Unlock();

            return count;
        }

        VIRTUAL PCByte STDCALL Get(Int32 index)
        {
            mLock.Lock();

            if ((index < 0) || (index >= mCount)) {
                mLock.Unlock();
                return NULL;
            }

            QueueItem<T> * item = mUsedList[index];
            PCByte ret = (PCByte)&item->value;

            mLock.Unlock();

            return ret;
        }

        PCByte STDCALL operator [] (Int32 index)
        {
            return Get(index);
        }

    protected:

        Queue() { }

        VIRTUAL Void STDCALL GetData(PCByte src, PCByte dst, Int32 length)
        {
            memcpy((PVoid)dst, (PVoid)src, length);
        }

        VIRTUAL Void STDCALL SetData(PCByte src, PCByte dst, Int32 length)
        {
            memcpy((PVoid)dst, (PVoid)src, length);
        }

    protected:

        Mutex mLock;
        PVoid mCache;
        Int32 mCapacity;
        Int32 mCount;
        Int32 mMaxItemCount;
        CDLinkList<QueueItem<T> > mUsedList;
        CDLinkList<QueueItem<T> > mFreeList;
    };

#pragma pack(1)

    template <Int32 capacity>
    class QueueDataItem
    {
    public:

        Byte mData[capacity];
    };

#pragma pack()

    template <Int32 capacity>
    class DataQueue : public Queue<QueueDataItem<capacity> >
    {
    public:

        DataQueue(UInt32 itemCount)
            : Queue<QueueDataItem<capacity> >(itemCount)
        {
            ASSERT(capacity == sizeof(QueueDataItem<capacity>));
        }
    };
} // namespace AXP

#endif // __QUEUE_H__