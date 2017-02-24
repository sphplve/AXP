
#ifdef PLATFORM_WIN32
#include <direct.h>
#include <io.h>
#include <malloc.h>
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
#include <unistd.h>
#include <sys/stat.h>
#include <wchar.h>
#include <stdlib.h>
#include <memory.h>
#endif // PLATFORM_WIN32

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "../../include/Common/Tracker.h"

#ifdef PLATFORM_WIN32
#define ACCESS _access
#define MKDIR(a) _mkdir((a))
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
#define ACCESS access
#define MKDIR(a) mkdir((a), 0644)
#endif // PLATFORM_WIN32

namespace AXP {
    namespace Libc {
        namespace Common {
            STATIC Mutex sMutex;
            STATIC FILE * sFile;

            ARESULT CTracker::Initialize(
                IN CONST Sp<String> & path)
            {
                if (path == NULL)
                    return AS_OK;

                return CreateFile(path);
            }

            ARESULT CTracker::CreateFile(
                IN CONST Sp<String> & path)
            {
                if (path == NULL)
                    return AE_INVALIDARG;

                PCWStr pathChar = (PCWStr)path->GetPayload();
                Sp<String> pathName;

                for (Int32 i = path->Length(); i > 0; i--) {
                    if ((pathChar[i] == L'/') || (pathChar[i] == L'\\')) {
                        pathName = path->SubString(0, i + 1);
                        if (pathName == NULL)
                            return AE_OUTOFMEMORY;
                        break;
                    }
                }

                if (pathName == NULL)
                    return AE_INVALIDARG;

                Sp<ByteArray> byteArray = pathName->GetBytes();
                PStr dirPath = (PStr)byteArray->GetPayload();

                for (Int32 i = 0; i < byteArray->GetUsed(); i++) {
                    if ((i == 0) && ((dirPath[i] == '\\') || (dirPath[i] == '/')))
                        continue;

                    if ((dirPath[i] == '\\') || (dirPath[i] == '/')) {
                        dirPath[i] = '\0';

                        // ACCESS 参数2
                        // 6 检查读写权限
                        // 4 检查读权限
                        // 2 检查写权限
                        // 1 检查执行权限
                        // 0 检查文件的存在性
                        if (ACCESS(dirPath, 0) != 0)
                        if (MKDIR(dirPath) != 0)
                            return AE_FAIL;

                        dirPath[i] = '/';
                    }
                }

                if (ACCESS(dirPath, 0) != 0)
                if (MKDIR(dirPath) != 0)
                    return AE_FAIL;

                Synchronized(&sMutex) {
                    if (sFile)
                        ::fclose(sFile);

                    byteArray = path->GetBytes();
                    if (byteArray == NULL)
                        return AE_FAIL;

#ifdef PLATFORM_WIN32
                    sFile = NULL;
                    ::fopen_s(&sFile, (PCStr)byteArray->GetPayload(), "w+");
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
                    sFile = ::fopen((PCStr)byteArray->GetPayload(), "w+");
#endif // PLATFORM_WIN32
                    if (!sFile)
                        return AE_FAIL;
                }

                return AS_OK;
            }

            Void CTracker::Uninitialize()
            {
                if (!sFile) {
                    ::fclose(sFile);
                    sFile = NULL;
                }
            }

            Void CTracker::Log(
                IN PCStr log,
                IN Int32 length)
            {
                PStr logText = (PStr)::malloc((length + 24) * sizeof(Char));
                if (!logText)
                    return;

                time_t curTime = ::time(NULL);

#ifdef PLATFORM_WIN32
                struct tm mt;
                ::localtime_s(&mt, &curTime);

                Int32 retLength = ::sprintf_s(
                    logText,
                    length + 24,
                    "[%04d-%02d-%02d %02d:%02d:%02d] %s\n",
                    mt.tm_year + 1900,
                    mt.tm_mon + 1,
                    mt.tm_mday,
                    mt.tm_hour,
                    mt.tm_min,
                    mt.tm_sec,
                    log);

#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
                struct tm * mt = ::localtime(&curTime);
                if (!mt) {
                    ::free(logText);
                    return;
                }

                Int32 retLength = ::sprintf(
                    logText,
                    "[%04d-%02d-%02d %02d:%02d:%02d] %s\n",
                    mt->tm_year + 1900,
                    mt->tm_mon + 1,
                    mt->tm_mday,
                    mt->tm_hour,
                    mt->tm_min,
                    mt->tm_sec,
                    log);
#endif // PLATFORM_WIN32

                Synchronized(&sMutex) {
                    if (sFile) {
                        ::fwrite(logText, retLength, 1, sFile);
                        ::fflush(sFile);
                    }
                }

                ::free(logText);
            }

