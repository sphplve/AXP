//
//  IPCMutex.h
//  AXP
//
//  Created by Liu Alex on 14/11/22.
//  Copyright (c) 2014年 Liu Alex. All rights reserved.
//

#ifndef __IPC_MUTEX_H__
#define __IPC_MUTEX_H__

#ifdef PLATFORM_IOS
#include <semaphore.h>
#endif // PLATFORM_IOS

#include "../type.h"
#include "../mutex.h"
#include "../aresult.h"
#include "../astring.h"

namespace AXP {
    namespace IPC {
        class IPCMutex : public IMutex
        {
        public:
            
            STATIC Sp<IPCMutex> Create(
                IN CONST Sp<String> & name)
            {
                AXP::Sp<IPCMutex> object = new IPCMutex();
                if (object == NULL)
                    return NULL;
                
                if (AFAILED(object->Init(name, TRUE)))
                    return NULL;
                
                return object;
            }
            
            STATIC Sp<IPCMutex> Open(
                IN CONST Sp<String> & name)
            {
                AXP::Sp<IPCMutex> object = new IPCMutex();
                if (object == NULL)
                    return NULL;
                
                if (AFAILED(object->Init(name, FALSE)))
                    return NULL;
                
                return object;
            }
            
        public:
            
            IPCMutex()
            {
#ifdef PLATFORM_IOS
                mSem = NULL;
#endif // PLATFORM_IOS
            }
            
            VIRTUAL ~IPCMutex()
            {
#ifdef PLATFORM_IOS
                if (mSem) {
                    ::sem_close(mSem);
                    ::sem_unlink((PCStr)mSemName->GetPayload());
                }
#endif // PLATFORM_IOS
            }
            
            ARESULT Init(
                IN CONST Sp<String> & name,
                IN Boolean isCreate)
            {
#ifdef PLATFORM_IOS
                ASSERT(!mSem);
                
                if (name == NULL)
                    return AE_INVALIDARG;
                
                mSemName = name->GetBytes();
                if (mSemName == NULL)
                    return AE_OUTOFMEMORY;
                
                if (isCreate) {
                    ::sem_unlink((PCStr)mSemName->GetPayload());
                    mSem = ::sem_open(
                        (PCStr)mSemName->GetPayload(),
                        O_CREAT | O_RDWR,
                        0666,
                        1);
                }
                else {
                    mSem = ::sem_open((PCStr)mSemName->GetPayload(), 0);
                }
                if (!mSem)
                    return AE_FAIL;
                
                return AS_OK;
#else
                return AE_NOTIMPL;
#endif // PLATFORM_IOS
            }
            
            VIRTUAL Void Lock()
            {
#ifdef PLATFORM_IOS
                ASSERT(mSem);
                
                ::sem_wait(mSem);
#endif // PLATFORM_IOS
            }
            
            VIRTUAL Void Unlock()
            {
#ifdef PLATFORM_IOS
                ASSERT(mSem);
                
                ::sem_post(mSem);
#endif // PLATFORM_IOS
            }
            
        private:

#ifdef PLATFORM_IOS
            sem_t * mSem;
            Sp<ByteArray> mSemName;
#endif // PLATFORM_IOS
        };
    } // namespace IPC
} // namespace AXP

#endif // __IPC_MUTEX_H__
