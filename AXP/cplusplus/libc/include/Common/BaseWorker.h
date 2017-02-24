
#ifndef __BASE_WORKER_H__
#define __BASE_WORKER_H__

#include "../../../xplatform/include/type.h"
#include "../../../xplatform/include/define.h"
#include "../../../xplatform/include/aresult.h"
#include "../../../xplatform/include/object.h"
#include "../../../xplatform/include/thread.h"
#include "../../../xplatform/include/mutex.h"
#include "../../../xplatform/include/atime.h"

#define WORKER_NAME_MAX (64)

namespace AXP {
    namespace Libc {
        namespace Common {
            typedef enum _WorkerStatus {
                WorkerStatus_Idle = 0, // 线程处于无动作状态
                WorkerStatus_Running = 1, // 线程处于运行状态
                WorkerStatus_WaitFor = 2, // 线程处于被等待结束状态
                WorkerStatus_Disposed = 3, // 线程处于已被释放状态
            } WorkerStatus;

            class CBaseWorker : public CObject
            {
            public:

                CBaseWorker();

                VIRTUAL ~CBaseWorker();

            public:

                VIRTUAL ARESULT STDCALL Start();

                VIRTUAL ARESULT STDCALL Join();

                VIRTUAL ARESULT STDCALL NotifyToStop(
                    IN Int32 userData);

                VIRTUAL ARESULT STDCALL Discard();

                VIRTUAL Boolean STDCALL IsTerminated();

                VIRTUAL Boolean STDCALL IsIdle();

                VIRTUAL Boolean STDCALL WaitForTimeout(
                    IN Int64 timeout);

            public:

                VIRTUAL ARESULT STDCALL Initialize();

                VIRTUAL ARESULT STDCALL Reset();

                VIRTUAL Void STDCALL Run();

            public:

                Void STDCALL RunInner();

                STATIC ULong STDCALL Execute(PVoid param);

            public:

                // 线程名称
                WChar mName[WORKER_NAME_MAX];

            protected:

                // 工作线程
                Thread mThread;

                // 等待线程数量
                Int32 mWaitCount;

                // 线程运行标志
                Boolean mRunFlag;

                // 线程状态
                WorkerStatus mStatus;

                // 用户数据
                Int32 mUserData;
            };
        } // namespace Common
    } // namespace Libc
} // namespace AXP

#endif // __BASE_WORKER_H__