            Void CTracker::Log(
                IN PCWStr log,
                IN Int32 length)
            {
                if (!log || length <= 0)
                    return;

                time_t curTime = ::time(NULL);

                Sp<String> logText;
#ifdef PLATFORM_WIN32
                struct tm mt;
                ::localtime_s(&mt, &curTime);
                logText = String::Create(
                    length + 24,
                    L"[%04d-%02d-%02d %02d:%02d:%02d] %ls\n",
                    mt.tm_year + 1900,
                    mt.tm_mon + 1,
                    mt.tm_mday,
                    mt.tm_hour,
                    mt.tm_min,
                    mt.tm_sec,
                    log);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
                struct tm * mt = ::localtime(&curTime);
                if (!mt)
                    return;

                logText = String::Create(
                    length + 24,
                    L"[%04d-%02d-%02d %02d:%02d:%02d] %ls\n",
                    mt->tm_year + 1900,
                    mt->tm_mon + 1,
                    mt->tm_mday,
                    mt->tm_hour,
                    mt->tm_min,
                    mt->tm_sec,
                    log);
#endif // PLATFORM_WIN32

                if (logText == NULL)
                    return;

                Synchronized(&sMutex) {
                    if (sFile) {
                        Sp<ByteArray> byteArray = logText->GetBytes();
                        if (byteArray == NULL)
                            return;

                        ::fwrite(byteArray->GetPayload(), 1, byteArray->GetUsed(), sFile);
                        ::fflush(sFile);
                    }
                }
            }

            Void CTracker::d(
                IN PCWStr content)
            {
                Sp<String> tag = String::Create("[DEBUG] ");
                if (tag == NULL)
                    return;

                tag = tag->Append(content);
                if (tag == NULL)
                    return;

                Log(*tag, tag->Length());
            }

            Void CTracker::d(
                IN PCStr content)
            {
                Sp<String> tag = String::Create("[DEBUG] ");
                if (tag == NULL)
                    return;

                tag = tag->Append(content);
                if (tag == NULL)
                    return;

                Log(*tag, tag->Length());
            }

            Void CTracker::d(
                IN CONST Sp<String> & content)
            {
                if (content == NULL)
                    return;

                Sp<String> tag = String::Create("[DEBUG] ");
                if (tag == NULL)
                    return;

                tag = tag->Append(content);
                if (tag == NULL)
                    return;

                Log(*tag, tag->Length());
            }

            Void CTracker::d(
                IN Int32 length,
                IN PCWStr content,
                IN ...)
            {
                PWStr log = (PWStr)::malloc(length * sizeof(WChar));
                if (log == NULL)
                    return;

                ::va_list varg;

#ifdef PLATFORM_WIN32
                va_start(varg, content);
                Int32 retLength = ::vswprintf_s(
                    log,
                    length * sizeof(WChar),
                    content,
                    varg);
                if (retLength < 0) {
                    ::free(log);
                    va_end(varg);
                    return;
                }

                va_end(varg);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
                ::va_start(varg, content);
                if (::vswprintf(log, length * sizeof(WChar), content, varg) < 0) {
                    ::free(log);
                    ::va_end(varg);
                    return;
                }

                ::va_end(varg);
#endif // PLATFORM_WIN32

                Sp<String> tag = String::Create("[DEBUG] ");
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                tag = tag->Append(log);
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                Log(*tag, tag->Length());

                ::free(log);
            }

