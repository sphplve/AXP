
#ifdef PLATFORM_WIN32
#include <Windows.h>
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
#include <pthread.h>
#endif // PLATFORM_WIN32

#include "../include/thread.h"

namespace AXP {
    Thread::~Thread() 
    {
        if (mHandle) {
#ifdef PLATFORM_WIN32
            ::CloseHandle(mHandle);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
            mThreadExists = FALSE;
#endif // PLATFORM_WIN32
        }
    }

    Boolean Thread::Start(PVoid routine, PVoid param) 
    {
        mRoutine = routine;

        return Start(param);
    }

    Boolean Thread::Start(PVoid param) 
    {
        WaitFor();
#ifdef PLATFORM_WIN32
        mHandle = ::CreateThread(
            NULL, 0, (LPTHREAD_START_ROUTINE)mRoutine, param, 0, (LPDWORD)&mId);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
        typedef void *(*PFUNC)(void*);
        if (::pthread_create(&mHandle, NULL, (PFUNC)mRoutine, param) != 0)
            return FALSE;
        
        mThreadExists = TRUE;
        mId = (DWord)mHandle;
#endif // PLATFORM_WIN32
        if (!mHandle)
            return FALSE;
        else
            return TRUE;
    }

    Boolean Thread::WaitFor() 
    {
#ifdef PLATFORM_WIN32
        if (mHandle) {
            ::WaitForSingleObject(mHandle, INFINITE);
            ::CloseHandle(mHandle);
            mId = 0;
            mHandle = NULL;
        }
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
        if (mThreadExists) {
            pthread_join(mHandle, NULL);
            mId = 0;
            mThreadExists = FALSE;
        }
#endif // PLATFORM_WIN32
        
        return TRUE;
    }

    ULong GetCurrentThreadId()
    {
#ifdef PLATFORM_WIN32
        return ::GetCurrentThreadId();
#else
        return (ULong)pthread_self();
#endif // PLATFORM_WIN32
    }
}
