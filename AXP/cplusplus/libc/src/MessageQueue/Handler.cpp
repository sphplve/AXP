
#include "../../include/MessageQueue/Handler.h"
#include "../../include/MessageQueue/Looper.h"
#include "../../include/MessageQueue/Message.h"
#include "../../include/MessageQueue/MessageQueue.h"

namespace AXP  {
    namespace Libc {
        namespace MessageQueue {
            Handler::Handler(
                IN CONST Sp<Looper> & looper)
            {
                mLooper = looper;
            }

            Handler::Handler(
                IN CONST Sp<Looper> & looper,
                CONST Sp<Callback> & callback)
            {
                mLooper = looper;
                mCallback = callback;
            }

            Handler::~Handler()
            {
            }

            Void Handler::DispatchMessage(
                IN CONST Sp<Message> & msg)
            {
                if (mCallback != NULL) {
                    if (mCallback->HandleMessage(msg)) {
                        return;
                    }
                }

                HandleMessage(msg);
            }

            Sp<Handler> Handler::Create(
                IN CONST Sp<Looper> & looper)
            {
                return Create(looper, NULL);
            }

            Sp<Handler> Handler::Create(
                IN CONST Sp<Looper> & looper,
                IN CONST Sp<Callback> & callback)
            {
                if (looper == NULL)
                    return NULL;

                return (new Handler(looper, callback));
            }

            Sp<Message> Handler::ObtainMessage()
            {
                return mLooper->Obtain(this);
            }

            Sp<Message> Handler::ObtainMessage(
                IN Int32 msgID)
            {
                return mLooper->Obtain(this, msgID);
            }

            Sp<Message> Handler::ObtainMessage(
                IN Int32 msgID,
                IN CONST Sp<CObject> & obj)
            {
                return mLooper->Obtain(this, msgID, obj);
            }

            Sp<Message> Handler::ObtainMessage(
                IN Int32 msgID,
                IN Int32 arg1,
                IN Int32 arg2)
            {
                return mLooper->Obtain(this, msgID, arg1, arg2);
            }

            Sp<Message> Handler::ObtainMessage(
                IN Int32 msgID,
                IN Int32 arg1,
                IN Int32 arg2,
                IN CONST Sp<CObject> & obj)
            {
                return mLooper->Obtain(this, msgID, arg1, arg2, obj);
            }

            Boolean Handler::BeginInvoke(
                IN CONST Sp<Message> & msg)
            {
                return BeginInvoke(msg, 0);
            }

            Boolean Handler::BeginInvoke(
                IN CONST Sp<Message> & msg,
                IN UInt64 uptimeMillis)
            {
                return EnqueueMessage(msg, uptimeMillis);
            }

            Boolean Handler::BeginInvoke(
                IN Int32 msgID)
            {
                return BeginInvoke(msgID, 0);
            }

            Boolean Handler::BeginInvoke(
                IN Int32 msgID,
                IN UInt64 uptimeMillis)
            {
                Sp<Message> msg = mLooper->Obtain(this);
                if (msg == NULL)
                    return FALSE;

                msg->mMsgID = msgID;

                return BeginInvoke(msg, uptimeMillis);
            }

            Boolean Handler::Invoke(
                IN CONST Sp<Message> & msg)
            {
                if (msg == NULL)
                    return FALSE;

                if (!EnqueueMessage(msg, 0))
                    return FALSE;

                msg->mIsSync = TRUE;
                if (msg->mEvent != NULL)
                    msg->mEvent->Join();

                return TRUE;
            }

            Boolean Handler::Invoke(
                IN CONST Sp<Message> & msg,
                IN UInt64 timeout)
            {
                if (msg == NULL)
                    return FALSE;

                if (!EnqueueMessage(msg, 0))
                    return FALSE;

                msg->mIsSync = TRUE;
                if (msg->mEvent != NULL)
                    msg->mEvent->Join(timeout);

                return TRUE;
            }

            Boolean Handler::Invoke(
                IN Int32 msgID)
            {
                Sp<Message> msg = mLooper->Obtain(this);
                if (msg == NULL)
                    return FALSE;

                msg->mMsgID = msgID;

                return Invoke(msg);
            }

            Boolean Handler::Invoke(
                IN Int32 msgID,
                IN UInt64 timeout)
            {
                Sp<Message> msg = mLooper->Obtain(this);
                if (msg == NULL)
                    return FALSE;

                msg->mMsgID = msgID;

                return Invoke(msg, timeout);
            }

            Boolean Handler::EnqueueMessage(
                IN CONST Sp<Message> & msg,
                IN UInt64 uptimeMillis)
            {
                msg->mTarget = this;
                Sp<MessageQueue> queue = mLooper->GetMessageQueue();
                if (queue == NULL)
                    return FALSE;

                return queue->EnqueueMessage(msg, uptimeMillis);
            }

            Sp<Looper> Handler::GetLooper()
            {
                return mLooper;
            }
        } // namespace MessageQueue
    } // namespace Libc
} // namespace AXP