            Void CTracker::d(
                IN Int32 length,
                IN PCStr content,
                IN ...)
            {
                PChar log = (PChar)::malloc(length);
                if (log == NULL)
                    return;

                ::va_list varg;


#ifdef PLATFORM_WIN32
                va_start(varg, content);
                Int32 retLength = vsprintf_s(log, length, content, varg);
                if (retLength < 0) {
                    ::free(log);
                    va_end(varg);
                    return;
                }

                va_end(varg);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
                ::va_start(varg, content);
                if (::vsprintf(log, content, varg) < 0) {
                    ::free(log);
                    ::va_end(varg);
                    return;
                }

                ::va_end(varg);
#endif // PLATFORM_WIN32

                Sp<String> tag = String::Create("[DEBUG] ");
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                tag = tag->Append(log);
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                Log(*tag, tag->Length());

                ::free(log);
            }

            Void CTracker::i(
                IN PCWStr content)
            {
                Sp<String> tag = String::Create("[INFO] ");
                if (tag == NULL)
                    return;

                tag = tag->Append(content);
                if (tag == NULL)
                    return;

                Log(*tag, tag->Length());
            }

            Void CTracker::i(
                IN PCStr content)
            {
                Sp<String> tag = String::Create("[INFO] ");
                if (tag == NULL)
                    return;

                tag = tag->Append(content);
                if (tag == NULL)
                    return;

                Log(*tag, tag->Length());
            }

            Void CTracker::i(
                IN CONST Sp<String> & content)
            {
                if (content == NULL)
                    return;

                Sp<String> tag = String::Create("[INFO] ");
                if (tag == NULL)
                    return;

                tag = tag->Append(content);
                if (tag == NULL)
                    return;

                Log(*tag, tag->Length());
            }

            Void CTracker::i(
                IN Int32 length,
                IN PCWStr content,
                IN ...)
            {
                PWStr log = (PWStr)::malloc(length * sizeof(WChar));
                if (log == NULL)
                    return;

                ::va_list varg;

#ifdef PLATFORM_WIN32
                va_start(varg, content);
                Int32 retLength = vswprintf_s(
                    log,
                    length * sizeof(WChar),
                    content,
                    varg);
                if (retLength < 0) {
                    ::free(log);
                    va_end(varg);
                    return;
                }

                va_end(varg);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
                ::va_start(varg, content);
                if (::vswprintf(log, length * sizeof(WChar), content, varg) < 0) {
                    ::free(log);
                    ::va_end(varg);
                    return;
                }

                ::va_end(varg);
#endif // PLATFORM_WIN32

                Sp<String> tag = String::Create("[INFO] ");
                if (tag == NULL){
                    ::free(log);
                    return;
                }

                tag = tag->Append(log);
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                Log(*tag, tag->Length());

                ::free(log);
            }

            Void CTracker::i(
                IN Int32 length,
                IN PCStr content,
                IN ...)
            {
                PChar log = (PChar)::malloc(length);
                if (log == NULL)
                    return;

                ::va_list varg;

#ifdef PLATFORM_WIN32
                va_start(varg, content);
                Int32 retLength = ::vsprintf_s(log, length, content, varg);
                if (retLength < 0) {
                    ::free(log);
                    va_end(varg);
                    return;
                }

                va_end(varg);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
                ::va_start(varg, content);
                if (::vsprintf(log, content, varg) < 0) {
                    ::free(log);
                    ::va_end(varg);
                    return;
                }

                ::va_end(varg);
#endif // PLATFORM_WIN32

                Sp<String> tag = String::Create("[INFO] ");
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                tag = tag->Append(log);
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                Log(*tag, tag->Length());

                ::free(log);
            }

            Void CTracker::w(
                IN PCWStr content)
            {
                Sp<String> tag = String::Create("[WARN] ");
                if (tag == NULL)
                    return;

                tag = tag->Append(content);
                if (tag == NULL)
                    return;

                Log(*tag, tag->Length());
            }

