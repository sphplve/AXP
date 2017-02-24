
#ifdef PLATFORM_WIN32
#include <Windows.h>
#elif defined(PLATFORM_IOS)
#include <sys/time.h>
#elif defined(PLATFORM_LINUX)
#include <sys/time.h>
#include <time.h>
#endif // PLATFORM_WIN32

#include "../include/atime.h"

namespace AXP {
#ifdef PLATFORM_WIN32
    Void Sleep(UInt32 elapsed)
    {
        ::Sleep(elapsed);
    }
    
    Int64 GetTickCount()
    {
        return ::GetTickCount();
    }
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
    Void Sleep(UInt32 elapsed)
    {
        struct timespec time;

        time.tv_sec = elapsed / 1000;
        time.tv_nsec = (elapsed % 1000) * 1000 * 1000;

        while (nanosleep(&time, &time) == -1);
    }

    Int64 GetTickCount()
    {
        struct timeval current;
        gettimeofday(&current, NULL);

        return (((UInt64)current.tv_sec * 1000) +
            ((UInt64)current.tv_usec / 1000));
    }
#endif // PLATFORM_WIN32
} // namespace AXP
