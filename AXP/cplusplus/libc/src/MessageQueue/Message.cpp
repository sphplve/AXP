
#include "../../include/MessageQueue/Message.h"
#include "../../include/MessageQueue/Bundle.h"
#include "../../include/MessageQueue/Handler.h"
#include "../../include/MessageQueue/MessageQueue.h"
#include "../../include/MessageQueue/Looper.h"

namespace AXP {
    namespace Libc {
        namespace MessageQueue {
            Sp<Message> Message::Create(
                IN CONST Sp<Handler> & target)
            {
                if (target == NULL)
                    return NULL;

                Sp<Message> msg = new Message();
                if (msg == NULL)
                    return NULL;

                msg->mEvent = Event::Create();
                if (msg->mEvent == NULL)
                    return NULL;

                msg->mTarget = target;

                return msg;
            }

            Message::Message()
            {
                mIsUsed = FALSE;
                mMsgID = -1;
                mArg1 = 0;
                mArg2 = 0;
                mObjArg = NULL;
                mWhen = 0;
                mTarget = NULL;
                mData = NULL;
            }

            Void Message::Recycle()
            {
                mIsUsed = FALSE;
                mMsgID = -1;
                mArg1 = 0;
                mArg2 = 0;
                mObjArg = NULL;
                mWhen = 0;
                mTarget = NULL;
                mData = NULL;
            }

            Void Message::SetTarget(
                IN CONST Sp<Handler> & target)
            {
                mTarget = target;
            }

            Sp<Handler> Message::GetTarget()
            {
                return mTarget;
            }

            Sp<Bundle> Message::GetData()
            {
                return mData;
            }

            Void Message::SetData(
                IN CONST Sp<Bundle> & data)
            {
                mData = data;
            }
        } // namespace MessageQueue
    } // namespace Libc
} // namespace AXP
