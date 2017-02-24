
#ifndef __MUTEX_H__
#define __MUTEX_H__

#include "type.h"
#include "refBase.h"

#ifdef PLATFORM_IOS
#include <pthread.h>
#endif // PLATFORM_IOS

#ifdef PLATFORM_LINUX
#include <pthread.h>
#endif // PLATFORM_LINUX

namespace AXP {
#ifdef PLATFORM_WIN32
#pragma pack(push, 8)
    typedef struct _AXP_RTL_CRITICAL_SECTION {
        PVoid DebugInfo;

        //
        //  The following three fields control entering and exiting the critical
        //  section for the resource
        //

        Long LockCount;
        Long RecursionCount;
        Handle OwningThread;        // from the thread's ClientId->UniqueThread
        Handle LockSemaphore;
        PULong SpinCount;        // force size on 64-bit systems when packed
    } AXP_RTL_CRITICAL_SECTION, *PAXP_RTL_CRITICAL_SECTION;
#pragma pack(pop)
    typedef AXP_RTL_CRITICAL_SECTION AXP_CRITICAL_SECTION;
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
    typedef pthread_mutex_t AXP_CRITICAL_SECTION;
#endif // PLATFORM_WIN32
    
    class IMutex : public RefBase
    {
    public:
        
        VIRTUAL Void Lock() = 0;
        
        VIRTUAL Void Unlock() = 0;
    };

    class Mutex : public IMutex
    {
    public:

        Mutex();

        VIRTUAL ~Mutex();

    public:

        VIRTUAL Void Lock();

        VIRTUAL Void Unlock();

    private:

        AXP_CRITICAL_SECTION mLock;
    };

    class CAutoMutex : public RefBase
    {
    public:

        explicit CAutoMutex(
            IN IMutex * mutex)
        {
            mMutex = mutex;
            mMutex->Lock();
            mLocked = TRUE;
        }

        VIRTUAL ~CAutoMutex()
        {
            if ((mLocked) && (mMutex))
                mMutex->Unlock();
        }

        Void Lock()
        {
            mMutex->Lock();
        }

        Void Unlock()
        {
            mLocked = FALSE;
            mMutex->Unlock();
        }

        Boolean IsLocked()
        {
            return mLocked;
        }

        Void Invalidate()
        {
            if ((mLocked) && (mMutex))
                mMutex->Unlock();

            mMutex = NULL;
        }

    private:

        CAutoMutex()
        {
        }

    private:

        IMutex * mMutex;
        Boolean mLocked;
    };

#define Synchronized(pMutex) \
    for (AXP::CAutoMutex lock((pMutex)); lock.IsLocked(); lock.Unlock())
} // namespace AXP

#endif // __MUTEX_H__
