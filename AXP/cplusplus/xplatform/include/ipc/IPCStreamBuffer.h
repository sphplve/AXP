//
//  IPCStreamBuffer.h
//  AXP
//
//  Created by Liu Alex on 14/11/23.
//  Copyright (c) 2014年 Liu Alex. All rights reserved.
//

#ifndef __IPC_STREAM_BUFFER_H__
#define __IPC_STREAM_BUFFER_H__

#ifdef PLATFORM_IOS
#include <sys/mman.h>
#endif // PLATFORM_IOS

#include "../type.h"
#include "../object.h"
#include "../streambuf.h"
#include "IPCMutex.h"
#include "IPCEvent.h"

namespace AXP {
    namespace IPC {
        class IIPCStreamBuffer : public CObject
        {
        public:
            
            VIRTUAL Void PushBack(PCByte data, Int64 length) = 0;
            
            VIRTUAL Void PopFirst(Int64 length) = 0;
            
            VIRTUAL Int64 PopFirst(PByte buffer, Int64 length) = 0;
            
            VIRTUAL Void Reset() = 0;
            
            VIRTUAL WaitResult Join() = 0;
            
            VIRTUAL WaitResult Join(Int64 milliseconds) = 0;
            
            VIRTUAL PCByte GetPayload() CONST = 0;
            
            VIRTUAL Boolean IsEmpty() CONST = 0;
            
            VIRTUAL Int64 GetUsed() CONST = 0;
            
            VIRTUAL Int64 GetCapacity() CONST = 0;
        };
        
        template <UInt32 capacity>
        class IPCStreamBuffer : public IIPCStreamBuffer
        {
        public:
            
            STATIC Sp<IPCStreamBuffer> Create(
                IN CONST Sp<String> & name)
            {
                AXP::Sp<IPCStreamBuffer> object = new IPCStreamBuffer();
                if (object == NULL)
                    return NULL;
                
                if (AFAILED(object->Init(name, TRUE)))
                    return NULL;
                
                return object;
            }
            
            STATIC Sp<IPCStreamBuffer> Open(
                IN CONST Sp<String> & name)
            {
                AXP::Sp<IPCStreamBuffer> object = new IPCStreamBuffer();
                if (object == NULL)
                    return NULL;
                
                if (AFAILED(object->Init(name, FALSE)))
                    return NULL;
                
                return object;
            }
            
        public:
            
            IPCStreamBuffer()
            {
#ifdef PLATFORM_IOS
                mShm = -1;
                mBuffer = (StreamBuffer<capacity>*)MAP_FAILED;
#endif // PLATFORM_IOS
            }
            
            VIRTUAL ~IPCStreamBuffer()
            {
#ifdef PLATFORM_IOS
                if (mBuffer != MAP_FAILED)
                    munmap(mBuffer, sizeof(StreamBuffer<capacity>));
                
                if (mShm != -1) {
                    if (mShmName != NULL)
                        ::shm_unlink((PCStr)mShmName->GetPayload());
                }
#endif // PLATFORM_IOS
            }
            
        public:
            
