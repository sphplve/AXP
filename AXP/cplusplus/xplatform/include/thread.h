
#ifndef __THREAD_H__
#define __THREAD_H__

#ifdef PLATFORM_IOS
#include <pthread.h>
#endif // PLATFORM_IOS

#ifdef PLATFORM_LINUX
#include <pthread.h>
#endif // PLATFORM_LINUX

#include "refBase.h"

namespace AXP {
    class Thread : public RefBase
    {
    public:

        Thread() 
        {
            mId = 0;
            mRoutine = NULL;
#ifdef PLATFORM_WIN32
            mHandle = NULL;
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
            mThreadExists = FALSE;
#endif // PLATFORM_WIN32
        }

        Thread(PVoid routine) 
        {
            mId = 0;
            mRoutine = routine;
#ifdef PLATFORM_WIN32
            mHandle = NULL;
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
            mThreadExists = FALSE;
#endif // PLATFORM_WIN32
        }

        VIRTUAL ~Thread();

    public:

        VIRTUAL Boolean Start(PVoid routine, PVoid param);

        VIRTUAL Boolean Start(PVoid param);

        VIRTUAL Boolean WaitFor();

        VIRTUAL ULong GetId() 
        {
            return mId;
        }

    private:

        ULong mId;
        PVoid mRoutine;
#ifdef PLATFORM_WIN32
        Handle mHandle;
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
        pthread_t mHandle;
        Boolean mThreadExists;
#endif // PLATFORM_WIN32
    };

    ULong GetCurrentThreadId();
} // namespace AXP

#endif // __THREAD_H__
