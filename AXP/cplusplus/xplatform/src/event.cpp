
#ifdef PLATFORM_WIN32
#include <Windows.h>
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
#include <pthread.h>
#include <errno.h>
#endif // PLATFORM_WIN32

#include "../include/event.h"

namespace AXP {
    Event::~Event()
    {
#ifdef PLATFORM_WIN32
        if (mEvent) {
            ::SetEvent(mEvent);
            ::CloseHandle(mEvent);
        }
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
        if (mEventExists) {
            ::pthread_mutex_lock(&mMutex);
            ::pthread_cond_broadcast(&mCondition);
            ::pthread_mutex_unlock(&mMutex);
            ::pthread_cond_destroy(&mCondition);
            ::pthread_mutex_destroy(&mMutex);
            mEventExists = FALSE;
        }
#endif // PLATFORM_WIN32
    }

    AXP::Sp<Event> Event::Create()
    {
        AXP::Sp<Event> object = new Event();
        if (object == NULL)
            return NULL;

#ifdef PLATFORM_WIN32
        object->mEvent = ::CreateEvent(NULL, false, false, NULL);
        if (!object->mEvent)
            return NULL;

#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
        ::pthread_mutexattr_t attr;
        ::pthread_mutexattr_init(&attr);
        ::pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
        ::pthread_mutex_init(&object->mMutex, &attr);
        ::pthread_mutexattr_destroy(&attr);
        
        ::pthread_cond_init(&object->mCondition, NULL);
        object->mEventExists = TRUE;
#endif // PLATFORM_WIN32

        return object;
    }

    Boolean Event::Notify()
    {
#ifdef PLATFORM_WIN32
        return (::SetEvent(mEvent) == TRUE);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
        ::pthread_mutex_lock(&mMutex);
        Boolean ret = (::pthread_cond_broadcast(&mCondition) == 0);
        ::pthread_mutex_unlock(&mMutex);
        return ret;
#endif // PLATFORM_WIN32
    }

    WaitResult Event::Join()
    {
#ifdef PLATFORM_WIN32
        DWORD ret = ::WaitForSingleObject(mEvent, INFINITE);
        if (WAIT_OBJECT_0 == ret)
            return WaitResult_OK;
        else if (WAIT_TIMEOUT == ret)
            return WaitResult_Timeout;
        else
            return WaitResult_Fail;
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
        ::pthread_mutex_lock(&mMutex);
        Int32 ret = ::pthread_cond_wait(&mCondition, &mMutex);
        ::pthread_mutex_unlock(&mMutex);
        if (ret == 0)
            return WaitResult_OK;
        else if (ret == ETIMEDOUT)
            return WaitResult_Timeout;
        else
            return WaitResult_Fail;
#endif // PLATFORM_WIN32
    }

    WaitResult Event::Join(Int64 milliseconds)
    {
#ifdef PLATFORM_WIN32
        DWORD ret = ::WaitForSingleObject(mEvent, (UInt32)milliseconds);
        if (WAIT_OBJECT_0 == ret)
            return WaitResult_OK;
        else if (WAIT_TIMEOUT == ret)
            return WaitResult_Timeout;
        else
            return WaitResult_Fail;
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
        struct timespec time;
        time.tv_sec = (Long)(milliseconds / 1000);
        time.tv_nsec = (milliseconds % 1000) * 1000;

        ::pthread_mutex_lock(&mMutex);
        Int32 ret = ::pthread_cond_timedwait(&mCondition, &mMutex, &time);
        ::pthread_mutex_unlock(&mMutex);
        if (ret == 0)
            return WaitResult_OK;
        else if (ret == ETIMEDOUT)
            return WaitResult_Timeout;
        else
            return WaitResult_Fail;
#endif // PLATFORM_WIN32
    }
}