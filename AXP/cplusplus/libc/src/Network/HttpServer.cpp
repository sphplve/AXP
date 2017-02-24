//
//  HttpServer.cpp
//  TestWebServer
//
//  Created by Liu Alex on 14-7-2.
//  Copyright (c) 2014年 Liu Alex. All rights reserved.
//

#include <stdio.h>

#ifdef PLATFORM_WIN32
#include <winsock.h>
typedef int socklen_t;
#define CloseSocket closesocket
#pragma comment(lib, "Ws2_32.lib")
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
typedef int SOCKET;
#define CloseSocket close
#endif // PLATFORM_WIN32

#include "../../../xplatform/include/aresult.h"
#include "../../../xplatform/include/vbuf.h"
#include "../../include/Network/HttpServer.h"

namespace AXP {
    namespace Libc {
        namespace Network {
            CHttpServer::CHttpServer()
            {
                mBackLog = 10;
                mIpAddress = INADDR_ANY;
                mPort = 0;
            }

            Void STDCALL CHttpServer::Run()
            {
                Synchronized(&mLock) {
                    if (AFAILED(Reset())) {
                        NotifyToStop(AE_FAIL);
                        return;
                    }
                }

#ifdef PLATFORM_WIN32
                WSADATA wsaData;
                if (WSAStartup(0x101, &wsaData) != 0) {
                    NotifyToStop(AE_FAIL);
                    return;
                }
#endif // PLATFORM_WIN32

                SOCKET sock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (sock < 0) {
                    NotifyToStop(AE_FAIL);
                    return;
                }

                Int32 yes = 1;
                if (::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (PCStr)&yes, sizeof(yes)) == -1) {
                    ::CloseSocket(sock);
                    NotifyToStop(AE_FAIL);
                    return;
                }

                sockaddr_in servAddr;
                servAddr.sin_family = AF_INET;
                servAddr.sin_addr.s_addr = mIpAddress;
                servAddr.sin_port = htons(mPort);

                if (::bind(
                    sock, (const struct sockaddr *)&servAddr, sizeof(servAddr)) == -1)
                {
                    ::CloseSocket(sock);
                    NotifyToStop(AE_FAIL);
                    return;
                }

                if (::listen(sock, mBackLog) == -1) {
                    ::CloseSocket(sock);
                    NotifyToStop(AE_FAIL);
                    return;
                }

                while (!IsTerminated()) {
                    sockaddr_in clientAddr;
                    socklen_t sockSize = sizeof(clientAddr);

                    int clientSock = ::accept(
                        sock, (struct sockaddr *)&clientAddr, &sockSize);
                    if (clientSock == -1)
                        continue;

                    AcceptRequest(clientSock);

                    ::shutdown(clientSock, 0);
                    ::CloseSocket(clientSock);
                }

                ::CloseSocket(sock);

#ifdef PLATFORM_WIN32
                WSACleanup();
#endif // PLATFORM_WIN32

                NotifyToStop(AS_OK);
            }

            Sp<String> CHttpServer::OnRequest(IN CONST Sp<String> & request)
            {
                return NULL;
            }

            ARESULT CHttpServer::AcceptRequest(Int32 sock)
            {
                mStreamBuffer.Reset();

                Int32 length = ReadLine(sock);
                if (length <= 0)
                    return AS_OK;

                VBuf line;

                line.Append(mStreamBuffer.GetPayload(), length);
                line.Append((PCByte)"\0", 1);
                mStreamBuffer.PopFirst(length);

                Sp<String> prefix = String::Create((PCByte)line.GetPayload(), 1, (Int32)line.GetUsed(), EncodingType_UTF8);
                if (prefix == NULL)
                    return AE_OUTOFMEMORY;

                if (!prefix->StartsWithIgnoreCase("GET", 3))
                    return AS_OK;

                Sp<String> request = String::Create(L"http://localhost");
                if (request == NULL)
                    return AE_OUTOFMEMORY;

                request = request->Append((PCByte)line.GetPayload() + 4, 1, (Int32)line.GetUsed(), EncodingType_UTF8);
                if (request == NULL)
                    return AE_OUTOFMEMORY;

                Int32 pos = request->LastIndexOf(L' ');
                if (pos > 0) {
                    request = request->SubString(0, pos);
                    if (request == NULL)
                        return AE_OUTOFMEMORY;
                }

                Sp<String> result = OnRequest(request);
                if (result == NULL) {
                    BadRequest(sock);
                    return AS_OK;
                }

                Sp<String> response = String::Create(
                    "HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n");
                if (response == NULL) {
                    BadRequest(sock);
                    return AS_OK;
                }

                response = response->Append(result);
                if (response == NULL) {
                    BadRequest(sock);
                    return AS_OK;
                }

                response = response->Append("\r\n");
                if (response == NULL) {
                    BadRequest(sock);
                    return AS_OK;
                }

                Sp<ByteArray> ba = response->GetBytes();
                if (ba == NULL) {
                    BadRequest(sock);
                    return AS_OK;
                }

                ::send(sock, (PCStr)ba->GetPayload(), ba->GetUsed(), 0);

                return AS_OK;
            }

            Int32 CHttpServer::ReadLine(Int32 sock)
            {
                Int32 result = GetLine();
                if (result > 0)
                    return result;

                Byte buffer[1024];

                Long length = ::recv(sock, (PChar)buffer, 1024, 0);
                if (length == -1)
                    return -1;

                mStreamBuffer.PushBack(buffer, length);

                return GetLine();
            }

            Int32 CHttpServer::GetLine()
            {
                PCByte p = mStreamBuffer.GetPayload();
                Int32 used = (Int32)mStreamBuffer.GetUsed();

                for (Int32 i = 1; i < used; ++i, ++p) {
                    if (*p == '\r') {
                        if (*(p + i) == '\n')
                            return i + 1;
                        else
                            return i;
                    }
                }

                return -1;
            }

            Void CHttpServer::BadRequest(Int32 sock)
            {
                ::send(sock,
                    "HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\nAE_FAIL\r\n", 53, 0);
            }
        } // namespace Network
    } // namespace Libc
} // namespace AXP
