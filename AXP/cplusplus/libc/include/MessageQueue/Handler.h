
#ifndef __HANDLER_H__
#define __HANDLER_H__

#include "../../../xplatform/include/type.h"
#include "../../../xplatform/include/astring.h"

namespace AXP {
    namespace Libc {
        namespace MessageQueue {
            class Message;
            class Looper;
            class MessageQueue;

            class Handler : public CObject
            {
            public:

                class Callback : public CObject
                {
                public:

                    VIRTUAL Boolean HandleMessage(IN CONST Sp<Message> & msg) = 0;
                };

                STATIC Sp<Handler> Create(
                    IN CONST Sp<Looper> & looper);

                STATIC Sp<Handler> Create(
                    IN CONST Sp<Looper> & looper,
                    IN CONST Sp<Callback> & callback);

                VIRTUAL Void HandleMessage(
                    IN CONST Sp<Message> & msg)
                {
                }

                Void DispatchMessage(
                    IN CONST Sp<Message> & msg);

                Sp<Message> ObtainMessage();

                Sp<Message> ObtainMessage(
                    IN Int32 msgID);

                Sp<Message> ObtainMessage(
                    IN Int32 msgID,
                    IN CONST Sp<CObject> & obj);

                Sp<Message> ObtainMessage(
                    IN Int32 msgID,
                    IN Int32 arg1,
                    IN Int32 arg2);

                Sp<Message> ObtainMessage(
                    IN Int32 msgID,
                    IN Int32 arg1,
                    IN Int32 arg2,
                    IN CONST Sp<CObject> & obj);

                Boolean BeginInvoke(
                    IN CONST Sp<Message> & msg);

                Boolean BeginInvoke(
                    IN Int32 msgID);

                Boolean BeginInvoke(
                    IN Int32 msgID,
                    IN UInt64 uptimeMillis);

                Boolean BeginInvoke(
                    IN CONST Sp<Message> & msg,
                    IN UInt64 uptimeMillis);

                Boolean Invoke(
                    IN CONST Sp<Message> & msg);

                Boolean Invoke(
                    IN CONST Sp<Message> & msg,
                    IN UInt64 timeout);

                Boolean Invoke(
                    IN Int32 msgID);

                Boolean Invoke(
                    IN Int32 msgID,
                    IN UInt64 timeout);

                Boolean EnqueueMessage(
                    CONST Sp<Message> & msg,
                    IN UInt64 uptimeMillis);

                Sp<Looper> GetLooper();

                VIRTUAL ~Handler();

            protected:

                Handler(
                    IN CONST Sp<Looper> & looper);

                Handler(
                    IN CONST Sp<Looper> & looper,
                    IN CONST Sp<Callback> & callback);

            private:

                Sp<Looper> mLooper;
                Sp<Callback> mCallback;
            };
        } // namespace MessageQueue
    } // namespace Libc
} // namespace AXP

#endif // __HANDLER_H__