            Void CTracker::w(
                IN PCStr content)
            {
                Sp<String> tag = String::Create("[WARN] ");
                if (tag == NULL)
                    return;

                tag = tag->Append(content);
                if (tag == NULL)
                    return;

                Log(*tag, tag->Length());
            }

            Void CTracker::w(
                IN CONST Sp<String> & content)
            {
                if (content == NULL)
                    return;

                Sp<String> tag = String::Create("[WARN] ");
                if (tag == NULL)
                    return;

                tag = tag->Append(content);
                if (tag == NULL)
                    return;

                Log(*tag, tag->Length());
            }

            Void CTracker::w(
                IN Int32 length,
                IN PCWStr content,
                IN ...)
            {
                PWStr log = (PWStr)::malloc(length * sizeof(WChar));
                if (log == NULL)
                    return;

                ::va_list varg;

#ifdef PLATFORM_WIN32
                va_start(varg, content);
                Int32 retLength = ::vswprintf_s(
                    log,
                    length * sizeof(WChar),
                    content,
                    varg);
                if (retLength < 0) {
                    ::free(log);
                    va_end(varg);
                    return;
                }

                va_end(varg);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
                ::va_start(varg, content);
                if (::vswprintf(log, length * sizeof(WChar), content, varg) < 0) {
                    ::free(log);
                    ::va_end(varg);
                    return;
                }

                ::va_end(varg);
#endif // PLATFORM_WIN32

                Sp<String> tag = String::Create("[WARN] ");
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                tag = tag->Append(log);
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                Log(*tag, tag->Length());

                ::free(log);
            }

            Void CTracker::w(
                IN Int32 length,
                IN PCStr content,
                IN ...)
            {
                PChar log = (PChar)::malloc(length);
                if (log == NULL)
                    return;

                ::va_list varg;

#ifdef PLATFORM_WIN32
                va_start(varg, content);
                Int32 retLength = ::vsprintf_s(log, length, content, varg);
                if (retLength < 0) {
                    ::free(log);
                    va_end(varg);
                    return;
                }

                va_end(varg);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
                ::va_start(varg, content);
                if (!::vsprintf(log, content, varg)) {
                    ::free(log);
                    ::va_end(varg);
                    return;
                }

                ::va_end(varg);
#endif // PLATFORM_WIN32

                Sp<String> tag = String::Create("[WARN] ");
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                tag = tag->Append(log);
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                Log(*tag, tag->Length());

                ::free(log);
            }

            Void CTracker::e(
                IN PCWStr content)
            {
                Sp<String> tag = String::Create("[ERROR] ");
                if (tag == NULL)
                    return;

                tag = tag->Append(content);
                if (tag == NULL)
                    return;

                Log(*tag, tag->Length());
            }

            Void CTracker::e(
                IN PCStr content)
            {
                Sp<String> tag = String::Create("[ERROR] ");
                if (tag == NULL)
                    return;

                tag = tag->Append(content);
                if (tag == NULL)
                    return;

                Log(*tag, tag->Length());
            }

            Void CTracker::e(
                IN CONST Sp<String> & content)
            {
                if (content == NULL)
                    return;

                Sp<String> tag = String::Create("[ERROR] ");
                if (tag == NULL)
                    return;

                tag = tag->Append(content);
                if (tag == NULL)
                    return;

                Log(*tag, tag->Length());
            }

            Void CTracker::e(
                IN Int32 length,
                IN PCWStr content,
                IN ...)
            {
                PWStr log = (PWStr)::malloc(length * sizeof(WChar));
                if (log == NULL)
                    return;

                ::va_list varg;

#ifdef PLATFORM_WIN32
                va_start(varg, content);
                Int32 retLength = ::vswprintf_s(
                    log,
                    length * sizeof(WChar),
                    content,
                    varg);
                if (retLength < 0) {
                    ::free(log);
                    va_end(varg);
                    return;
                }

                va_end(varg);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
                ::va_start(varg, content);
                if (::vswprintf(log, length * sizeof(WChar), content, varg) < 0) {
                    ::free(log);
                    ::va_end(varg);
                    return;
                }

                ::va_end(varg);
#endif // PLATFORM_WIN32

                Sp<String> tag = String::Create("[ERROR] ");
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                tag = tag->Append(log);
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                Log(*tag, tag->Length());

                ::free(log);
            }

