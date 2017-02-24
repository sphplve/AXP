//
//  IPCStreamBuffer.h
//  AXP
//
//  Created by Liu Alex on 14/11/23.
//  Copyright (c) 2014年 Liu Alex. All rights reserved.
//

#ifndef __IPC_FIFO_BUFFER_H__
#define __IPC_FIFO_BUFFER_H__

#ifdef PLATFORM_IOS
#include <fcntl.h>
#include <sys/stat.h>
#endif // PLATFORM_IOS

#include "../type.h"
#include "../object.h"
#include "../atime.h"
#include "IPCEvent.h"

#define IPC_FIFO_BUFFER_LENGTH (4 * 1024)

namespace AXP {
    namespace IPC {
        class IPCFIFOBuffer : public CObject
        {
        public:
            
            STATIC Sp<IPCFIFOBuffer> Create(
                IN CONST Sp<String> & name)
            {
                AXP::Sp<IPCFIFOBuffer> object = new IPCFIFOBuffer();
                if (object == NULL)
                    return NULL;
                
                if (AFAILED(object->Init(name, TRUE)))
                    return NULL;
                
                return object;
            }
            
            STATIC Sp<IPCFIFOBuffer> Open(
                IN CONST Sp<String> & name)
            {
                AXP::Sp<IPCFIFOBuffer> object = new IPCFIFOBuffer();
                if (object == NULL)
                    return NULL;
                
                if (AFAILED(object->Init(name, FALSE)))
                    return NULL;
                
                return object;
            }
            
        public:
            
            IPCFIFOBuffer()
            {
#ifdef PLATFORM_IOS
                mFile = -1;
                mRunFlag = TRUE;
#endif // PLATFORM_IOS
            }
            
            VIRTUAL ~IPCFIFOBuffer()
            {
#ifdef PLATFORM_IOS
                if (mFile >= 0) {
                    ::fsync(mFile);
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
                        return AXP::AE_FAIL;
                    
                    mFile = ::open((PCStr)fileName->GetPayload(),
                        O_RDWR | O_NONBLOCK,
                        0666);
                    if (mFile < 0) {
                        ::unlink((PCStr)fileName->GetPayload());
                        return AXP::AE_FAIL;
                    }
                    
                    mFileName = fileName;
                }
                else {
                    mFile = ::open((PCStr)fileName->GetPayload(),
                        O_RDONLY | O_NONBLOCK,
                        0666);
                    if (mFile < 0)
                        return AXP::AE_FAIL;
                }
                
                return AS_OK;
#else
                return AE_NOTIMPL;
#endif // PLATFORM_IOS
            }
            
        public:
            
            Void PushBack(PCByte data, Int64 length)
            {
#ifdef PLATFORM_IOS
                ASSERT(mFile >= 0);
                
                while (mRunFlag && (length > 0)) {
                    Int64 temp = (length > IPC_FIFO_BUFFER_LENGTH) ?
                        IPC_FIFO_BUFFER_LENGTH: length;
                    
                    Long written = ::write(mFile, data, (Size)temp);
                    if (written > 0) {
                        data += written;
                        length -= written;
                    }
                    else {
                        if (errno == EAGAIN)
                            AXP::Sleep(10);
                    }
                }
#endif // PLATFORM_IOS
            }

            Int64 PopFirst(PByte buffer, Int64 length)
            {
#ifdef PLATFORM_IOS
                ASSERT(mFile >= 0);
                
                return (Int64)::read(mFile, buffer, (Size)length);
#else
                return 0;
#endif // PLATFORM_IOS
            }
            
            Void Reset()
            {
#ifdef PLATFORM_IOS
                mRunFlag = TRUE;
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
                
                return WaitResult_OK;
#else
                return WaitResult_Fail;
#endif // PLATFORM_IOS
            }
            
            WaitResult Join(Int64 milliseconds)
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
                
                return WaitResult_OK;
#else
                return WaitResult_Fail;
#endif // PLATFORM_IOS
            }
            
            Void Discard()
            {
#ifdef PLATFORM_IOS
                mRunFlag = FALSE;
#endif // PLATFORM_IOS
            }
            
        private:
            
#ifdef PLATFORM_IOS
            Int32 mFile;
            Sp<ByteArray> mFileName;
            Boolean mRunFlag;
#endif // PLATFORM_IOS
        };
    } // namespace IPC
} // namespace AXP

#endif // __IPC_STREAM_BUFFER_H__
