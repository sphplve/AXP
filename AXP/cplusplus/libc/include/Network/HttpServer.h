//
//  HttpServer.h
//  TestWebServer
//
//  Created by Liu Alex on 14-7-2.
//  Copyright (c) 2014年 Liu Alex. All rights reserved.
//

#ifndef __TestWebServer__HttpServer__
#define __TestWebServer__HttpServer__

#include "../../../xplatform/include/type.h"
#include "../../../xplatform/include/refBase.h"
#include "../../../xplatform/include/streambuf.h"
#include "../../../xplatform/include/astring.h"
#include "../Common/BaseWorker.h"

namespace AXP {
    namespace Libc {
        namespace Network {
            class CHttpServer : public Common::CBaseWorker
            {
            public:

                STATIC Sp<CBaseWorker> Create()
                {
                    CBaseWorker * obj = new CHttpServer();
                    if (!obj)
                        return NULL;

                    if (AFAILED(obj->Initialize())) {
                        delete obj;
                        return NULL;
                    }

                    return obj;
                }

                CHttpServer();

            public:

                VIRTUAL Void STDCALL Run();

            protected:

                VIRTUAL Sp<String> STDCALL OnRequest(
                    IN CONST Sp<String> & request);

            private:

                ARESULT AcceptRequest(Int32 sock);

                Int32 ReadLine(Int32 sock);

                Int32 GetLine();

                Void BadRequest(Int32 sock);

            protected:

                Int32 mBackLog;

                UInt32 mIpAddress;

                Int32 mPort;

            private:

                StreamBuffer<64 * 1024> mStreamBuffer;
            };
        } // namespace Network
    } // namespace Libc
} // namespace AXP

#endif /* defined(__TestWebServer__HttpServer__) */