            Void CTracker::e(
                IN Int32 length,
                IN PCStr content,
                IN ...)
            {
                PChar log = (PChar)::malloc(length);
                if (log == NULL)
                    return;

                ::va_list varg;

#ifdef PLATFORM_WIN32
                va_start(varg, content);
                Int32 retLength = ::vsprintf_s(log, length, content, varg);
                if (retLength < 0) {
                    ::free(log);
                    va_end(varg);
                    return;
                }

                va_end(varg);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
                ::va_start(varg, content);
                if (!::vsprintf(log, content, varg)) {
                    ::free(log);
                    ::va_end(varg);
                    return;
                }

                ::va_end(varg);
#endif // PLATFORM_WIN32

                Sp<String> tag = String::Create("[ERROR] ");
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                tag = tag->Append(log);
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                Log(*tag, tag->Length());

                ::free(log);
            }

            Void CTracker::f(
                IN PCWStr content)
            {
                Sp<String> tag = String::Create("[FATAL] ");
                if (tag == NULL)
                    return;

                tag = tag->Append(content);
                if (tag == NULL)
                    return;

                Log(*tag, tag->Length());
            }

            Void CTracker::f(
                IN PCStr content)
            {
                Sp<String> tag = String::Create("[FATAL] ");
                if (tag == NULL)
                    return;

                tag = tag->Append(content);
                if (tag == NULL)
                    return;

                Log(*tag, tag->Length());
            }

            Void CTracker::f(
                IN CONST Sp<String> & content)
            {
                if (content == NULL)
                    return;

                Sp<String> tag = String::Create("[FATAL] ");
                if (tag == NULL)
                    return;

                tag = tag->Append(content);
                if (tag == NULL)
                    return;

                Log(*tag, tag->Length());
            }

            Void CTracker::f(
                IN Int32 length,
                IN PCWStr content,
                IN ...)
            {
                PWStr log = (PWStr)::malloc(length * sizeof(WChar));
                if (log == NULL)
                    return;

                ::va_list varg;

#ifdef PLATFORM_WIN32
                va_start(varg, content);
                Int32 retLength = ::vswprintf_s(
                    log,
                    length * sizeof(WChar),
                    content,
                    varg);
                if (retLength < 0) {
                    ::free(log);
                    va_end(varg);
                    return;
                }

                va_end(varg);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
                ::va_start(varg, content);
                if (::vswprintf(log, length * sizeof(WChar), content, varg) < 0) {
                    ::free(log);
                    ::va_end(varg);
                    return;
                }

                ::va_end(varg);
#endif // PLATFORM_WIN32

                Sp<String> tag = String::Create("[FATAL] ");
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                tag = tag->Append(log);
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                Log(*tag, tag->Length());

                ::free(log);
            }

            Void CTracker::f(
                IN Int32 length,
                IN PCStr content,
                IN ...)
            {
                PChar log = (PChar)::malloc(length);
                if (log == NULL)
                    return;

                ::va_list varg;

#ifdef PLATFORM_WIN32
                va_start(varg, content);
                Int32 retLength = ::vsprintf_s(log, length, content, varg);
                if (retLength < 0) {
                    ::free(log);
                    va_end(varg);
                    return;
                }

                va_end(varg);
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
                ::va_start(varg, content);
                if (!::vsprintf(log, content, varg)) {
                    ::free(log);
                    ::va_end(varg);
                    return;
                }

                ::va_end(varg);
#endif // PLATFORM_WIN32
                Sp<String> tag = String::Create("[FATAL] ");
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                tag = tag->Append(log);
                if (tag == NULL) {
                    ::free(log);
                    return;
                }

                Log(*tag, tag->Length());

                ::free(log);
            }
        } // namespace Common
    } // namespace Libc
} // namespace AXP
