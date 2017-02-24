//
//  IPCEvent.h
//  AXP
//
//  Created by Liu Alex on 14/11/23.
//  Copyright (c) 2014年 Liu Alex. All rights reserved.
//

#ifndef __IPC_EVENT_H__
#define __IPC_EVENT_H__

#ifdef PLATFORM_IOS
#include <unistd.h>
#include <sys/stat.h>
#endif // PLATFORM_IOS

#include "../type.h"
#include "../refBase.h"
#include "../aresult.h"
#include "../astring.h"

namespace AXP {
    namespace IPC {
        typedef enum _WaitResult {
            WaitResult_Fail = -1,
            WaitResult_OK = 0,
            WaitResult_Timeout = 1
        } WaitResult;
        
        class IPCEvent : public RefBase
        {
        public:
            
#ifdef PLATFORM_IOS
            struct SharedMemorySyncObject
            {
                pthread_mutex_t mutex;
                pthread_cond_t condition;
            };
#endif // PLATFORM_IOS
            
            STATIC Sp<IPCEvent> Create(
                IN CONST Sp<String> & name)
            {
                AXP::Sp<IPCEvent> object = new IPCEvent();
                if (object == NULL)
                    return NULL;
                
                if (AFAILED(object->Init(name, TRUE)))
                    return NULL;
                
                return object;
            }
            
            STATIC Sp<IPCEvent> Open(
                IN CONST Sp<String> & name)
            {
                AXP::Sp<IPCEvent> object = new IPCEvent();
                if (object == NULL)
                    return NULL;
                
                if (AFAILED(object->Init(name, FALSE)))
                    return NULL;
                
                return object;
            }
            
        public:
            
            IPCEvent()
            {
#ifdef PLATFORM_IOS
                mFile = -1;
#endif // PLATFORM_IOS
            }
            
            VIRTUAL ~IPCEvent()
            {
#ifdef PLATFORM_IOS
                if (mFile >= 0) {
                    ::close(mFile);
                    if (mFileName != NULL)
                        ::unlink((PCStr)mFileName->GetPayload());
                }
#endif // PLATFORM_IOS
            }
            
        public:
            
            ARESULT Init(
                IN CONST Sp<String> & name,
                IN Boolean isCreate)
            {
#ifdef PLATFORM_IOS
                ASSERT(mFile < 0);
                
                if (name == NULL)
                    return AE_INVALIDARG;
                
                Sp<ByteArray> fileName = name->GetBytes();
                if (fileName == NULL)
                    return AE_OUTOFMEMORY;
                
                if (isCreate) {
                    ::unlink((PCStr)fileName->GetPayload());
                    
                    if (::mkfifo((PCStr)fileName->GetPayload(),
                        O_CREAT | O_RDWR | O_EXCL | 0666) != 0)
                        return AE_FAIL;
                    
                    mFile = ::open((PCStr)fileName->GetPayload(),
                        O_RDWR | O_NONBLOCK,
                        0666);
                    if (mFile < 0) {
                        ::unlink((PCStr)fileName->GetPayload());
                        return AE_FAIL;
                    }
                    
                    mFileName = fileName;
                }
                else {
                    mFile = ::open((PCStr)fileName->GetPayload(),
                        O_RDWR | O_NONBLOCK,
                        0666);
                    if (mFile < 0)
                        return AE_FAIL;
                }
                
                return AS_OK;
#else
                return AE_NOTIMPL;
#endif // PLATFORM_IOS
            }
            
            Boolean Notify()
            {
#ifdef PLATFORM_IOS
                ASSERT(mFile >= 0);
                
                Byte data = 0;
                
                return (::write(mFile, &data, sizeof(data)) == sizeof(data));
#else
                return FALSE;
#endif // PLATFORM_IOS
            }
            
            WaitResult Join()
            {
#ifdef PLATFORM_IOS
                ASSERT(mFile >= 0);
                
                struct fd_set fds;
                
                FD_ZERO(&fds);
                FD_SET(mFile, &fds);
                
                Int32 ret = ::select(mFile + 1, &fds, NULL, NULL, NULL);
                if (ret < 0)
                    return WaitResult_Fail;
                
                if (!FD_ISSET(mFile, &fds))
                    return WaitResult_Fail;
                
                if (ret == 0)
                    return WaitResult_Timeout;
                
                Byte data = 0;
                
                ::read(mFile, &data, sizeof(data));
                
                return WaitResult_OK;
#else
                return WaitResult_Fail;
#endif // PLATFORM_IOS
            }
            
            WaitResult Join(
                IN Int64 milliseconds)
            {
#ifdef PLATFORM_IOS
                ASSERT(mFile >= 0);
                
                struct fd_set fds;
                struct timeval time;
                
                time.tv_sec = (Long)(milliseconds / 1000);
                time.tv_usec = (Int32)(milliseconds % 1000);
                
                FD_ZERO(&fds);
                FD_SET(mFile, &fds);
                
                Int32 ret = ::select(mFile + 1, &fds, NULL, NULL, &time);
                if (ret < 0)
                    return WaitResult_Fail;
                
                if (!FD_ISSET(mFile, &fds))
                    return WaitResult_Fail;
                
                if (ret == 0)
                    return WaitResult_Timeout;
                
                Byte data = 0;
                
                ::read(mFile, &data, sizeof(data));
                
                return WaitResult_OK;
#endif // PLATFORM_IOS
            }
            
        private:
            
#ifdef PLATFORM_IOS
            Sp<ByteArray> mFileName;
            Int32 mFile;
#endif // PLATFORM_IOS
        };
    } // namespace IPC
} // namespace AXP

#endif // __IPC_EVENT_H__
