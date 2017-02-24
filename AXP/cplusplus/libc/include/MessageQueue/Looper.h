
#ifndef __LOOPER_H__
#define __LOOPER_H__

#include "../../../xplatform/include/type.h"
#include "../../../xplatform/include/astring.h"

namespace AXP {
    namespace Libc {
        namespace MessageQueue {
            class MessageQueue;
            class Message;
            class Handler;

            class Looper : public CObject
            {
            public:

                STATIC Sp<Looper> Create();

            public:

                Void Loop();

                ARESULT ExitLoop();

                Sp<MessageQueue> GetMessageQueue();

                Sp<Message> Obtain(
                    IN CONST Sp<Message> & msg);

                Sp<Message> Obtain(
                    IN CONST Sp<Handler> & h);

                Sp<Message> Obtain(
                    IN CONST Sp<Handler> & h,
                    IN Int32 msgID);

                Sp<Message> Obtain(
                    IN CONST Sp<Handler> & h,
                    IN Int32 msgID,
                    IN CONST Sp<CObject> & obj);

                Sp<Message> Obtain(
                    IN CONST Sp<Handler> & h,
                    IN Int32 msgID,
                    IN Int32 arg1,
                    IN Int32 arg2);

                Sp<Message> Obtain(
                    IN CONST Sp<Handler> & h,
                    IN Int32 msgID,
                    IN Int32 arg1,
                    IN Int32 arg2,
                    IN CONST Sp<CObject> & obj);

                Void Recycle(
                    IN CONST Sp<Message> & msg);

            private:

                Looper();

            private:

                Sp<MessageQueue> mQueue;

                Sp<Message> mFreeMessagePool;

                Int32 mPoolSize;

                VOLATILE Boolean mRun;

                ULong mCurrendThreadId;
            };
        } // namespace MessageQueue
    } // namespace Libc
} // namespace AXP

#endif // __LOOPER_H__
