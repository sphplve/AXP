
#include "../../../xplatform/include/atime.h"
#include "../../include/MessageQueue/MessageQueue.h"
#include "../../include/MessageQueue/Message.h"
#include "../../include/MessageQueue/Handler.h"
#include "../../include/MessageQueue/Looper.h"

namespace AXP   {
    namespace Libc {
        namespace MessageQueue {
            MessageQueue::MessageQueue()
            {
                mEvent = Event::Create();
                mQuiting = FALSE;
                mMessages = NULL;
            }

            MessageQueue::~MessageQueue()
            {
                Clean();
            }

            Void MessageQueue::Clean()
            {
                if (mEvent != NULL)
                    mEvent->Notify();

                Quit();
            }

            Sp<Message> MessageQueue::Next()
            {
                Int32 nextPollTimeoutMillis = 0;

                for (;;) {
                    if (nextPollTimeoutMillis <= 0)
                        nextPollTimeoutMillis = 1;

                    mEvent->Join(nextPollTimeoutMillis);

                    Synchronized(&mLock) {
                        UInt64 now = GetTickCount();
                        Sp<Message> prevMsg;
                        Sp<Message> msg = mMessages;

                        if (msg != NULL) {
                            if (now < msg->mWhen) {
                                nextPollTimeoutMillis = (Int32)(msg->mWhen - now);
                            }
                            else {
                                if (prevMsg != NULL) {
                                    prevMsg->mNext = msg->mNext;
                                }
                                else {
                                    mMessages = msg->mNext;
                                }

                                msg->mNext = NULL;
                                msg->mIsUsed = TRUE;
                                return msg;
                            }
                        }
                        else
                            nextPollTimeoutMillis = -1;

                        if (mQuiting) {
                            Clean();
                            return NULL;
                        }
                    }
                }
            }

            Void MessageQueue::Quit()
            {
                Synchronized(&mLock) {
                    if (mQuiting)
                        return;

                    mQuiting = TRUE;

                    RemoveAllFutureMessagesLocked();
                }

                if (mEvent != NULL)
                    mEvent->Notify();
            }

            Boolean MessageQueue::EnqueueMessage(
                IN CONST Sp<Message> & msg, IN UInt64 when)
            {
                if ((msg == NULL) || msg->mIsUsed)
                    return FALSE;

                if (msg->mTarget == NULL)
                    return FALSE;

                Synchronized(&mLock) {
                    if (mQuiting)
                        return FALSE;

                    msg->mWhen = when;
                    Sp<Message> p = mMessages;
                    if (p == NULL) {
                        msg->mNext = p;
                        mMessages = msg;
                    }
                    else if ((when == 0) || (when < p->mWhen)) {
                        Sp<Message> prev;
                        for (;;) {
                            prev = p;
                            p = p->mNext;
                            if (p == NULL || ((when > p->mWhen) && (when != 0)))
                                break;
                        }

                        msg->mNext = p;
                        prev->mNext = msg;
                    }
                    else {
                        Sp<Message> prev;
                        for (;;) {
                            prev = p;
                            p = p->mNext;
                            if ((p == NULL) || (when < p->mWhen))
                                break;
                        }

                        msg->mNext = p;
                        prev->mNext = msg;
                    }
                }

                mEvent->Notify();

                return TRUE;
            }

            Void MessageQueue::RemoveAllMessagesLocked()
            {
                Sp<Message> p = mMessages;
                while (p != NULL) {
                    Sp<Message> n = p->mNext;
                    p->Recycle();
                    p = n;
                }

                mMessages = NULL;
            }

            Void MessageQueue::RemoveAllFutureMessagesLocked()
            {
                UInt64 now = GetTickCount();
                Sp<Message> p = mMessages;
                if (p != NULL) {
                    if (p->mWhen > now) {
                        RemoveAllMessagesLocked();
                    }
                    else {
                        Sp<Message> n;
                        for (;;) {
                            n = p->mNext;
                            if (n == NULL) {
                                return;
                            }
                            if (n->mWhen > now) {
                                break;
                            }
                            p = n;
                        }

                        p->mNext = NULL;
                        do {
                            p = n;
                            n = p->mNext;
                            p->Recycle();
                        } while (n != NULL);
                    }
                }
            }
        } // namespace MessageQueue
    } // namespace Libc
} // namespace AXP
