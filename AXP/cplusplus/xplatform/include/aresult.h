
#ifndef __ARESULT_H__
#define __ARESULT_H__

namespace AXP {
    enum ARESULT {
        AS_OK = 0,
        AE_FAIL = -1,
        AE_INVALID_OPERATION = -2,
        AE_DOES_NOT_EXISTS = -3,
        AE_ALREADY_EXISTS = -4,
        AE_OUTOFMEMORY = -5,
        AE_INVALIDARG = -6,
        AE_NOTIMPL = -7,
        AE_EXHAUSTED_RESOURCE = -8,
        AE_BUSY = -9,
        AE_TIMEOUT = -10,
        AE_NOT_SUPPORTED = -11,
        AE_BUFFERTOOSMALL = -12,
        AE_READFAULT = -13,
        AE_WRITEFAULT = -14,
        AE_NOT_FOUND = -15,
        AE_ALREADY_INIT = -16,
        AE_OUTOFALIGNMENT = -17,
        AE_IO_ERROR = -18,
        AE_TERMINATED = -19,
        AE_NOT_ENOUGH_USER_STACK = -100
    };

    static inline bool ASUCCEEDED(AXP::ARESULT ar)
    {
        return (0 == (ar));
    }

    static inline bool AFAILED(AXP::ARESULT ar)
    {
        return (0 != (ar));
    }
} // namespace AXP

#endif // __ARESULT_H__
