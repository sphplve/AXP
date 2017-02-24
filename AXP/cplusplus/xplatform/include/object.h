
#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "type.h"
#include "refBase.h"
#include "mutex.h"

namespace AXP {
    class CObject : public RefBase
    {
    public:

        VIRTUAL ~CObject()
        {
        }

    public:

        Mutex mLock;
    };

    STATIC Sp<CObject> NULL_REFERENCE;
} // namespace AXP

#endif // __OBJECT_H__
