
#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include "../../../xplatform/include/type.h"
#include "../../../xplatform/include/astring.h"
#include "../../../xplatform/include/event.h"

namespace AXP {
    namespace Libc {
        namespace MessageQueue {
            class Bundle;
            class Handler;

            class Message : public CObject
            {
            public:

                STATIC Sp<Message> Create(
                    IN CONST Sp<Handler> & target);

            public:

                Void Recycle();

                Void SetTarget(
                    IN CONST Sp<Handler> & target);

                Sp<Handler> GetTarget();

                Sp<Bundle> GetData();

                Void SetData(
                    IN CONST Sp<Bundle> & data);

            private:

                Message();

            public:

                Int32 mMsgID;
                Boolean mIsUsed;
                Boolean mIsSync;
                Int32 mArg1;
                Int32 mArg2;
                Sp<CObject> mObjArg;
                UInt64 mWhen;
                Sp<Bundle> mData;
                Sp<Handler> mTarget;
                Sp<Message> mNext;
                Sp<Event> mEvent;
            };
        } // namespace MessageQueue
    } // namespace Libc
} // namespace AXP

#endif // __MESSAGE_H__