            ARESULT Init(
                IN CONST Sp<String> & name,
                IN Boolean isCreate)
            {
#ifdef PLATFORM_IOS
                if (name == NULL)
                    return AE_INVALIDARG;
                
                Sp<String> mutexName = name->Append(L"_mutex");
                if (mutexName == NULL)
                    return AE_OUTOFMEMORY;
                
                Sp<String> eventName = name->Append(L"_event");
                if (eventName == NULL)
                    return AE_OUTOFMEMORY;
                
                Sp<ByteArray> shmName = name->GetBytes();
                if (shmName == NULL)
                    return AE_OUTOFMEMORY;
                
                if (isCreate) {
                    ::shm_unlink((PCStr)shmName->GetPayload());
                    
                    mMutex = IPCMutex::Create(mutexName);
                    if (mMutex == NULL)
                        return AE_FAIL;
                    
                    mEvent = IPCEvent::Create(eventName);
                    if (mEvent == NULL)
                        return AE_FAIL;
                    
                    mShm = ::shm_open(
                        (PCStr)shmName->GetPayload(), O_CREAT | O_RDWR, 0666);
                    if (mShm == -1)
                        return AE_FAIL;
                    
                    if (::ftruncate(mShm, sizeof(StreamBuffer<capacity>)) == -1) {
                        ::shm_unlink((PCStr)shmName->GetPayload());
                        mShm = -1;
                        return AE_FAIL;
                    }
                    
                    mBuffer = (StreamBuffer<capacity>*)::mmap(
                        NULL,
                        sizeof(StreamBuffer<capacity>),
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED,
                        mShm,
                        SEEK_SET);
                    if (mBuffer == MAP_FAILED) {
                        ::shm_unlink((PCStr)shmName->GetPayload());
                        mShm = -1;
                        return AE_FAIL;
                    }
                    
                    mBuffer->Init();
                    mShmName = shmName;
                }
                else {
                    mMutex = IPCMutex::Open(mutexName);
                    if (mMutex == NULL)
                        return AE_FAIL;
                    
                    mEvent = IPCEvent::Open(eventName);
                    if (mEvent == NULL)
                        return AE_FAIL;
                    
                    mShm = ::shm_open(
                        (PCStr)shmName->GetPayload(), O_CREAT | O_RDWR, 0666);
                    if (mShm == -1)
                        return AE_FAIL;
                    
                    mBuffer = (StreamBuffer<capacity>*)::mmap(
                        NULL,
                        sizeof(StreamBuffer<capacity>),
                        PROT_READ | PROT_WRITE,
                        MAP_SHARED,
                        mShm,
                        SEEK_SET);
                    if (mBuffer == MAP_FAILED) {
                        mShm = -1;
                        return AE_FAIL;
                    }
                }
                
                return AS_OK;
#else
                return AE_NOTIMPL;
#endif // PLATFORM_IOS
            }
            
        public:
            
            VIRTUAL Void PushBack(PCByte data, Int64 length)
            {
                Synchronized(mMutex.Get()) {
                    mBuffer->PushBack(data, length);
                }
                
                mEvent->Notify();
            }
            
            VIRTUAL Void PopFirst(Int64 length)
            {
                Synchronized(mMutex.Get()) {
                    mBuffer->PopFirst(length);
                }
            }
            
            VIRTUAL Int64 PopFirst(PByte buffer, Int64 length)
            {
                mMutex->Lock();
                Int64 ret = mBuffer->PopFirst(buffer, length);
                mMutex->Unlock();
                
                return ret;
            }
            
            VIRTUAL Void Reset()
            {
                Synchronized(mMutex.Get()) {
                    mBuffer->Reset();
                }
            }
            
            VIRTUAL WaitResult Join()
            {
                return mEvent->Join();
            }
            
            VIRTUAL WaitResult Join(Int64 milliseconds)
            {
                return mEvent->Join(milliseconds);
            }
            
            VIRTUAL PCByte GetPayload() CONST
            {
                mMutex->Lock();
                PCByte ret = mBuffer->GetPayload();
                mMutex->Unlock();
                
                return ret;
            }
            
            VIRTUAL Boolean IsEmpty() CONST
            {
                mMutex->Lock();
                Boolean ret = mBuffer->IsEmpty();
                mMutex->Unlock();
                
                return ret;
            }
            
            VIRTUAL Int64 GetUsed() CONST
            {
                mMutex->Lock();
                Int64 ret = mBuffer->GetUsed();
                mMutex->Unlock();
                
                return ret;
            }
            
            VIRTUAL Int64 GetCapacity() CONST
            {
                mMutex->Lock();
                Int64 ret = mBuffer->GetCapacity();
                mMutex->Unlock();
                
                return ret;
            }
            
        private:
            
#ifdef PLATFORM_IOS
            Int32 mShm;
            Sp<ByteArray> mShmName;
            StreamBuffer<capacity> * mBuffer;
            Sp<IPCMutex> mMutex;
            Sp<IPCEvent> mEvent;
#endif // PLATFORM_IOS
        };
    } // namespace IPC
} // namespace AXP

#endif // __IPC_STREAM_BUFFER_H__
