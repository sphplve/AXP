
#ifdef PLATFORM_WIN32
#include <Windows.h>
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
#include <pthread.h>
#endif // PLATFORM_WIN32

#include "../include/mutex.h"

namespace AXP {
    Mutex::Mutex()
    {
#ifdef PLATFORM_WIN32
        ::InitializeCriticalSection((CRITICAL_SECTION*)&mLock);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
        ::pthread_mutexattr_t attr;
        ::pthread_mutexattr_init(&attr);
        ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        ::pthread_mutex_init(&mLock, &attr);
        ::pthread_mutexattr_destroy(&attr);
#endif // PLATFORM_WIN32
    }

    Mutex::~Mutex()
    {
#ifdef PLATFORM_WIN32
        ::DeleteCriticalSection((CRITICAL_SECTION*)&mLock);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
        ::pthread_mutex_destroy(&mLock);
#endif // PLATFORM_WIN32
    }

    Void Mutex::Lock()
    {
#ifdef PLATFORM_WIN32
        ::EnterCriticalSection((CRITICAL_SECTION*)&mLock);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
        ::pthread_mutex_lock(&mLock);
#endif // PLATFORM_WIN32
    }

    Void Mutex::Unlock()
    {
#ifdef PLATFORM_WIN32
        ::LeaveCriticalSection((CRITICAL_SECTION*)&mLock);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
        ::pthread_mutex_unlock(&mLock);
#endif // PLATFORM_WIN32
    }
}