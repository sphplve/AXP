
#ifndef __EVENT_H__
#define __EVENT_H__

#ifdef PLATFORM_IOS
#include <pthread.h>
#endif // PLATFORM_IOS

#ifdef PLATFORM_LINUX
#include <pthread.h>
#endif // PLATFORM_LINUX

#include "type.h"
#include "refBase.h"

namespace AXP {
    typedef enum _WaitResult {
        WaitResult_Fail = -1,
        WaitResult_OK = 0,
        WaitResult_Timeout = 1
    } WaitResult;

    class Event : public RefBase
    {
    public:

        Event() 
        {
#ifdef PLATFORM_WIN32
            mEvent = NULL;
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
            mEventExists = FALSE;
#endif // PLATFORM_WIN32
        }

        VIRTUAL ~Event();

    public:

        STATIC AXP::Sp<Event> Create();

        Boolean Notify();

        WaitResult Join();

        WaitResult Join(Int64 milliseconds);

    private:

#ifdef PLATFORM_WIN32
        Handle mEvent;
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
        Boolean mEventExists;
        pthread_mutex_t mMutex;
        pthread_cond_t mCondition;
#endif // PLATFORM_WIN32
    };
} // namespace AXP

#endif // __EVENT_H__
