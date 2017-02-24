
#ifndef __KEY_VALUE_LIST_H__
#define __KEY_VALUE_LIST_H__

#ifdef PLATFORM_WIN32
#include <malloc.h>
#elif defined(PLATFORM_IOS) || defined(PLATFORM_LINUX)
#include <memory.h>
#endif // PLATFORM_WIN32
#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "aresult.h"
#include "linkedlist.h"
#include "astring.h"

namespace AXP {
    // BKDR Hash Function
    INLINE UInt32 BKDRHash(PCWStr str)
    {
        UInt32 seed = 131; // 31 131 1313 13131 131313 etc..
        UInt32 hash = 0;

        while (*str) {
            hash = hash * seed + (*str++);
        }

        return (hash & 0x7FFFFFFF);
    }

    class KeyValuePair : public DoubleLinkNode
    {
    public:

        KeyValuePair()
        {
            DoubleLinkNode::Initialize();
            hash = 0;
            key = NULL;
            value = NULL;
        }

    public:

        UInt32 hash;
        PWStr key;
        PWStr value;
    };

    class KeyValueList
    {
    public:

        KeyValueList()
        {
            mOriginalString = NULL;
        }

        ~KeyValueList()
        {
            if (mOriginalString)
                free(mOriginalString);
        }

    public:

        ARESULT Initialize(
            IN CONST String & buffer)
        {
            return Initialize((PString)&buffer);
        }
        
        ARESULT Initialize(
            IN CONST Sp<String> & buffer)
        {
            if ((buffer == NULL) || (buffer->IsNullOrEmpty()))
                return AE_INVALIDARG;

            if (mOriginalString)
                return AE_INVALID_OPERATION;

            mOriginalString = Wcsdup_s(*buffer, buffer->GetUsed() + 1);
            if (!mOriginalString)
                return AE_OUTOFMEMORY;

            PWStr p = mOriginalString;
            WChar v;
            KeyValuePair * pair = NULL;

            while ((v = *p) != EOS) {
                if (!pair) {
                    pair = new KeyValuePair();
                    if (!pair)
                        return AE_OUTOFMEMORY;

                    pair->key = p;
                }

                if (v == L'=') {
                    *p = EOS;
                    if (pair->value) {
                        delete pair;
                        return AE_INVALIDARG;
                    }

                    pair->hash = BKDRHash(pair->key);
                    pair->value = p + 1;
                }
                else if (v == L'&') {
                    *p = EOS;
                    if (!pair->value) {
                        delete pair;
                        return AE_INVALIDARG;
                    }

                    mList.InsertLast(pair);
                    pair = NULL;
                }

                p++;
            }

            if (pair) {
                if (!pair->value) {
                    delete pair;
                    return AE_INVALIDARG;
                }

                mList.InsertLast(pair);
                pair = NULL;
            }

            return AS_OK;
        }

        Void Reset()
        {
            mList.Clean();

            if (mOriginalString) {
                free(mOriginalString);
                mOriginalString = NULL;
            }
        }

        Sp<String> Get(PCWStr key)
        {
            UInt32 hash = BKDRHash(key);
            KeyValuePair * pair;

            ForeachDLinkNode(KeyValuePair, pair, &mList) {
                if ((pair->hash == hash)
                    && (0 == Wcscmp(pair->key, key)))
                {
                    return String::Create(pair->value);
                }
            }

            return NULL;
        }

    private:

        PWStr mOriginalString;
        CDLinkList<KeyValuePair> mList;
    };
}

#endif // __KEY_VALUE_LIST_H__
