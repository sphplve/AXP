
#ifndef __URI_H__
#define __URI_H__

#include "type.h"
#include "aresult.h"
#include "object.h"
#include "astring.h"

namespace AXP {
    class Uri;
    typedef Uri * PUri;
    
    class Uri : public CObject 
    {
    public:
        
        STATIC Sp<Uri> Create(
            IN CONST String & uri)
        {
            return Create((PString)&uri);
        }
        
        STATIC Sp<Uri> Create(
            IN CONST Sp<String> & uri)
        {
            Sp<Uri> ret = new Uri(uri);
            if ((ret == NULL) || (!ret->IsValid()))
                return NULL;
            
            return ret;
        }
        
        STATIC Sp<Uri> Create(
            IN PCWStr uri)
        {
            Sp<Uri> ret = new Uri(uri);
            if ((ret == NULL) || (!ret->IsValid()))
                return NULL;
            
            return ret;
        }

    public:
        
        Uri() { }
        
        Uri(IN CONST Sp<String> & uri)
        {
            mUri = String::Create(uri);
        }

        Uri(IN PCWStr uri)
        {
            mUri = String::Create(uri);
        }

    public:

        Boolean IsValid()
        {
            return ((mUri != NULL) && (!mUri->IsNullOrEmpty()));
        }

        CONST Sp<String> GetUri() CONST
        {
            return mUri;
        }

        CONST Sp<String> GetScheme() CONST
        {
            Int32 pos = mUri->IndexOfString(L"://");
            if (pos < 0)
                return NULL;

            return mUri->SubString(0, pos);
        }

        CONST Sp<String> GetDomain() CONST
        {
            Int32 pos1 = mUri->IndexOfString(L"://");
            if (pos1 < 0)
                return NULL;

            Int32 pos2 = mUri->IndexOf(L'/', pos1 + 3);
            if (pos2 < 0)
                return NULL;

            return mUri->SubString(pos1 + 3, pos2 - pos1 - 3);
        }

        CONST Sp<String> GetRequest() CONST
        {
            Int32 pos1 = mUri->IndexOfString(L"://");
            if (pos1 < 0)
                return NULL;

            Int32 pos2 = mUri->IndexOf(L'/', pos1 + 3);
            if (pos2 < 0)
                return NULL;

            Int32 pos3 = mUri->IndexOf(L'?', pos2 + 1);
            if (pos3 < 0)
                return mUri->SubString(pos2 + 1);
            else
                return mUri->SubString(pos2 + 1, pos3 - pos2 - 1);
        }

        CONST Sp<String> GetArguments() CONST
        {
            Int32 pos1 = mUri->IndexOfString(L"://");
            if (pos1 < 0)
                return NULL;

            Int32 pos2 = mUri->IndexOf(L'/', pos1 + 3);
            if (pos2 < 0)
                return NULL;

            Int32 pos3 = mUri->IndexOf(L'?', pos2 + 1);
            if (pos3 < 0)
                return NULL;

            return mUri->SubString(pos3 + 1);
        }
        
        Boolean Equals(IN CONST Sp<CObject> & obj)
        {
            if (obj == NULL)
                return FALSE;
            
            if (((Sp<Uri>)obj).Get() == this)
                return TRUE;
            
            return (mUri->Equals(((Sp<Uri>)obj)->mUri));
        }

    private:

        Sp<String> mUri;
    };
}

#endif // __URI_H__
