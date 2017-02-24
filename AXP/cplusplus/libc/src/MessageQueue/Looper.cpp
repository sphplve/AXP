
#include "../../../xplatform/include/thread.h"
#include "../../include/MessageQueue/Looper.h"
#include "../../include/MessageQueue/Message.h"
#include "../../include/MessageQueue/MessageQueue.h"
#include "../../include/MessageQueue/Handler.h"
#include "../../include/MessageQueue/Bundle.h"

namespace AXP  {
    namespace Libc {
        namespace MessageQueue {
#define MAX_POOL_SIZE (50)

            Sp<Looper> Looper::Create()
            {
                Sp<Looper> looper = new Looper();
                if (looper == NULL)
                    return NULL;

                looper->mQueue = new MessageQueue();
                if (looper->mQueue == NULL)
                    return NULL;

                looper->mRun = TRUE;
                looper->mCurrendThreadId = GetCurrentThreadId();

                return looper;
            }

            Looper::Looper()
            {
            }

            Void Looper::Loop()
            {
                ASSERT(mCurrendThreadId == GetCurrentThreadId());
                while (mRun) {
                    Sp<Message> msg = mQueue->Next();
                    if (msg == NULL)
                        return;

                    if (msg->mTarget != NULL)
                        msg->mTarget->DispatchMessage(msg);

                    Recycle(msg);
                    if ((msg->mIsSync) && (msg->mEvent != NULL))
                        msg->mEvent->Notify();
                }

                mRun = TRUE;
            }

            ARESULT Looper::ExitLoop()
            {
                Synchronized(&mLock) {
                    mRun = FALSE;
                }

                return AS_OK;
            }

            Sp<MessageQueue> Looper::GetMessageQueue()
            {
                return mQueue;
            }

            Sp<Message> Looper::Obtain(
                IN CONST Sp<Message> & msg)
            {
                Sp<Message> m = Obtain(msg->mTarget);
                if (m == NULL)
                    return NULL;

                m->mMsgID = msg->mMsgID;
                m->mArg1 = msg->mArg1;
                m->mArg2 = msg->mArg2;
                m->mObjArg = msg->mObjArg;
                if (msg->mData != NULL) {
                    m->mData = Bundle::Create(msg->mData);
                    if (m->mData == NULL)
                        return NULL;
                }

                return m;
            }

            Sp<Message> Looper::Obtain(
                IN CONST Sp<Handler> & h)
            {
                Synchronized(&mLock) {
                    if (mFreeMessagePool != NULL) {
                        Sp<Message> m = mFreeMessagePool;
                        mFreeMessagePool = m->mNext;
                        m->mNext = NULL;
                        mPoolSize--;
                        m->mTarget = h;
                        return m;
                    }
                }

                return Message::Create(h);
            }

            Sp<Message> Looper::Obtain(
                IN CONST Sp<Handler> & h,
                IN Int32 msgID)
            {
                Sp<Message> m = Obtain(h);
                if (m == NULL)
                    return NULL;

                m->mMsgID = msgID;

                return m;
            }

            Sp<Message> Looper::Obtain(
                IN CONST Sp<Handler> & h,
                IN Int32 msgID,
                IN CONST Sp<CObject> & obj)
            {
                Sp<Message> m = Obtain(h);
                if (m == NULL)
                    return NULL;

                m->mMsgID = msgID;
                m->mObjArg = obj;

                return m;
            }

            Sp<Message> Looper::Obtain(
                IN CONST Sp<Handler> & h,
                IN Int32 msgID,
                IN Int32 arg1,
                IN Int32 arg2)
            {
                Sp<Message> m = Obtain(h);
                if (m == NULL)
                    return NULL;

                m->mTarget = h;
                m->mMsgID = msgID;
                m->mArg1 = arg1;
                m->mArg2 = arg2;

                return m;
            }

            Sp<Message> Looper::Obtain(
                IN CONST Sp<Handler> & h,
                IN Int32 msgID,
                IN Int32 arg1,
                IN Int32 arg2,
                IN CONST Sp<CObject> & obj)
            {
                if (h == NULL)
                    return NULL;

                Sp<Message> m = Obtain(h);
                if (m == NULL)
                    return NULL;

                m->mTarget = h;
                m->mMsgID = msgID;
                m->mArg1 = arg1;
                m->mArg2 = arg2;
                m->mObjArg = obj;

                return m;
            }

            Void Looper::Recycle(
                IN CONST Sp<Message> & msg)
            {
                msg->Recycle();
                Synchronized(&mLock) {
                    if (mPoolSize < MAX_POOL_SIZE) {
                        msg->mNext = mFreeMessagePool;
                        mFreeMessagePool = msg;
                        mPoolSize++;
                    }
                }
            }
        } // namespace MessageQueue
    } // namespace Libc
} // namespace AXP
