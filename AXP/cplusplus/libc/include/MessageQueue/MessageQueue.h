
#ifndef __MESSAGE_QUEUE_H__
#define __MESSAGE_QUEUE_H__

#include "../../../xplatform/include/type.h"
#include "../../../xplatform/include/event.h"
#include "../../../xplatform/include/object.h"

namespace AXP {
    namespace Libc {
        namespace MessageQueue {
            class Message;
            class Handler;

            class MessageQueue : public CObject
            {
            public:

                MessageQueue();

                ~MessageQueue();

                Void Clean();

                Sp<Message> Next();

                Boolean EnqueueMessage(
                    IN CONST Sp<Message> & msg, UInt64 when);

            private:

                Void Quit();

                Void RemoveAllMessagesLocked();

                Void RemoveAllFutureMessagesLocked();

            private:

                Sp<Message> mMessages;
                Boolean mQuiting;
                Sp<Event> mEvent;
            };
        } // namespace MessageQueue
    } // namespace Libc
} // namespace AXP

#endif // __MESSAGE_QUEUE_H__