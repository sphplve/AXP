
#ifndef __TRACKER_H__
#define __TRACKER_H__

#include "../../../xplatform/include/type.h"
#include "../../../xplatform/include/aresult.h"
#include "../../../xplatform/include/astring.h"

namespace AXP {
    namespace Libc {
        namespace Common {
            class CTracker
            {
            public:

                STATIC ARESULT STDCALL Initialize(
                    IN CONST Sp<String> & path);

                STATIC Void STDCALL Uninitialize();

            public:

                STATIC Void STDCALL d(
                    IN PCWStr content);

                STATIC Void STDCALL d(
                    IN PCStr content);

                STATIC Void STDCALL d(
                    IN CONST Sp<String> & content);

                STATIC Void STDCALL d(
                    IN Int32 length,
                    IN PCWStr content,
                    IN ...);

                STATIC Void STDCALL d(
                    IN Int32 length,
                    IN PCStr content,
                    IN ...);

                STATIC Void STDCALL i(
                    IN PCWStr content);

                STATIC Void STDCALL i(
                    IN PCStr content);

                STATIC Void STDCALL i(
                    IN CONST Sp<String> & content);

                STATIC Void STDCALL i(
                    IN Int32 length,
                    IN PCWStr content,
                    IN ...);

                STATIC Void STDCALL i(
                    IN Int32 length,
                    IN PCStr content,
                    IN ...);

                STATIC Void STDCALL w(
                    IN PCWStr content);

                STATIC Void STDCALL w(
                    IN PCStr content);

                STATIC Void STDCALL w(
                    IN CONST Sp<String> & content);

                STATIC Void STDCALL w(
                    IN Int32 length,
                    IN PCWStr content,
                    IN ...);

                STATIC Void STDCALL w(
                    IN Int32 length,
                    IN PCStr content,
                    IN ...);

                STATIC Void STDCALL e(
                    IN PCWStr content);

                STATIC Void STDCALL e(
                    IN PCStr content);

                STATIC Void STDCALL e(
                    IN CONST Sp<String> & content);

                STATIC Void STDCALL e(
                    IN Int32 length,
                    IN PCWStr content,
                    IN ...);

                STATIC Void STDCALL e(
                    IN Int32 length,
                    IN PCStr content,
                    IN ...);

                STATIC Void STDCALL f(
                    IN PCWStr content);

                STATIC Void STDCALL f(
                    IN PCStr content);

                STATIC Void STDCALL f(
                    IN CONST Sp<String> & content);

                STATIC Void STDCALL f(
                    IN Int32 length,
                    IN PCWStr content,
                    IN ...);

                STATIC Void STDCALL f(
                    IN Int32 length,
                    IN PCStr content,
                    IN ...);

            private:

                STATIC Void STDCALL Log(
                    IN PCStr log,
                    IN Int32 length);

                STATIC Void STDCALL Log(
                    IN PCWStr log,
                    IN Int32 length);

            private:

                STATIC ARESULT STDCALL CreateFile(
                    IN CONST Sp<String> & path);
            };
        } // namespace Common
    } // namespace Libc
} // namespace AXP

#endif // __TRACKER_H__
