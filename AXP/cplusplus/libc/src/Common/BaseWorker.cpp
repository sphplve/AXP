
#include "../../include/Common/BaseWorker.h"

#define SLEEP_DELAY (10)

namespace AXP {
    namespace Libc {
        namespace Common {
            CBaseWorker::CBaseWorker()
            {
                mName[0] = L'\0';
                mWaitCount = 0;
                mRunFlag = FALSE;
                mStatus = WorkerStatus_Idle;
                mUserData = 0;
            }

            CBaseWorker::~CBaseWorker()
            {
                Discard();
                Join();
            }

            ARESULT STDCALL CBaseWorker::Start()
            {
                Synchronized(&mLock) {
                    if (mStatus != WorkerStatus_Idle) {
                        return AE_FAIL;
                    }

                    mStatus = WorkerStatus_Running;
                    mRunFlag = TRUE;

                    if (!mThread.Start((PVoid)Execute, (PVoid)this)) {
                        mStatus = WorkerStatus_Idle;
                        mRunFlag = FALSE;
                        return AE_FAIL;
                    }
                }

                return AS_OK;
            }

            ARESULT STDCALL CBaseWorker::Join()
            {
                Synchronized(&mLock) {
                    if (mStatus == WorkerStatus_Idle) {
                        return AE_FAIL;
                    }

                    mStatus = WorkerStatus_WaitFor;
                    mWaitCount++;
                }

                mThread.WaitFor();

                Synchronized(&mLock) {
                    if (--mWaitCount > 0) {
                        return AS_OK;
                    }

                    mStatus = WorkerStatus_Idle;
                }

                return AS_OK;
            }

            ARESULT STDCALL CBaseWorker::NotifyToStop(
                IN Int32 userData)
            {
                Synchronized(&mLock) {
                    if (mRunFlag) {
                        mRunFlag = FALSE;
                        mUserData = userData;
                    }
                }

                return AS_OK;
            }

            ARESULT STDCALL CBaseWorker::Discard()
            {
                Synchronized(&mLock) {
                    NotifyToStop(AE_TERMINATED);
                }

                return AS_OK;
            }

            Boolean STDCALL CBaseWorker::IsTerminated()
            {
                Synchronized(&mLock) {
                    return (mRunFlag == FALSE);
                }

                return FALSE;
            }

            Boolean STDCALL CBaseWorker::IsIdle()
            {
                Synchronized(&mLock) {
                    return (mStatus == WorkerStatus_Idle);
                }

                return FALSE;
            }

            Boolean STDCALL CBaseWorker::WaitForTimeout(
                IN Int64 timeout)
            {
                while (!IsTerminated()) {
                    timeout -= SLEEP_DELAY;
                    if (timeout <= 0)
                        return TRUE;
                    else
                        AXP::Sleep(SLEEP_DELAY);
                }

                return FALSE;
            }

            ARESULT STDCALL CBaseWorker::Initialize()
            {
                return AS_OK;
            }

            ARESULT STDCALL CBaseWorker::Reset()
            {
                return AS_OK;
            }

            Void STDCALL CBaseWorker::Run()
            {
            }

            Void STDCALL CBaseWorker::RunInner()
            {
                Run();

                Synchronized(&mLock) {
                    if (mStatus == WorkerStatus_Running)
                        mStatus = WorkerStatus_Idle;

                    NotifyToStop(AS_OK);
                }
            }

            ULong STDCALL CBaseWorker::Execute(PVoid param)
            {
                ((CBaseWorker*)param)->RunInner();

                return 0;
            }
        } // namespace Common
    } // namespace Libc
} // namespace AXP
